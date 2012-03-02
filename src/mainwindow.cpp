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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QDesktopWidget>
#include "dict.h"
#include <QMessageBox>
#include <QDesktopServices>
#include <QFile>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    int x=(qApp->desktop()->width() - this->width())/2;
    int y=(qApp->desktop()->height() - this->height())/2;
    // this->setGeometry (x,y,this->width(),this->height());
    this->move (x,y);
    l=new Login();
    ismyadd=false;
    connect(ui->queryButton,SIGNAL(clicked()),this,SLOT(queryWordClick()));
    connect(ui->addwordButton,SIGNAL(clicked()),this,SLOT(addWordClick()));
    connect(ui->playButton,SIGNAL(clicked()),this,SLOT(playClick()));
    connect(ui->lineEdit,SIGNAL(returnPressed()),this,SLOT(queryWordClick()));
    connect(ui->backward,SIGNAL(Clicked()),this,SLOT(backwardClick()));
    connect(ui->forward,SIGNAL(Clicked()),this,SLOT(forwardClick()));
    //connect(qApp->clipboard(),SIGNAL(dataChanged()),this,SLOT(clipboardQuery()));

    //qDebug()<<qApp->clipboard()->supportsSelection()<<"supportsSelection";

    connect(l,SIGNAL(signalLoginOk()),this,SLOT(loginOk()));
    connect(DICT::msg,SIGNAL(signalMainQueryFinished()),this,SLOT(queryWordFinished()));
    connect(DICT::shanbayNet,SIGNAL(signalAddwordFinished(bool)),this,SLOT(addWordFinshed(bool)));
    connect(ui->menuButton,SIGNAL(clicked()),this,SLOT(menuClick()));

    ui->labelWord->clear();
    ui->playButton->hide();
    ui->addwordButton->hide();
    ui->playButton->setText("");
    ui->playButton->setPicture(":/img/sound0.ico",":/img/sound1.ico");
    ui->addwordButton->setText("");
    ui->addwordButton->setPicture(":/img/addword0.ico",":/img/addword1.ico");
    //QPixmap p(":/pic/logo.png");
    //p.save("d:/app.ico");
    //ui->pushButton->setIcon(p.copy(32,64,32,32));
    //ui->pushButton->setIconSize(QSize(32,32));
    //ui->playButton->setText("");
    //ui->playButton->setIconSize(QSize(32,32));
    //p.copy(32,6,48,48).save("d:/sound1.png");
    // ui->playButton->setIcon(QIcon(":/pic/sound1.png"));
    //ui->playButton->setIcon(QIcon(":/pic/logo.png"));
    //ui->playButton->setStyleSheet("QPushButton:hover{border-image:url(:/pic/sound2.png)}");
    // ui->playButton->setCursor(Qt::OpenHandCursor);
    ui->centralwidget->setLayout(ui->mainLayout);
}
MainWindow::~MainWindow()
{
    //qDebug()<<"MainWindow close";//这个析构函数不执行啊
    delete ui;
    delete l;

}
void MainWindow::closeEvent(QCloseEvent *event) {
    // qDebug()<<"MainWindow coseEventcoseEventcoseEventcoseEventcoseEvent!";
    if (DICT::trayIcon->isVisible()) {
        //        QMessageBox::information(this, tr("Systray"), tr("The program will keep running in the "
        //			"system tray. To terminate the program, "
        //			"choose <b>Quit</b> in the context menu "
        //			"of the system tray entry."));

        hide();
        event->ignore();
    }
}
void MainWindow::loginOk(){
    l->hide();
    DICT::loginFinshed(true);


    //    //////////////////////////
    //    QWebSettings *webSettings = QWebSettings::globalSettings();
    //        webSettings->setAttribute(QWebSettings::AutoLoadImages, true);
    //        webSettings->setAttribute(QWebSettings::JavascriptEnabled, true);
    //        webSettings->setAttribute(QWebSettings::PluginsEnabled, true);
    //        webSettings->setAttribute(QWebSettings::DnsPrefetchEnabled,true);
    //        //webSettings->setAttribute(QWebSettings::LinksIncludedInFocusChain, true);

    //    #if QT_VERSION >= 0x040500
    //        webSettings->setAttribute(QWebSettings::ZoomTextOnly, true);
    //    #endif
    //    #ifdef DEBUG
    //        webSettings->setAttribute(QWebSettings::DeveloperExtrasEnabled,true);
    //    #endif

    //        QString url("www.shanbay.com");

    //        QWebPage *webpage=new QWebPage();
    //        webpage->setNetworkAccessManager(DICT::shanbayNet->http);
    //        BrowserWindow *browser = new BrowserWindow(url,webpage);
    //        //browser->show();
    //        QVBoxLayout *layout = new QVBoxLayout;
    //        layout->addWidget(browser, 1);
    //        //layout->addWidget(buttonBox);
    //        ui->mainLayout->addLayout(layout);


    //    ////////////////////////
}
void MainWindow::menuClick(){
    DICT::dictMenu->popup(QCursor::pos());
}

void MainWindow::queryWordClick(){

    ui->hintinfo->clear();
    if(ui->lineEdit->text().trimmed().isEmpty()) return;
    DICT::forMainWinQuery(ui->lineEdit->text().trimmed());
    ui->lineEdit->selectAll();

}
void MainWindow::setHistoryStatus(){
    ui->backward->setEnabled(DICT::history.canBackward());
    ui->forward->setEnabled(DICT::history.canForward());
    ui->backward->setToolTip(DICT::history.getTooltip());
    ui->forward->setToolTip(DICT::history.getTooltip());
}

void MainWindow::backwardClick(){
    *DICT::word=DICT::history.backward();
    queryWordFinished();
    setHistoryStatus();

}
void MainWindow::forwardClick(){
    *DICT::word=DICT::history.forward();
    queryWordFinished();
    setHistoryStatus();

}

void MainWindow::queryWordFinished(){

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
    //    ui->labelWord->setText(QString("<font size='20' color='Fuchsia' face='Arial, Helvetica, Geneva'><b>%1</b></font><span style='font-family:\"MS Gothic\";color:\"Gray\";font-size:18px;'>[%2]</span>")
    //                           .arg(DICT::word->name)
    //                           .arg(DICT::word->pron));

    ui->wordInfo->setText(QString("<font color='Black' face='Arial, Helvetica, Geneva'>%1<span style='font-family:Segoe UI, Verdana, Lucida Sans Regular, Lucida Sans Unicode, Arial, sans-serif;color:\"Gray\"'>[%2]</span><br>%3</font><br><font color='Gray'>%4</font>")
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
        setHistoryStatus();
    }
}
void MainWindow::addWordClick(){
    ismyadd=true;
    DICT::shanbayNet->addWord();
}
void MainWindow::addWordFinshed(bool ok){
    if(!ismyadd) return;
    ismyadd=false;
    if(ok){
        qDebug()<<"添加单词成功";
        ui->hintinfo->setText("添加单词成功");
    }else{
        qDebug()<<"添加单词失败";
        ui->hintinfo->setText("添加单词失败");
    }

}

void MainWindow::playClick(){

    DICT::speak(DICT::word);

}
void MainWindow::login(){
    l->show();
}


void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if (this->isVisible()) {
            if (this->isMinimized()) {
                this->showNormal();
                this->raise();
                this->activateWindow();
                ui->lineEdit->setFocus();
                ui->lineEdit->selectAll();
            } else {
                this->hide();
            }

        } else {
            this->show();
            this->raise();
            this->activateWindow();
            ui->lineEdit->setFocus();
            ui->lineEdit->selectAll();
        }
        break;
    case QSystemTrayIcon::MiddleClick:

        break;
    default:
        ;
    }
}
void MainWindow::help(){
    qDebug("open doc");
    if(QFile::exists("help.doc")){
        QDesktopServices::openUrl(QUrl::fromLocalFile("help.doc"));
    }else{
        QMessageBox::information(this, tr("%1帮助").arg(DICT::versioninformation), tr("感谢使用扇贝词典！<br> "
                                                                "屏幕取词使用方法：鼠标双击单词或者按住鼠标左键选中单词<br>"
                                                                "屏幕鼠标取词只有在菜单中选中鼠标取词才生效。<br>"
                                                                "更多帮助信息请进入<a href='http://www.shanbay.com'><b>扇贝网</b></a>或加入扇贝词典QQ群咨询<br>"
                                                                "程序设计jafed,email:jafed@qq.com;QQ:1069759117 "
                                                                "加入扇贝词典QQ群：130406877，交流背单词经验，及时了解程序更新信息，提供软件修改建议"));
    }

}

void MainWindow::about(){
    QMessageBox::about(this, tr("关于%1").arg(DICT::versioninformation), DICT::licenceinformation);
}
