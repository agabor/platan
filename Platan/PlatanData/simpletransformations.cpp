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

float FloatTransformation::convert(QString data) const
{
    QStringList string_list = data.split(decimal);
    const int length = string_list.length();

    bool error_found = false;
    if (length == 0)
    {
        errorList.push_back(data);
        error_found = true;
        return 0;
    }

    bool ok;
    const int w = string_list.at(0).toInt(&ok);

    if (!ok && !error_found)
    {
        errorList.push_back(data);
        error_found = true;
    }

    if (length == 1)
        return (float)w;

    const QString frac_string = string_list.at(1);
    const int frac = frac_string.toInt(&ok);

    if (!ok && !error_found)
    {
        errorList.push_back(data);
        error_found = true;
    }

    const int decimal_places = frac_string.length();

    float frac_mul = 1;
    for (int i = 0; i < decimal_places; ++i)
        frac_mul /= 10;

    return w + (w < 0 ? -1 : 1) * frac * frac_mul;
}

char FloatTransformation::getDecimal() const
{
    return decimal;
}
void FloatTransformation::setDecimal(char value)
{
    decimal = value;
}

QString FloatTransformation::getErrorMessage() const
{
    QString result = QObject::tr("The following values do not look like real numbers: ");
    return result + errorListString();
}
