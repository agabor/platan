#ifndef CSVIMPORTWIDGET_H
#define CSVIMPORTWIDGET_H

#include <QWidget>
#include <csvreader.h>
#include <memory>

class QComboBox;
class QCheckBox;

class CSVImportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSVImportWidget(QWidget *parent = 0);
    void setReader(CSVReader *r);
    void SelectData(QComboBox *comboBox, char c);
signals:
    void readerParametersChanged();
private slots:
    void setSeparator(int index);
    void setQuote(int index);
    void setHeaders(bool b);

private:
    CSVReader* reader;
    QComboBox* quote;
    QComboBox* separator;
    QCheckBox* headers;
};


#endif // CSVIMPORTWIDGET_H
