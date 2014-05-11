#include "projectswindow.h"
#include "ui_projectswindow.h"
#include <QFileDialog>

ProjectsWindow::ProjectsWindow(Statements &statements, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProjectsWindow),
    statements(statements)
{
    ui->setupUi(this);
    ListModel = new QStandardItemModel();
    ui->listView->setModel(ListModel);
}

ProjectsWindow::~ProjectsWindow()
{
    delete ui;
}

void ProjectsWindow::on_actionNew_Project_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("New project"), "",
                                                    tr("Platan files (*.plat)"));
    if (!fileName.isEmpty())
        statements.New(fileName);
    ListModel->appendRow(new QStandardItem(fileName));
}
