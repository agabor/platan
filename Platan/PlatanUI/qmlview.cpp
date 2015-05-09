#include "qmlview.h"
#include "ui_qmlview.h"

QMLView::QMLView(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::QMLView)
{
  ui->setupUi(this);
}

QMLView::~QMLView()
{
  delete ui;
}
