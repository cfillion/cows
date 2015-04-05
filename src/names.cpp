#include "names.h"

#include "user.h"

#define COMMAND(n, c) {QStringLiteral(n), c},

Names::NameList Names::COMMANDS = {
  COMMAND("msg", User::send)
  COMMAND("action", User::send)
  COMMAND("join", User::join)
  COMMAND("part", User::part)
};

#undef COMMAND
