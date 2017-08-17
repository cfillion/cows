#include "logger.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <fstream>

#include "logging.hpp"

using namespace boost;
using namespace std;

LOG_MODULE("logger");

stack<Logger *> Logger::s_stack;

Logger::Logger(const Level level, ostream *stream)
  : m_level(level), m_stream(stream)
{
  s_stack.push(this);

}

Logger::~Logger()
{
  assert(s_stack.top() == this);

  s_stack.pop();
}

bool Logger::open_file(const string &fn, ofstream *stream)
{
  stream->open(fn, ofstream::trunc);

  if(stream->good()) {
    m_stream = stream;
    return true;
  }
  else {
    LOG_ERROR(format("cannot open '%1%' for writing: %2%")
      % fn % strerror(errno));
    return false;
  }
}

void Logger::log(const Level level,
  const string &module, const string &message)
{
  if(level < m_level)
    return;

  *m_stream << str(format("[%1%] (%2%%2%) %3%: %4%")
    % posix_time::microsec_clock::local_time()
    % prefix_for(level)
    % module
    % message
  ) << endl;
}

void Logger::log(const Level level, const string &module,
  const format &format)
{
  log(level, module, format.str());
}

char Logger::prefix_for(const Level level)
{
  switch(level) {
  case DEBUG:
    return 'D';
  case INFO:
    return 'I';
  case WARNING:
    return 'W';
  case ERROR:
    return 'E';
  case FATAL:
    return 'F';
  }
}
