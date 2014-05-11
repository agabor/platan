#ifndef PROJECTSWINDOW_H
#define PROJECTSWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include "statements.h"

namespace Ui {
class ProjectsWindow;
}

class ProjectsWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ProjectsWindow(Statements &statements, QWidget *parent = 0);
    ~ProjectsWindow();

private slots:
    void on_actionNew_Project_triggered();

private:
    Ui::ProjectsWindow *ui;
    Statements &statements;
    QStandardItemModel* ListModel;
};

#endif // PROJECTSWINDOW_H
