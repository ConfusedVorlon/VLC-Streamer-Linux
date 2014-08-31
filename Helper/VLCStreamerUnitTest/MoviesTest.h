/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef MOVIESTEST_H
#define MOVIESTEST_H

#include <QtCore/QString>
#include <QtTest/QtTest>

class MoviesTest : public QObject
{
  Q_OBJECT
public:
  MoviesTest();

private slots:

  void testMovieComplete();
  void testMovieProgress();

  void testMovieJustCompleted();

};

#endif // MOVIESTEST_H
