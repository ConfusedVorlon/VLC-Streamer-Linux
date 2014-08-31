/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/
#include "HttpServer.h"
#include "Logger.h"
#include "UrlParser.h"

#include "MoviesHandler.h"
#include "CreateHandler.h"

#include <QtNetwork/QTcpSocket>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QSettings>

HttpServer::HttpServer(QObject *parent)
  :QTcpServer(parent)
{
  m_pMoviesHandler = 0;
  m_pCreateHandler = 0;

  QSettings settings;
  m_sRootFolder = settings.value("root").toString();

  connect(this,SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
}

HttpServer::~HttpServer()
{
  stopServer();
}

void HttpServer::startServer()
{
  QSettings settings;

  int nPort     = settings.value("port").toInt();
  m_sRootFolder = settings.value("root").toString();

  LOG_IN(QString("HTTP server started on port %1").arg(nPort));

  m_pMoviesHandler = new MoviesHandler(m_sRootFolder, this);
  m_pCreateHandler = new CreateHandler(m_sRootFolder, this);

  listen(QHostAddress::Any, nPort);
}

void HttpServer::stopServer()
{
  delete m_pMoviesHandler;
  delete m_pCreateHandler;

  m_pMoviesHandler = 0;
  m_pCreateHandler = 0;

  close();
}

void HttpServer::slotNewConnection()
{
  QTcpSocket* socket = nextPendingConnection();

  LOG_DB(QString("HTTP new connection from %1").arg(socket->peerAddress().toString()));

  connect(socket, SIGNAL(readyRead()), this, SLOT(slotReadClient()));
  connect(socket, SIGNAL(disconnected()), this, SLOT(slotDiscardClient()));
}

void HttpServer::slotDiscardClient()
{
  QTcpSocket* socket = (QTcpSocket*)sender();

  LOG_DB(QString("HTTP discard connection from %1").arg(socket->peerAddress().toString()));

  socket->close();
  socket->deleteLater();
}


void HttpServer::slotReadClient()
{
  QTcpSocket* socket = (QTcpSocket*)sender();

  if (socket->canReadLine())
  {
    QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));

    if (tokens.size()<2)
    {
      sendError404(socket);
      return;
    }

    QString verb = tokens[0];
    QString path = tokens[1];

    if (verb == "GET")
    {

      LOG_IN(QString("HTTP request %1").arg(QUrl::fromPercentEncoding(path.toLatin1())));

      // Test
      if ((path == QString("/"))||(path == QString("/T") )||(path == QString("/t") ))
      {
        LOG_DB("HTTP test");
        respondTest(socket);
        return;
      }

      //Commands
      if (path.contains("/secure"))
      {
        LOG_DB("HTTP command");
        respondCommand(path, socket);
        return;
      }

      //Playlists
      if (path.contains(".m3u8"))
      {
        LOG_DB("HTTP playlist");
        respondPlaylist(path, socket);
        return;
      }

      //TS files
      if (path.contains(".ts"))
      {
        LOG_DB("HTTP ts file");
        respondTSFile(path, socket);
        return;
      }


      respondTest(socket);
    }
  }
}


QString HttpServer::createResponseHeader(QString sMime/*="text/html"*/)
{
  QString sHeader;

  sHeader += "HTTP/1.1 200 Ok\r\n";
  sHeader += "Date: " + QDateTime::currentDateTime().toString() +"\r\n";
  // TODO: add correct length
  //sHeader += "Content-Length: 579\r\n";
  sHeader += QString("Content-Type: %1;charset=UTF-8\r\n").arg(sMime);
  sHeader += "Server: QtSocket Web Server \r\n";
  sHeader += "Keep-Alive: timeout=20, max=400\r\n";
  sHeader += "Connection: Keep-Alive\r\n";
  sHeader += "\r\n";

  return sHeader;
}


void HttpServer::sendError404(QTcpSocket* pSocket)
{
  //TODO: response with the correct 404 code
  QTextStream os(pSocket);
  os.setAutoDetectUnicode(true);
  os << createResponseHeader();
  os << "<h1>Error 404 </h1>\n";
  pSocket->close();
}



void HttpServer::respondTest(QTcpSocket* pSocket)
{
  QTextStream os(pSocket);
  os.setAutoDetectUnicode(true);
  os << createResponseHeader();

  os << "<html xmlns='http://www.w3.org/1999/xhtml'>"
        "<head>"
        "<meta http-equiv='Content-Type' content='text/html; charset=UTF-8' />"
        "<title>VLC Streamer</title>"
        "</head>"
        "<body>"
        "<h2>This is the VLC Steamer</h2>"
        "<p>For more information, <a href='http://Hobbyistsoftware.com/VLCStreamer'>Click Here</a> </p>"
        "</body>"
        "</html>";

  pSocket->close();
}


void HttpServer::respondCommand(QString sPath, QTcpSocket* pSocket)
{
  QTextStream os(pSocket);
  os.setAutoDetectUnicode(true);
  os << createResponseHeader();

  QString sCommand = UrlParser::extractCommand(sPath);

  if(sCommand == "browse")
  {
    respondBrowse(sPath, &os);
  }
  else if (sCommand == "getpath")
  {
    respondGetPath(sPath, &os);
  }
  else if(sCommand == "movies")
  {
    respondMovies(sPath, &os);
  }
  else if (sCommand == "create")
  {
    respondCreate(sPath, &os);
  }
  else if (sCommand == "delete")
  {
    respondDelete(sPath, &os);
  }

  pSocket->close();
}

// browse
void HttpServer::respondBrowse(QString sPath, QTextStream* os)
{
  QString sResponse;

  QString sDir = UrlParser::extractDirFromBrowse(sPath);
  QFileInfo sDirInfo(sDir);

  if (!sDirInfo.exists())
  {
    sResponse += "{\"result\":\"\",\"errorMessage\":\"File or folder not found\"}";
  }
  else
  {
    sResponse += "{\"files\" : [ \r\n";

    QDir folder(sDir);

    QFileInfoList entries = folder.entryInfoList(QDir::Dirs|QDir::Files|QDir::NoDotAndDotDot/*, QDir::DirsFirst|QDir::Name*/);

    for (int i=0; i<entries.size(); i++)
    {
      QFileInfo entry = entries.at(i);

      sResponse += QString("{ \"name\" :\"%1\", ").arg(entry.fileName());
      sResponse += QString("  \"path\" :\"%1\", ").arg(entry.absoluteFilePath());
      QString sType = "file";
      if (entry.isDir()) sType = "dir";
      sResponse += QString("  \"type\" :\"%1\" }").arg(sType);

      if (i!=entries.size()-1)
      {
        sResponse += ",\r\n";
      }

    }

    sResponse += QString("\r\n], \"root\":\"%1\"}").arg(sDir);
  }

  LOG_DB("HTTP response browse");
  LOG_DB("---------------");
  LOG_DB(sResponse);
  LOG_DB("---------------");

  (*os) << sResponse;
}

// getpath
void HttpServer::respondGetPath(QString sPath, QTextStream* os)
{
  QString sResponse;

  QString sDir = UrlParser::extractDirFromGetPath(sPath);

  sResponse = QString("{\"root\":\"%1\"}").arg(sDir);

  LOG_DB("HTTP response getpath");
  LOG_DB("---------------");
  LOG_DB(sResponse);
  LOG_DB("---------------");

  (*os) << sResponse;
}


void HttpServer::respondMovies(QString sPath, QTextStream* os)
{
  QString sResponse;

  sResponse = m_pMoviesHandler->getResponse();

  LOG_DB("HTTP response Movies");
  LOG_DB("---------------");
  LOG_DB(sResponse);
  LOG_DB("---------------");

  (*os) << sResponse;
}


void HttpServer::respondCreate(QString sPath, QTextStream* os)
{
  QString sResponse = m_pCreateHandler->createMovie(sPath);


  LOG_DB("HTTP response create");
  LOG_DB("---------------");
  LOG_DB(sResponse);
  LOG_DB("---------------");

  (*os) << sResponse;
}


void HttpServer::respondDelete(QString sPath, QTextStream *os)
{
  QString sResponse;

  QString sName = UrlParser::extractNameFromDelete(sPath);

  LOG_IN(QString("DELETE movie in %1").arg(sName));

  removeDir(m_sRootFolder + "/" + sName);

  LOG_DB("HTTP response create");
  LOG_DB("---------------");
  LOG_DB(sResponse);
  LOG_DB("---------------");

  (*os) << sResponse;
}


void HttpServer::respondPlaylist(QString sPath, QTcpSocket* pSocket)
{
  QTextStream os(pSocket);
  os.setAutoDetectUnicode(true);
  os << createResponseHeader("application/vnd.apple.mpegurl");

  QString sResponse;

  QString sName = UrlParser::extractNameFromPlaylist(sPath);
  QString sTag  = UrlParser::extractTagFromPlaylist(sPath);

  LOG_IN(QString("PLAYLIST for movie %1 with tag \"%2\" ").arg(sName).arg(sTag));
  LOG_IN(QString("   (tag should be empty for current version)"))

  Movie* pMovie = m_pMoviesHandler->getMovieFromName(sName);

  if (pMovie != 0)
  {
    if (!m_TagCounter.contains(sTag))
    {
      m_TagCounter[sTag] = 0;
    }
    else
    {
      m_TagCounter[sTag] += 1;
    }

    LOG_DB(QString("PLAYLIST tag count %1 #%2").arg(sTag).arg(m_TagCounter[sTag]));

    if ((m_TagCounter[sTag]>=4) || (pMovie->getStatus() == "complete"))
    {
      LOG_DB("HTTP playlist full");
      sResponse += pMovie->getPlaylist();
    }
    else
    {
      LOG_IN("HTTP playlist short");
      sResponse =
          "#EXTM3U\n"
          "#EXT-X-TARGETDURATION:10\n"
          "#EXT-X-MEDIA-SEQUENCE:1\n"
          "#EXTINF:10,\n"
          "stream-001.ts\n";
    }
  }
  else
  {
    LOG_ER(QString("PLAYLIST no movie with name %1").arg(sName));
  }

  LOG_DB("HTTP response playlist");
  LOG_DB("---------------");
  LOG_DB(sResponse);
  LOG_DB("---------------");

  os << sResponse;

  pSocket->close();
}

void HttpServer::respondTSFile(QString sPath, QTcpSocket* pSocket)
{
  QString sTSName = m_sRootFolder + QUrl::fromPercentEncoding(sPath.toLatin1());

  QFile in(sTSName);

  if (in.open(QIODevice::ReadOnly))
  {
    long nBytes = pSocket->write(in.readAll());

    if (!pSocket->waitForBytesWritten())
    {
      //TODO sending error
    }

    LOG_DB(QString("HTTP respond TS %1 %2 Bytes").arg(sPath).arg(nBytes));
  }
  else
  {
    LOG_ER(QString("HTTP opening file %1").arg(sTSName));
  }

  pSocket->close();
}


bool HttpServer::removeDir(QString dirName)
{
  bool result = true;
  QDir dir(dirName);

  if (dir.exists(dirName))
  {
    foreach(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
    {
      if (info.isDir())
      {
        result = removeDir(info.absoluteFilePath());
      }
      else
      {
        result = QFile::remove(info.absoluteFilePath());
      }

      if (!result)
      {
        return result;
      }
    }
    result = dir.rmdir(dirName);
  }

  return result;
}

