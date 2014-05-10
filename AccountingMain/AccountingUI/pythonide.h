#ifndef PYTHONIDE_H
#define PYTHONIDE_H

#include <QMainWindow>

namespace Ui {
class PythonIDE;
}

class PythonIDE : public QMainWindow
{
    Q_OBJECT

public:
    explicit PythonIDE(QWidget *parent = 0);
    ~PythonIDE();

private:
    Ui::PythonIDE *ui;
};

#endif // PYTHONIDE_H
