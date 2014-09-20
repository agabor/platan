#ifndef VALIDATION_H
#define VALIDATION_H

class QString;
class QRegExp;
template<typename K, typename V>
class QMap;
class QChar;
template <class T1, class T2>
struct QPair;
template <class T>
struct QVector;


class Ereaser
{
public:
    Ereaser(QString tagName, QString regexp);
    void ReplaceAll(QString &data);
    bool exactMatch ( const QString & str );
    virtual QString getTag ( const QString & str ) const;
    QVector<QPair<int, QString>> getMatches(const QString &data) const;
protected:
    QPair<int, QString> nextValidMatch(const QString &data, int p) const;
    virtual bool isValid (const QString & ) const {return true;}
    const QRegExp regexp;
    const QString tagName;
    QMap<QString, int> codes;
};

class IBANEreaser : public Ereaser
{
public:
    IBANEreaser();
protected:
    bool isValid (const QString &input ) const;
};


class BICEreaser : public Ereaser
{
public:
    BICEreaser();
protected:
    bool isValid (const QString &input ) const;
};

class DateEreaser : public Ereaser
{
public:
    DateEreaser(QString tag1, QString tag2, QString tag3, QChar sep);
private:
    static QString getDateRegexp(QString tag1, QString tag2, QString tag3, QChar sep);
    static const QString DD;
    static const QString MM;
    static const QString YY;
    static const QString YYYY;
};

class AmountEreaser :public Ereaser
{
public:
    AmountEreaser(QChar sep);
    QString getTag ( const QString & ) const;
private:
    QChar sep;
};

class NumberEreaser : public Ereaser
{
public:
    NumberEreaser();
    QString getTag (const QString & str) const;
};

#endif // VALIDATION_H
