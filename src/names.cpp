#include "names.hpp"

#include "user.hpp"

Names::NameList Names::COMMANDS = {
  {"msg", User::send},
  {"action", User::send},
  {"join", User::join},
  {"part", User::part},
};
