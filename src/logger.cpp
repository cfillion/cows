#include "logger.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include <ctime>
#include <iostream>

Logger *Logger::s_instance = 0;

void Logger::open(const std::string &log_file, const Level log_level)
{
  Logger::open(new Logger(log_file, log_level));
}

void Logger::open(Logger *replacement)
{
  if(s_instance)
    delete s_instance;

  s_instance = replacement;
}

Logger *Logger::instance()
{
  return s_instance;
}

Logger::Logger(const std::string &log_file, const Level log_level)
  : m_level(log_level)
{
  // The macros LOG_* can't be used here since s_instance isn't set yet.

  if(log_file != "-") {
    m_file.open(log_file, std::ofstream::trunc);

    if(m_file.fail()) {
      log(ERROR, "logger", str(
        boost::format("can not open '%1%' for writing: %2%")
        % log_file % strerror(errno)
      ));
    }
  }
}

Logger::~Logger()
{
  if(m_file.is_open())
    m_file.close();
}

void Logger::log(const Level level,
  const std::string &module, const std::string &message)
{
  namespace pt = boost::posix_time;

  if(level < m_level)
    return;

  const pt::ptime time = pt::microsec_clock::local_time();

  const std::string line = str(
    boost::format("[%1%] (%2%) %3%: %4%")
    % time % level2string(level) % module % message
  );

  std::ostream *stream = m_file.is_open() ? &m_file : &std::cerr;
  *stream << line << std::endl;
}

std::string Logger::level2string(const Level level) const
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
