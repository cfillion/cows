#ifndef CHAT_H
#define CHAT_H

#include "module.h"

class Chat : public Module
{
public:
  QStringList knownCommands() const Q_DECL_OVERRIDE;
  void processMessage(const Message &message) Q_DECL_OVERRIDE;
};

#endif
