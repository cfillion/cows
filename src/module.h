#ifndef MODULE_H
#define MODULE_H

#include <QStringList>

class Message;
class Server;

class Module
{
public:
  virtual ~Module() {}

  virtual QStringList knownCommands() const = 0;
  virtual void processMessage(const Message &message) = 0;
};

#endif
