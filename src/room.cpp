#include "room.h"

#include "errors.h"
#include "peer.h"

Room::Type Room::typeOf(const QString &name)
{
  const QChar prefix = name[0];

  if(prefix == '#')
    return Room::Public;

  return Room::Private;
}

Room::Room(const QString &name, QObject *parent)
  : QObject(parent), m_name(name)
{
}

bool Room::hasPeer(Peer *peer) const
{
  return m_peers.contains(peer);
}

int Room::addPeer(Peer *peer)
{
  if(hasPeer(peer))
    return Cows::ALREADY_JOINED;

  connect(peer, &Peer::disconnected, this, &Room::peerDisconnected);

  m_peers << peer;
  return Cows::OK;
}

int Room::removePeer(Peer *peer)
{
  peer->disconnect(this);
  m_peers.removeOne(peer);

  return Cows::OK;
}

void Room::peerDisconnected()
{
  removePeer(qobject_cast<Peer *>(sender()));
}

int Room::broadcast(const Command &command)
{
  Q_FOREACH(Peer *peer, m_peers)
    peer->send(command);

  return Cows::OK;
}
