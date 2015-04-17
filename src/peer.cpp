#include "peer.hpp"

#include <boost/format.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "errors.hpp"
#include "logging.hpp"
// #include "room.h"
#include "server.hpp"

using namespace boost;
using namespace asio::ip;

LOG_MODULE("peer");

Peer::Peer(tcp::socket socket, Server *server)
  : m_socket(std::move(socket)), m_server(server),
    m_uuid(uuids::to_string(uuids::random_generator()()))
{
  // socket is no more, long life to m_socket!

  m_ip_address = m_socket.remote_endpoint().address().to_string();
  m_remote_port = m_socket.remote_endpoint().port();

  LOG_INFO(format("address=%s:%d uuid=%s") % m_ip_address % m_remote_port % m_uuid);
}

Peer::~Peer()
{
  LOG_INFO(format("%s destroyed") % m_uuid);
}

void Peer::start()
{
  on_disconnect();
}

// void Peer::send(const CommandList &commands)
// {
//   if(commands.isEmpty())
//     return;
//
//   LOG_DEBUG(QString("sending %1 command(s) to %2")
//     .arg(commands.count()).arg(m_uuid.toString()));
//
//   Q_FOREACH(const Command &command, commands)
//     LOG_DEBUG(QString("outbound command: %1").arg(command.toString()));
//
//   const QString message = Command::serialize(commands);
//   m_socket->sendTextMessage(message);
// }
//
// void Peer::send(const Command &command)
// {
//   send(CommandList() << command);
// }
//
// void Peer::send(const QString &commandName, const QString &roomName,
//   const QStringList &arguments)
// {
//   send(Command(commandName, roomName, arguments));
// }
//
// void Peer::messageReceived(const QString &message)
// {
//   CommandList commands = Command::unserialize(message, this);
//
//   LOG_DEBUG(QString("%1 commands(s) received from %2")
//     .arg(commands.count()).arg(m_uuid.toString()));
//
//   Q_FOREACH(const Command &command, commands) {
//     LOG_DEBUG(QString("inbound command: %1").arg(command.toString()));
//
//     m_server->execute(command);
//   }
// }
