#include "user.h"

#include "command.h"
#include "peer.h"
#include "server.h"
#include "room.h"

void User::sendMessage(const Command &command)
{
  const QString roomName = command.argument(0);
  const QString message = command.argument(1);

  QList<Peer *> recipients;

  Command broadcast(command.name());
  broadcast.addArgument(roomName);
  broadcast.addArgument(command.peer()->uuid().toString());
  broadcast.addArgument(message);

  switch(Room::typeOf(roomName)) {
  case Room::Private: {
    recipients << command.server()->findPeers(roomName);
    recipients.removeOne(command.peer());

    if(recipients.isEmpty())
      return command.reply(Cows::PEER_NOT_FOUND);

    // send the unmodified command to the message's author
    command.peer()->send(broadcast);

    // set the author's nick as the room name sent to the recipients
    broadcast.setArgument(0, broadcast.argument(1));
    break;
  }
  case Room::Public:
    break;
  }

  Q_FOREACH(Peer *peer, recipients)
    peer->send(broadcast);
}
