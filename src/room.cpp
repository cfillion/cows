#include "room.h"

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
