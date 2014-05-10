#include "pythonide.h"
#include "ui_pythonide.h"

PythonIDE::PythonIDE(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PythonIDE)
{
    ui->setupUi(this);
}

PythonIDE::~PythonIDE()
{
    delete ui;
}
