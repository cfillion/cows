#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QString>

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

  static void open(const QString &logFile = QString(),
    const Level logLevel = Level::INFO);
  static void open(Logger *replacement);

  static Logger *instance();

  Logger(const QString &logFile, const Level logLevel);
  ~Logger();

  void log(const Level level,
    const QString &module, const QString &message);

private:
  QString level2string(const Level level) const;

  static Logger *s_instance;

  QFile m_file;
  int m_level;
};

#endif
