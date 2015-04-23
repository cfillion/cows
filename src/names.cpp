#include "names.hpp"

#include <string>

#include "user.hpp"

const Names::NameList Names::COMMANDS {
  {"msg", User::send},
  {"action", User::send},
  {"join", User::join},
  {"part", User::part},
};
