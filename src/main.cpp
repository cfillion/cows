#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

#include "logging.h"
#include "server.h"

LOG_MODULE("main");

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  app.setApplicationName(QStringLiteral("cows"));
  app.setApplicationVersion(QStringLiteral("0.0.1"));
  app.setOrganizationName(QStringLiteral("cfillion"));

  const QCommandLineOption listenOption(
    QStringList() << "L" << "listen",
    QObject::tr("listen for connections on the specified address (mandatory)"),
    QObject::tr("ADDRESS"), "0.0.0.0:7169"
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
    << listenOption << logFileOption
  );

  parser.process(app);

  if(!parser.isSet(listenOption)) {
    parser.showHelp();
    // showHelp stops the program execution
  }

  Logger::open(parser.value(logFileOption));

  // For some reasons that I don't quite understand, both the server and client
  // aren't listening/connecting if they are allocated on the stack.
  //
  // Help is welcome.

  Server *server = new Server;

  if(!server->open(parser.value(listenOption)))
    return -1;

  return app.exec();
}
