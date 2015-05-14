#ifndef JSOBJECT_H
#define JSOBJECT_H

#include <QObject>
#include "pluginengine.h"

class JSObject : public QObject
{
  Q_OBJECT
public:
  explicit JSObject(Plugin *parent);

protected:
  Plugin *m_plugin;
};

#endif // JSOBJECT_H
