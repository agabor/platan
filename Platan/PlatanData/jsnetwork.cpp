#include <QSharedPointer>

#include "jsnetwork.h"
#include "networkhandler.h"


JSNetwork::JSNetwork(Plugin *parent) : JSObject(parent)
{

}

void JSNetwork::request(QString txt, QString url, QJSValue callback)
{
  QSharedPointer<NetworkHandler> handler(new NetworkHandler);

  m_plugin->startScript();
  auto networkCallBack = [callback, this,handler](QString data) mutable
  {
    QJSValueList args;
    args << QJSValue::NullValue;
    args << QJSValue(data);
    callback.call(args);
    m_plugin->scriptFinished();
  };

  handler->setCallback(networkCallBack);
  handler->post(url, txt);
}

