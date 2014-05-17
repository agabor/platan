#ifndef CSVIMPORTWIDGET_H
#define CSVIMPORTWIDGET_H

#include <QWidget>

class QAbstractTableModel;
class QTableView;
class ColumnPropertiesWidget;

class CSVImportWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CSVImportWidget(QWidget *parent = 0);
    void setTableModel (QAbstractTableModel *model);
private:
    QTableView *tableView;
    ColumnPropertiesWidget *columnPropertiesWidget;
};

#endif // CSVIMPORTWIDGET_H
