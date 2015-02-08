#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class QWebSocketServer;

class Peer;

class Server : public QObject
{
  Q_OBJECT

public:
  Server(QObject *parent = 0);
  ~Server();

public Q_SLOTS:
  bool open(const QString &address);

private Q_SLOTS:
  void newPeer();

private:
  QWebSocketServer *m_server;
  QList<Peer *> m_peers;
};

#endif
