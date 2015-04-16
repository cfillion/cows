#ifndef SERVER_H
#define SERVER_H

#include <boost/asio/io_service.hpp>
#include <string>

// class Command;
// class Peer;
// class Room;

class Server
{
public:
  Server();
  virtual ~Server();

  // void execute(const Command &command) const;
  //
  // QList<Peer *> peers() const { return m_peers; }
  // QList<Peer *> findPeers(const QString &search) const;
  //
  // QList<Room *> rooms() const { return m_rooms.values(); }
  // Room *findRoom(const QString &name) const;

  bool run(const std::string &listen_addr);
  void stop();

private:
  boost::asio::io_service m_io;

// private Q_SLOTS:
//   void createPeer();
//   void destroyPeer();
//
// private:
//   QList<Peer *> m_peers;
//   QMap<QString, Room *> m_rooms;
//
//   QWebSocketServer *m_server;
};

#endif
