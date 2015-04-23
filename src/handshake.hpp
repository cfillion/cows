#ifndef HTTP_PARSER_HPP
#define HTTP_PARSER_HPP

#include <boost/logic/tribool.hpp>
#include <string>
#include <vector>

enum HttpStatus
{
  HTTP_CONTINUE = 100,
  HTTP_SWITCH_PROTOCOL = 101,

  HTTP_BAD_REQUEST = 400,
  HTTP_NOT_FOUND = 404,
  HTTP_URI_TOO_LONG = 414,

  HTTP_NOT_IMPLEMENTED = 501,
};

struct HttpHeader
{
  std::string name;
  std::string value;
};

struct HttpReply
{
  HttpStatus status;
  std::vector<HttpHeader> headers;
};

class Handshake
{
public:
  Handshake();
  void reset();

  template <typename Iterator>
  boost::tribool parse(Iterator begin, Iterator end)
  {
    while(begin != end) {
      consume(*begin++);

      switch(m_reply.status) {
      case HTTP_CONTINUE:
        break;
      case HTTP_SWITCH_PROTOCOL:
        return true;
      default:
        return false;
      }
    }

    return boost::indeterminate;
  }

private:
  enum State
  {
    METHOD,
    TARGET,
    VERSION_H,
    VERSION_T_1,
    VERSION_T_2,
    VERSION_P,
    VERSION_SLASH,
    VERSION_MAJOR,
    VERSION_DOT,
    VERSION_MINOR,
    START_BREAK_CR,
    HEADER_BREAK_LF,
    HEADER_NAME,
    HEADER_VALUE,
    BODY_LF,
    BODY,
  };

  void consume(const char c);
  void finalize();

  bool iscr(const char c) { return c == '\r'; }
  bool islf(const char c) { return c == '\n'; }
  bool istchar(const char c)
  {
    switch(c) {
    case '!': case '#': case '$': case '%': case '&': case '\'': case '*':
    case '+': case '-': case '.': case '^': case '_': case '`': case '|':
    case '~':
      return true;
    }

    return isalnum(c);
  }

  bool expect(const bool test, const State pass)
  {
    if(test)
      m_state = pass;
    else
      set_reply(HTTP_BAD_REQUEST);

    return test;
  }

  void set_reply(const HttpStatus status,
    const std::vector<HttpHeader> &h = {});

  State m_state;
  std::string m_method;
  std::string m_target;
  std::vector<HttpHeader> m_headers;
  HttpReply m_reply;
};

#endif
