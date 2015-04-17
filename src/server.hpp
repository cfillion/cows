#ifndef SERVER_HPP
#define SERVER_HPP

#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <set>
#include <string>

// class Command;
class Peer;
// class Room;

typedef std::shared_ptr<Peer> PeerPtr;

class Server
{
public:
  Server();

  // void execute(const Command &command) const;
  //
  // QList<Peer *> peers() const { return m_peers; }
  // QList<Peer *> findPeers(const QString &search) const;
  //
  // QList<Room *> rooms() const { return m_rooms.values(); }
  // Room *findRoom(const QString &name) const;

  bool run(const std::string &host, const std::string &port);
  void stop();

private:
  boost::asio::io_service m_io;
  boost::asio::ip::tcp::acceptor m_acceptor;
  boost::asio::ip::tcp::socket m_next_socket;

  std::set<PeerPtr> m_peers;

  void accept_client();
  void create_peer();
  void destroy_peer(PeerPtr peer);

//   QMap<QString, Room *> m_rooms;
};

#endif
