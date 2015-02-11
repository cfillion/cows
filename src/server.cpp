#include "server.h"

#include <QCoreApplication>
#include <QUrl>
#include <QWebSocketServer>

#include "chat.h"
#include "logging.h"
#include "peer.h"

LOG_MODULE("server");

Server::Server(QObject *parent)
  : QObject(parent)
{
  m_server = new QWebSocketServer(qApp->applicationName(),
    QWebSocketServer::NonSecureMode, this);

  connect(m_server, &QWebSocketServer::newConnection, this, &Server::createPeer);
  connect(m_server, &QWebSocketServer::closed, qApp, &QCoreApplication::quit);

  loadModule(new Chat);
}

Server::~Server()
{
  qDeleteAll(m_modules);

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
  LOG_DEBUG(QString("registering new peer (%1)").arg(m_peers.count() + 1));

  Peer *peer = new Peer(m_server->nextPendingConnection(), this);
  m_peers << peer;
}

void Server::loadModule(Module *module)
{
  m_modules << module;
}

Module *Server::moduleForCommand(const QString &command) const
{
  Q_FOREACH(Module *module, m_modules) {
    if(module->knownCommands().contains(command))
      return module;
  }

  return 0;
}

void Server::execute(const Command &command) const
{
  Module *module = moduleForCommand(command.name());

  if(module)
    module->processCommand(command);
  else
    command.reply("error", QStringList() << "unknown command");
}
