#include "pythonide.h"
#include <Qsci/qscilexerpython.h>
#include "pythonapi.h"
#include "ui_pythonide.h"

PythonIDE::PythonIDE(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PythonIDE)
{
    ui->setupUi(this);
    ui->code->setLexer(new QsciLexerPython());

    for (auto action : ui->toolBar->actions())
    {
        action->setStatusTip(action->toolTip());
    }
}

void PythonIDE::PushOutput(QString output)
{
    ui->output->insertPlainText(output);
}

void PythonIDE::PushError(QString output)
{
    ui->output->insertHtml(QString("<font color=\"red\">%1</font><br>").arg(output));
}

PythonIDE::~PythonIDE()
{
    delete ui;
}

void PythonIDE::run()
{
    PythonAPI::run(ui->code->text().toStdString());
}

void PythonIDE::on_actionRun_triggered()
{
    run();
}
