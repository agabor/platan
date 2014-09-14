#include <string>
#include <QMap>
#include <algorithm>
#include <cctype>
#include <QString>
#include <QStringList>
#include <QRegExp>

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

void Ereaser::ReplaceAll(QString &data)
{
    int pos = 0;
    QStringList ibanlist;
    while ((pos = regexp.indexIn(data, pos)) != -1)
    {
        QString s = regexp.cap(0);
        if (isValid(s))
        {
            ibanlist << s;
            if(!codes.contains(s))
                codes.insert(s, codes.count() + 1);
        }
        pos += regexp.matchedLength();
    }
    for (auto s : ibanlist)
    {
        QString tag{"<%1 %2>"};
        tag = tag.arg(tagName).arg(codes[s]);
        data.replace(s, tag);
    }
}

//from http://rosettacode.org/wiki/IBAN
bool IBANEreaser::isValid(const QString &input)
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


bool BICEreaser::isValid(const QString &input)
{
    if (input.length() != 8 && input.length() != 11)
        return false;
    return countrycodes.keys().contains(input.mid(4, 2));
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
    QString sepStr = sep == '.' ? QString{"\\."} : QString{sep};
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
