#include "handshake.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <boost/format.hpp>
#include <unistd.h>

#include "logging.hpp"

LOG_MODULE("handshake");

const unsigned int MAX_LENGTH = getpagesize();
const unsigned int MAX_HEADERS = 64;

using namespace boost;

Handshake::Handshake()
  : m_reply{HTTP_CONTINUE, {}}
{
  reset();
}

void Handshake::reset()
{
  m_state = METHOD;
  m_method.clear();
  m_target.clear();
  m_headers.clear();
}

void Handshake::consume(const char c)
{
  switch(m_state) {
  case METHOD:
    if(m_method.size() >= 3 && m_method != "GET") {
      LOG_DEBUG(format("unsupported method: %s") % m_method);
      set_reply(HTTP_NOT_IMPLEMENTED);
      break;
    }

    if(isalpha(c))
      m_method.push_back(c);
    else if(c == '\x20')
      m_state = TARGET;
    else
      set_reply(HTTP_BAD_REQUEST);

    break;
  case TARGET:
    if(c == '\x20') {
      if(m_target != "/")
        set_reply(HTTP_NOT_FOUND);

      m_state = VERSION_H;
    }
    else if(isspace(c))
      set_reply(HTTP_BAD_REQUEST);
    else {
      if(m_target.size() == 1)
        set_reply(HTTP_URI_TOO_LONG);
      else
        m_target.push_back(c);
    }

    break;
  case VERSION_H:
    expect(c == 'H', VERSION_T_1);
    break;
  case VERSION_T_1:
    expect(c == 'T', VERSION_T_2);
    break;
  case VERSION_T_2:
    expect(c == 'T', VERSION_P);
    break;
  case VERSION_P:
    expect(c == 'P', VERSION_SLASH);
    break;
  case VERSION_SLASH:
    expect(c == '/', VERSION_MAJOR);
    break;
  case VERSION_MAJOR:
    expect(isdigit(c), VERSION_DOT);
    break;
  case VERSION_DOT:
    expect(c == '.', VERSION_MINOR);
    break;
  case VERSION_MINOR:
    expect(isdigit(c), START_BREAK_CR);
    break;
  case START_BREAK_CR:
    expect(iscr(c), HEADER_BREAK_LF);
    break;
  case HEADER_BREAK_LF:
    if(expect(islf(c), HEADER_NAME)) {
      if(m_headers.size() <= MAX_HEADERS)
        m_headers.push_back({});
      else {
        LOG_DEBUG("header count exceeded maximum allowed count");
        set_reply(HTTP_BAD_REQUEST);
      }
    }

    break;
  case HEADER_NAME:
    if(istchar(c)) {
      if(m_headers.back().name.size() <= MAX_LENGTH)
        m_headers.back().name.push_back(c);
      else {
        LOG_DEBUG("header name exceeded maximum size");
        set_reply(HTTP_BAD_REQUEST);
      }
    }
    else if(c == ':')
      m_state = HEADER_VALUE;
    else if(iscr(c) && m_headers.back().name.empty()) {
      m_headers.pop_back(); // remove the trailing (empty) header
      m_state = BODY_LF;
    }
    else 
      set_reply(HTTP_BAD_REQUEST);

    break;
  case HEADER_VALUE:
    if(iscr(c)) {
      algorithm::trim(m_headers.back().value);
      m_state = HEADER_BREAK_LF;
    }
    else if(m_headers.back().value.size() > MAX_LENGTH) {
      LOG_DEBUG(format("header %s exceeded maximum size")
        % m_headers.back().name
      );

      set_reply(HTTP_BAD_REQUEST);
    }
    else
      m_headers.back().value.push_back(c);

    break;
  case BODY_LF:
    if(expect(islf(c), BODY))
      finalize();

    break;
  case BODY:
    break;
  };
}

void Handshake::finalize()
{
  for(HttpHeader &h : m_headers)
    LOG_DEBUG(format("%s = %s") % h.name % h.value);

  set_reply(HTTP_SWITCH_PROTOCOL);
}

void Handshake::set_reply(const HttpStatus s, const std::vector<HttpHeader> &h)
{
  LOG_DEBUG(format("reporting http code %d at stage %d") % s % m_state);
  m_reply = {s, h};
};
