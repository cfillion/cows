#include "peer.hpp"

#include <boost/asio/write.hpp>
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
  : m_socket(std::move(socket)), m_server(server), m_state(Handshake),
    m_uuid(uuids::to_string(uuids::random_generator()()))
{
  // socket is dead. long live m_socket!

  m_ip_address = m_socket.remote_endpoint().address().to_string();
  m_remote_port = m_socket.remote_endpoint().port();

  LOG_DEBUG(format("address=%s:%d uuid=%s")
    % m_ip_address
    % m_remote_port
    % m_uuid
  );
}

void Peer::start()
{
  read();
}

void Peer::kill()
{
  on_disconnect(shared_from_this());
}

void Peer::read()
{
  m_socket.async_read_some(asio::buffer(m_buffer),
    bind(&Peer::shake_hands, this, _1, _2));
}

void Peer::shake_hands(system::error_code ec, std::size_t bytes)
{
  if(ec)
    return kill();

  LOG_DEBUG(format("parsing %d-bytes handshake from %s") % bytes % m_uuid);

  const tribool status = m_handshake.parse(
    m_buffer.begin(), m_buffer.begin() + bytes);

  if(indeterminate(status))
    return read();

  asio::async_write(m_socket, asio::buffer(m_handshake.encode_reply()),
  [&status, this](system::error_code ec, std::size_t)
  {
    if(!status || ec)
      kill();
    else {
      m_state = DataTransfer;
      read();
    }
  });
}

void Peer::write()
{
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
