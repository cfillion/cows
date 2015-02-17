#include "chat.h"

#include "peer.h"
#include "server.h"

const QString MSG_COMMAND = QStringLiteral("msg");
const QString ACTION_COMMAND = QStringLiteral("action");

QStringList Chat::knownCommands() const
{
  return QStringList() << MSG_COMMAND << ACTION_COMMAND;
}

void Chat::processCommand(const Command &command)
{
  const Server *server = command.server();

  Command broadcast(command.name());
  broadcast.addArgument(command.argument(0));
  broadcast.addArgument(command.peer()->uuid().toString());
  broadcast.addArgument(command.argument(1));

  Q_FOREACH(Peer *peer, server->peers()) {
    peer->send(broadcast);
  }
}
