#include <string>
#include <QMap>
#include <algorithm>
#include <cctype>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QPair>
#include <QVector>

#include "validation.h"

QMap<QString, int> countrycodes
{ {"AL" , 28} , {"AD" , 24} , {"AT" , 20} , {"AZ" , 28 } ,
    {"BE" , 16} , {"BH" , 22} , {"BA" , 20} , {"BR" , 29 } ,
    {"BG" , 22} , {"CR" , 21} , {"HR" , 21} , {"CY" , 28 } ,
    {"CZ" , 24} , {"DK" , 18} , {"DO" , 28} , {"EE" , 20 } ,
    {"FO" , 18} , {"FI" , 18} , {"FR" , 27} , {"GE" , 22 } ,
    {"DE" , 22} , {"GI" , 23} , {"GR" , 27} , {"GL" , 18 } ,
    {"GT" , 28} , {"HU" , 28} , {"IS" , 26} , {"IE" , 22 } ,
    {"IL" , 23} , {"IT" , 27} , {"KZ" , 20} , {"KW" , 30 } ,
    {"LV" , 21} , {"LB" , 28} , {"LI" , 21} , {"LT" , 20 } ,
    {"LU" , 20} , {"MK" , 19} , {"MT" , 31} , {"MR" , 27 } ,
    {"MU" , 30} , {"MC" , 27} , {"MD" , 24} , {"ME" , 22 } ,
    {"NL" , 18} , {"NO" , 15} , {"PK" , 24} , {"PS" , 29 } ,
    {"PL" , 28} , {"PT" , 25} , {"RO" , 24} , {"SM" , 27 } ,
    {"SA" , 24} , {"RS" , 22} , {"SK" , 24} , {"SI" , 19 } ,
    {"ES" , 24} , {"SE" , 24} , {"CH" , 21} , {"TN" , 24 } ,
    {"TR" , 26} , {"AE" , 23} , {"GB" , 22} , {"VG" , 24 } } ;

IBANEreaser::IBANEreaser()
    : Ereaser("IBAN",
              "([a-zA-Z]\\s?){2}([0-9]\\s?){2}([a-zA-Z0-9]\\s?){4}([0-9]\\s?){7}([a-zA-Z0-9]?\\s?){0,16}")
{
}

Ereaser::Ereaser(QString tagName, QString regexp)
    : regexp(regexp), tagName(tagName)
{

}



bool Ereaser::exactMatch(const QString &str)
{
    return regexp.exactMatch(str) && isValid(str);
}

QString Ereaser::getTag(const QString &str)
{
    QString tag{"<%1 %2>"};
    return tag.arg(tagName).arg(getCode(str));
}

QPair<int, QString> Ereaser::nextValidMatch(const QString &data, int position) const
{
    QString s;
    do
    {
        position= regexp.indexIn(data, position);
        if (position == -1)
            return QPair<int, QString>(-1, QString{});
        s = regexp.cap(0);
        if (isValid(data, position, s.length()))
            break;
        position += regexp.matchedLength();
    } while (position != -1);
    return QPair<int, QString>(position, s);
}

bool Ereaser::isValid(const QString &s, int position, int length) const
{
    return isValid(s.mid(position, length));
}

int Ereaser::getCode(QString str)
{
    if(!codes.contains(str))
        codes.insert(str, codes.count() + 1);
    return codes[str];
}

QVector<QPair<int, QString> > Ereaser::getMatches(const QString &data)
{
    QVector<QPair<int, QString> > result;
    QPair<int, QString> match;
    int pos = 0;
    while ((match = nextValidMatch(data, pos)).first != -1)
    {
        result.push_back(match);
        pos = match.first + match.second.length();
    }
    return result;
}

//from http://rosettacode.org/wiki/IBAN
bool IBANEreaser::isValid(const QString &input) const
{
    QString teststring( input ) ;
    //erase_all( teststring , " " ) ; //defined in boost/algorithm/string.hpp
    teststring.replace(QRegExp("\\s*"), "");
    if ( !countrycodes.contains(teststring.mid(0 , 2 )))
        return false ;
    if ( teststring.length( ) != countrycodes[ teststring.mid( 0 , 2 ) ] )
        return false ;
    //if ( ! ( all ( teststring , is_alnum( ) ) ) )
    //    return false ;
    teststring = teststring.toUpper();
    teststring = teststring.append( teststring.mid( 0 , 4 ) ) ;
    teststring =  teststring.mid( 4 ) ;
    QString numberstring ;//will contain the letter substitutions
    for ( int i = 0 ; i < teststring.length( ) ; i++ ) {
        if ( teststring.at(i).isDigit() )
            numberstring = numberstring +  teststring[ i ]  ;
        if ( teststring.at(i).isUpper() )
            numberstring = numberstring +  QString::number( static_cast<int>( teststring.at(i).toLatin1() ) - 55 ) ;
    }
    //implements a stepwise check for mod 97 in chunks of 9 at the first time
    // , then in chunks of seven prepended by the last mod 97 operation converted
    //to a string
    int segstart = 0 ;
    int step = 9 ;
    QString prepended ;
    long number = 0 ;
    while ( segstart  < numberstring.length( ) - step ) {
        number = ( prepended + numberstring.mid( segstart , step ) ).toLong() ;
        int remainder = number % 97 ;
        prepended =  QString::number( remainder ) ;
        if ( remainder < 10 )
            prepended = "0" + prepended ;
        segstart = segstart + step ;
        step = 7 ;
    }
    number = ( prepended + numberstring.mid( segstart )).toLong() ;
    return ( number % 97 == 1 ) ;
}


bool BICEreaser::isValid(const QString &input) const
{
    if (input.length() != 8 && input.length() != 11)
        return false;
    return countrycodes.keys().contains(input.mid(4, 2));
}

bool BICEreaser::isValid(const QString &s, int position, int length) const
{
    if (position != 0 && s.at(position-1).isLetterOrNumber())
        return false;
    if (position + length != s.length() && s.at(position + length).isLetterOrNumber())
        return false;
    return isValid(s.mid(position, length));
}


BICEreaser::BICEreaser()
    : Ereaser("BIC",
              "[a-zA-Z]{6}[0-9a-zA-Z]{2}([0-9a-zA-Z]{3})?")
{
}

const QString DateEreaser::DD{"((0[1-9])|([12][0-9])|30|31)"};
const QString DateEreaser::MM{"((0[1-9])|(1[0-2]))"};
const QString DateEreaser::YY{"((8|9|0|1)\\d)"};
const QString DateEreaser::YYYY{"((19|20)\\d{2})"};

QString DateEreaser::getDateRegexp(QString tag1, QString tag2, QString tag3, QChar sep)
{
    QString regexp;
    QString sepStr = QRegExp::escape(sep);
    for (auto s : {tag1, tag2, tag3})
    {
        if (!regexp.isEmpty())
            regexp += sepStr;
        if (s == "DD")
        {
            regexp += DD;
        } else if (s == "MM")
        {
            regexp += MM;
        } else if (s == "YY")
        {
            regexp += YY;
        } else if (s == "YYYY")
        {
            regexp += YYYY;
        }
    }
    return regexp;
}

DateEreaser::DateEreaser(QString tag1, QString tag2, QString tag3, QChar sep)
    : Ereaser(QString{"%1%4%2%4%3"}.arg(tag1).arg(tag2).arg(tag3).arg(sep),
              getDateRegexp(tag1, tag2, tag3, sep))
{
}


AmountEreaser::AmountEreaser(QChar sep)
    : Ereaser("Amount", QString{"\\-?(([1-9][0-9\\s]+)|[0-9])(%1[0-9\\s]+)"}.arg(QRegExp::escape(sep))),
      sep(sep)
{

}

QString AmountEreaser::getTag(const QString &)
{
    QString tag{"<%1 %2>"};
    return tag.arg(tagName).arg(sep);
}


NumberEreaser::NumberEreaser()
    : Ereaser("Number", "[0-9]{4,}")
{

}

QString NumberEreaser::getTag(const QString &str)
{
    QString tag{"<%1 %2 digits %3>"};
    return tag.arg(tagName).arg(str.length()).arg(getCode(str));
}
