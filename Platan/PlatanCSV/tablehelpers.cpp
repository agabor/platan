#include <tablehelpers.h>

#include <QTableView>

void resizeToContents(QTableView *tableView)
{
    tableView->resizeColumnsToContents();
    for (int i = 0; i < tableView->model()->columnCount(); ++i)
    {
        if (tableView->columnWidth(i) > 250)
            tableView->setColumnWidth(i, 250);
    }
    tableView->resizeRowsToContents();
}
