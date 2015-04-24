#include "handshake.hpp"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/format.hpp>
#include <botan/filters.h>
#include <map>
#include <sstream>
#include <unistd.h>

#include "logging.hpp"

LOG_MODULE("handshake");

const unsigned int MAX_LENGTH = getpagesize();
const unsigned int MAX_HEADERS = 64;

const std::map<HttpStatus, std::string> STATUS_STRINGS {
  {HTTP_CONTINUE, "Continue"},
  {HTTP_SWITCH_PROTOCOLS, "Switch Protocols"},

  {HTTP_BAD_REQUEST, "Bad Request"},
  {HTTP_NOT_FOUND, "Not Found"},
  {HTTP_URI_TOO_LONG, "URI Too Long"},
  {HTTP_UPGRADE_REQUIRED, "Upgrade Required"},

  {HTTP_INTERNAL_ERROR, "Internal Server Error"},
  {HTTP_NOT_IMPLEMENTED, "Not Implemented"},
};

const std::string MAGIC_STRING = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

using namespace boost;
using namespace Botan;

Handshake::Handshake()
  : m_state(METHOD), m_reply{HTTP_CONTINUE, {}, ""}
{
}

void Handshake::consume(const char c)
{
  switch(m_state) {
  case METHOD:
    if(m_method.size() >= 3 && m_method != "GET") {
      m_reply = {HTTP_NOT_IMPLEMENTED, {}, "Unsupported method."};
      break;
    }

    if(isalpha(c))
      m_method.push_back(c);
    else
      expect(c == '\x20', TARGET);

    break;
  case TARGET:
    if(c == '\x20') {
      if(m_target != "/")
        m_reply = {HTTP_NOT_FOUND, {}, "There's no such thing here."};

      m_state = VERSION_H;
    }
    else if(expect(!isspace(c), m_state)) {
      if(m_target.size() > 37)
        m_reply = {HTTP_URI_TOO_LONG, {},
          "Target URI exceeded maximum allowed size."};
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
      if(m_headers.size() > MAX_HEADERS)
        m_reply = {HTTP_BAD_REQUEST, {},
          "Header count exceeded maximum allowed count."};
      else
        m_headers.push_back({});
    }

    break;
  case HEADER_NAME:
    if(istchar(c)) {
      if(m_headers.back().name.size() > MAX_LENGTH)
        m_reply = {HTTP_BAD_REQUEST, {},
          "Header name exceeded maximum allowed size."};
      else
        m_headers.back().name.push_back(tolower(c));
    }
    else if(c == ':')
      m_state = HEADER_VALUE;
    else if(iscr(c) && m_headers.back().name.empty()) {
      m_headers.pop_back(); // remove the trailing (empty) header
      m_state = BODY_LF;
    }
    else 
      m_reply = {HTTP_BAD_REQUEST, {}, "Invalid headers."};

    break;
  case HEADER_VALUE:
    if(iscr(c)) {
      algorithm::trim(m_headers.back().value);
      m_state = HEADER_BREAK_LF;
    }
    else if(m_headers.back().value.size() > MAX_LENGTH)
      m_reply = {HTTP_BAD_REQUEST, {},
        str(format("Header %s exceeded maximum allowed size.")
        % m_headers.back().name)};
    else
      m_headers.back().value.push_back(c);

    break;
  case BODY_LF:
    if(expect(islf(c), BODY))
      finalize();

    break;
  case BODY:
    // We should not be asked to parse anything once the handshake is complete,
    // which should be the case since we already finished reading the header.

    m_reply = {HTTP_INTERNAL_ERROR, {}, "Something went wrong server-side."};
    break;
  };
}

bool Handshake::expect(const bool test, const State pass)
{
  if(test)
    m_state = pass;
  else
    m_reply = {HTTP_BAD_REQUEST, {}, "Failed expectation."};

  return test;
}

std::string Handshake::header_value(const std::string &key) const
{
  for(const HttpHeader &h : m_headers) {
    if(h.name == key)
      return h.value;
  }

  return "";
}

void Handshake::finalize()
{
  const std::string protocol = header_value("sec-websocket-protocol");

  bool is_ws = true;
  is_ws = is_ws && iequals(header_value("upgrade"), "websocket");
  is_ws = is_ws && iequals(header_value("connection"), "upgrade");

  if(!is_ws) {
    m_reply = {HTTP_UPGRADE_REQUIRED,
      {{"Upgrade", "websocket"}}, "Hello World"};

    return;
  }

  Pipe pipe(new Chain(
    new Hash_Filter("SHA-1"),
    new Base64_Encoder
  ));

  pipe.process_msg(header_value("sec-websocket-key") + MAGIC_STRING);
  const std::string accept_key = pipe.read_all_as_string(0);

  m_reply = {HTTP_SWITCH_PROTOCOLS, {
    {"Upgrade", "websocket"},
    {"Connection", "Upgrade"},
    {"Sec-WebSocket-Accept", accept_key},
    {"Sec-WebSocket-Protocol", protocol == "cows1" ? protocol : "null"},
  }, ""};
}

std::string Handshake::encode_reply() const
{
  std::string status_string = "Unknown";
  if(STATUS_STRINGS.count(m_reply.status))
    status_string = STATUS_STRINGS.at(m_reply.status);

  LOG_DEBUG(format("status is %d %s \"%s\" at stage %d")
    % m_reply.status % status_string % m_reply.body % m_state);

  HttpReply reply = m_reply;
  reply.headers.insert(reply.headers.begin(), {
    {"Content-Type", "text/html"},
    {"Server", "cows"},
  });

  if(!m_reply.body.empty()) {
    reply.body = str(format(
      "<html><head><title>%1% %2%</title></head>"
      "<body><h1>%1% %2%</h1><p>%3%</p><hr>"
      "<address>C.O.W.S Server</address></body></html>\r\n")
      % m_reply.status % status_string
      % m_reply.body
    );

    reply.headers.insert(reply.headers.begin(), {
      {"Content-Length", std::to_string(reply.body.size())}
    });
  }

  std::ostringstream out;
  out << format("HTTP/1.1 %d %s\r\n") % reply.status % status_string;

  for(HttpHeader &h : reply.headers)
    out << format("%s: %s\r\n") % h.name % h.value;

  out << "\r\n" << reply.body;

  return out.str();
}
