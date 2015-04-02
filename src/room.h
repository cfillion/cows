#ifndef ROOM_H
#define ROOM_H

#include <QObject>

#include "command.h"

class Peer;

class Room : public QObject
{
  Q_OBJECT

public:
  enum Type
  {
    Invalid,
    Public,
    Private,
  };

  static Type typeOf(const QString &name);

  Room(const QString &name, QObject *parent = 0);
  ~Room();

  const QString &name() const { return m_name; }
  const QList<Peer *> &peers() const { return m_peers; }

  bool hasPeer(Peer *peer) const;
  int addPeer(Peer *peer);
  int removePeer(Peer *peer);

  int broadcast(const Command &command);

private Q_SLOTS:
  void peerDisconnected();

private:
  QString m_name;
  QList<Peer *> m_peers;
  QList<Command> m_backlog;
};

#endif
