#ifndef JSLOGGER_H
#define JSLOGGER_H

#include <QObject>
#include "jsobject.h"
#include "pluginengine.h"

class JSLogger : public JSObject
{
  Q_OBJECT
public:
  explicit JSLogger(Plugin *parent = 0);
public slots:
  void print(QString msg);
};

#endif // JSLOGGER_H
