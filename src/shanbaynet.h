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
#ifndef SHANBAYNET_H
#define SHANBAYNET_H
#include <QObject>
namespace QJson {
class Parser;
}
class QNetworkAccessManager;
class QNetworkRequest;
class QNetworkReply;

enum HttpAction{NoAction,GetSessionidAction,LoginAction,QueryWordAction,AddWordAction};
class ShanbayNet : public QObject
{
    Q_OBJECT
public:
    explicit ShanbayNet(QObject *parent = 0);

signals:
    void signalLoginFinished(bool,const QString &);
    void signalQueryFinished();
    void signalAddwordFinished(bool);
public slots:
    void httpfinished(QNetworkReply*);
public:
    void login(const QString &u,const QString &p);
    void queryWord(const QString &word);
    void addWord(const QString &word);
    void addWord();
    QNetworkAccessManager * http;
private:
    void loginShanbay();
    QJson::Parser * jsonParser;
    bool jsonok;

    HttpAction httpAction;
    QString getCookie(const QString &name);
    void getSessionid();
    bool sessionidOk(){
        return !sessionid.isEmpty();
    }
    QString sessionid;
public:
    QString queryword;
    QString nickname;
    QString username;
    QString password;


};

#endif // SHANBAYNET_H
