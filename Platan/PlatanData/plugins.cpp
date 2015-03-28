
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

#include "plugins.h"
#include "networkhandler.h"

QScriptValue run(QScriptContext *ctx, QScriptEngine *eng);
QScriptValue print(QScriptContext *ctx, QScriptEngine *eng);
QScriptValue networkRequest(QScriptContext *ctx, QScriptEngine *eng);

QScriptEngine *engine;

void Plugins::setUp()
{
	engine = new QScriptEngine;
	engine->globalObject().setProperty("run", engine->newFunction(run));
	engine->globalObject().setProperty("print", engine->newFunction(print));
  engine->globalObject().setProperty("networkRequest", engine->newFunction(networkRequest));
}

void Plugins::runScript(QString &fileName)
{
	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "Could not open script file.";
		return;
	}
  QTextStream in(&file);
	qDebug() << engine->evaluate(in.readAll()).toString();
}

QScriptValue run(QScriptContext *ctx, QScriptEngine *eng)
{
	QStringList args;
	args.append(ctx->argument(1).toString());
	QProcess p;
	p.start(ctx->argument(0).toString(), args);
	if (!p.waitForStarted())
		qDebug() << "not started";
	if (!p.waitForFinished())
		qDebug() << "not finished";
  return QString(p.readAllStandardOutput());
}


QScriptValue print(QScriptContext *ctx, QScriptEngine *eng)
{
	qDebug() << ctx->argument(0).toString();
  return QScriptValue::NullValue;
}


QScriptValue networkRequest(QScriptContext *ctx, QScriptEngine *eng)
{
  QString txt = ctx->argument(0).toString();
  QString url = ctx->argument(1).toString();
  QScriptValue callback = ctx->argument(2);
  Q_ASSERT(callback.isFunction());

  auto *handler = new NetworkHandler;

  auto scriptCallBack =[eng, callback](QString data) mutable
  {
    QScriptValueList args;
    args << QScriptValue::NullValue;
    args << QScriptValue(eng, data);
    callback.call(QScriptValue(QScriptValue::NullValue), args);
  };

  handler->setCallback(scriptCallBack);
  handler->post(url, txt);

  return QScriptValue::NullValue;
}

void Plugins::tearDown()
{
  delete engine;
}
