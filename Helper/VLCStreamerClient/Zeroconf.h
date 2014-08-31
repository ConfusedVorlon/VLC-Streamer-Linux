/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef ZEROCONF_H
#define ZEROCONF_H

#include <QtCore/QObject>

class QProcess;

class Zeroconf : public QObject
{
  Q_OBJECT
public:
  explicit Zeroconf(QObject *parent = 0);
  ~Zeroconf(void);

  void startBroadcast(void);
  void stopBroadcast(void);

private:

  QProcess* m_pPublishProcess;

};

#endif // ZEROCONF_H
