#ifndef PEER_H
#define PEER_H

#include <QObject>

#include <QHostAddress>
#include <QStringList>
#include <QUuid>

#include "command.h"

class QWebSocket;

class Room;
class Server;

class Peer : public QObject
{
  Q_OBJECT

public:
  Peer(QWebSocket *socket, Server *server);

  void send(const CommandList &commands);
  void send(const Command &commands);
  void send(const QString &cmdName, const QStringList &args = QStringList());

  const QUuid &uuid() const { return m_uuid; }
  Server *server() const { return m_server; }

  Room *findRoom(const QString &name);

private Q_SLOTS:
  void messageReceived(const QString &message);

Q_SIGNALS:
  void disconnected();

private:
  QWebSocket *m_socket;
  Server *m_server;

  QUuid m_uuid;
  QHostAddress m_address;
  int m_port;

  QList<Room *> m_rooms;
};

#endif
