#include "command.h"

#include "logging.h"
#include "peer.h"

LOG_MODULE("command");

static QChar COMMAND_SEPARATOR = '\x1E';
static QChar PART_SEPARATOR = '\x1F';

static QString ERROR_CMD = QStringLiteral("error");

QString Command::serialize(const CommandList &commands)
{
  QStringList textParts;

  Q_FOREACH(const Command &command, commands) {
    if(!command.isValid()) {
      LOG_ERROR("tried to serialize an invalid command");
      continue;
    }

    textParts << command.serialize();
  }

  return textParts.join(COMMAND_SEPARATOR);
}

CommandList Command::unserialize(const QString &text, Peer *peer)
{
  CommandList commands;

  const QStringList splitted = text.split(COMMAND_SEPARATOR);

  Q_FOREACH(const QString &commandText, splitted) {
    QStringList parts = commandText.split(PART_SEPARATOR);

    commands << Command(parts.takeFirst(), parts, peer);
  }

  return commands;
}

Command::Command(const QString &name, const QStringList &args, Peer *peer)
  : m_name(name), m_arguments(args), m_peer(peer)
{
}

bool Command::containsSeparators(const QString &input) const
{
  return input.contains(COMMAND_SEPARATOR) || input.contains(PART_SEPARATOR);
}

bool Command::isValid() const
{
  if(containsSeparators(m_name))
    return false;

  Q_FOREACH(const QString &argument, m_arguments) {
    if(containsSeparators(argument))
      return false;
  }

  return true;
}

QString Command::serialize() const
{
  const QStringList parts = QStringList() << m_name << m_arguments;
  return parts.join(PART_SEPARATOR);
}

QString Command::toString() const
{
  QStringList parts = QStringList() << QString("CMD %1").arg(m_name);

  Q_FOREACH(const QString &argument, m_arguments)
    parts << QString("ARG %1").arg(argument);

  return parts.join('\x20');
}

Server *Command::server() const
{
  if(!m_peer)
    return 0;

  return m_peer->server();
}

void Command::reply(const QString &command, const QStringList &args) const
{
  if(!m_peer)
    return LOG_ERROR("cannot reply to an unbound command");

  m_peer->send(command, args);
}

void Command::reply(int errorCode) const
{
  reply(ERROR_CMD, QStringList() << QString::number(errorCode));
}
