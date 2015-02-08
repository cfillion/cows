#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QCoreApplication>

#include "client.h"
#include "logging.h"
#include "server.h"

LOG_MODULE("main");

int main(int argc, char *argv[])
{
  QCoreApplication app(argc, argv);

  app.setApplicationName(QStringLiteral("cows"));
  app.setApplicationVersion(QStringLiteral("0.0.1"));
  app.setOrganizationName(QStringLiteral("cfillion"));

  const QCommandLineOption connectOption(
    QStringList() << "C" << "connect",
    QObject::tr("open a interactive connection with a server"),
    QObject::tr("URL")
  );

  const QCommandLineOption listenOption(
    QStringList() << "S" << "listen",
    QObject::tr("start a server and start listening for connections"),
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
    << connectOption << listenOption
    << logFileOption
  );

  parser.process(app);

  Logger::open(parser.value(logFileOption));

  const bool isClient = parser.isSet(connectOption);
  const bool isServer = parser.isSet(listenOption);

  // For some reasons that I don't quite understand, both the server and client
  // aren't listening/connecting if they are allocated on the stack.
  //
  // Help is welcome.

  if(isClient && isServer) {
    LOG_FATAL("only one mode may be used at a tme");
    return -1;
  }
  else if(isClient) {
    Client *client = new Client;

    if(!client->open(parser.value(connectOption)))
      return -1;
  }
  else if(isServer) {
    Server *server = new Server;

    if(!server->open(parser.value(listenOption)))
      return -1;
  }
  else
    parser.showHelp();

  return app.exec();
}
