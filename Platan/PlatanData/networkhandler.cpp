#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QByteArray>

#include "networkhandler.h"

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

void NetworkHandler::post(QString &url, QString &data)
{
  QNetworkAccessManager *manager = new QNetworkAccessManager(this);
  connect(manager, SIGNAL(finished(QNetworkReply*)),
          this, SLOT(replyFinished(QNetworkReply*)));

  QByteArray postData;
  postData.append(base64_encode(data));

  QNetworkRequest req{(QUrl(url))};
  req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("text/xml")));
  req.setHeader(QNetworkRequest::ContentLengthHeader, QVariant(qulonglong(postData.size())));

  manager->post(req, postData);
  qDebug() << "POST";
}

void NetworkHandler::setCallback(std::function<void (QString)> callback)
{
  m_callback = callback;
}

NetworkHandler::NetworkHandler()
{
  qDebug() << "NetworkHandler create";
}

NetworkHandler::~NetworkHandler()
{
  qDebug() << "NetworkHandler destroy";

}

void NetworkHandler::replyFinished(QNetworkReply *reply)
{
  qDebug() << "reply";
  QString str = base64_decode(QString(reply->readAll()));
  m_callback(str);
}

