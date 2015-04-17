#ifndef NAMES_HPP
#define NAMES_HPP

#include <map>

class Command;

namespace Names
{
  typedef void (*Callback)(const Command &);
  typedef std::map<std::string, Callback> NameList;

  extern NameList COMMANDS;
};

#endif
