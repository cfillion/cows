#ifndef MESSAGE_H
#define MESSAGE_H

#include <QStringList>

class Peer;
class Server;

class Message;
typedef QList<Message> MessageList;

class Message
{
public:
  static QString serialize(const MessageList &messages);
  static MessageList unserialize(const QString &serializedMessages,
    Peer *peer = 0);

  Message(const QString &cmd, const QStringList &args = QStringList(),
    Peer *peer = 0);

  bool isValid() const;
  QString serialize() const;
  QString toString() const;

  const QString &command() const { return m_command; }
  const QStringList &arguments() const { return m_arguments; }

  Peer *peer() const { return m_peer; }
  Server *server() const;

private:
  bool containsSeparators(const QString &string) const;

  QString m_command;
  QStringList m_arguments;
  Peer *m_peer;
};

#endif
