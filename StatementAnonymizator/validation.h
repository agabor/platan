#ifndef VALIDATION_H
#define VALIDATION_H

class QString;
class QRegExp;
template<typename K, typename V>
class QMap;
class QChar;

class Ereaser
{
public:
    Ereaser(QString tagName, QString regexp);
    void ReplaceAll(QString &data);
protected:
    virtual bool isValid (const QString & ) {return true;}
    const QRegExp regexp;
    const QString tagName;
    QMap<QString, int> codes;
};

class IBANEreaser : public Ereaser
{
public:
    IBANEreaser();
protected:
    bool isValid (const QString &input );
};


class BICEreaser : public Ereaser
{
public:
    BICEreaser();
protected:
    bool isValid (const QString &input );
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

#endif // VALIDATION_H
