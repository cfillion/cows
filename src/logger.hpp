#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <boost/format/format_fwd.hpp>
#include <fstream>
#include <stack>

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

  static Logger *instance() { return s_stack.top(); }

  Logger(const std::string &log_file = "-", const Level log_level = INFO);
  ~Logger();

  void log(const Level level,
    const std::string &module, const std::string &message);
  void log(const Level level,
    const std::string &module, const boost::format &format);

private:
  std::string level_prefix(const Level level) const;

  static std::stack<Logger *> s_stack;

  std::ofstream m_file;
  int m_level;
};

#endif
