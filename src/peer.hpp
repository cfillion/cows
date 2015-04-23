#ifndef PEER_HPP
#define PEER_HPP

#include <boost/asio/ip/tcp.hpp>
#include <boost/signals2.hpp>

// #include "command.h"
#include "handshake.hpp"

// class Room;
class Server;
class Peer;

typedef std::shared_ptr<Peer> PeerPtr;

class Peer : public std::enable_shared_from_this<Peer>
{
public:
  Peer(boost::asio::ip::tcp::socket socket, Server *server);

  const std::string &uuid() const { return m_uuid; }
  std::string ip_address() const { return m_ip_address; }
  int remote_port() const { return m_remote_port; }
  Server *server() const { return m_server; }

  void start();
  void kill();

//   void send(const CommandList &commands);
//   void send(const Command &commands);
//   void send(const QString &cmdName, const QString &roomName,
//     const QStringList &args = QStringList());

  boost::signals2::signal<void(PeerPtr)> on_disconnect;

private:
  enum SocketState
  {
    Handshake,
    DataTransfer,
  };

  void read();
  void shake_hands(boost::system::error_code ec, std::size_t bytes);

  void write();

  boost::asio::ip::tcp::socket m_socket;
  Server *m_server;

  SocketState m_state;
  std::string m_uuid;
  std::string m_ip_address;
  int m_remote_port;

  std::array<char, 8192> m_buffer;
  ::Handshake m_handshake;

// private Q_SLOTS:
//   void messageReceived(const QString &message);
};

#endif
