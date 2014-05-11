#include "helpwindow.h"
#include "ui_helpwindow.h"

HelpWindow::HelpWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::HelpWindow)
{
    ui->setupUi(this);
    AddHeader(1, tr("Platan Python scripting help"));
    AddParagraph(tr("This is the documentation of the Platan Python API."));
}

void HelpWindow::AddHeader(int headerRank, QString header)
{
    ui->textEdit->insertHtml(QString("<h%1>%2</h%1><br>").arg(headerRank).arg(header));
}

void HelpWindow::AddParagraph(QString text)
{
    ui->textEdit->insertHtml(QString("<p>%1</p><br>").arg(text));
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
