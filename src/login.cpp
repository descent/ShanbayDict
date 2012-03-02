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
#include "login.h"
#include "ui_login.h"
#include "dict.h"
#include <QDesktopServices>
#include <QUrl>
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    connect(DICT::shanbayNet,SIGNAL(signalLoginFinished(bool,QString)),this,SLOT(loginFinshed(bool,QString)));
    connect(ui->loginButton,SIGNAL(clicked()),this,SLOT(loginClick()));
    connect(ui->registerButton,SIGNAL(clicked()),this,SLOT(registerClick()));
    connect(ui->labelLogo,SIGNAL(Clicked()),this,SLOT(logoClick()));


    ui->username->setText(DICT::cfg->getUsername());
    if(!ui->username->text().isEmpty()) ui->password->setFocus();
    ui->checkBox_pass->setChecked(DICT::cfg->isSavepass());
    ui->checkBox_auto->setChecked(DICT::cfg->isAutologin());
    if(ui->checkBox_pass->isChecked()||ui->checkBox_auto->isChecked()){
        ui->password->setText(DICT::cfg->getUserpass());
    }
    if(ui->checkBox_auto->isChecked()){
        loginClick();
    }
}

Login::~Login()
{
    delete ui;
}
void Login::loginFinshed(bool ok,const QString &message){
    if(ok){
        DICT::cfg->setUsername(ui->username->text());
        if(ui->checkBox_pass->isChecked()||ui->checkBox_auto->isChecked()){
            DICT::cfg->setUserpass(ui->password->text());
        }else{
            DICT::cfg->setUserpass("");
        }
        DICT::cfg->setSavepass(ui->checkBox_pass->isChecked());
        DICT::cfg->setAutologin(ui->checkBox_auto->isChecked());
        emit signalLoginOk();
    }else{
        ui->logininfo->setText(message);
    }
}
void Login::loginClick(){
    ui->logininfo->setText("正在连接扇贝网......");
    DICT::shanbayNet->login(ui->username->text(),ui->password->text());
}
void Login::registerClick(){
    QDesktopServices::openUrl(QUrl("http://shanbay.com/referral/ref/ae222/"));
}
void Login::logoClick(){
    QDesktopServices::openUrl(QUrl("http://www.shanbay.com/"));
}
