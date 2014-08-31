/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef TST_URLPARSERTEST_H
#define TST_URLPARSERTEST_H

#include <QtCore/QString>
#include <QtTest/QtTest>

class UrlParserTest : public QObject
{
  Q_OBJECT

public:
  UrlParserTest();

private slots:

  void testGetCommand_data();
  void testGetCommand();

  void testDirFromBrowse_data();
  void testDirFromBrowse();

  void testDirFromGetPath_data();
  void testDirFromGetPath();

  void testSetFromCreate_data();
  void testSetFromCreate();

  void testExtractValue_data();
  void testExtractValue();

  void testExtractTag_data();
  void testExtractTag();

  void testExtractName_data();
  void testExtractName();


};

#endif // TST_URLPARSERTEST_H
