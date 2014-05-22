#ifndef PYTHONIDE_H
#define PYTHONIDE_H

#include <QMainWindow>
#include <memory>
#include <QFile>
#include <QString>
#include <QTextCursor>
#include <QByteArray>
#include "helpwindow.h"

namespace Ui{
class PythonIDE;
}

class PythonIDE : public QMainWindow
{
    Q_OBJECT

public:
    explicit PythonIDE(QMainWindow *mainWindow);
    void PushOutput(QString output);
    void PushError(QString output);
    ~PythonIDE();
    QTextCursor GetOutputEndCursor();
private slots:
    void run();

    void textChanged();

    void on_actionRun_triggered();

    void on_actionSave_as_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionHelp_triggered();

    void on_actionTile_windows_triggered();

private:
    Ui::PythonIDE *ui;
    std::unique_ptr<QFile> TryOpenFile(QString fileName, QIODevice::OpenModeFlag flag);
    QString activeFileName;
    QString GetWindowTitle();
    void SetWindowTitle();
    void SetWindowTitleChanged();
    void saveCodeToFile(QString fileName);
    bool text_changed;
    QString saved_code;
    std::unique_ptr<HelpWindow> helpwindow;
    QMainWindow *mainWindow;
    std::unique_ptr<QByteArray> geometry;
    std::unique_ptr<QByteArray> mainWindowGeometry;
    std::unique_ptr<QByteArray> helpWindowGeometry;
    void RestoreWindowGeometries();
    void SaveWindowGeometries();
    void TileWindows();
protected:
     void closeEvent(QCloseEvent *event);
};

#endif // PYTHONIDE_H
