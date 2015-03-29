#ifndef RUNSCRIPTDIALOG_H
#define RUNSCRIPTDIALOG_H

#include <QDialog>

namespace Ui {
class RunScriptDialog;
}

class RunScriptDialog : public QDialog
{
  Q_OBJECT

public:
  explicit RunScriptDialog(QWidget *parent = 0);
  ~RunScriptDialog();
  QString scriptFile() const;

private:
  Ui::RunScriptDialog *ui;
};

#endif // RUNSCRIPTDIALOG_H
