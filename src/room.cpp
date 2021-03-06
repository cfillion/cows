#include "room.h"

#include "errors.h"
#include "logging.h"
#include "peer.h"

LOG_MODULE("room")

static const int BACKLOG_SIZE = 255;

Room::Type Room::typeOf(const QString &name)
{
  if(name.isEmpty())
    return Invalid;

  const QChar prefix = name[0];

  if(prefix == '#')
    return Public;

  return Private;
}

Room::Room(const QString &name, QObject *parent)
  : QObject(parent), m_name(name)
{
  LOG_DEBUG(QString("constructed room %1").arg(m_name));
}

Room::~Room()
{
  LOG_DEBUG(QString("destroyed room %1").arg(m_name));
}

bool Room::hasPeer(Peer *peer) const
{
  return m_peers.contains(peer);
}

int Room::addPeer(Peer *peer)
{
  if(hasPeer(peer))
    return Errno::ALREADY_JOINED;

  LOG_INFO(QString("peer %1 joined %2").arg(peer->uuid().toString(), m_name));

  connect(peer, &Peer::disconnected, this, &Room::peerDisconnected);

  peer->send(m_backlog);

  m_peers << peer;

  Command announcement(QStringLiteral("join"), m_name);
  announcement.addArgument(peer->uuid().toString());
  broadcast(announcement);

  return Errno::OK;
}

int Room::removePeer(Peer *peer, const PartReason reason,
  const QString &userString)
{
  LOG_INFO(QString("peer %1 left %2").arg(peer->uuid().toString(), m_name));

  Command announcement(QStringLiteral("part"), m_name);
  announcement.addArgument(peer->uuid().toString());
  announcement.addArgument(QString::number(reason));
  announcement.addArgument(userString);
  broadcast(announcement);

  peer->disconnect(this);
  m_peers.removeOne(peer);

  return Errno::OK;
}

void Room::peerDisconnected()
{
  removePeer(qobject_cast<Peer *>(sender()), PeerLost);
}

int Room::broadcast(const Command &command)
{
  m_backlog << command;

  if(m_backlog.size() > BACKLOG_SIZE)
    m_backlog.removeFirst();

  Q_FOREACH(Peer *peer, m_peers)
    peer->send(command);

  return Errno::OK;
}
