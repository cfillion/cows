#ifndef COMMAND_H
#define COMMAND_H

#include <QStringList>

class Peer;
class Server;

class Command;
typedef QList<Command> CommandList;

class Command
{
public:
  static QString serialize(const CommandList &commands);
  static CommandList unserialize(const QString &text, Peer *peer);

  Command(const QString &name, const QStringList &args = QStringList(),
    Peer *peer = 0);

  bool isValid() const;
  QString serialize() const;
  QString toString() const;

  const QString &name() const { return m_name; }
  QString argument(const int index) const { return m_arguments.value(index); }
  const QStringList &arguments() const { return m_arguments; }

  void addArgument(const QString &newArg) { m_arguments << newArg; }

  Peer *peer() const { return m_peer; }
  Server *server() const;

  void reply(const QString &command, const QStringList &args = QStringList()) const;

private:
  bool containsSeparators(const QString &text) const;

  QString m_name;
  QStringList m_arguments;

  Peer *m_peer;
};

#endif
