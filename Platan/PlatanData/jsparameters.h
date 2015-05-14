#ifndef JSPARAMETERS_H
#define JSPARAMETERS_H

#include <QObject>
#include <QJSValue>
#include "jsobject.h"

class JSParameters : public JSObject
{
  Q_OBJECT
public:
  explicit JSParameters(Plugin *parent);
public slots:
  QJSValue get(QString name);
};

#endif // JSPARAMETERS_H
