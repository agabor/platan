#ifndef JSNETWORK_H
#define JSNETWORK_H

#include "jsobject.h"
#include "pluginengine.h"

class JSNetwork  : public JSObject
{
  Q_OBJECT
public:
  explicit JSNetwork(Plugin *parent);

public slots:
  void request(QString txt, QString url, QJSValue callback);
};

#endif // JSNETWORK_H
