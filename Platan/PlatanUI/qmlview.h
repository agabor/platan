#ifndef QMLVIEW_H
#define QMLVIEW_H

#include <QDialog>

namespace Ui {
class QMLView;
}

class QMLView : public QDialog
{
  Q_OBJECT

public:
  explicit QMLView(QWidget *parent = 0);
  ~QMLView();

private:
  Ui::QMLView *ui;
};

#endif // QMLVIEW_H
