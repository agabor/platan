#include <QString>
#include <QStringList>

#include <simpletransformations.h>

IdentityTransFormation::IdentityTransFormation()
{

}

QString IdentityTransFormation::convert(QString Data) const
{
    return Data;
}

QString IdentityTransFormation::getErrorMessage() const
{
    return QString();
}


IntTransformation::IntTransformation()
{

}

int IntTransformation::convert(QString data) const
{
    bool ok;
    int result = data.toInt(&ok);
    if (!ok)
        errorList.push_back(data);
    return result;
}

QString IntTransformation::getErrorMessage() const
{
    QString result= QObject::tr("The following values do not look like integers: ");
    return result + errorListString();
}


FloatTransformation::FloatTransformation()
{

}

float convertStrToFloat(QString whole_string, QString frac_string, bool *ok)
{
    *ok = true;
    const int w = whole_string.toInt(ok);

    if (!*ok)
        return 0;

    if (frac_string.isEmpty())
        return w;

    const int frac = frac_string.toInt(ok);

    if (!*ok)
        return w;

    const int decimal_places = frac_string.length();

    float frac_mul = 1;
    for (int i = 0; i < decimal_places; ++i)
        frac_mul /= 10;
    return w + (w < 0 ? -1 : 1) * frac * frac_mul;
}

float FloatTransformation::convert(QString data) const
{
    QStringList string_list = data.split(QRegExp("[\\.,]"));
    const int length = string_list.length();

    if (length == 0)
    {
        errorList.push_back(data);
        return 0;
    }

    QString whole_string = string_list.at(0);
    QString frac_string = length == 1 ? "" : string_list.at(1);

    bool ok;
    float result = convertStrToFloat(whole_string, frac_string, &ok);
    if (!ok)
    {
        errorList.push_back(data);
        return 0;
    }
    return result;
}

QString FloatTransformation::getErrorMessage() const
{
    QString result = QObject::tr("The following values do not look like real numbers: ");
    return result + errorListString();
}
