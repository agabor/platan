#ifndef PROJECTSWINDOW_H
#define PROJECTSWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QString>
#include <QVector>
#include "statements.h"

class MainApplication;
class QItemSelectionModel;

namespace Ui {
class ProjectsWindow;
}

class ProjectsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjectsWindow(MainApplication* application, Statements &statements, QWidget *parent = 0);
    ~ProjectsWindow();

    QString GetSelectedProjectPath();
    void AddProjetPath(QString fileName);
    QString GetProjectPath(int row) const;
    int GetSelectedProjectRow();
private slots:
    void on_actionNew_Project_triggered();

    void on_openButton_clicked();

    void on_actionLoad_Project_triggered();

    void on_actionRemove_triggered();

    void onSelectedProjectChanged();

private:
    Ui::ProjectsWindow *ui;
    Statements &statements;
    QStandardItemModel* ListModel;
    MainApplication* application;
    QVector<QString> ProjectPaths() const;
    QItemSelectionModel *projectSelection;
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // PROJECTSWINDOW_H
