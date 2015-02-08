#ifndef LOGGER_H
#define LOGGER_H

#include <cstdio>

class QString;

class Logger
{
public:
  static void open(FILE *output);
  static void open(Logger *replacement);

  static Logger *instance();

  enum Level
  {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
  };

  Logger(FILE *output);

  void log(const Level level,
    const QString &module, const QString &message) const;

private:
  QString level2string(const Level level) const;

  static Logger *s_instance;
  FILE *m_output;
};

#define LOG_MODULE(name) \
  static QString _LOG_MODULE_NAME = QStringLiteral(name);

#define LOG_HERE(level, message) \
  Logger::instance()->log(level, _LOG_MODULE_NAME, message);

#define LOG_FATAL(msg) \
  LOG_HERE(Logger::FATAL, msg);

#endif
