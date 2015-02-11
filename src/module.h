#ifndef MODULE_H
#define MODULE_H

#include <QStringList>

class Command;
class Server;

class Module
{
public:
  virtual ~Module() {}

  virtual QStringList knownCommands() const = 0;
  virtual void processCommand(const Command &command) = 0;
};

#endif
