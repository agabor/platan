#ifndef PROJECTSWINDOW_H
#define PROJECTSWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QString>
#include <QVector>
#include "statements.h"

class MainApplication;

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
private slots:
    void on_actionNew_Project_triggered();

    void on_pushButton_clicked();

    void on_actionLoad_Project_triggered();

private:
    Ui::ProjectsWindow *ui;
    Statements &statements;
    QStandardItemModel* ListModel;
    MainApplication* application;
    QVector<QString> ProjectPaths() const;
protected:
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);
};

#endif // PROJECTSWINDOW_H
