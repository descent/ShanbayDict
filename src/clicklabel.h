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
#ifndef CLICKLABEL_H
#define CLICKLABEL_H

#include <QLabel>

class ClickLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickLabel(QWidget *parent = 0);

signals:
    void Clicked();
public slots:
protected:
    void mouseReleaseEvent( QMouseEvent* ){
        emit Clicked();
    }
};

#endif // CLICKLABEL_H
