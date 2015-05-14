#ifndef PLUGINS_H
#define PLUGINS_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QJSValue>
#include <QVector>
#include <QJSEngine>

#include <functional>

#include "statement.h"

class QScriptContext;

class Plugin : public QJSEngine
{
  Q_OBJECT
public:
  explicit Plugin(QString &fileName);
  virtual ~Plugin();
  void addParameter(QString name, QString value);
  void run();
  QJSValue getParameter(const QString &name) const;
  void setCallBack(std::function<void(void)> callback);
  void startScript();
  void scriptFinished();

signals:
  void message(QString &msg);

protected:
  template<class T>
  void setObject(const QString &name)
  {
    globalObject().setProperty(name, newQObject(new T(this)));
  }

protected:
  const QString m_fileName;
  QMap<QString, QJSValue> m_parameters;

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
