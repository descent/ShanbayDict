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
#include "englishword.h"
EnglishWord::EnglishWord()
{

}
EnglishWord::EnglishWord(const EnglishWord &w){

    name=w.name;
    learning_id=w.learning_id;
    definition=w.definition;
    en_definitions=w.en_definitions;
    pron=w.pron;
    audio=w.audio;

}

EnglishWord::~EnglishWord(){
}
bool EnglishWord::operator ==(const EnglishWord &w){
    return name==w.name;
}



void EnglishWord::clear(){
    this->name.clear();
    this->learning_id.clear();
    this->definition.clear();
    this->en_definitions.clear();
    this->pron.clear();
    this->audio.clear();
}
