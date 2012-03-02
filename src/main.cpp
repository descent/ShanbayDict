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
#include <QtGui>
#include "dict.h"

int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("gb18030"));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("gb18030"));
    QTextCodec::setCodecForTr(QTextCodec::codecForName("gb18030"));
    //�ȱ��ʵ�汾��Ϣ
    DICT::versioninformation=QObject::tr("�ȱ��ʵ�1.201202.c");
    //����ʹ�ñ����ȫ�����߲��ִ���ĳ��������Ա�뱣��lience��Ϣ��������������������ڴ����н�����ʾ��
    DICT::licenceinformation=
    QObject::tr("<h2>%1</h2>"
                        "�ȱ��ʵ�����<a href='http://www.shanbay.com'><b>�ȱ���</b></a>������API��ƣ��ȱ�������רҵ�ı�������վ<br> "
                        "<p>Copyright(C) 2012 Jafed Li All rights reserved.</p>"
                        "<p>��ȫ��ѭLGPLЭ��,δ��������������Ϊ��ҵ��;��<a href='http://jafed.github.com/ShanbayDict/'><b>�ȱ��ʵ俪Դ��ַ</b></a></p>"
                        "<p><h3>�������:Jafed Li<br>email:vipjafed@gmail.com<br>"
                        "QQ:1069759117 �ȱ��ʵ�QQȺ��130406877</h3</p>"
                        "<p>������ж��ƽ̨�İ汾����linux��mac��windows��</p>").arg(DICT::versioninformation);
    DICT::login();
    return a.exec();
}
