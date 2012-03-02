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
#include "shanbaynet.h"
#include <QtNetwork>
#include "QJson/Parser"
#include "dict.h"
ShanbayNet::ShanbayNet(QObject *parent) :
    QObject(parent)
{
    http=new QNetworkAccessManager(this);
    jsonParser = new QJson::Parser();
    http->setCookieJar(new QNetworkCookieJar(this));
    connect(http,SIGNAL(finished(QNetworkReply*)),this,SLOT(httpfinished(QNetworkReply*)));   
}
void ShanbayNet::getSessionid(){
    QNetworkRequest request;
    request.setUrl(QUrl("http://www.shanbay.com/accounts/login/"));
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    //request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","www.shanbay.com");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
    httpAction=GetSessionidAction;
    http->get(request);
}

void ShanbayNet::queryWord(const QString &word){
    QNetworkRequest request;
    queryword=word;
    request.setUrl(QUrl("http://www.shanbay.com/api/word/"+word));
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    //request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","www.shanbay.com");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
    httpAction=QueryWordAction;
    //qDebug()<<"Query "+word;
    http->get(request);
}
void ShanbayNet::addWord(){

    if(DICT::word->name.isEmpty()){
        emit signalAddwordFinished(false);
        return;
    }
    addWord(DICT::word->name);

}
void ShanbayNet::addWord(const QString &word){
    QNetworkRequest request;
    request.setUrl(QUrl("http://www.shanbay.com/api/learning/add/"+word));
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    //request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","www.shanbay.com");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
    httpAction=AddWordAction;
    http->get(request);
}

void ShanbayNet::login(const QString &u,const QString &p){
   username=u;
   password=p;
   getSessionid();
}
void ShanbayNet::loginShanbay(){
    QNetworkRequest request;
    request.setUrl(QUrl("http://www.shanbay.com/accounts/login/"));
    request.setRawHeader("Accept","text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Charset","GBK,utf-8;q=0.7,*;q=0.3");
    //request.setRawHeader("Accept-Encoding","gzip,deflate,sdch");
    request.setRawHeader("Accept-Language","zh-CN,zh;q=0.8");
    request.setRawHeader("Cache-Control","max-age=0");
    request.setRawHeader("Connection","keep-alive");
    request.setRawHeader("Host","www.shanbay.com");
    request.setRawHeader("User-Agent","Mozilla/5.0 (Windows NT 6.1) AppleWebKit/535.7 (KHTML, like Gecko) Chrome/16.0.912.63 Safari/535.7");
    request.setRawHeader("Origin","http//www.shanbay.com");
    request.setRawHeader("Referer","http://www.shanbay.com/accounts/login/");
    request.setRawHeader("Host","www.shanbay.com");
    request.setRawHeader("Content-Type","application/x-www-form-urlencoded");
    QByteArray postData;
    postData.append(QString("csrfmiddlewaretoken=%1&").arg(sessionid));
    postData.append(QString("username=%1&password=%2&").arg(QUrl::toPercentEncoding(username).constData()).arg(password));
    //qDebug()<<QString("username=%1&password=%2&").arg(QUrl::toPercentEncoding(username).constData()).arg(password);
    postData.append("login=登录&continue=home&u=1&next=");
    request.setHeader(QNetworkRequest::ContentLengthHeader,postData.size());
    httpAction=LoginAction;
    http->post(request,postData);
}

void ShanbayNet::httpfinished(QNetworkReply* reply){
//    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString().isEmpty()){
//        qDebug()<<QString::fromUtf8(reply->readAll());
//    }
    //qDebug()<<"Http request finished!"<<reply->error()<<reply->errorString()<<reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()<<reply->url();
     QVariant jsondata;
     QVariantMap jsonMap,vocMap,en_definitionsMap;
     QVariantList enList;
    switch(httpAction){
    case NoAction:
        break;
    case GetSessionidAction:
        sessionid=getCookie("csrftoken");
        //qDebug()<<"sessionid="<<sessionid<<reply->readAll();

        if(sessionidOk()){
            emit signalLoginFinished(false,"已经连接扇贝网，正在验证用户名密码...");
            loginShanbay();
        }else{
            emit signalLoginFinished(false,"无法连接扇贝网，请稍后重试");
        }
        break;
    case LoginAction:
        httpAction=NoAction;
        if(0==reply->readAll().size()){
            nickname=QUrl::fromPercentEncoding(getCookie("nickname").toAscii());
            //qDebug()<<"Login OK!nickname="<<nickname;
            emit signalLoginFinished(true,"登录成功！");
        }else{
            //qDebug()<<"Login failed!";
            emit signalLoginFinished(false,"登录失败！用户名或者密码错误");
        }
        break;
    case QueryWordAction:
        jsondata=jsonParser->parse(reply->readAll(),&jsonok);
        if(jsonok){
            jsonMap=jsondata.toMap();
            vocMap=jsonMap.value("voc").toMap();
            en_definitionsMap=vocMap.value("en_definitions").toMap();
            DICT::word->clear();
            DICT::word->name=vocMap.value("content").toString();
            DICT::word->learning_id=jsonMap.value("learning_id").toString();
            DICT::word->definition=vocMap.value("definition").toString();
            DICT::word->pron=vocMap.value("pron").toString();
            DICT::word->audio=vocMap.value("audio").toString();
            QVariantMap::const_iterator i=en_definitionsMap.constBegin();
            while(i!=en_definitionsMap.constEnd()){
                //qDebug()<<i.key()<<i.value();
                DICT::word->en_definitions+=QString("<font color='black'><b>%1.</b></font>").arg(i.key());
                enList=i.value().toList();
                for(int j=0;j<enList.size();j++){
                    DICT::word->en_definitions+=QString("%1<br>").arg(enList.at(j).toString());
                }
                i++;
            }

//            qDebug()<<QString("wordname=%1,learning_id=%2,definition=%3,pron=[%4],audio=%5,en_definition=%6")
//                      .arg(DICT::word->name)
//                      .arg(DICT::word->learning_id)
//                      .arg(DICT::word->definition)
//                      .arg(DICT::word->pron)
//                      .arg(DICT::word->audio)
//                      .arg(DICT::word->en_definitions);
            emit signalQueryFinished();

            //qDebug()<<jsonMap.value("voc").toMap().value("definition").toString();
            //qDebug()<<jsondata;
        }
        break;
    case AddWordAction:
        //qDebug()<<QString::fromUtf8(reply->readAll());
        jsondata=jsonParser->parse(reply->readAll(),&jsonok);
        if(jsonok){
            jsonMap=jsondata.toMap();
            //qDebug()<<"add word result learning id="<<jsonMap.value("id").toString();
            if(jsonMap.value("id").toString()=="0"){
                emit signalAddwordFinished(false);
            }else{
                emit signalAddwordFinished(true);
            }
        }
        break;
    default:
        //        qDebug()<<reply->rawHeader("Content-Encoding");//gzip
        //        qDebug()<<QString::fromUtf8(reply->readAll());
        break;

    }

}
QString ShanbayNet::getCookie(const QString &name){
    //qDebug()<<"get cookie";
    foreach(QNetworkCookie cookie , http->cookieJar()->cookiesForUrl(QUrl("http://www.shanbay.com/"))){
        //qDebug()<<cookie.name();
        if(cookie.name()==name){
            return cookie.value();
        }
    }
    return "";
}
