#include "user.h"

#include "command.h"
#include "peer.h"
#include "server.h"

void User::sendMessage(const Command &command)
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
