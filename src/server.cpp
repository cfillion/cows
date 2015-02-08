#include "server.h"

#include <QCoreApplication>
#include <QUrl>
#include <QWebSocketServer>

#include "logging.h"
#include "peer.h"

LOG_MODULE("server");

Server::Server(QObject *parent)
  : QObject(parent)
{
  m_server = new QWebSocketServer(qApp->applicationName(),
    QWebSocketServer::NonSecureMode, this);

  connect(m_server, &QWebSocketServer::newConnection, this, &Server::newPeer);
  connect(m_server, &QWebSocketServer::closed, qApp, &QCoreApplication::quit);
}

Server::~Server()
{
  m_server->close();
}

bool Server::open(const QString &address)
{
  QStringList components = address.split(QStringLiteral(":"));

  if(components.size() != 2) {
    LOG_FATAL(tr("invalid address: %1. format must be IP:PORT")
      .arg(address));

    return false;
  }

  const QString hostInput = components.takeFirst();
  const QString portInput = components.takeFirst();
  
  const QHostAddress host = QHostAddress(hostInput);

  if(host.isNull()) {
    LOG_FATAL(tr("invalid ip: %1").arg(hostInput));
    return false;
  }

  bool portConverted;
  const int port = portInput.toInt(&portConverted);

  if(!portConverted) {
    LOG_FATAL(tr("invalid port number: %1").arg(portInput));
    return false;
  }

  if(!m_server->listen(host, port)) {
    LOG_FATAL(tr("can not listening on %1: %2")
      .arg(address, m_server->errorString()));

    return false;
  }

  LOG_INFO(tr("initialization completed. clients are now accepted on %3")
    .arg(m_server->serverUrl().toString()));

  return true;
}

void Server::newPeer()
{
  LOG_DEBUG("registering new peer");

  Peer *peer = new Peer(m_server->nextPendingConnection(), this);
  m_peers << peer;
}
