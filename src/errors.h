#ifndef ERRORS_H
#define ERRORS_H

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
