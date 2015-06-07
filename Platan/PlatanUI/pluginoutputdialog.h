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
  void enableFinishButton();
private slots:
  void on_finishButton_clicked();

private:
  Ui::PluginOutputDialog *ui;
};

#endif // PLUGINOUTPUTDIALOG_H
