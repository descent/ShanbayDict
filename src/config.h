/****************************************************************************
**
** Copyright (C) 2012 Jafed Li.
** All rights reserved.
** Contact: vipjafed@gmail.com
**
** This file is part of the ShanbayDict.
**
** $BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
**
**
** $END_LICENSE$
**
****************************************************************************/
#ifndef CONFIG_H
#define CONFIG_H
#include <QSettings>
class Config
{
public:
    Config();
    ~Config();
private:
    QString username,userpass;
    bool savepass,autologin,autoplay,getscreentext,getclipboardtext,
        showquerylogo,autorun,autominmum;
    QSettings *settings;
    void load();
public:

    void save();
    QString getUsername();
    void setUsername(QString name);
    QString getUserpass();
    void setUserpass(QString text);
    bool isSavepass();
    void setSavepass(bool value);
    bool isAutologin();
    void setAutologin(bool value);
    bool isAutoplay();
    void setAutoplay(bool value);
    bool isGetscreentext();
    void setScreentext(bool value);
    bool isGetclipboardtext();
    void setClipboardtext(bool value);
    bool isShowquerylogo();
    void setShowquerylogo(bool value);
    bool isAutorun();
    void setAutorun(bool value);
    bool isAutominmum();
    void setAutominmum(bool value);
};

#endif // CONFIG_H
