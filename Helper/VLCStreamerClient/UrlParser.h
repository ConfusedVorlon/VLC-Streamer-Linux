/****************************************************************************
**
** Copyright (C) 2011 Jens Stockhausen
** Contact: (vlc@famst.de)
**
** This file is part of the VLCStreamerClient application.
**
**
****************************************************************************/


#ifndef URLPARSER_H
#define URLPARSER_H

#include <QtCore/QString>
#include <QtCore/QHash>

class UrlParser
{
public:
    UrlParser();

    static QString extractCommand(QString sPath);

    static QString extractDirFromBrowse(QString sPath);

    static QString extractDirFromGetPath(QString sPath);

    static QHash<QString, QString> extractSetFromCreate(QString sPath);

    static QString extractValue(QString sName, QString sDecodePath);

    static QString extractNameFromPlaylist(QString sPath);

    static QString extractTagFromPlaylist(QString sPath);

    static QString extractNameFromDelete(QString sPath);

};

#endif // URLPARSER_H
