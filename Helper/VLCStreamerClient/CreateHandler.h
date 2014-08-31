/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef CREATEHANDLER_H
#define CREATEHANDLER_H

#include <QtCore/QObject>
#include <QtCore/QHash>

class CreateHandler : public QObject
{
    Q_OBJECT
public:
    CreateHandler(QString sRootFolder, QObject *parent = 0);
    ~CreateHandler();

    QString createMovie(QString sPath);

signals:

public slots:

private:

  QString createTargetFolder(QString sName);

  void writeParamsFile(QString sTargetFolder, QHash<QString, QString> sSet);

  QString m_sRootFolder;

};

#endif // CREATEHANDLER_H
