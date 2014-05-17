#ifndef CSVCONFIGWIDGET_H
#define CSVCONFIGWIDGET_H

#include <QWidget>
#include <QString>

class CSVReader;
class CSVTableModel;
class CSVPropertiesWidget;
class QTableView;

class CSVConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSVConfigWidget(QWidget *parent);
    void setReader(QString filename, CSVReader *r);
    CSVTableModel *getTableModel() const;

private slots:
    void readCSV();
private:
    CSVReader *reader;
    QByteArray input;
    CSVPropertiesWidget *CSVReaderProperties;
    QTableView *tableView;
    CSVTableModel *tableModel;
};

#endif // CSVCONFIGWIDGET_H
