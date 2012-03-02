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
#include "dict.h"
#include "winhook.h"

#include <phonon>
#include <QMessageBox>
#include <QDate>
namespace DICT{
Msg *msg;
Config *cfg;
ShanbayNet *shanbayNet;
MainWindow *mainWindow=0;
BallonWindow *ballonWindow;
QSystemTrayIcon *trayIcon;
EnglishWord *word;
QMenu *dictMenu ;
QAction *getscreenwordAction;
QAction *autologinAction;
QAction *autoplayAction;
QAction *quitAction;
QAction *aboutAction;
QAction *helpAction;
bool isMainWinQuery;
Phonon::MediaObject *moo=0;
History history;

QString licenceinformation;
QString versioninformation;
void speak(EnglishWord* w){
    if(!w->audioOk()) return;
    moo->setCurrentSource(w->audio);
    moo->play();
}

Msg::Msg(){
}
Msg::~Msg(){
}
void Msg::quit(){
    close();
}
void Msg::slotQueryFinished(){
    if(isMainWinQuery){
        emit signalMainQueryFinished();
    }else{
        emit signalBallonQueryFinished();
    }
}
void Msg::getscreenwordActionClick(){
    cfg->setScreentext(getscreenwordAction->isChecked());
}
void Msg::autologinActionClick(){
    cfg->setAutologin(autologinAction->isChecked());
}
void Msg::autoplayActionClick(){
    cfg->setAutoplay(autoplayAction->isChecked());
}

void querySelectedText(QString text){
    if(cfg->isGetscreentext()){
        ballonWindow->showQueryLogo(text);
    }
}
void forlicence(){
   
}

void forBallonWinQuery(QString word){
    isMainWinQuery=false;
    shanbayNet->queryWord(word);
}

void forMainWinQuery(QString word){
    forlicence();
    isMainWinQuery=true;
    shanbayNet->queryWord(word);
}


void close(){
    //qDebug()<<autologinAction->isChecked();
    WINHOOK::unHook();
    cfg->save();
    trayIcon->hide();
    delete msg;
    delete cfg;
    delete mainWindow;
    delete ballonWindow;
    delete shanbayNet;
    //delete trayIcon;
    delete word;
    delete dictMenu;
    delete moo;
    qApp->quit();
}
void loginFinshed(bool ok){
    if(ok){
        WINHOOK::setHook();
        QString title=QObject::tr("%1-%2-%3").arg(DICT::versioninformation).arg(DICT::shanbayNet->username).arg(DICT::shanbayNet->nickname);
        mainWindow->setWindowTitle(title);
        trayIcon->setToolTip(title);
        trayIcon->show();
        if(!(cfg->isAutorun()&&cfg->isAutominmum())){
            mainWindow->show();
        }
        moo=new Phonon::MediaObject;
        Phonon::AudioOutput *device=new Phonon::AudioOutput;
        Phonon::createPath(moo,device);

    }


}

void login(){
    if(mainWindow!=0){
        delete msg;
        delete cfg;
        delete mainWindow;
        delete ballonWindow;
        delete shanbayNet;
        //delete trayIcon;
        delete word;
        delete dictMenu;
    }
    msg=new Msg();
    cfg=new Config();
    shanbayNet=new ShanbayNet();
    QObject::connect(shanbayNet,SIGNAL(signalQueryFinished()),msg,SLOT(slotQueryFinished()));
    mainWindow=new MainWindow();
    ballonWindow=new BallonWindow();
    ballonWindow->setWindowFlags(Qt::Popup);
    dictMenu  = new QMenu("菜单",QApplication::desktop());
    //dictMenu->setLayoutDirection(Qt::LeftToRight);
    helpAction =new QAction(QObject::tr("帮助"),mainWindow);
    helpAction->setIcon(QIcon(":/img/help.png"));
    quitAction = new QAction(QObject::tr("退出程序"), mainWindow);
    autologinAction=new QAction(QObject::tr("自动登录"),mainWindow);
    autoplayAction=new QAction(QObject::tr("自动发音"),mainWindow);
    getscreenwordAction=new QAction(QObject::tr("屏幕取词"),mainWindow);
    aboutAction=new QAction(QObject::tr("关于"),mainWindow);
    aboutAction->setIcon(QIcon(":/img/about.png"));
    autologinAction->setCheckable(true);
    autoplayAction->setCheckable(true);
    getscreenwordAction->setCheckable(true);
    autoplayAction->setChecked(cfg->isAutoplay());
    getscreenwordAction->setChecked(cfg->isGetscreentext());
    autologinAction->setChecked(cfg->isAutologin());
    quitAction->setIcon(QIcon(":/img/quit.png"));

    dictMenu->addAction(helpAction);
    dictMenu ->addAction(getscreenwordAction);
    dictMenu ->addAction(autoplayAction);
    dictMenu ->addAction(autologinAction);
    dictMenu->addAction(aboutAction);
    dictMenu ->addSeparator();
    dictMenu ->addAction(quitAction);
    QObject::connect(getscreenwordAction,SIGNAL(triggered()),msg,SLOT(getscreenwordActionClick()));
    QObject::connect(autoplayAction,SIGNAL(triggered()),msg,SLOT(autoplayActionClick()));
    QObject::connect(autologinAction,SIGNAL(triggered()),msg,SLOT(autologinActionClick()));
    QObject::connect(quitAction, SIGNAL(triggered()),msg, SLOT(quit()));
    QObject::connect(aboutAction,SIGNAL(triggered()),mainWindow,SLOT(about()));
    QObject::connect(helpAction,SIGNAL(triggered()),mainWindow,SLOT(help()));
    word=new EnglishWord();
    if(trayIcon==0){
        trayIcon = new QSystemTrayIcon(mainWindow);
        QIcon icon = QIcon(":/img/logo.png");
        trayIcon->setContextMenu(dictMenu );
        trayIcon->setIcon(icon);
    }

    //trayIcon->setToolTip(tr(""));
    QObject::connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), mainWindow, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    mainWindow->login();
    //mainWindow->show();
}
}
