#include "projectswindow.h"
#include "ui_projectswindow.h"
#include <QFileDialog>
#include "mainapplication.h"

ProjectsWindow::ProjectsWindow(MainApplication *application, Statements &statements, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProjectsWindow),
    statements(statements),
    application(application)
{
    ui->setupUi(this);
    ListModel = new QStandardItemModel();
    ui->listView->setModel(ListModel);
}

ProjectsWindow::~ProjectsWindow()
{
    delete ui;
}

void ProjectsWindow::AddProjetPath(QString fileName)
{
    ListModel->appendRow(new QStandardItem(fileName));
}

void ProjectsWindow::on_actionNew_Project_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("New project"), "",
                                                    tr("Platan files (*.plat)"));
    if (!fileName.isEmpty())
    {
        statements.New(fileName);
        application->OpenProject(fileName);
    }
    AddProjetPath(fileName);
    close();
}

QString ProjectsWindow::GetProjectPath(int row) const
{
    return ListModel->item(row)->text();
}

QString ProjectsWindow::GetSelectedProjectPath()
{
    QModelIndex idx = ui->listView->selectionModel()->selectedIndexes().at(0);

    return GetProjectPath(idx.row());
}

void ProjectsWindow::on_pushButton_clicked()
{
    QString project_path = GetSelectedProjectPath();
    application->OpenProject(project_path);
}

void ProjectsWindow::on_actionLoad_Project_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open project"), "",
                                                    tr("Platan files (*.plat)"));
    if (!fileName.isEmpty())
        application->OpenProject(fileName);
    AddProjetPath(fileName);
    close();
}

QVector<QString> ProjectsWindow::ProjectPaths() const
{
   int count = ListModel->rowCount();
   QVector<QString> result;
   for (int i = 0; i < count; ++i)
       result.push_back(GetProjectPath(i));
   return result;
}

void ProjectsWindow::showEvent(QShowEvent *event)
{
    for(auto path : application->RecentProjects())
        AddProjetPath(path);
}

void ProjectsWindow::closeEvent(QCloseEvent *event)
{
    application->SaveProjectPaths(ProjectPaths());
}
