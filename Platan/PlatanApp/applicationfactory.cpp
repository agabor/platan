#include <applicationfactory.h>
#include <rulemapper.h>
#include <statementmapper.h>
#include <mainapplication.h>
#include <statements.h>
#include <rules.h>
#include <countrymapper.h>
#include <pythonide.h>
#include <pythonapi.h>
#include <sqlitedb.h>
#include <applicationdb.h>

void ApplicationFactory::construct(int &argc, char *argv[])
{
    applicationDB.reset(new ApplicationDB);
    project_db.reset(new SQLiteDB(getProjectDBSchema(), "pro"));
    statements.reset(new Statements(*project_db));
    rules.reset(new Rules(*project_db));
    viewModel.reset(new ViewModel(*statements));

    mainApplication.reset(new MainApplication(argc, argv, *applicationDB, *project_db));

    mainWindow.reset(new MainWindow(*statements, *rules, *viewModel));
    mainApplication->setMainWindow(mainWindow);
    mainWindow->setPythonIDE(pythonConsole);
    pythonConsole.reset(new PythonIDE());
    pythonConsole->setMainWindow(mainWindow.get());
    PythonAPI::setPythonIDE(pythonConsole.get());
}

void ApplicationFactory::init()
{
    applicationDB->init();
}

MainApplication &ApplicationFactory::application()
{
    return *mainApplication.get();
}

DataBaseSchema ApplicationFactory::getProjectDBSchema()
{
    DataBaseSchema schema;
    schema.addTable(RuleMapper::getStructure());
    schema.addTable(StatementMapper::getStructure());

    return schema;
}

