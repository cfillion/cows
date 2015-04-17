#include "server.hpp"

#include <boost/format.hpp>

#include "logging.hpp"
#include "names.hpp"
#include "peer.hpp"
// #include "room.h"

using namespace std;
using namespace boost;
using namespace asio::ip;

LOG_MODULE("server");

Server::Server()
  : m_acceptor(m_io), m_next_socket(m_io)
{
  // m_rooms["#hello_world"] = new Room("#hello_world", this);
  // m_rooms["#42"] = new Room("#42", this);
}

bool Server::run(const string &host, const string &port)
{
  LOG_INFO("initializing...");

  try {
    tcp::resolver resolver(m_io);
    const tcp::resolver::query query(host, port);
    const tcp::endpoint endpoint = *resolver.resolve(query);
    m_acceptor = tcp::acceptor(m_io, endpoint);

    accept_client();

    // endpoint doesn't know the effective port if the OS assigned a random one
    // however m_acceptor.local_endpoint() does

    LOG_INFO(
      format("initialization completed. accepting connections on ws://%s:%d/")
      % m_acceptor.local_endpoint().address()
      % m_acceptor.local_endpoint().port()
    );
  }
  catch(const system::system_error &err) {
    LOG_FATAL(format("unable to listen on %s:%s - %s")
      % host % port
      % err.what()
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

void Server::accept_client()
{
  m_acceptor.async_accept(m_next_socket, [this](system::error_code ec)
  {
    if(!ec)
      create_peer();

    accept_client();
  });
}

void Server::create_peer()
{
  LOG_INFO(format("registering new peer in slot %s") % m_peers.size());

  PeerPtr peer = make_shared<Peer>(std::move(m_next_socket), this);
  m_peers.insert(peer);
  peer->on_disconnect.connect(bind(&Server::destroy_peer, this, _1));
  peer->start();
}

void Server::destroy_peer(PeerPtr peer)
{
  LOG_INFO(format("unregistering peer %s") % peer->uuid());

  m_peers.erase(peer);
}

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
