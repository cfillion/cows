#ifndef LOGGER_H
#define LOGGER_H

#include <boost/format/format_fwd.hpp>
#include <fstream>

class Logger
{
public:
  enum Level
  {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
  };

  static void open(const std::string &log_file = "-",
    const Level log_level = INFO);
  static void open(Logger *replacement);

  static Logger *instance();

  Logger(const std::string &log_file, const Level log_level);

  void log(const Level level,
    const std::string &module, const std::string &message);
  void log(const Level level,
    const std::string &module, const boost::format &format);

private:
  std::string level2string(const Level level) const;

  static Logger *s_instance;

  std::ofstream m_file;
  int m_level;
};

#endif
