#include <applicationfactory.h>
#include <rulemapper.h>
#include <statementmapper.h>
#include <mainapplication.h>
#include <statements.h>
#include <rules.h>
#include <countrymapper.h>
#include <pythonide.h>
#include <pythonapi.h>

void ApplicationFactory::construct(int &argc, char *argv[])
{
    application_db.reset(new SQLiteDB(getApplicationDBSchema(), "app"));
    project_db.reset(new SQLiteDB(getProjectDBSchema(), "pro"));
    countryMapper.reset(new CountryMapper(*application_db.get()));
    statements.reset(new Statements(*project_db.get()));
    rules.reset(new Rules(*project_db.get()));
    viewModel.reset(new ViewModel(*statements.get()));

    mainApplication.reset(new MainApplication(argc, argv, *countryMapper, *project_db));

    mainWindow.reset(new MainWindow(*statements, *rules, *viewModel));
    mainApplication->setMainWindow(mainWindow);
    mainWindow->setPythonIDE(pythonConsole);
    pythonConsole.reset(new PythonIDE());
    pythonConsole->setMainWindow(mainWindow.get());
    PythonAPI::setPythonIDE(pythonConsole.get());

}

void ApplicationFactory::init()
{
    application_db->setPath("platandata");
    application_db->open();
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

DataBaseSchema ApplicationFactory::getApplicationDBSchema()
{
    DataBaseSchema schema;
    schema.addTable(RuleMapper::getStructureWithCountry());

    TableStructure countries{"countries"};
    countries.addField("ID", SQLType::Integer().PK());
    countries.addField("Code", SQLType::Text());
    schema.addTable(countries);
    return schema;
}
