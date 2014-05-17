#include "csvimportwidget.h"
#include <QTableView>
#include <QVBoxLayout>
#include <columnpropertieswidget.h>

CSVImportWidget::CSVImportWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayount = new QVBoxLayout(this);
    columnPropertiesWidget = new ColumnPropertiesWidget(this);
    mainLayount->addWidget(columnPropertiesWidget);
    tableView = new QTableView(this);
    mainLayount->addWidget(tableView);
}

void CSVImportWidget::setTableModel(QAbstractTableModel *model)
{
    tableView->setModel(model);
}
