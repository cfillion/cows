#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

#include "logger.h"

LOG_MODULE("main");

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  app.setApplicationName(QStringLiteral("cows"));
  app.setApplicationVersion(QStringLiteral("0.0.1"));
  app.setOrganizationName(QStringLiteral("cfillion"));

  const QCommandLineOption connectMode(
    QStringList() << "C" << "connect",
    QObject::tr("open a interactive connection with a server"),
    QObject::tr("URL")
  );

  const QCommandLineOption listenMode(
    QStringList() << "S" << "listen",
    QObject::tr("start a server and start listening for connections"),
    QObject::tr("HOST"), "0.0.0.0:7169"
  );

  const QCommandLineOption logFileOption(
    QStringList() << "l" << "logfile",
    QObject::tr("write log messages to a file instead of dumping them to stderr"),
    QObject::tr("FILE")
  );

  QCommandLineParser parser;
  parser.setApplicationDescription(QStringLiteral("Chat on Web Sockets (COWS)"));

  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOptions(QList<QCommandLineOption>()
    << connectMode << listenMode
    << logFileOption
  );

  parser.process(app);

  Logger::open(stderr);

  const bool isClient = parser.isSet(connectMode);
  const bool isServer = parser.isSet(listenMode);

  if(isClient && isServer) {
    LOG_FATAL(QObject::tr("can not act as both a client and a server"));
    return -1;
  }
  else if(isClient) {
  }
  else if(isServer) {
  }
  else
    parser.showHelp();

  return app.exec();
}
