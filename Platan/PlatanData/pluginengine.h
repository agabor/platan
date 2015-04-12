#ifndef PLUGINS_H
#define PLUGINS_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QScriptValue>
#include <QVector>
#include <QScriptEngine>

#include <functional>

#include "statement.h"

class QScriptContext;

class Plugin : public QScriptEngine
{
  Q_OBJECT
public:
  explicit Plugin(QString &fileName);
  void addParameter(QString name, QString value);
  void run();
  QScriptValue getParameter(QString &name) const;
  void setCallBack(std::function<void(void)> callback);
  std::function<void (QScriptValue &, QScriptValueList &)> createPluginCallBack(QScriptValue& function);

signals:
  void message(QString &msg);

protected:
  QScriptValue getParameter(QScriptContext *ctx, QScriptEngine *eng);
  void startScript();
  void scriptFinished();

protected:
  const QString m_fileName;
  QMap<QString, QScriptValue> m_parameters;

private:
  int m_script_run_counter;
  std::function<void(void)> m_callback;
};

class ImportPlugin : public Plugin
{
public:
  explicit ImportPlugin(QString &fileName);
  void addStatement(Statement &s);
  QVector<Statement> statements() const;
private:
  QVector<Statement> m_statements;
};

#endif
