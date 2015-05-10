#include "pluginoutputdialog.h"
#include "ui_pluginoutputdialog.h"

#include <QTextCursor>
#include <QString>

PluginOutputDialog::PluginOutputDialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::PluginOutputDialog)
{
  ui->setupUi(this);
  setWindowFlags(Qt::FramelessWindowHint);
  setWindowFlags(Qt::WindowTitleHint);
}

PluginOutputDialog::~PluginOutputDialog()
{
  delete ui;
}

void PluginOutputDialog::showMessage(QString &msg)
{
  ui->output->appendPlainText(msg);
  QTextCursor cursor(ui->output->textCursor());
  cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
  ui->output->setTextCursor(cursor);
}
