#ifndef MESSAGE_H
#define MESSAGE_H

#include <QStringList>

class Message;
typedef QList<Message> MessageList;

class Message
{
public:
  static QString serialize(const MessageList &messages);
  static MessageList unserialize(const QString &serializedMessages);

  Message(const QString &cmd = QString(), const QStringList &args = QStringList());

  QString serialize() const;
  QString toString() const;

  const QString &command() const { return m_command; }
  const QStringList &arguments() const { return m_arguments; }

private:
  QString m_command;
  QStringList m_arguments;
};

#endif
