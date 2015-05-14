#ifndef NETWORKHANDLER_H
#define NETWORKHANDLER_H

#include <QObject>
#include <QString>
#include <QJSValue>

#include <functional>

#include "jsobject.h"
#include "pluginengine.h"

class QNetworkAccessManager;
class QNetworkReply;

class JSNetworkHandler : public JSObject
{
  Q_OBJECT
public:
  explicit JSNetworkHandler(Plugin *parent);
  ~JSNetworkHandler();

public slots:
  void request(const QString &txt, const QString &url, QJSValue callback);

private slots:
  void replyFinished(QNetworkReply* reply);

private:
  void post(const QString &url, const QString &data);
  void setCallback(std::function<void(QString)> callback);

  QString m_url;
  std::function<void (QString)> m_callback;
};

#endif // NETWORKHANDLER_H
