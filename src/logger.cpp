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
  namespace pt = boost::posix_time;

  if(level < m_level)
    return;

  const pt::ptime time = pt::microsec_clock::local_time();

  const string line = str(boost::format("[%s] (%s) %s: %s")
    % time
    % level_prefix(level)
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

string Logger::level_prefix(const Level level) const
{
  switch(level) {
  case DEBUG:
    return "DD";
  case INFO:
    return "II";
  case WARNING:
    return "WW";
  case ERROR:
    return "EE";
  case FATAL:
    return "FF";
  }
}
