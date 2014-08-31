/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/
#include "MoviesHandler.h"

#include "Logger.h"

#include <QtCore/QDebug>

Movie::Movie(QString sPath, QObject *parent)
  :QObject(parent)
{
  LOG_DB(QString("MOVIE new from %1").arg(sPath));

  m_MovieDirInfo.setFile(sPath);

  m_sName = m_MovieDirInfo.fileName();

  // get values from params.txt
  QString paramsFileName(sPath + "/params.txt");
  QFile paramsFile(paramsFileName);

  if (paramsFile.open(QIODevice::ReadOnly|QIODevice::Text))
  {
    m_Params = paramsFile.readAll();

    paramsFile.close();
  }

  m_Params = m_Params.replace(QRegExp("index=\"(.*\\.m3u8)\""), "index=\\\"\\1\\\"");
  m_Params = m_Params.replace(QRegExp("dst=\"(.*\\.ts)\""),   "dst=\\\"\\1\\\"");

  {
    QString streamFileName(sPath + "/stream.m3u8");
    QFile streamFile(streamFileName);
    if (streamFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
      m_Playlist = streamFile.readAll();
    }
  }

  m_Status = "not processed";

  QDir movieDir(sPath);

  QStringList entries = movieDir.entryList();

  int nSegments = 0;

  if (entries.contains("complete.txt"))
  {
    m_Status = "complete";
  }
  else if (entries.contains("stream.m3u8"))
  {
    QString streamFileName(sPath + "/stream.m3u8");

    QFile streamFile(streamFileName);

    if (streamFile.open(QIODevice::ReadOnly|QIODevice::Text))
    {
      QString sPlayListContent = streamFile.readAll();

      streamFile.close();

      nSegments = sPlayListContent.count("stream-");

      if (sPlayListContent.contains("#EXT-X-ENDLIST"))
      {
        m_Status = "complete";

        LOG_DB("MOVIE completed");

        // create complete.txt
        QFile compFile(sPath + "/complete.txt");
        compFile.open(QIODevice::WriteOnly|QIODevice::Text);
        compFile.close();

        // patch last part from 10 to 130 seconds
        if (streamFile.open(QIODevice::WriteOnly|QIODevice::Text))
        {
          int nIndex = sPlayListContent.lastIndexOf("#EXTINF:10,");
          sPlayListContent = sPlayListContent.replace(nIndex, 12, "#EXTINF:130,\n");

          LOG_IN(QString("MOVIE playlist patched at %1").arg(nIndex));

          streamFile.write(sPlayListContent.toLatin1());
          streamFile.close();
        }

      }
    }
    else
    {
      LOG_ER(QString("MOVIE error opening file %1").arg(streamFileName));
    }

    if (m_Status == "not processed")
    {
      LOG_DB(QString("MOVIE in progress %1").arg(nSegments));

      m_Status = QString("segments-%1").arg(nSegments);
    }
  }

}


MoviesHandler::MoviesHandler(QString sRootFolder, QObject *parent) :
  QObject(parent)
{
  m_sRootFolder = sRootFolder;
  m_RootDir.setPath(m_sRootFolder);

  updateMovies();
}

MoviesHandler::~MoviesHandler()
{
}


void MoviesHandler::updateMovies()
{
  foreach(Movie* pMovie, m_Movies)
  {
    delete pMovie;
  }

  m_Movies.clear();

  QFileInfoList entries = m_RootDir.entryInfoList(QDir::Dirs|QDir::NoDotAndDotDot);

  foreach(QFileInfo entry, entries)
  {
    Movie* pMovie = new Movie(entry.absoluteFilePath(), this);
    m_Movies[pMovie->getName()] = pMovie;
  }

}

Movie* MoviesHandler::getMovieFromName(QString sName)
{
  if (m_Movies.contains(sName))
  {
    return m_Movies[sName];
  }

  return 0;
}


QString MoviesHandler::getResponse()
{
  updateMovies();

  QString sResponse;

  sResponse += QString("{\"movies\": [ \n");

  QStringList movieNames = m_Movies.keys();

  for (int i=0; i<movieNames.size(); i++)
  {
    sResponse += "{ ";

    QString movieName = movieNames[i];

    Movie* movie = m_Movies[movieName];

    sResponse += QString(" \"name\": \"%1\", \n").arg(movie->getName());

    sResponse += QString(" \"status\": \"%1\", \n").arg(movie->getStatus());

    sResponse += QString(" \"params\": %1 ").arg(movie->getParams());

    sResponse += " }\n";

    if (i != movieNames.size()-1)
    {
      sResponse += QString(", \n");
    }
  }

  sResponse += QString("\n] } \n");

  return sResponse;
}

QStringList MoviesHandler::getMovieNames()
{
  return m_Movies.keys();
}



