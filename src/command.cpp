#include "command.h"

#include "logging.h"
#include "peer.h"

LOG_MODULE("command");

static const QChar COMMAND_SEPARATOR = '\x1E';
static const QChar PART_SEPARATOR = '\x1F';

static const int MAX_LENGTH = 64; // applied to the command name, room and key
static const int MAX_ARG_COUNT = 4;
static const int MAX_ARG_LENGTH = 10000;

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

    const QString name = parts.isEmpty() ? QString() : parts.takeFirst();
    const QString key = parts.isEmpty() ? QString() : parts.takeFirst();
    const QString room = parts.isEmpty() ? QString() : parts.takeFirst();

    Command command(name, room, parts, peer);
    command.setKey(key);

    if(command.isValid())
      commands << command;
    else
      LOG_WARNING("dropped an invalid command (read)");
  }

  return commands;
}

Command::Command(const QString &name, const QString &roomName,
  const QStringList &args, Peer *peer)
  : m_name(name), m_roomName(roomName), m_arguments(args), m_peer(peer)
{
}

bool Command::containsIllegal(const QString &input) const
{
  // detects ASCII control characters
  // see http://en.wikipedia.org/wiki/ASCII#ASCII_control_code_chart

  Q_FOREACH(const QChar &c, input) {
    if((c <= 0x1F || c == 0x7F) && !QStringLiteral("\r\n\t").contains(c))
      return true;
  }

  return false;
}

bool Command::isValid() const
{
  if(m_name.length() > MAX_LENGTH || m_key.length() > MAX_LENGTH
    || m_key.length() > MAX_LENGTH)
    return false;

  if(containsIllegal(m_name) || containsIllegal(m_key) ||
    containsIllegal(m_roomName))
    return false;

  if(m_arguments.size() > MAX_ARG_COUNT)
    return false;

  Q_FOREACH(const QString &argument, m_arguments) {
    if(argument.length() > MAX_ARG_LENGTH || containsIllegal(argument))
      return false;
  }

  return true;
}

QString Command::serialize() const
{
  const QStringList parts = QStringList()
    << m_name << m_key << m_roomName << m_arguments;

  return parts.join(PART_SEPARATOR);
}

QString Command::toString() const
{
  QStringList parts = QStringList()
    << QString("CMD %1").arg(m_name)
    << QString("KEY %1").arg(m_key)
    << QString("IN %1").arg(m_roomName);

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

  Command cmd(command, m_roomName, args);
  cmd.setKey(m_key);

  m_peer->send(cmd);
}

void Command::reply(const int errorCode) const
{
  reply(QStringLiteral("error"),
    QStringList() << QString::number(errorCode));
}
