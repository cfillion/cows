#include "user.h"

#include "command.h"
#include "peer.h"
#include "server.h"
#include "room.h"

void User::send(const Command &command)
{
  const QString roomName = command.argument(0);
  const QString message = command.argument(1);

  Command broadcast(command.name());
  broadcast.addArgument(roomName);
  broadcast.addArgument(command.peer()->uuid().toString());
  broadcast.addArgument(message);

  switch(Room::typeOf(roomName)) {
  case Room::Private: {
    QList<Peer *> recipients = command.server()->findPeers(roomName);
    recipients.removeOne(command.peer());

    if(recipients.isEmpty())
      return command.reply(Cows::PEER_NOT_FOUND);

    // send the unmodified command to the message's author
    command.peer()->send(broadcast);

    // set the author's nick as the room name sent to the recipients
    broadcast.setArgument(0, broadcast.argument(1));

    Q_FOREACH(Peer *peer, recipients)
      peer->send(broadcast);

    break;
  }
  case Room::Public:
    Room *room = command.server()->findRoom(roomName);

    if(!room || !room->hasPeer(command.peer()))
      return command.reply(Cows::FOREIGN_ROOM);

    const int sendError = room->broadcast(broadcast);

    if(sendError != Cows::OK)
      command.reply(sendError);

    break;
  }
}

void User::join(const Command &command)
{
  const QString roomName = command.argument(0);
  Room *room = command.server()->findRoom(roomName);

  if(!room)
    return command.reply(Cows::ROOM_NOT_FOUND);

  const int joinStatus = room->addPeer(command.peer());

  if(joinStatus != Cows::OK)
    command.reply(joinStatus);
}
