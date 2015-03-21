
#include <QScriptEngine>
#include <QScriptValue>
#include <QDebug>
#include <QProcess>
#include <QStringList>
#include <QFile>
#include <QTextStream>

#include "plugins.h"

QScriptValue run(QScriptContext *ctx, QScriptEngine *eng);

QScriptEngine *engine;

void Plugins::setUp()
{
	engine = new QScriptEngine;
	engine->globalObject().setProperty("run", engine->newFunction(run));
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

void Plugins::tearDown()
{
  delete engine;
}
