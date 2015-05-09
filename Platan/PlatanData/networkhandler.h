#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QObject>
#include <QString>

#include <functional>

class QNetworkAccessManager;
class QNetworkReply;

class NetworkHandler : public QObject
{
  Q_OBJECT
public:
  void post(QString &url, QString &data);
  void setCallback(std::function<void(QString)> callback);
  NetworkHandler();
  ~NetworkHandler();
private slots:
  void replyFinished(QNetworkReply* reply);
private:
  QString m_url;
  std::function<void (QString)> m_callback;
};

#endif // NETWORKHANDLER_H
