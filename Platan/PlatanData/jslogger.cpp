#include <QDebug>

#include "jslogger.h"

JSLogger::JSLogger(Plugin *parent) : JSObject(parent)
{

}

void JSLogger::print(QString msg)
{
  emit m_plugin->message(msg);
  qDebug() << msg;
}

