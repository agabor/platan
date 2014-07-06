#ifndef APPLICATIONFACTORY_H
#define APPLICATIONFACTORY_H

#include <memory>

#include <mainwindow.h>
#include <pythonide.h>
#include <rules.h>
#include <applicationdb.h>

class MainApplication;
class SQLiteDB;
class DataBaseSchema;
class CountryMapper;
class Statements;
class ViewModel;

class ApplicationFactory
{
public:
    void construct(int &argc, char *argv[]);
    void init();
    MainApplication &application();
private:
    std::unique_ptr<MainApplication> mainApplication;
    std::unique_ptr<SQLiteDB> project_db;
    std::unique_ptr<Statements> statements;
    std::unique_ptr<Rules> rules;
    std::unique_ptr<ViewModel> viewModel;
    std::unique_ptr<ApplicationDB> applicationDB;
    std::shared_ptr<MainWindow> mainWindow;
    std::shared_ptr<PythonIDE> pythonConsole;
    DataBaseSchema getProjectDBSchema();


};

#endif // APPLICATIONFACTORY_H
