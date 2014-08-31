/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/
#include "Zeroconf.h"

#include <QtCore/QProcess>
#include <QtCore/QDebug>
#include <QtCore/QSettings>

#include "Logger.h"

Zeroconf::Zeroconf(QObject *parent)
  :QObject(parent)
{
  m_pPublishProcess = new QProcess(this);
}

Zeroconf::~Zeroconf()
{
  stopBroadcast();
}

void Zeroconf::startBroadcast()
{
  QSettings settings;

  QString sHostName = settings.value("hostname").toString();
  QString sPort     = settings.value("port").toString();

  LOG_IN(QString("ZEROCONF start broadcast"));
  LOG_IN(QString("ZEROCONF   host: %1").arg(sHostName));
  LOG_IN(QString("ZEROCONF   port: %1").arg(sPort));

  QStringList arguments;
  arguments << "-s" ;
  arguments << sHostName;
  arguments << "_hs-vlcstream._tcp";
  arguments << QString("%1").arg(sPort);
  arguments << QString("http=%1").arg(sPort);
  arguments << "version=2" << "minVersion=1" << "platform=mac";

  QString program("avahi-publish-service");

  m_pPublishProcess->start(program, arguments);
  m_pPublishProcess->waitForStarted();

}

void Zeroconf::stopBroadcast(void)
{
  LOG_IN(QString("ZEROCONF stop broadcast"));

  m_pPublishProcess->terminate();
  m_pPublishProcess->waitForFinished();
}
