#include "server.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/asio.hpp>
#include <boost/format.hpp>

#include "logging.hpp"
// #include "names.h"
// #include "peer.h"
// #include "room.h"

LOG_MODULE("server");

Server::Server()
{
  // m_server = new QWebSocketServer(qApp->applicationName(),
  //   QWebSocketServer::NonSecureMode, this);


  // connect(m_server, &QWebSocketServer::newConnection, this, &Server::createPeer);
  // connect(m_server, &QWebSocketServer::closed, qApp, &QCoreApplication::quit);
  //
  // m_rooms["#hello_world"] = new Room("#hello_world", this);
  // m_rooms["#42"] = new Room("#42", this);
}

Server::~Server()
{
  // m_server->close();
}

bool Server::run(const std::string &listen_addr)
{
  using namespace boost;
  using namespace asio::ip;

  std::vector<std::string> addr_parts;
  split(addr_parts, listen_addr, is_any_of(":"));

  if(addr_parts.size() != 2) {
    LOG_FATAL(
      format("invalid address: '%s'. format must be HOST:PORT") % listen_addr
    );

    return false;
  }

  try {
    LOG_INFO("initializing...");

    tcp::resolver resolver(m_io);
    const tcp::resolver::query query(addr_parts[0], addr_parts[1]);
    const tcp::endpoint endpoint = *resolver.resolve(query);
    tcp::acceptor acceptor(m_io, endpoint);

    // endpoint doesn't know the effective port if the OS assigned a random one
    // however acceptor.local_endpoint() does

    LOG_INFO(
      format("initialization completed. clients are now accepted on ws://%s:%d/")
      % acceptor.local_endpoint().address() % acceptor.local_endpoint().port()
    );
  }
  catch(const system::system_error &err) {
    LOG_FATAL(
      format("can not listen on '%s': %s") % listen_addr % err.what()
    );
    return false;
  }

  m_io.run();

  return true;
}

void Server::stop()
{
  m_io.stop();
}

// void Server::createPeer()
// {
//   LOG_INFO(QString("registering new peer (%1)").arg(m_peers.count() + 1));
//
//   Peer *peer = new Peer(m_server->nextPendingConnection(), this);
//   connect(peer, &Peer::disconnected, this, &Server::destroyPeer);
//
//   m_peers << peer;
// }
//
// void Server::destroyPeer()
// {
//   Peer *peer = qobject_cast<Peer *>(sender());
//
//   LOG_INFO(QString("unregistering peer %1").arg(peer->uuid().toString()));
//
//   m_peers.removeAll(peer);
//   peer->deleteLater();
// }
//
// void Server::execute(const Command &command) const
// {
//   const QString name = command.name();
//
//   if(Names::COMMANDS.count(name) == 0) {
//     command.reply(Errno::COMMAND_NOT_FOUND);
//     return;
//   }
//
//   Names::COMMANDS[name](command);
// }
//
// QList<Peer *> Server::findPeers(const QString &search) const
// {
//   QList<Peer *> matches;
//
//   const QUuid uuid(search);
//
//   Q_FOREACH(Peer *peer, m_peers) {
//     if(peer->uuid() == uuid)
//       matches << peer;
//   }
//
//   return matches;
// }
//
// Room *Server::findRoom(const QString &name) const
// {
//   return m_rooms.value(name);
// }
