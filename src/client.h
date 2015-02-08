#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

#include <QAbstractSocket>

class QWebSocket;

class Client : public QObject
{
  Q_OBJECT

public:
  Client(QObject *parent = 0);

  bool open(const QString &url);

private Q_SLOTS:
  void connected();
  void disconnected();
  void failed(const QAbstractSocket::SocketError error);

private:
  QWebSocket *m_socket;
};

#endif
