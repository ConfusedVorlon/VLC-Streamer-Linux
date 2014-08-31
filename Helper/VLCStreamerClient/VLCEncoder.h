/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef VLCENCODER_H
#define VLCENCODER_H

#include <QtCore/QObject>

class QProcess;

class VLCEncoder : public QObject
{
    Q_OBJECT
public:
    explicit VLCEncoder(QString sRootFolder, QObject *parent = 0);

    ~VLCEncoder(void);

    static bool hasSystemValidVLC(void);

signals:

public slots:

  void startEncoding(QString sInputFile, QString sDestDir, QString sout);

  void slotEncodingDone();

private:

  void detectVLC(void);
  QString writeBashScripte(QString sDestDir, QStringList args);
  QString m_sRootFolder;

  QProcess* m_pVLCProcess;

};

#endif // VLCENCODER_H
