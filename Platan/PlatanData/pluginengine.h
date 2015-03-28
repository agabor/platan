#ifndef PLUGINS_H
#define PLUGINS_H

#include <QObject>
#include <QMap>
#include <QString>
#include <QScriptValue>

class QScriptEngine;

class Plugin{
  friend class PluginEngine;

public:
  void addParameter(QString name, QString value);
private:
  Plugin(QString &fileName, PluginEngine *engine);
  const QString m_fileName;
  PluginEngine * const m_engine;
  QMap<QString, QScriptValue> m_parameters;
};

class PluginEngine : QObject
{
  Q_OBJECT
  friend class Plugin;

public:
  Plugin createScript(QString &filename);
  void runScript(Plugin &script);
private:
  void setupEngine();
  QScriptEngine *engine = nullptr;
};

#endif
