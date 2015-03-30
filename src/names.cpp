#include "names.h"

#include "user.h"

Cows::NameList Cows::CALLBACKS = {
  {"msg", User::sendMessage},
  {"action", User::sendMessage},
};
