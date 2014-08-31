/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/

#include <QtGui/QApplication>
#include <QtGui/QDesktopServices>

#include <QtCore/QSettings>
#include <QtCore/QDir>

#include <QtNetwork/QHostInfo>

#include "MainDialog.h"

#include "Logger.h"
#include "VLCStreamer.h"
#include "VLCEncoder.h"
//#include "GlobalTools.h"

void parseArgs(QStringList args)
{
  QSettings settings;

  QString sDefaultHostName = QHostInfo::localHostName();
  QString sHostName = settings.value("hostname", QVariant(sDefaultHostName)).toString();
  settings.setValue("hostname", QVariant(sHostName));
  LOG_IN(QString("SETTINGS host name: %1").arg(sHostName));

  QString sDefaultPort = "54340";
  QString sPort = settings.value("port", QVariant(sDefaultPort)).toString();
  settings.setValue("port", QVariant(sPort));
  LOG_IN(QString("SETTINGS host port: %1").arg(sPort));

  QString sDefaultRootPath = QString("%1/.Hobbyist_Software/VLC_Streamer/Root").arg(QDir::homePath());
  QString sRootPath = settings.value("root", QVariant(sDefaultRootPath)).toString();
  settings.setValue("root", QVariant(sRootPath));
  LOG_IN(QString("SETTINGS root: %1").arg(sRootPath));

  LOG_IN(QString("SETTINGS settings file %1").arg(settings.fileName()));

  if (!settings.isWritable())
  {
    LOG_ER(QString("SETTINGS could not write file"));
  }

  settings.sync();
}


int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  int nLevel = 1;

  if (a.arguments().contains("--verbose"))
  {
    nLevel = 0;
  }

  Logger::setInstance(new Logger(nLevel, QString("%1/.Hobbyist_Software/VLC_Streamer/").arg(QDir::homePath())));

  LOG_IN("VLCStreamer Client Started");



  a.setOrganizationName("Hobbyist_Software");
  a.setOrganizationDomain("Hobbyist_Software.Com");
  a.setApplicationName("VLC_Streamer");
  a.setApplicationVersion(APP_VERSION);


  LOG_IN(QString("  Version %1").arg(a.applicationVersion()));
  LOG_IN(QString("  Beta"));
  //LOG_IN(QString("  (c) J.Stockhausen"));

  parseArgs(a.arguments());

  if (!VLCEncoder::hasSystemValidVLC())
  {
    return 1;
  }

  VLCStreamer streamer;

  MainDialog w(&streamer);
  w.show();

  streamer.slotStartService();

  int ret = a.exec();

  streamer.slotStopService();

  Logger::setInstance(0);

  return ret;
}
