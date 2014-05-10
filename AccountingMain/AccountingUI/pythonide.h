#ifndef PYTHONIDE_H
#define PYTHONIDE_H

#include <QMainWindow>
#include <memory>

namespace Ui{
class PythonIDE;
}

class PythonIDE : public QMainWindow
{
    Q_OBJECT

public:
    explicit PythonIDE(QWidget *parent = 0);
    void PushOutput(QString output);
    void PushError(QString output);
    ~PythonIDE();
private slots:
    void run();

    void on_actionRun_triggered();

private:
    Ui::PythonIDE *ui;
};

#endif // PYTHONIDE_H
