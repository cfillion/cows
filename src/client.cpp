#include "client.h"

#include <QCoreApplication>
#include <QHostInfo>
#include <QWebSocket>

#include "logging.h"

LOG_MODULE("client");

Client::Client(QObject *parent)
  : QObject(parent)
{
  m_socket = new QWebSocket(QHostInfo::localHostName(),
    QWebSocketProtocol::VersionLatest, this);

  connect(m_socket, &QWebSocket::connected, this, &Client::connected);
  connect(m_socket, &QWebSocket::disconnected, this, &Client::disconnected);

  connect(m_socket, // ugh, this is ugly
    static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>
    (&QWebSocket::error), this, &Client::failed);
}

bool Client::open(const QString &url)
{
  m_socket->open(QUrl(url));
  return true;
}

void Client::connected()
{
  LOG_INFO("connection established");
}

void Client::disconnected()
{
  LOG_FATAL("connection closed");

  qApp->quit();
}

void Client::failed(const QAbstractSocket::SocketError)
{
  LOG_ERROR(tr("system message: %1").arg(m_socket->errorString()));
}
