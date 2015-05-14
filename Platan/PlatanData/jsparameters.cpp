#include "jsparameters.h"


JSParameters::JSParameters(Plugin *parent) : JSObject(parent)
{

}

QJSValue JSParameters::get(QString name)
{
  return m_plugin->getParameter(name);
}
