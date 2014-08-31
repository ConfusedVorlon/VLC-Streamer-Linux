/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#include "Logger.h"

#include <QtCore/QDebug>
#include <QtCore/QDateTime>
#include <QtCore/QTextStream>

#include <QtCore/QFile>

Logger* Logger::m_pInstance = 0;

Logger::Logger(int nLogLevel, QString sLogFolder)
{
  m_nLogLevel = nLogLevel;

  m_pOutFile = new QFile(sLogFolder + "/VLCStreamer.log");
}

Logger::~Logger()
{
  delete m_pOutFile;
}

void Logger::setInstance(Logger *pLogger)
{
  if (m_pInstance != 0)
  {
    delete m_pInstance;
  }

  m_pInstance = pLogger;
}


void Logger::log(int nLevel, QString sMessage)
{
  if (m_pInstance != 0)
  {
    m_pInstance->doLog(nLevel, sMessage);
  }
}


void Logger::doLog(int nLevel, QString sMessage)
{
  if (nLevel < m_nLogLevel) return;

  QString sLogMessage;

  sLogMessage = QString("%1:%2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss:zzz")).arg(sMessage);

  qDebug() << sLogMessage;

  if (m_pOutFile->open(QIODevice::Append|QIODevice::Text))
  {
      QTextStream out(m_pOutFile);
      out.setCodec("UTF-8");

      out << sLogMessage + "\n";

      m_pOutFile->close();
  }

}


