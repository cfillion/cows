#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QMap>

#include "logging.h"
#include "server.h"

LOG_MODULE("main");

bool setupLogging(const QString &logFile, const QString &logLevelName)
{
  QMap<QString, Logger::Level> logLevels;
  logLevels["debug"] = Logger::DEBUG;
  logLevels["info"] = Logger::INFO;
  logLevels["warning"] = Logger::WARNING;
  logLevels["error"] = Logger::ERROR;
  logLevels["fatal"] = Logger::FATAL;

  if(!logLevels.keys().contains(logLevelName.toLower())) {
    Logger::open(logFile, Logger::FATAL);

    LOG_FATAL(QString("invalid log level: %1").arg(logLevelName));

    return false;
  }

  Logger::open(logFile, logLevels[logLevelName]);

  return true;
}

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  app.setApplicationName(QStringLiteral("cows"));
  app.setApplicationVersion(QStringLiteral("0.0.1"));
  app.setOrganizationName(QStringLiteral("cfillion"));

  const QString listenDefault = QStringLiteral("0.0.0.0:7169");
  const QCommandLineOption listenOption(
    QStringList() << "L" << "listen",
    QObject::tr("listen for connections on the specified address and port"
      " (default is %1)").arg(listenDefault),
    QObject::tr("ADDRESS"), listenDefault
  );

  const QCommandLineOption logFileOption(
    QStringList() << "logfile",
    QObject::tr("write log messages to a file instead of dumping them to stderr"),
    QObject::tr("FILE")
  );

  const QCommandLineOption logLevelOption(
    QStringList() << "loglevel",
    QObject::tr("change the log verbosity level. Possible values are: "
      "FATAL, ERROR, WARNING, INFO (default) and DEBUG"),
    QObject::tr("LEVEL"), "info"
  );

  QCommandLineParser parser;
  parser.setApplicationDescription(QStringLiteral("Chat on Web Sockets (COWS)"));

  parser.addHelpOption();
  parser.addVersionOption();

  parser.addOptions(QList<QCommandLineOption>()
    << listenOption << logFileOption << logLevelOption
  );

  parser.process(app);

  if(!setupLogging(parser.value(logFileOption), parser.value(logLevelOption)))
    return -1;

  // For some reasons that I don't quite understand, both the server and client
  // aren't listening/connecting if they are allocated on the stack.
  //
  // Help is welcome.

  Server *server = new Server;

  if(!server->open(parser.value(listenOption)))
    return -1;

  return app.exec();
}
