#include "jsobject.h"

JSObject::JSObject(Plugin *parent) : QObject(parent)
{
  m_plugin = parent;
}

