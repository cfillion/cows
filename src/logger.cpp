#include "logger.h"

#include <QDateTime>
#include <QFile>
#include <QtGlobal>

Logger *Logger::s_instance = 0;

void Logger::open(const QString &logFile)
{
  Logger::open(new Logger(logFile));
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

Logger::Logger(const QString &logFile)
  : m_file(logFile)
{
  // The macros LOG_* can't be used here since s_instance isn't set yet.

  if(!logFile.isEmpty()) {
    if(!m_file.open(QIODevice::Append | QIODevice::Text))  {
      log(ERROR, QStringLiteral("logger"),
        QString("can not open '%1' for writing: %2")
        .arg(logFile, m_file.errorString()));
    }
  }
}

Logger::~Logger()
{
  if(m_file.isOpen())
    m_file.close();
}

void Logger::log(const Level level,
  const QString &module, const QString &message)
{
  const QDateTime now = QDateTime::currentDateTime();

  const QString logLine = QString("[%1] (%2) %3: %4\n").arg(
    now.toString(Qt::RFC2822Date), level2string(level), module, message
  );

  // The value returned by qPrintable cannot be reused multiple times.
  // Storing it in a variable causes strange bugs and it might be dangerous.

  fputs(qPrintable(logLine), stderr);

  if(m_file.isOpen()) {
    m_file.write(qPrintable(logLine));
    m_file.flush();
  }
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
