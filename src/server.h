#ifndef SERVER_H
#define SERVER_H

#include <QObject>

class QWebSocketServer;

class Module;
class Peer;

class Server : public QObject
{
  Q_OBJECT

public:
  Server(QObject *parent = 0);
  ~Server();

  Module *commandModule(const QString &command) const;
  QList<Peer *> peers() const { return m_peers; }

public Q_SLOTS:
  bool open(const QString &address);

private Q_SLOTS:
  void newPeer();

private:
  void loadModule(Module *module);

  QList<Module *> m_modules;
  QList<Peer *> m_peers;
  QWebSocketServer *m_server;
};

#endif
