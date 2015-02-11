#include "chat.h"

#include "peer.h"
#include "server.h"

const QString MSG_COMMAND = QStringLiteral("msg");

QStringList Chat::knownCommands() const
{
  return QStringList() << MSG_COMMAND;
}

void Chat::processCommand(const Command &command)
{
  const Server *server = command.server();

  Command broadcast(MSG_COMMAND);
  broadcast.addArgument(command.peer()->uuid().toString());
  broadcast.addArgument(command.argument(0));

  Q_FOREACH(Peer *peer, server->peers()) {
    peer->send(broadcast);
  }
}
