#include <QJSEngine>
#include <QJSValue>
#include <QDebug>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QSharedPointer>

#include <functional>

#include "pluginengine.h"
#include "jsnetworkhandler.h"
#include "jsparameters.h"
#include "jslogger.h"
#include "jsapi.h"

void Plugin::addParameter(QString name, QString value)
{
  m_parameters.insert(name, QJSValue(value));
}

void Plugin::run()
{
  startScript();
  QFile file(m_fileName);
  if (!file.open(QIODevice::ReadOnly))
  {
    qDebug() << "Could not open script file.";
    return;
  }

  QTextStream in(&file);
  qDebug() << evaluate(in.readAll()).toString();
  scriptFinished();
}

QJSValue Plugin::getParameter(const QString &name) const
{
  return m_parameters.value(name);
}

void Plugin::setCallBack(std::function<void ()> callback)
{
  m_callback = callback;
}

std::function<void (QJSValueList &)> Plugin::createPluginCallBack(QJSValue &function)
{
  startScript();
  return [this, function](QJSValueList &arguments) mutable
  {
    function.call(arguments);
    scriptFinished();
  };
}

void Plugin::startScript()
{
  ++m_script_run_counter;
}

void Plugin::scriptFinished()
{
  if (--m_script_run_counter == 0)
  {
    if (m_callback)
      m_callback();
  }
}

Plugin::Plugin(QString &fileName) : m_fileName(fileName)
{
  setObject<JSLogger>("log");
  setObject<JSNetworkHandler>("network");
  setObject<JSParameters>("parameters");
  m_callback = nullptr;
  m_script_run_counter = 0;
  qDebug() << "Plugin created";
}

Plugin::~Plugin()
{
  qDebug() << "Plugin destroyed";
}

ImportPlugin::ImportPlugin(QString &fileName) : Plugin(fileName)
{
  globalObject().setProperty("api", newQObject(new JSAPI(this)));
}

void ImportPlugin::addStatement(Statement &s)
{
  m_statements.append(s);
}

QVector<Statement> ImportPlugin::statements() const
{
  return m_statements;
}
