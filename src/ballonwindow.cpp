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
#include "ballonwindow.h"
#include "ui_ballonwindow.h"
#include "dict.h"
#include <QMovie>
#include <QDesktopWidget>
#include <QClipboard>

BallonWindow::BallonWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BallonWindow)
{
    ui->setupUi(this);
    timer=new QTimer();
    connect(timer,SIGNAL(timeout()), this, SLOT(autohide()));
    connect(ui->playButton,SIGNAL(clicked()),this,SLOT(playClick()));
    connect(ui->addwordButton,SIGNAL(clicked()),this,SLOT(addWordClick()));
    connect(DICT::msg,SIGNAL(signalBallonQueryFinished()),this,SLOT(queryWordFinished()));
    connect(DICT::shanbayNet,SIGNAL(signalAddwordFinished(bool)),this,SLOT(addWordFinshed(bool)));
    ismyadd=false;
    ui->labelWord->setText("");
    ui->playButton->hide();
    ui->addwordButton->hide();
    //QPixmap p(":/pic/icons.png");
    //ui->pushButton->setIcon(p.copy(32,64,32,32));
    //ui->pushButton->setIconSize(QSize(32,32));
    ui->playButton->setText("");
    ui->playButton->setPicture(":/img/sound0.ico",":/img/sound1.ico");
    ui->addwordButton->setText("");
    ui->addwordButton->setPicture(":/img/addword0.ico",":/img/addword1.ico");
    //ui->playButton->setIconSize(QSize(32,32));
    //p.copy(32,6,48,48).save("d:/sound1.png");
    // ui->playButton->setIcon(QIcon(":/pic/sound1.png"));
    //ui->playButton->setIcon(QIcon(":/pic/logo.png"));
    // ui->addwordButton->setStyleSheet("QPushButton{border-image:url(:/img/speak0.png);}QPushButton:hover{border-image:url(:/img/speak1.png);}");
    // ui->playButton->setCursor(Qt::OpenHandCursor);
    // this->setLayout(ui->mainLayout);

    logo=new ClickLabel();
    logo->setWindowFlags(Qt::FramelessWindowHint|Qt::ToolTip);
    //logo->setWindowFlags(Qt::Popup);
    connect(logo,SIGNAL(Clicked()),this,SLOT(logoClick()));
    //    QMovie *movie=new QMovie(":/pic/loading.gif");
    //    logo->setMovie(movie);
    //    movie->start();
    //logo->setGeometry(0,0,32,32);
    logo->setPixmap(QPixmap(":/img/logo.png").scaled(QSize(32,32), Qt::IgnoreAspectRatio));
}

BallonWindow::~BallonWindow()
{
    delete ui;
}
void BallonWindow::addWordClick(){
    ismyadd=true;
    DICT::shanbayNet->addWord();
}
void BallonWindow::addWordFinshed(bool ok){
    if(!ismyadd) return;
    ismyadd=false;
    if(ok){
        //qDebug()<<"添加单词成功";
        ui->hintinfo->setText("添加单词成功");
    }else{
        //qDebug()<<"添加单词失败";
        ui->hintinfo->setText("添加单词失败");
    }

}
void BallonWindow::clipboardQuery(){
    query(qApp->clipboard()->text());
}
void BallonWindow::logoClick(){
    logo->hide();
    ui->hintinfo->clear();
    query(queryText);
}
void BallonWindow::query(QString text){
    queryText=text.trimmed();
    if(queryText.isEmpty()||queryText.length()>20) return;
    DICT::forBallonWinQuery(queryText);
}

void BallonWindow::showBallon(){
    QPoint point=logo->pos()+QPoint(36,50);
    if((point.x()+this->width())>qApp->desktop()->width()){
        point.setX(qApp->desktop()->width()-this->width()-100);
    }
    if((point.y()+this->height())>qApp->desktop()->height()){
        point.setY(logo->pos().y()-this->height()+36);
    }

    this->move(point);
    this->show();
}
void BallonWindow::showQueryLogo(QString text){
    //qDebug()<<QCursor::pos()-QPoint(32,32)<<QCursor::pos();
    logo->hide();
    timer->stop();
    logo->setWindowOpacity(qreal(0.9));
    logo->move(QCursor::pos()-QPoint(36,46));
    logo->show();
    timercount=0.9;
    timer->start(500);
    queryText=text;
}

void BallonWindow::queryWordFinished(){

    showBallon();
    if(DICT::word->name.isEmpty()){
        ui->labelWord->clear();
        ui->wordInfo->setText(QString("<font size='20' color='Fuchsia'><b>%1</b></font><font size='18' color='Gray'><br>单词没找到！</font>")
                              .arg(DICT::shanbayNet->queryword));
        ui->playButton->hide();
        ui->addwordButton->hide();
        return;
    }
    ui->labelWord->setText(QString("<font size='20' color='Fuchsia' face='Arial, Helvetica, Geneva'><b>%1</b></font><span style='font-family:Segoe UI, Verdana, Lucida Sans Regular, Lucida Sans Unicode, Arial, sans-serif;color:\"Gray\";font-size:18px;'>[%2]</span>")
                           .arg(DICT::word->name)
                           .arg(DICT::word->pron));


    ui->wordInfo->setText(QString("<font color='Black' face='Arial, Helvetica, Geneva'>%1<span style='font-family:\"MS Gothic\";color:\"Gray\"'>[%2]</span><br>%3</font><br><font color='Gray'>%4</font>")
                          .arg(DICT::word->name)
                          .arg(DICT::word->pron)
                          .arg(DICT::word->definition.replace("\n","<br>"))
                          .arg(DICT::word->en_definitions));

    ui->playButton->setVisible(DICT::word->audioOk());
    ui->addwordButton->setVisible(DICT::word->wordOk());
    if(DICT::cfg->isAutoplay()){
        DICT::speak(DICT::word);
    }
    if(DICT::history.addWord(*DICT::word)){
        DICT::mainWindow->setHistoryStatus();
    }

}
void BallonWindow::playClick(){

    DICT::speak(DICT::word);

}

void BallonWindow::autohide(){
    //qDebug()<<"autohide"<<this->underMouse()<<this->hasFocus()<<timercount<<this->isActiveWindow();


    if(!logo->rect().contains(logo->mapFromGlobal(QCursor::pos()))){
        logo->setWindowOpacity(timercount);
        if(timercount<0){
            timer->stop();
            logo->hide();
        }
        timercount-=0.1;
        mousein=false;

    }else{
        if(!mousein){
            mousein=true;
            timercount=0.9;
            logo->setWindowOpacity(qreal(0.9));
        }
    }

}

void BallonWindow::closeEvent(QCloseEvent *event) {
    //qDebug()<<"closeEvent";
    event->ignore();
    this->hide();

}

void BallonWindow::showEvent(QShowEvent *event){
    //qDebug()<<"showEvent";
    //this->setWindowFlags(Qt::Dialog);
    //timercount=0;
    //timer->start(1000);
}
void BallonWindow::focusOutEvent(QFocusEvent * event){
    //qDebug("focusout");
    //this->hide();
}

