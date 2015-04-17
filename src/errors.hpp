#ifndef ERRORS_HPP
#define ERRORS_HPP

namespace Errno
{
  enum ERRNO
  {
    OK,
    COMMAND_NOT_FOUND,
    PEER_NOT_FOUND,
    ROOM_NOT_FOUND,
    FOREIGN_ROOM,
    ALREADY_JOINED,
    INVALID_ROOM,
  };
};

#endif
