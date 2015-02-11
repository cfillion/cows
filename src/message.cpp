#include "message.h"

#include "logging.h"
#include "peer.h"

LOG_MODULE("message");

static QChar MESSAGE_SEPARATOR = '\x1E';
static QChar PART_SEPARATOR = '\x1F';

QString Message::serialize(const MessageList &messages)
{
  QStringList serializedMessages;

  Q_FOREACH(const Message &message, messages) {
    if(!message.isValid()) {
      LOG_ERROR("tried to serialize an invalid message");
      continue;
    }

    serializedMessages << message.serialize();
  }

  return serializedMessages.join(MESSAGE_SEPARATOR);
}

MessageList Message::unserialize(const QString &serializedMessages, Peer *peer)
{
  MessageList messages;

  const QStringList splitted = serializedMessages.split(MESSAGE_SEPARATOR);

  Q_FOREACH(const QString &messageContents, splitted) {
    QStringList parts = messageContents.split(PART_SEPARATOR);

    messages << Message(parts.takeFirst(), parts, peer);
  }

  return messages;
}

Message::Message(const QString &cmd, const QStringList &args, Peer *peer)
  : m_command(cmd), m_arguments(args), m_peer(peer)
{
}

bool Message::containsSeparators(const QString &input) const
{
  return input.contains(MESSAGE_SEPARATOR) || input.contains(PART_SEPARATOR);
}

bool Message::isValid() const
{
  if(containsSeparators(m_command))
    return false;

  Q_FOREACH(const QString &argument, m_arguments) {
    if(containsSeparators(argument))
      return false;
  }

  return true;
}

QString Message::serialize() const
{
  const QStringList parts = QStringList() << m_command << m_arguments;
  return parts.join(PART_SEPARATOR);
}

QString Message::toString() const
{
  QStringList parts = QStringList() << QString("CMD %1").arg(m_command);

  Q_FOREACH(const QString &argument, m_arguments)
    parts << QString("ARG %1").arg(argument);

  return parts.join('\x20');
}

Server *Message::server() const
{
  if(!m_peer)
    return 0;

  return m_peer->server();
}
