/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#include "VLCStreamer.h"

#include "Zeroconf.h"
#include "HttpServer.h"

#include <QtCore/QDir>
#include <QtCore/QSettings>

VLCStreamer::VLCStreamer(QObject *parent) :
  QObject(parent)
{
  m_bRunning = false;

  createRootFolder();

  m_pZeroconf   = new Zeroconf(this);
  m_pHttpServer = new HttpServer(this);
}


void  VLCStreamer::createRootFolder()
{
  QSettings settings;

  m_sRootPath = settings.value("root").toString();

  QFileInfo rootPathInfo(m_sRootPath);

  if (!rootPathInfo.exists())
  {
    QDir dir;
    dir.mkpath(m_sRootPath);
  }
}


VLCStreamer::~VLCStreamer(void)
{
  slotStopService();
}


void VLCStreamer::slotStartService()
{
  if (m_bRunning) return;

  createRootFolder();

  m_pHttpServer->startServer();
  m_pZeroconf->startBroadcast();

  m_bRunning = true;

  emit sigStatusChanged(m_bRunning);
}


void VLCStreamer::slotStopService()
{
  if (!m_bRunning) return;

  m_pZeroconf->stopBroadcast();
  m_pHttpServer->stopServer();

  m_bRunning = false;

  emit sigStatusChanged(m_bRunning);
}


