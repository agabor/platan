#ifndef PLUGINOUTPUTDIALOG_H
#define PLUGINOUTPUTDIALOG_H

#include <QDialog>

namespace Ui {
class PluginOutputDialog;
}
class QString;

class PluginOutputDialog : public QDialog
{
  Q_OBJECT

public:
  explicit PluginOutputDialog(QWidget *parent = 0);
  ~PluginOutputDialog();
public slots:
  void showMessage(QString &msg);
private:
  Ui::PluginOutputDialog *ui;
};

#endif // PLUGINOUTPUTDIALOG_H
