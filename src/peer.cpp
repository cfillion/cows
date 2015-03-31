#include "peer.h"

#include <QWebSocket>

#include "logging.h"
#include "room.h"
#include "server.h"

LOG_MODULE("peer");

Peer::Peer(QWebSocket *socket, Server *server)
  : QObject(server), m_socket(socket), m_server(server)
{
  m_uuid = QUuid::createUuid();

  m_address = m_socket->peerAddress();
  m_port = m_socket->peerPort();

  connect(m_socket, &QWebSocket::textMessageReceived,
    this, &Peer::messageReceived);
  connect(m_socket, &QWebSocket::disconnected,
    this, &Peer::disconnected);

  LOG_INFO(QString("address=%1:%2 uuid=%3")
    .arg(m_address.toString()).arg(m_port).arg(m_uuid.toString()));
}

void Peer::send(const CommandList &commands)
{
  LOG_DEBUG(QString("sending %1 command(s) to %2")
    .arg(commands.count()).arg(m_uuid.toString()));

  Q_FOREACH(const Command &command, commands)
    LOG_DEBUG(QString("outbound command: %1").arg(command.toString()));

  const QString message = Command::serialize(commands);
  m_socket->sendTextMessage(message);
}

void Peer::send(const Command &command)
{
  send(CommandList() << command);
}

void Peer::send(const QString &commandName, const QStringList &arguments)
{
  send(Command(commandName, arguments));
}

void Peer::messageReceived(const QString &message)
{
  CommandList commands = Command::unserialize(message, this);

  LOG_DEBUG(QString("%1 commands(s) received from %2")
    .arg(commands.count()).arg(m_uuid.toString()));

  Q_FOREACH(const Command &command, commands) {
    LOG_DEBUG(QString("inbound command: %1").arg(command.toString()));

    m_server->execute(command);
  }
}
