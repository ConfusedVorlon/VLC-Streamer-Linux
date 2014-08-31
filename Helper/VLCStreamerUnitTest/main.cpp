/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/
#include <QtTest/QtTest>

#include <QtCore/QProcess>

#include "UrlParserTest.h"
#include "MoviesTest.h"

int main(int argc, char *argv[])
{
  int nError = 0;

  UrlParserTest urlParserTest;
  nError += QTest::qExec(&urlParserTest, argc, argv);

  MoviesTest moviesTest;
  nError += QTest::qExec(&moviesTest, argc, argv);

  if (nError != 0)
  {
    qDebug() << "==== \nERRORS occured! \n";
  }

  return nError;

}
