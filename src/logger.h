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

#endif
