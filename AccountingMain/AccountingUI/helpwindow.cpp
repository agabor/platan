#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HelpWindow),
    shown{false}
{
    ui->setupUi(this);
    AddHeader(1, tr("Platan Python scripting help"));
    AddParagraph(tr("This is the documentation of the Platan Python API. You can run any valid \
Python 3.2 script here (altough you can not use GUI). Additionally Platan \
provides the following functions. To use these you must first import the \
Platan module, by typing \"import Platan\""));
}

void HelpWindow::AddHeader(int headerRank, QString header)
{
    ui->textEdit->insertHtml(QString("<h%1>%2</h%1><br>").arg(headerRank).arg(header));
}

void HelpWindow::AddParagraph(QString text)
{
    ui->textEdit->insertHtml(QString("<p>%1</p><br>").arg(text));
}

void HelpWindow::closeEvent(QCloseEvent *event)
{
    shown = false;
}

void HelpWindow::showEvent(QShowEvent *event)
{
    shown = true;
}

HelpWindow::~HelpWindow()
{
    delete ui;
}

void HelpWindow::AddFunctionDoc(QString name, QString doc)
{
    AddHeader(2, name);
    AddParagraph(doc);
}

bool HelpWindow::isShown() const
{
    return shown;
}
