#ifndef IMPORTCOLUMNPROPERTIES_H
#define IMPORTCOLUMNPROPERTIES_H

#include <QWidget>
class QComboBox;
class QLabel;

class ColumnPropertiesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ColumnPropertiesWidget(QWidget *parent = 0);

private slots:
    void setType(int idx);

private:
    QComboBox *useAs;
    QComboBox *separator;
    QLabel *separatorLabel;
    QComboBox *format;
    QLabel *formatLabel;
};

#endif // IMPORTCOLUMNPROPERTIES_H
