#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QByteArray>
#include <QDesktopWidget>
#include <QFile>
#include <QTextCursor>

#include <utility>

#include "pythonide.h"
#include <Qsci/qscilexerpython.h>
#include "pythonapi.h"
#include "ui_pythonide.h"
#include "helpwindow.h"

using namespace std;

PythonIDE::PythonIDE(QMainWindow *mainWindow) :
    QMainWindow(nullptr),
    ui(new Ui::PythonIDE),
    helpwindow{new HelpWindow},
    mainWindow{mainWindow},
    geometry{nullptr}
{
    ui->setupUi(this);
    ui->code->setLexer(new QsciLexerPython());
    ui->code->setMarginLineNumbers(1, true);

    for (auto action : ui->toolBar->actions())
    {
        action->setStatusTip(action->toolTip());
    }

    connect(ui->code, SIGNAL(textChanged()), this, SLOT(textChanged()));
    SetWindowTitle();
#ifdef PYTHON_API
    for(pair<QString, QString> doc : PythonAPI::GetFunctionDocs())
    {
        helpwindow->AddFunctionDoc(doc.first, doc.second);
    }
#endif
}

QTextCursor PythonIDE::GetOutputEndCursor()
{
    auto cursor = ui->output->textCursor();
    cursor.movePosition(QTextCursor::End);

    return cursor;
}

void PythonIDE::PushOutput(QString output)
{
    GetOutputEndCursor().insertText(output);
}

void PythonIDE::PushError(QString output)
{
    GetOutputEndCursor().insertHtml(QString("<font color=\"red\">%1</font><br>").arg(output));
}

PythonIDE::~PythonIDE()
{
    delete ui;
}

void PythonIDE::run()
{
#ifdef PYTHON_API
    PythonAPI::run(ui->code->text().toStdString());
#endif
}

void PythonIDE::textChanged()
{
    text_changed = ui->code->text() != saved_code;
    if (text_changed)
        SetWindowTitleChanged();
    else
        SetWindowTitle();
}

void PythonIDE::on_actionRun_triggered()
{
    run();
}

std::unique_ptr<QFile> PythonIDE::TryOpenFile(QString fileName, QIODevice::OpenModeFlag flag)
{
    std::unique_ptr<QFile> file(new QFile(fileName));
    if (!file->open(flag)) {
        QMessageBox::information(this, tr("Unable to open file"),
                                 file->errorString());

        file.reset();
    }
    return file;
}

QString PythonIDE::GetWindowTitle()
{
    QString fileName = activeFileName;
    if (fileName.isEmpty())
        fileName = tr("Untitled");
    return tr("Python Script - %1").arg(fileName);
}

void PythonIDE::SetWindowTitle()
{
   setWindowTitle(GetWindowTitle());
}

void PythonIDE::SetWindowTitleChanged()
{
   setWindowTitle(GetWindowTitle() + " *");
}

void PythonIDE::saveCodeToFile(QString fileName)
{
    std::unique_ptr<QFile>  file = TryOpenFile(fileName, QIODevice::WriteOnly);

    if (file.get() == nullptr)
        return;

    QTextStream output(file.get());
    output << ui->code->text();
    saved_code = ui->code->text();
    text_changed = false;
}

void PythonIDE::closeEvent(QCloseEvent *event)
{
    if (!text_changed)
        return;
    auto reply = QMessageBox::question(this, tr("Save Changes"),
                                       tr("Do you want to save your changes?"),
                                       QMessageBox::Yes|QMessageBox::No);
    if (reply != QMessageBox::Yes)
        return;

    on_actionSave_triggered();
}

void PythonIDE::on_actionSave_as_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save script"), "",
                                                    tr("Python script (*.py)"));
    if (fileName.isEmpty())
        return;
    saveCodeToFile(fileName);
    activeFileName = fileName;
    SetWindowTitle();
}

void PythonIDE::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Load script"), "",
                                                    tr("Python script (*.py)"));

    if (fileName.isEmpty())
        return;

    std::unique_ptr<QFile>  file = TryOpenFile(fileName, QIODevice::ReadOnly);

    if (file.get() == nullptr)
        return;

    QTextStream input(file.get());
    ui->code->setText(input.readAll());
    activeFileName = fileName;
    SetWindowTitle();
    saved_code = ui->code->text();
    text_changed = false;
}

void PythonIDE::on_actionSave_triggered()
{
    if (activeFileName.isEmpty())
        on_actionSave_as_triggered();
    else
        saveCodeToFile(activeFileName);
}

void PythonIDE::on_actionHelp_triggered()
{
    helpwindow->show();
}

void PythonIDE::RestoreWindowGeometries()
{
    if (mainWindowGeometry.get() != nullptr)
        mainWindow->restoreGeometry(*mainWindowGeometry.get());
    if (geometry.get() != nullptr)
        restoreGeometry(*geometry.get());
    if (helpWindowGeometry.get() != nullptr)
        helpwindow->restoreGeometry(*helpWindowGeometry.get());
}

void PythonIDE::SaveWindowGeometries()
{
    mainWindowGeometry.reset(new QByteArray(mainWindow->saveGeometry()));
    geometry.reset(new QByteArray(saveGeometry()));
    helpWindowGeometry.reset(new QByteArray(helpwindow->saveGeometry()));
}

void PythonIDE::TileWindows()
{
    QWidget *desk = QApplication::desktop()->screen(QApplication::desktop()->primaryScreen());
    mainWindow->resize(desk->width()/2, desk->height());
    mainWindow->move(0,0);
    if(helpwindow->isShown())
    {
        move(mainWindow->width(),0);
        resize(desk->width() - mainWindow->width(), desk->height() / 2);
        helpwindow->move(mainWindow->width(),desk->height() / 2);
        helpwindow->resize(desk->width() - mainWindow->width(), desk->height() / 2);
    } else
    {
        move(mainWindow->width(),0);
        resize(desk->width() - mainWindow->width(), desk->height());
    }
}

void PythonIDE::on_actionTile_windows_triggered()
{
    if (ui->actionTile_windows->isChecked())
    {
        SaveWindowGeometries();
        TileWindows();
    } else
    {
        RestoreWindowGeometries();
    }
}
