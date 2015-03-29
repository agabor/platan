#include "runscriptdialog.h"
#include "ui_runscriptdialog.h"

#include <QListWidgetItem>

RunScriptDialog::RunScriptDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::RunScriptDialog)
{
  ui->setupUi(this);

  QListWidgetItem *item = new QListWidgetItem(tr("HBCI import"));
  item->setData(Qt::UserRole, QString(":/plugins/plugins/hbci.js"));
  ui->scripts->addItem(item);
}

RunScriptDialog::~RunScriptDialog()
{
  delete ui;
}

QString RunScriptDialog::scriptFile() const
{
  QList<QListWidgetItem*> items = ui->scripts->selectedItems();
  if (items.isEmpty())
    return QString{};
  return items.at(0)->data(Qt::UserRole).toString();
}
