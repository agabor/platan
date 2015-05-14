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
  void addStatement(float amount, const QString &type, QJSValue date, const QString &payee, const QString &payeeAccount, const QString &description);
};

#endif // JSAPI_H
