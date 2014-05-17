#ifndef CSVCONFIGWIDGET_H
#define CSVCONFIGWIDGET_H

#include <QWidget>
#include <QString>

class CSVReader;
class CSVPropertiesWidget;
class QTableView;

class CSVConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSVConfigWidget(QWidget *parent);
    void setReader(QString filename, CSVReader *r);
private slots:
    void readCSV();
private:
    CSVReader *reader;
    QByteArray input;
    CSVPropertiesWidget *CSVReaderProperties;
    QTableView *tableView;
};

#endif // CSVCONFIGWIDGET_H
