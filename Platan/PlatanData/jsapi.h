#ifndef JSAPI_H
#define JSAPI_H

#include <QObject>
#include "jsobject.h"
#include "pluginengine.h"

class JSAPI : public JSObject
{
  Q_OBJECT
public:
  explicit JSAPI(ImportPlugin *parent = 0);

public slots:
  void addStatement(float amount, QString type, QJSValue date, QString payee, QString payeeAccount, QString description);
};

#endif // JSAPI_H
