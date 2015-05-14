#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>

#include "jsnetworkhandler.h"

QString base64_encode(QString string){
    QByteArray ba;
    ba.append(string);
    return ba.toBase64();
}

QString base64_decode(QString string){
    QByteArray ba;
    ba.append(string);
    return QByteArray::fromBase64(ba);
}

void JSNetworkHandler::post(const QString &url, const QString &data)
{
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(replyFinished(QNetworkReply*)));

  QByteArray postData;
  postData.append(base64_encode(data));

  QNetworkRequest req{QUrl{url}};
  req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/xml")));
  req.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(qulonglong(postData.size())));

  manager->post(req, postData);
  qDebug() << "POST";
}

void JSNetworkHandler::setCallback(std::function<void (QString)> callback)
{
  m_callback = callback;
}

JSNetworkHandler::JSNetworkHandler(Plugin *parent) : JSObject(parent)
{
  qDebug() << "NetworkHandler create";
}

JSNetworkHandler::~JSNetworkHandler()
{
  qDebug() << "NetworkHandler destroy";

}

void JSNetworkHandler::request(const QString &txt, const QString &url, QJSValue callback)
{
  auto plugin_callback = m_plugin->createPluginCallBack(callback);

  auto scriptCallBack = [plugin_callback](QString data) mutable
  {
    QJSValueList args;
    args << QJSValue::NullValue;
    args << QJSValue(data);
    plugin_callback(args);
  };

  setCallback(scriptCallBack);
  post(url, txt);
}

void JSNetworkHandler::replyFinished(QNetworkReply *reply)
{
  qDebug() << "reply";
  QString str = base64_decode(QString(reply->readAll()));
  m_callback(str);
}

