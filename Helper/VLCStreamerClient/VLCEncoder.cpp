/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/

#include "VLCEncoder.h"
#include "Logger.h"

#include <QtCore/QProcess>
#include <QtCore/QFile>
#include <QtCore/QUrl>
#include <QtCore/QTextStream>

VLCEncoder::VLCEncoder(QString sRootFolder, QObject *parent) :
  QObject(parent)
{
  m_sRootFolder = sRootFolder;

  m_pVLCProcess = new QProcess(this);


  connect(m_pVLCProcess, SIGNAL(finished(int)), this, SLOT(slotEncodingDone()));

  LOG_IN("ENCODER created");
}

VLCEncoder::~VLCEncoder()
{
  m_pVLCProcess->terminate();
  m_pVLCProcess->waitForFinished();
}


void VLCEncoder::startEncoding(QString sInputFile, QString sDestDir, QString sout)
{
  m_pVLCProcess->setStandardOutputFile(sDestDir + "/logVLC.log", QIODevice::Append);
  m_pVLCProcess->setStandardErrorFile(sDestDir  + "/errVLC.log", QIODevice::Append);

  QStringList args;

  args << "-I" << "dummy";
  //args << "-v";
  args << "--ignore-config";
  args << "\"" + sInputFile + "\"";
  args << "'"+sout.replace("##dest##", sDestDir)+"'";
  args << "vlc://quit";

  QString scriptFile = writeBashScripte(sDestDir, args);

  LOG_IN(QString("ENCODER started for file %1").arg(sInputFile));
  LOG_DB(QString("ENCODER execute vlc with %1").arg(args.join(" ")));

  LOG_IN(QString("ENCODER launch script %1").arg(scriptFile));

  //  m_pVLCProcess->start("vlc", args);
  m_pVLCProcess->start("/bin/bash", QStringList() << scriptFile);
  m_pVLCProcess->waitForStarted();

}

QString VLCEncoder::writeBashScripte(QString sDestDir, QStringList args)
{
  QString scriptFile = sDestDir + "/encode.sh";

  QFile encodeScript(scriptFile);
  if (encodeScript.open(QIODevice::WriteOnly|QIODevice::Text))
  {
    QTextStream out(&encodeScript);
    out.setCodec("UTF-8");

    out << "#!/bin/bash \n";
    out << "vlc ";
    out << args.join(" ");
    out << "\n";

    encodeScript.setPermissions(QFile::ReadOwner|QFile::WriteOwner|QFile::ExeOwner);

    encodeScript.close();
  }

  return scriptFile;
}

void VLCEncoder::slotEncodingDone()
{
  LOG_IN(QString("ENCODER finished withc code %1").arg(m_pVLCProcess->exitCode()));

  deleteLater();
}


bool VLCEncoder::hasSystemValidVLC(void)
{
  QProcess vlcTest;

  vlcTest.start("vlc", QStringList() << "-l");

  if (!vlcTest.waitForFinished())
  {
    LOG_ER(QString("ENCODER VLC is not working \n%1 \n%2")
           .arg(QString((vlcTest.readAllStandardOutput()))
           .arg(QString(vlcTest.readAllStandardError()))) );

    return false;
  }

  LOG_IN(QString("ENCODER VLC Version:\n  %1").arg(QString(vlcTest.readAllStandardError())));

  QString modules = vlcTest.readAllStandardOutput();
  LOG_DB(QString("ENCODER modules: \n%1").arg(modules));

  if (!modules.contains("access_output_livehttp"))
  {
    LOG_ER(QString("ENCODER Wrong version of VLC. Module access_output_livehttp is missing"));
    return false;
  }

  return true;

}

