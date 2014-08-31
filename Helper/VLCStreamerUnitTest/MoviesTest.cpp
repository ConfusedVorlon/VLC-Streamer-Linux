/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#include "MoviesTest.h"

#include "MoviesHandler.h"

MoviesTest::MoviesTest()
{
}

void MoviesTest::testMovieComplete()
{
  Movie movie("../../VLCStreamer/VLCStreamerUnitTest/data/RootMovies/MyMovie");

  QCOMPARE(movie.getName(), QString("MyMovie") );
  QCOMPARE(movie.getParams().left(30), QString("{\"set\":\"Standard Low Bandwidth") );

  QCOMPARE(movie.getStatus(), QString("complete"));
}


void MoviesTest::testMovieProgress()
{
  Movie movie("../../VLCStreamer/VLCStreamerUnitTest/data/RootMovies/MyMovieProgress");

  QCOMPARE(movie.getName(), QString("MyMovieProgress") );
  QCOMPARE(movie.getParams().left(30), QString("{\"set\":\"Standard Low Bandwidth") );
  QCOMPARE(movie.getStatus(), QString("segments-206"));

  QCOMPARE(movie.getPlaylist().left(30), QString("#EXTM3U\n#EXT-X-TARGETDURATION:"));

}


void MoviesTest::testMovieJustCompleted()
{
  QDir dataDir("../../VLCStreamer/VLCStreamerUnitTest/data/RootMovies/MyMovieDone");
  dataDir.remove("complete.txt");

  Movie movie("../../VLCStreamer/VLCStreamerUnitTest/data/RootMovies/MyMovieDone");

  QCOMPARE(movie.getName(), QString("MyMovieDone") );
  QCOMPARE(movie.getParams().left(30), QString("{\"set\":\"Standard Low Bandwidth") );
  QCOMPARE(movie.getStatus(), QString("complete"));
  QCOMPARE(movie.getPlaylist().left(30), QString("#EXTM3U\n#EXT-X-TARGETDURATION:"));

  QStringList entries = dataDir.entryList();

  QVERIFY( entries.contains("complete.txt"));

}



