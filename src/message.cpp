#include "message.h"

static QChar MESSAGE_SEPARATOR = '\30';
static QChar PART_SEPARATOR = '\31';

QString Message::serialize(const MessageList &messages)
{
  QStringList serializedMessages;

  Q_FOREACH(const Message &message, messages)
    serializedMessages << message.serialize();

  return serializedMessages.join(MESSAGE_SEPARATOR);
}

MessageList Message::unserialize(const QString &serializedMessages)
{
  MessageList messages;

  const QStringList splitted = serializedMessages.split(MESSAGE_SEPARATOR);

  Q_FOREACH(const QString &messageParts, splitted) {
    QStringList parts = messageParts.split(PART_SEPARATOR);

    messages << Message(parts.takeFirst(), parts);
  }

  return messages;
}

Message::Message(const QString &command, const QStringList &arguments)
  : m_command(command), m_arguments(arguments)
{
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
