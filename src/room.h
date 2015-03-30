#ifndef ROOM_H
#define ROOM_H

#include <QObject>

class Peer;

class Room : public QObject
{
public:
  enum Type
  {
    Public,
    Private,
  };

  static Type typeOf(const QString &name);

  Room(const QString &name, QObject *parent = 0);

  const QString &name() const { return m_name; }

private:
  QString m_name;
  QList<Peer *> m_peers;
};

#endif
