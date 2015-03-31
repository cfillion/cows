#ifndef USER_H
#define USER_H

class Command;

namespace User
{
  void send(const Command &command);
  void join(const Command &command);
};

#endif
