/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/
#ifndef LOGGER_H
#define LOGGER_H

#define LOG_DB(msg) Logger::log(0, QString("DEBUG: %1").arg(msg));
#define LOG_IN(msg) Logger::log(1, QString("INFO : %1").arg(msg));
#define LOG_ER(msg) Logger::log(2, QString("ERROR: %1").arg(msg));

#include <QtCore/QString>

class QFile;
class QTextStream;

class Logger
{
public:

    Logger(int nLogLevel = 1, QString sLogFolder = "");
    ~Logger();

    static void setInstance(Logger* pLogger);
    static void log(int nLevel, QString sMessage);

private:
  static Logger* m_pInstance;

  void doLog(int nLevel, QString sMessage);

  int m_nLogLevel;

  QFile*       m_pOutFile;

};

#endif // LOGGER_H
