#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QString>

class Logger
{
public:
  static void open(const QString &logFile = QString());
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

  Logger(const QString &logFile);
  ~Logger();

  void log(const Level level,
    const QString &module, const QString &message);

private:
  QString level2string(const Level level) const;

  static Logger *s_instance;

  QFile m_file;
};

#define LOG_MODULE(name) \
  static QString _LOG_MODULE_NAME = QStringLiteral(name);

#define LOG_HERE(level, message) \
  Logger::instance()->log(level, _LOG_MODULE_NAME, message);

#define LOG_DEBUG(msg) \
  LOG_HERE(Logger::DEBUG, msg);

#define LOG_INFO(msg) \
  LOG_HERE(Logger::INFO, msg);

#define LOG_WARNING(msg) \
  LOG_HERE(Logger::WARNING, msg);

#define LOG_ERROR(msg) \
  LOG_HERE(Logger::ERROR, msg);

#define LOG_FATAL(msg) \
  LOG_HERE(Logger::FATAL, msg);

#endif
