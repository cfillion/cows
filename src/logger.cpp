#include "logger.h"

#include <QDateTime>
#include <QString>
#include <QtGlobal>

Logger *Logger::s_instance = 0;

void Logger::open(FILE *output)
{
  Logger::open(new Logger(output));
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

Logger::Logger(FILE *output)
  : m_output(output)
{
}

void Logger::log(const Level level,
  const QString &module, const QString &message) const
{
  const QDateTime now = QDateTime::currentDateTime();

  fputs(qPrintable(QString("[%1] (%2) %3: %4\n").arg(
    now.toString(Qt::RFC2822Date), level2string(level), module, message
  )), m_output);
}

QString Logger::level2string(const Level level) const
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
