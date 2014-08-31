/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QtNetwork/QTcpServer>

class MoviesHandler;
class CreateHandler;

class HttpServer : public QTcpServer
{
  Q_OBJECT

public:

  HttpServer(QObject* parent = 0);
  virtual ~HttpServer();

  void startServer(void);
  void stopServer(void);

  MoviesHandler* getMoviesHandler() { return m_pMoviesHandler; };
  CreateHandler* getCreateHandler() { return m_pCreateHandler; };

public slots:

  void slotNewConnection(void);
  void slotReadClient(void);
  void slotDiscardClient(void);

signals:

private:

  QString createResponseHeader(QString sMime="text/html");

  void sendError404(QTcpSocket* pSocket);

  void respondTest(QTcpSocket* pSocket);

  void respondCommand(QString sPath, QTcpSocket* pSocket);
  void respondBrowse(QString sPath, QTextStream* os);
  void respondGetPath(QString sPath, QTextStream* os);
  void respondMovies(QString sPath, QTextStream* os);
  void respondCreate(QString sPath, QTextStream* os);
  void respondDelete(QString sPath, QTextStream* os);

  void respondPlaylist(QString sPath, QTcpSocket* pSocket);
  void respondTSFile(QString sPath, QTcpSocket* pSocket);


//  void dispatch()

  QString m_sRootFolder;

  MoviesHandler* m_pMoviesHandler;
  CreateHandler* m_pCreateHandler;

  bool removeDir(QString dirNam);

  QHash<QString, int> m_TagCounter;

};

#endif // HTTPSERVER_H
