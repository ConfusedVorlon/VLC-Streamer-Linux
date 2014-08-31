/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/

#include "CreateHandler.h"

#include "Logger.h"
#include "UrlParser.h"
#include "VLCEncoder.h"

#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QTextStream>

CreateHandler::CreateHandler(QString sRootFolder, QObject *parent) :
  QObject(parent)
{
  m_sRootFolder = sRootFolder;
}


CreateHandler::~CreateHandler()
{
}


QString CreateHandler::createMovie(QString sPath)
{
  QString sResponse;

  QHash<QString, QString> sSet = UrlParser::extractSetFromCreate(sPath);

  LOG_IN(QString("CREATE file %1").arg(sSet["file"]));

  QFileInfo fileInfo(sSet["file"]);

  if (!fileInfo.exists())
  {
    LOG_ER(QString("CREATE file no found %1").arg(sSet["file"]));

    sResponse += "{\"errorMessage\":\"Couldn't find file\"}";

    return sResponse;
  }

  QString targetFolder = createTargetFolder(fileInfo.completeBaseName());

  writeParamsFile(targetFolder, sSet);

  VLCEncoder* pVLCEncoder = new VLCEncoder(m_sRootFolder, this);
  pVLCEncoder->startEncoding(sSet["file"], targetFolder, sSet["sout"]);

  sResponse += "{\"result\":\"encoding started\"}";

  return sResponse;
}

QString CreateHandler::createTargetFolder(QString sName)
{
  QString targetFolder = QString("%1/%2").arg(m_sRootFolder).arg(sName);

  QFileInfo targetFolderInfo(targetFolder);

  if( targetFolderInfo.exists() )
  {
    int nPostFix = 1;
    QString newTargetFolder;

    while(targetFolderInfo.exists())
    {
      newTargetFolder = QString("%1_%2").arg(targetFolder).arg(nPostFix);
      targetFolderInfo.setFile(newTargetFolder);
      nPostFix += 1;
    }

    targetFolder = newTargetFolder;
  }

  LOG_DB(QString("CREATE into folder %1").arg(targetFolder));

  QDir dir;
  dir.mkpath(targetFolder);

  return targetFolder;
}

void CreateHandler::writeParamsFile(QString sTargetFolder, QHash<QString, QString> sSet)
{
  QString paramsFileName(sTargetFolder + "/params.txt");

  LOG_DB(QString("CREATE write params %1").arg(paramsFileName));

  QFile paramsFile(paramsFileName);

  if (paramsFile.open(QIODevice::WriteOnly|QIODevice::Text))
  {
    QTextStream out(&paramsFile);

    out.setCodec("UTF-8");

    out << "{";

    for (int i=0; i<sSet.keys().size(); i++)
    {
      QString name = sSet.keys()[i];

      out << QString(" \"%1\": \"%2\" ").arg(name).arg(sSet[name]);

      if (i != sSet.keys().size()-1)
      {
        out << ", ";
      }
    }

    out << "}";

    paramsFile.close();
  }

}


