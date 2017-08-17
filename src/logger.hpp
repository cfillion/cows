#ifndef COWS_LOGGER_HPP
#define COWS_LOGGER_HPP

#include <boost/format/format_fwd.hpp>
#include <fstream>
#include <stack>

class Logger {
public:
  enum Level {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
  };

  static Logger *instance() { return s_stack.top(); }

  Logger(const std::string &log_file = "-", Level log_level = INFO);
  ~Logger();

  void log(Level level, const std::string &module, const std::string &message);
  void log(Level level, const std::string &module, const boost::format &format);

private:
  char prefix_for(Level level) const;

  static std::stack<Logger *> s_stack;

  std::ofstream m_file;
  int m_level;
};

#endif
