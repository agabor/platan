#ifndef CSVCONFIGWIDGET_H
#define CSVCONFIGWIDGET_H

#include <QWidget>
#include <QString>

class CSVReader;
class CSVImportWidget;
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
    CSVImportWidget *CSVReaderProperties;
    QTableView *tableView;
};

#endif // CSVCONFIGWIDGET_H
