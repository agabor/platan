#include <QScriptEngine>
#include <QScriptValue>
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
#include "networkhandler.h"

QScriptValue print(QScriptContext *ctx, QScriptEngine *eng);
QScriptValue networkRequest(QScriptContext *ctx, QScriptEngine *eng);
QScriptValue addStatement(QScriptContext *ctx, QScriptEngine *eng);
QScriptValue getParameter(QScriptContext *ctx, QScriptEngine *eng);

QScriptValue null()
{
  return QScriptValue(QScriptValue::NullValue);
}

QScriptValue undefined()
{
  return QScriptValue(QScriptValue::UndefinedValue);
}

QScriptValue print(QScriptContext *ctx, QScriptEngine *eng)
{
  Q_UNUSED(eng);
	qDebug() << ctx->argument(0).toString();
  return null();
}

QScriptValue networkRequest(QScriptContext *ctx, QScriptEngine *eng)
{
  Plugin* plugin = dynamic_cast<Plugin*>(eng);
  if (!plugin)
  {
    //Should not happen!
    return undefined();
  }

  QString txt = ctx->argument(0).toString();
  QString url = ctx->argument(1).toString();
  auto callback = plugin->createPluginCallBack(ctx->argument(2));

  QSharedPointer<NetworkHandler> handler(new NetworkHandler);

  auto scriptCallBack = [eng, callback, handler, plugin](QString data) mutable
  {
    QScriptValueList args;
    args << QScriptValue::NullValue;
    args << QScriptValue(eng, data);
    callback(null(), args);
  };

  handler->setCallback(scriptCallBack);
  handler->post(url, txt);

  return null();
}

QString getString(QScriptValue &value)
{
  if (value.isUndefined())
    return QString{};
  return value.toString();
}

QScriptValue addStatement(QScriptContext *ctx, QScriptEngine *eng)
{  ImportPlugin* plugin = dynamic_cast<ImportPlugin*>(eng);
   if (!plugin)
   {
     //Should not happen!
     Q_ASSERT(false);
     return undefined();
   }
   Statement s;
   s.amount = ctx->argument(0).toNumber();
   s.type = getString(ctx->argument(1));
   QScriptValue date = ctx->argument(2);
   int y = date.property("y").toInt32();
   int m = date.property("m").toInt32();
   int d = date.property("d").toInt32();
   s.date = SQLiteDate(y, m, d);
   s.payee = getString(ctx->argument(3));
   s.payeeAccount = getString(ctx->argument(4));
   s.description = getString(ctx->argument(5));
   s.category = 0;
   plugin->addStatement(s);
   return null();
}

void Plugin::addParameter(QString name, QString value)
{
  m_parameters.insert(name, QScriptValue(this, value));
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

QScriptValue Plugin::getParameter(QString &name) const
{
  return m_parameters.value(name);
}

void Plugin::setCallBack(std::function<void ()> callback)
{
  m_callback = callback;
}

std::function<void (QScriptValue &, QScriptValueList &)> Plugin::createPluginCallBack(QScriptValue &function)
{
  Q_ASSERT(function.isFunction());
  startScript();
  return [this, function](QScriptValue &thisObject, QScriptValueList &arguments) mutable
  {
    function.call(thisObject, arguments);
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

QScriptValue getParameter(QScriptContext *ctx, QScriptEngine *eng)
{
  Plugin* plugin = dynamic_cast<Plugin*>(eng);
  if (!plugin)
  {
    //Should not happen!
    return undefined();
  }
  QString param_name = ctx->argument(0).toString();
  return plugin->getParameter(param_name);
}

Plugin::Plugin(QString &fileName) : m_fileName(fileName)
{
  globalObject().setProperty("print", newFunction(print));
  globalObject().setProperty("networkRequest", newFunction(networkRequest));
  globalObject().setProperty("getParameter", newFunction(::getParameter));
  m_callback = nullptr;
  m_script_run_counter = 0;
}

ImportPlugin::ImportPlugin(QString &fileName) : Plugin(fileName)
{
  globalObject().setProperty("addStatement", newFunction(::addStatement));
}

void ImportPlugin::addStatement(Statement &s)
{
  m_statements.append(s);
}

QVector<Statement> ImportPlugin::statements() const
{
  return m_statements;
}
