#include "chat.h"

#include "peer.h"
#include "server.h"

const QString MSG_COMMAND = QStringLiteral("msg");

QStringList Chat::knownCommands() const
{
  return QStringList() << MSG_COMMAND;
}

void Chat::processMessage(const Message &message)
{
  const Server *server = message.server();

  Q_FOREACH(Peer *peer, server->peers()) {
    peer->send("msg", message.arguments());
  }
}
