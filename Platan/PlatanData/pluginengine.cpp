
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

#include <functional>

#include "pluginengine.h"
#include "networkhandler.h"

QScriptValue print(QScriptContext *ctx, QScriptEngine *eng);
QScriptValue networkRequest(QScriptContext *ctx, QScriptEngine *eng);

QScriptValue null()
{
  return QScriptValue(QScriptValue::NullValue);
}

QScriptValue undefined()
{
  return QScriptValue(QScriptValue::UndefinedValue);
}

void PluginEngine::setupEngine()
{
  engine = new QScriptEngine(this);
  engine->globalObject().setProperty("print", engine->newFunction(print));
  engine->globalObject().setProperty("networkRequest", engine->newFunction(networkRequest));
}

Plugin PluginEngine::createScript(QString &filename)
{
  if (!engine)
    setupEngine();

  return Plugin(filename, this);
}

void PluginEngine::runScript(Plugin &script)
{
  if (!engine)
    setupEngine();

  for(auto p : script.m_parameters.keys())
    engine->globalObject().setProperty(p, script.m_parameters.value(p));

  QFile file(script.m_fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Could not open script file.";
		return;
	}
  QTextStream in(&file);
	qDebug() << engine->evaluate(in.readAll()).toString();

  for(auto p : script.m_parameters.keys())
    engine->globalObject().setProperty(p, undefined());
}

QScriptValue print(QScriptContext *ctx, QScriptEngine *eng)
{
	qDebug() << ctx->argument(0).toString();
  return null();
}


QScriptValue networkRequest(QScriptContext *ctx, QScriptEngine *eng)
{
  QString txt = ctx->argument(0).toString();
  QString url = ctx->argument(1).toString();
  QScriptValue callback = ctx->argument(2);
  Q_ASSERT(callback.isFunction());

  auto *handler = new NetworkHandler;

  auto scriptCallBack = [eng, callback, handler](QString data) mutable
  {
    QScriptValueList args;
    args << QScriptValue::NullValue;
    args << QScriptValue(eng, data);
    callback.call(null(), args);
    delete handler;
  };

  handler->setCallback(scriptCallBack);
  handler->post(url, txt);

  return null();
}



void Plugin::addParameter(QString name, QString value)
{
  m_parameters.insert(name, QScriptValue(m_engine->engine, value));
}

Plugin::Plugin(QString &fileName, PluginEngine *engine) : m_fileName(fileName), m_engine(engine)
{

}
