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

#include <QtCore/QRegExp>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QStringList>

#include <QtGui/QDesktopServices>

UrlParser::UrlParser()
{
}


QString UrlParser::extractCommand(QString sPath)
{
  QString sCommand;

  QRegExp regCommand("/secure\\?command=(\\w+)");

  int pos = regCommand.indexIn(sPath);

  if ((pos > -1) && (regCommand.captureCount()>0))
  {
    sCommand = regCommand.cap(1);
  }

  return sCommand;
}

QString UrlParser::extractDirFromBrowse(QString sPath)
{
  QString sDecodePath = QUrl::fromPercentEncoding(sPath.toLatin1());

  QString sDir;
  QRegExp regDir("/secure\\?command=browse&dir=(.*)");

  int pos = regDir.indexIn(sDecodePath);

  if ((pos > -1) && (regDir.captureCount()>0))
  {
    sDir = regDir.cap(1);
  }

  return sDir;
}


QString UrlParser::extractDirFromGetPath(QString sPath)
{
  QString sDir;

  if (sPath.contains("...home..."))
  {
    sDir = QDesktopServices::storageLocation(QDesktopServices::MoviesLocation);
  }

  if  (sPath.contains("...drives..."))
  {
    sDir = "/media";
  }

  return sDir;
}



QHash<QString, QString> UrlParser::extractSetFromCreate(QString sPath)
{
  QString sDecodePath = QUrl::fromPercentEncoding(sPath.toLatin1());

  QStringList params = sDecodePath.split("&");

  QHash<QString, QString> sValues;

  QRegExp regParam("^(\\w+)=(.*)$");
  regParam.setMinimal(true);

  foreach( QString param, params)
  {
    if (!param.contains(QRegExp("^/secure")))
    {
      int pos = regParam.indexIn(param);

      if ((pos > -1) && (regParam.captureCount() == 2))
      {
        sValues[regParam.cap(1)] = regParam.cap(2);
      }
    }
  }

  return sValues;

}



QString UrlParser::extractValue(QString sName, QString sDecodePath)
{
  QString sValue;

  QRegExp regSetName(QString("%1=(.*)&").arg(sName));

  if (sName == "sout")
  {
    regSetName.setPattern("sout=(.*)&");
  }

  QStringList numericals = QStringList() << "width" << "fr" << "ab" << "vb";

  if ( numericals.contains(sName ))
  {
    regSetName.setPattern(QString("%1=(\\d+)&").arg(sName));
  }

  regSetName.setMinimal(true);

  int pos = regSetName.indexIn(sDecodePath);

  if ((pos > -1) && (regSetName.captureCount()>0))
  {
    sValue = regSetName.cap(1);
  }

  return sValue;
}


QString UrlParser::extractNameFromPlaylist(QString sPath)
{
  QString sDecodePath = QUrl::fromPercentEncoding(sPath.toLatin1());

  QString sName;
  QRegExp regName("/(.*)/stream");

  int pos = regName.indexIn(sDecodePath);

  if ((pos > -1) && (regName.captureCount()>0))
  {
    sName = regName.cap(1);
  }

  return sName;
}



QString UrlParser::extractTagFromPlaylist(QString sPath)
{
  QString sTag;
  QRegExp regTag("tag=(.*)");

  int pos = regTag.indexIn(sPath);

  if ((pos > -1) && (regTag.captureCount()>0))
  {
    sTag = regTag.cap(1);
  }

  return sTag;
}

QString UrlParser::extractNameFromDelete(QString sPath)
{
  QString sDecodePath = QUrl::fromPercentEncoding(sPath.toLatin1());

  QString sName;
  QRegExp regName("/secure\\?command=delete&name=(.*)");

  int pos = regName.indexIn(sDecodePath);

  if ((pos > -1) && (regName.captureCount()>0))
  {
    sName = regName.cap(1);
  }

  return sName;
}
