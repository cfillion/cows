#include "user.h"

#include "command.h"
#include "peer.h"
#include "server.h"
#include "room.h"

void User::send(const Command &command)
{
  const QString roomName = command.argument(0);
  const QString message = command.argument(1);

  Command broadcast(command.name(), roomName);
  broadcast.addArgument(command.peer()->uuid().toString());
  broadcast.addArgument(message);

  switch(Room::typeOf(roomName)) {
  case Room::Invalid:
    command.reply(Errno::INVALID_ROOM);
    break;
  case Room::Private: {
    QList<Peer *> recipients = command.server()->findPeers(roomName);
    recipients.removeOne(command.peer());

    if(recipients.isEmpty())
      return command.reply(Errno::PEER_NOT_FOUND);

    // send the unmodified command to the message's author
    command.peer()->send(broadcast);

    // set the author's nick as the room name sent to the recipients
    broadcast.setRoomName(broadcast.argument(0));

    Q_FOREACH(Peer *peer, recipients)
      peer->send(broadcast);

    break;
  }
  case Room::Public:
    Room *room = command.server()->findRoom(roomName);

    if(!room || !room->hasPeer(command.peer()))
      return command.reply(Errno::FOREIGN_ROOM);

    const int sendError = room->broadcast(broadcast);

    if(sendError != Errno::OK)
      command.reply(sendError);

    break;
  }
}

void User::join(const Command &command)
{
  const QString roomName = command.argument(0);
  Room *room = command.server()->findRoom(roomName);

  if(!room)
    return command.reply(Errno::ROOM_NOT_FOUND);

  const int joinStatus = room->addPeer(command.peer());

  if(joinStatus != Errno::OK)
    command.reply(joinStatus);
}

void User::part(const Command &command)
{
  const QString roomName = command.argument(0);
  const QString reason = command.argument(1);

  Room *room = command.server()->findRoom(roomName);

  if(!room || !room->hasPeer(command.peer()))
    return command.reply(Errno::FOREIGN_ROOM);

  room->removePeer(command.peer(), Room::UserPart, reason);
}
