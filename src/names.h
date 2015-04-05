#ifndef NAMES_H
#define NAMES_H

#include <map>
#include <QString>

class Command;

namespace Names
{
  typedef void (*Callback)(const Command &);
  typedef std::map<QString, Callback> NameList;

  extern NameList COMMANDS;
};

#endif
