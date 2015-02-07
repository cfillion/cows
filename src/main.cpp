#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>

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
    QObject::tr("HOST")
  );

  QCommandLineParser parser;
  parser.setApplicationDescription(QStringLiteral("Chat on Web Sockets (COWS)"));

  parser.addHelpOption();
  parser.addVersionOption();
  parser.addOptions(QList<QCommandLineOption>() << connectMode << listenMode);

  parser.process(app);

  return app.exec();
}
