#include "names.h"

#include "user.h"

#define COMMAND(n, c) {QStringLiteral(n), c},

Cows::NameList Cows::CALLBACKS = {
  COMMAND("msg", User::sendMessage)
  COMMAND("action", User::sendMessage)
};

#undef COMMAND
