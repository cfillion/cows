#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QMap>

class QWebSocketServer;

class Command;
class Peer;
class Room;

class Server : public QObject
{
  Q_OBJECT

public:
  Server(QObject *parent = 0);
  ~Server();

  void execute(const Command &command) const;

  QList<Peer *> peers() const { return m_peers; }
  QList<Peer *> findPeers(const QString &search) const;

  QList<Room *> rooms() const { return m_rooms.values(); }
  Room *findRoom(const QString &name) const;

public Q_SLOTS:
  bool open(const QString &address);

private Q_SLOTS:
  void createPeer();
  void destroyPeer();

private:
  QList<Peer *> m_peers;
  QMap<QString, Room *> m_rooms;

  QWebSocketServer *m_server;
};

#endif
