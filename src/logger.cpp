#include "logger.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <cassert>
#include <ctime>
#include <iostream>

#include "logging.hpp"

using namespace std;

LOG_MODULE("logger");

stack<Logger *> Logger::s_stack;

Logger::Logger(const string &log_file, const Level log_level)
  : m_level(log_level)
{
  s_stack.push(this);

  if(log_file != "-") {
    m_file.open(log_file, ofstream::trunc);

    if(m_file.fail())
      LOG_ERROR(boost::format("can not open '%1%' for writing: %2%")
        % log_file % strerror(errno)
      );
  }
}

Logger::~Logger()
{
  assert(s_stack.top() == this);

  s_stack.pop();
}

void Logger::log(const Level level,
  const string &module, const string &message)
{
  if(level < m_level)
    return;

  const auto &time = boost::posix_time::microsec_clock::local_time();

  const string line = str(boost::format("[%1%] (%2%%2%) %3%: %4%")
    % time
    % prefix_for(level)
    % module
    % message
  );

  ostream *stream = m_file.is_open() ? &m_file : &cerr;
  *stream << line << endl;
}

void Logger::log(const Level level, const string &module,
  const boost::format &format)
{
  log(level, module, format.str());
}

char Logger::prefix_for(const Level level) const
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
