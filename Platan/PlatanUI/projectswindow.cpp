#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QString>
#include <QVector>

#include "projectswindow.h"
#include "ui_projectswindow.h"
#include "mainapplication.h"
#include <newprojectdialog.h>
#include <statements.h>

ProjectsWindow::ProjectsWindow(MainApplication *application, Statements &statements, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ProjectsWindow),
    statements(statements),
    application(application)
{
    ui->setupUi(this);
    ListModel = new QStandardItemModel();
    ui->listView->setModel(ListModel);
    projectSelection = ui->listView->selectionModel();
    connect(projectSelection, SIGNAL(selectionChanged(QItemSelection,QItemSelection)), this, SLOT(onSelectedProjectChanged()));


    for (auto action : ui->toolBar->actions())
    {
        action->setStatusTip(action->toolTip());
    }
}

ProjectsWindow::~ProjectsWindow()
{
    delete ui;
}

void ProjectsWindow::addProjetPath(QString fileName)
{
    ListModel->appendRow(new QStandardItem(fileName));
}

bool ProjectsWindow::openProject(QString fileName)
{
    if(!application->OpenProject(fileName))
    {
        QMessageBox::warning(this, tr("Failed to open file"),
                             tr("The selected file does not seem to be a valid Platan project"));
        return false;
    }
    return true;
}

void ProjectsWindow::on_actionNew_Project_triggered()
{
    NewProjectDialog d;

    if (d.exec() != QDialog::Accepted)
        return;

    QString fileName = d.fileName();
    if (!fileName.isEmpty())
    {
        application->create(fileName, d.countryCode());
        if(!openProject(fileName))
                return;
        addProjetPath(fileName);
        close();
    }
}

QString ProjectsWindow::GetProjectPath(int row) const
{
    return ListModel->item(row)->text();
}

int ProjectsWindow::GetSelectedProjectRow()
{
    QModelIndexList selectedIndexes = projectSelection->selectedIndexes();
    if (selectedIndexes.isEmpty())
        return -1;
    return selectedIndexes.at(0).row();
}

QString ProjectsWindow::GetSelectedProjectPath()
{
    int row = GetSelectedProjectRow();
    if (row == -1)
        return QString();
    return GetProjectPath(row);

}

void ProjectsWindow::on_openButton_clicked()
{
    if(!openProject(GetSelectedProjectPath()))
        return;
    close();
}

void ProjectsWindow::on_actionLoad_Project_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open existing project"), "",
                                                    tr("Platan files (*.plat)"));
    if (!fileName.isEmpty())
    {
        if(!openProject(fileName))
            return;
        addProjetPath(fileName);
        close();
    }
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
        addProjetPath(path);
}

void ProjectsWindow::closeEvent(QCloseEvent *event)
{
    application->SaveProjectPaths(ProjectPaths());
}

void ProjectsWindow::on_actionRemove_triggered()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Delete project"),
                                  tr("Would you also like to remove the project file from hard disk?"),
                                  QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
    if (reply == QMessageBox::Cancel)
        return;
    if (reply == QMessageBox::Yes)
    {
        QFile file(GetSelectedProjectPath());
        file.remove();
    }
    ListModel->removeRow(GetSelectedProjectRow());
}

void ProjectsWindow::onSelectedProjectChanged()
{
    bool enabled = !GetSelectedProjectPath().isEmpty();
    ui->openButton->setEnabled(enabled);
    ui->actionRemove->setEnabled(enabled);
}
