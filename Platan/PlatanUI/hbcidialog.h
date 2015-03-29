#ifndef HBCIDIALOG_H
#define HBCIDIALOG_H

#include <QDialog>

namespace Ui {
class HBCIDialog;
}

class HBCIDialog : public QDialog
{
  Q_OBJECT

public:
  explicit HBCIDialog(QWidget *parent = 0);
  ~HBCIDialog();
  QString getURL() const;
  QString getBLZ() const;
  QString getUserID() const;
  QString getPin() const;

private slots:
  void setHBCIURL();
private:
  Ui::HBCIDialog *ui;
};

#endif // HBCIDIALOG_H
