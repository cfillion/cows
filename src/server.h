#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class QWebSocketServer;

class Command;
class Peer;

class Server : public QObject
{
  Q_OBJECT

public:
  Server(QObject *parent = 0);
  ~Server();

  void execute(const Command &command) const;

  QList<Peer *> peers() const { return m_peers; }
  QList<Peer *> findPeers(const QString &search) const;

public Q_SLOTS:
  bool open(const QString &address);

private Q_SLOTS:
  void createPeer();
  void destroyPeer();

private:
  QList<Peer *> m_peers;
  QWebSocketServer *m_server;
};

#endif
