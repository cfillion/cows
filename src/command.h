#ifndef COMMAND_H
#define COMMAND_H

#include <QStringList>

#include "errors.h"

class Peer;
class Server;

class Command;
typedef QList<Command> CommandList;

class Command
{
public:
  static QString serialize(const CommandList &commands);
  static CommandList unserialize(const QString &text, Peer *peer);

  Command(const QString &name, const QString &roomName,
      const QStringList &args = QStringList(), Peer *peer = 0);

  bool isValid() const;
  QString serialize() const;
  QString toString() const;

  const QString &name() const { return m_name; }
  const QString &roomName() const { return m_roomName; }
  const QString &key() const { return m_key; }
  QString argument(const int index) const { return m_arguments.value(index); }
  const QStringList &arguments() const { return m_arguments; }

  void setKey(const QString &newKey) { m_key = newKey; }
  void setRoomName(const QString &newRoom) { m_roomName = newRoom; }
  void addArgument(const QString &newArg) { m_arguments << newArg; }
  void setArgument(const int index, const QString &newValue)
    { m_arguments[index] = newValue; }

  Peer *peer() const { return m_peer; }
  Server *server() const;

  void reply(const QString &command,
      const QStringList &args = QStringList()) const;
  void reply(const int errorCode) const;

private:
  bool containsIllegal(const QString &text) const;

  QString m_name;
  QString m_key;
  QString m_roomName;
  QStringList m_arguments;

  Peer *m_peer;
};

#endif
