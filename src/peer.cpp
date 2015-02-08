#include "peer.h"

#include <QWebSocket>

#include "logging.h"
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

  LOG_INFO(QString("address=%1:%2 uuid=%3")
    .arg(m_address.toString()).arg(m_port).arg(m_uuid.toString()));
}

void Peer::send(const MessageList &messages)
{
  LOG_DEBUG(tr("sending %1 messages to %2")
    .arg(messages.count()).arg(m_uuid.toString()));

  const QString serializedMessages = Message::serialize(messages);

  Q_FOREACH(const Message &message, messages)
    LOG_DEBUG(tr("outbound message: %1").arg(message.toString()));

  m_socket->sendTextMessage(serializedMessages);
}

void Peer::send(const Message &message)
{
  send(MessageList() << message);
}

void Peer::send(const QString &command, const QStringList &arguments)
{
  send(Message(command, arguments));
}

void Peer::messageReceived(const QString &serializedMessages)
{
  MessageList messages = Message::unserialize(serializedMessages);

  LOG_DEBUG(tr("received %1 messages from %2")
    .arg(messages.count()).arg(m_uuid.toString()));

  Q_FOREACH(const Message &message, messages) {
    LOG_DEBUG(tr("inbound message: %1").arg(message.toString()));

    // TODO: do something with message
  }

  send("hello");
}
