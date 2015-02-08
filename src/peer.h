#ifndef PEER_H
#define PEER_H

#include <QObject>

#include <QHostAddress>
#include <QStringList>
#include <QUuid>

#include "message.h"

class QWebSocket;

class Server;

class Peer : public QObject
{
  Q_OBJECT

public:
  Peer(QWebSocket *socket, Server *server);

  void send(const MessageList &messages);
  void send(const Message &messages);
  void send(const QString &cmd, const QStringList &args = QStringList());

  const QUuid &uuid() const { return m_uuid; }

private Q_SLOTS:
  void messageReceived(const QString &serializedMessages);

private:
  QWebSocket *m_socket;
  Server *m_server;

  QUuid m_uuid;
  QHostAddress m_address;
  int m_port;
};

#endif
