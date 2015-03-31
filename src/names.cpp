#include "names.h"

#include "user.h"

#define COMMAND(n, c) {QStringLiteral(n), c},

Cows::NameList Cows::CALLBACKS = {
  COMMAND("msg", User::send)
  COMMAND("action", User::send)
  COMMAND("join", User::join)
};

#undef COMMAND
