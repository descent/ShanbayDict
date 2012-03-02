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
#ifndef BALLONWINDOW_H
#define BALLONWINDOW_H
#include <QDebug>
#include <QWidget>
#include <QCloseEvent>
#include <QFocusEvent>
#include <QEvent>
#include <QShowEvent>
#include <QTimer>
#include "clicklabel.h"

namespace Ui {
class BallonWindow;
}

class BallonWindow : public QWidget
{
    Q_OBJECT
    
public:
    explicit BallonWindow(QWidget *parent = 0);
    ~BallonWindow();
    
private:
    Ui::BallonWindow *ui;
    void setWordInfo();
protected:
     void closeEvent(QCloseEvent *event);
     void showEvent (QShowEvent * event );
     void focusOutEvent(QFocusEvent *);
private slots:
     void autohide();
     void playClick();
     void queryWordFinished();
     void addWordClick();
     void addWordFinshed(bool ok);
     void logoClick();
     void clipboardQuery();

private:
    ClickLabel *logo;
    QTimer *timer;
    qreal timercount;
    bool mousein;
    void showBallon();
    bool ismyadd;
    QString queryText;
    void query(QString text);
public:
    void showQueryLogo(QString text);
};

#endif // BALLONWINDOW_H
