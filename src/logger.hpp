#ifndef COWS_LOGGER_HPP
#define COWS_LOGGER_HPP

#include <boost/format/format_fwd.hpp>
#include <iostream>
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

  Logger(Level = INFO, std::ostream * = &std::clog);
  ~Logger();

  std::ostream *stream() const { return m_stream; }
  bool open_file(const std::string &fn, std::ofstream *);

  void log(Level level, const char *module, const char *message);
  void log(Level level, const char *module, const boost::format &format);

private:
  static char prefix_for(Level level);

  static std::stack<Logger *> s_stack;

  Level m_level;
  std::ostream *m_stream;
};

#endif
