#include "server.h"

#include <QCoreApplication>
#include <QUrl>
#include <QWebSocketServer>

#include "logging.h"
#include "names.h"
#include "peer.h"
#include "room.h"

LOG_MODULE("server");

Server::Server(QObject *parent)
  : QObject(parent)
{
  m_server = new QWebSocketServer(qApp->applicationName(),
    QWebSocketServer::NonSecureMode, this);

  connect(m_server, &QWebSocketServer::newConnection, this, &Server::createPeer);
  connect(m_server, &QWebSocketServer::closed, qApp, &QCoreApplication::quit);

  m_rooms["#hello_world"] = new Room("#hello_world", this);
  m_rooms["#42"] = new Room("#42", this);
}

Server::~Server()
{
  m_server->close();
}

bool Server::open(const QString &address)
{
  QStringList components = address.split(QStringLiteral(":"));

  if(components.size() != 2) {
    LOG_FATAL(QString("invalid address: %1. format must be IP:PORT")
      .arg(address));

    return false;
  }

  const QString hostInput = components.takeFirst();
  const QString portInput = components.takeFirst();
  
  const QHostAddress host = QHostAddress(hostInput);

  if(host.isNull()) {
    LOG_FATAL(QString("invalid ip: %1").arg(hostInput));
    return false;
  }

  bool portConverted;
  const int port = portInput.toInt(&portConverted);

  if(!portConverted) {
    LOG_FATAL(QString("invalid port number: %1").arg(portInput));
    return false;
  }

  if(!m_server->listen(host, port)) {
    LOG_FATAL(QString("can not listening on %1: %2")
      .arg(address, m_server->errorString()));

    return false;
  }

  LOG_INFO(QString("initialization completed. clients are now accepted on %3")
    .arg(m_server->serverUrl().toString()));

  return true;
}

void Server::createPeer()
{
  LOG_INFO(QString("registering new peer (%1)").arg(m_peers.count() + 1));

  Peer *peer = new Peer(m_server->nextPendingConnection(), this);
  connect(peer, &Peer::disconnected, this, &Server::destroyPeer);

  m_peers << peer;
}

void Server::destroyPeer()
{
  Peer *peer = qobject_cast<Peer *>(sender());

  LOG_INFO(QString("unregistering peer %1").arg(peer->uuid().toString()));

  m_peers.removeAll(peer);
  peer->deleteLater();
}

void Server::execute(const Command &command) const
{
  const QString name = command.name();

  if(Cows::CALLBACKS.count(name) == 0) {
    command.reply(Cows::COMMAND_NOT_FOUND);
    return;
  }

  Cows::CALLBACKS[name](command);
}

QList<Peer *> Server::findPeers(const QString &search) const
{
  QList<Peer *> matches;

  const QUuid uuid(search);

  Q_FOREACH(Peer *peer, m_peers) {
    if(peer->uuid() == uuid)
      matches << peer;
  }

  return matches;
}

Room *Server::findRoom(const QString &name) const
{
  return m_rooms.value(name);
}
