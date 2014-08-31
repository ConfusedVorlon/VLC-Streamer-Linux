/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef VLCSTREAMER_H
#define VLCSTREAMER_H

#include <QtCore/QObject>

class Zeroconf;
class HttpServer;

class VLCStreamer : public QObject
{
    Q_OBJECT
public:
    explicit VLCStreamer(QObject *parent = 0);
    ~VLCStreamer(void);

    bool isRunning(void) { return m_bRunning; };

    HttpServer* getHttpServer() { return m_pHttpServer; };

signals:

  void sigStatusChanged(bool isRunning);

public slots:

  void slotStartService(void);
  void slotStopService(void);

private:

  void createRootFolder(void);

  QString m_sRootPath;

  bool m_bRunning;

  Zeroconf*   m_pZeroconf;
  HttpServer* m_pHttpServer;

};

#endif // VLCSTREAMER_H
