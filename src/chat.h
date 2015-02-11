#ifndef CHAT_H
#define CHAT_H

#include "module.h"

class Chat : public Module
{
public:
  virtual QStringList knownCommands() const Q_DECL_OVERRIDE;
  virtual void processCommand(const Command &command) Q_DECL_OVERRIDE;
};

#endif
