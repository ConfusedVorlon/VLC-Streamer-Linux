/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef MOVIESHANDLER_H
#define MOVIESHANDLER_H

#include <QtCore/QObject>

#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QHash>

class Movie : public QObject
{
  Q_OBJECT

public:

  Movie(QString sPath, QObject* parent=0);
  ~Movie() {}

  QString getName() { return m_sName; }
  QString getStatus() { return m_Status; }
  QString getParams() { return m_Params; }

  QString getPlaylist() { return m_Playlist; }

private:

  QFileInfo m_MovieDirInfo;

  QString m_sName;
  QString m_Status;
  QString m_Params;

  QString m_Playlist;

};




class MoviesHandler : public QObject
{
    Q_OBJECT
public:
    explicit MoviesHandler(QString sRootFolder, QObject *parent = 0);
    ~MoviesHandler(void);

    Movie* getMovieFromName(QString sName);

    QStringList getMovieNames();

    QString getResponse();

signals:

public slots:

  void updateMovies(void);

private:

  QString m_sRootFolder;
  QDir    m_RootDir;

  QHash<QString, Movie*> m_Movies;

};

#endif // MOVIESHANDLER_H
