#include "chat.h"

#include "peer.h"
#include "server.h"

const QString MSG_COMMAND = QStringLiteral("msg");

QStringList Chat::knownCommands() const
{
  return QStringList() << MSG_COMMAND;
}

void Chat::processMessage(const Message &command)
{
  const Server *server = command.server();

  Message reply("msg");
  reply.addArgument(command.peer()->uuid().toString());
  reply.addArgument(command.argument(0));

  Q_FOREACH(Peer *peer, server->peers()) {
    peer->send(reply);
  }
}
