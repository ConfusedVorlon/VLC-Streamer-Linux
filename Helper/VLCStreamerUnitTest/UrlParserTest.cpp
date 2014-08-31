/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#include "UrlParser.h"
#include "UrlParserTest.h"

#include <QtGui/QDesktopServices>

UrlParserTest::UrlParserTest()
{
}


void UrlParserTest::testGetCommand_data()
{
  QTest::addColumn<QString>("path");
  QTest::addColumn<QString>("command");

  QTest::newRow("browse") << "/secure?command=browse&dir=/var/user/joe%20nobody" << "browse";
  QTest::newRow("movies") << "/secure?command=movies" << "movies";
  QTest::newRow("getpath_home") << "/secure?command=getpath&dir=...home..." << "getpath";
  QTest::newRow("getpath_drives") << "/secure?command=getpath&dir=...drives..." << "getpath";
}

void UrlParserTest::testGetCommand()
{
  QFETCH(QString, path);
  QFETCH(QString, command);

  QCOMPARE(UrlParser::extractCommand(path), command);
}


void UrlParserTest::testDirFromBrowse_data()
{
  QTest::addColumn<QString>("path");
  QTest::addColumn<QString>("dir");

  QTest::newRow("browse default") << "/secure?command=browse&dir=%2Fhome%2Fuser" << "/home/user";
  QTest::newRow("browse with spaces") << "/secure?command=browse&dir=%2Fvar%2Fuser%2Fjoe%20nobody" << "/var/user/joe nobody";
}

void UrlParserTest::testDirFromBrowse()
{
  QFETCH(QString, path);
  QFETCH(QString, dir);

  QCOMPARE(UrlParser::extractDirFromBrowse(path), dir);
}

void UrlParserTest::testDirFromGetPath_data()
{
  QTest::addColumn<QString>("path");
  QTest::addColumn<QString>("dir");

  QTest::newRow("home") << "/secure?command=getpath&dir=...home..."
                        << QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);

  QTest::newRow("drives") << "/secure?command=gtepath&dir=...drives..."
                          << "/media";
}

void UrlParserTest::testDirFromGetPath()
{
  QFETCH(QString, path);
  QFETCH(QString, dir);

  QCOMPARE(UrlParser::extractDirFromGetPath(path), dir);
}

void UrlParserTest::testSetFromCreate_data()
{
  QTest::addColumn<QString>("path");
  QTest::addColumn<QString>("set");

  QTest::newRow("home") << "/secure?command=create&set=Standard%20Low%20Bandwidth&fr=15&keyFrame=45&vb=100&"
                        << "Standard Low Bandwidth";

}

void UrlParserTest::testSetFromCreate()
{
  QFETCH(QString, path);
  QFETCH(QString, set);

  QCOMPARE(UrlParser::extractSetFromCreate(path)["set"], set);
}


void UrlParserTest::testExtractValue_data()
{

  QString sPath =
  "/secure?command=create&"
  "padding=300&"
  "type=file&"
  "keyFrame=45&"
  "fr=15&"
  "file=/home/jens/Videos/RxPart1_ch9.mp4&"
  "sout=#transcode{vcodec=h264,soverlay,acodec=mp3,channels=2,venc=x264{profile=baseline,level=2.2,keyint=45,bframes=0,ref=1,nocabac},width=640,fps=25,vb=1200,ab=200}:std{access=livehttp{seglen=10,index=\"##dest##/stream.m3u8\",index-url=stream-###.ts},mux=ts{use-key-frames},dst=\"##dest##/stream-###.ts\"}&"
  "set=High Res, High Bandwidth&"
  "ab=200&"
  "width=320&"
  "vb=1200&"
  "date=2011-01-21 19:51:48 +0000&"
  "audioSet=Higher";


  QTest::addColumn<QString>("path");
  QTest::addColumn<QString>("name");
  QTest::addColumn<QString>("value");

  QTest::newRow("padding")  << sPath << "padding" << "300";
  QTest::newRow("type")     << sPath << "type" << "file";
  QTest::newRow("keyFrame") << sPath << "keyFrame" << "45";
  QTest::newRow("fr")       << sPath << "fr" << "15";
  QTest::newRow("file")     << sPath << "file" << "/home/jens/Videos/RxPart1_ch9.mp4";
  QTest::newRow("sout")     << sPath << "sout" << "#transcode{vcodec=h264,soverlay,acodec=mp3,channels=2,venc=x264{profile=baseline,level=2.2,keyint=45,bframes=0,ref=1,nocabac},width=640,fps=25,vb=1200,ab=200}:std{access=livehttp{seglen=10,index=\"##dest##/stream.m3u8\",index-url=stream-###.ts},mux=ts{use-key-frames},dst=\"##dest##/stream-###.ts\"}";
  QTest::newRow("set")      << sPath << "set" << "High Res, High Bandwidth";
  QTest::newRow("ab")       << sPath << "ab" << "200";
  QTest::newRow("width")    << sPath << "width" << "320";
  QTest::newRow("vb")       << sPath << "vb" << "1200";
  QTest::newRow("date")     << sPath << "date" << "2011-01-21 19:51:48 +0000";
  QTest::newRow("audioSet") << sPath << "audioSet" << "Higher";

}

void UrlParserTest::testExtractValue()
{
  QFETCH(QString, path);
  QFETCH(QString, name);
  QFETCH(QString, value);

  QHash<QString, QString> settings = UrlParser::extractSetFromCreate(path);

  QCOMPARE(settings[name], value);
}

void UrlParserTest::testExtractTag_data()
{
  QTest::addColumn<QString>("path");
  QTest::addColumn<QString>("tag");

  QTest::newRow("default") << "/RxPart1_ch9/stream.m3u8?tag=44au" << "44au";
}

void UrlParserTest::testExtractTag()
{
  QFETCH(QString, path);
  QFETCH(QString, tag);

  QCOMPARE(UrlParser::extractTagFromPlaylist(path), tag);
}

//extractNameFromPlaylist
void UrlParserTest::testExtractName_data()
{
  QTest::addColumn<QString>("path");
  QTest::addColumn<QString>("name");

  QTest::newRow("default") << "/MyMovie%20Name/stream.m3u8?tag=44au" << "MyMovie Name";
  QTest::newRow("default") << "/RxPart1_ch9/stream-001.ts" << "RxPart1_ch9";

}

void UrlParserTest::testExtractName()
{
  QFETCH(QString, path);
  QFETCH(QString, name);

  QCOMPARE(UrlParser::extractNameFromPlaylist(path), name);
}


