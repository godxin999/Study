#include "tperson.h"

Tperson::Tperson(QString aName,QObject *parent)
    : QObject{parent}
    /*,m_name(aName)*/
{
    m_name=aName;
}

Tperson::~Tperson(){
    qDebug("Tperson对象被删除了");//在析构函数内显式信息，可以看到对象是否被删除
}

int Tperson::age(){
    return m_age;
}

void Tperson::setAge(int value){
    if(m_age!=value){
        m_age=value;
        emit ageChanged(m_age);//发射信号
    }
}

void Tperson::incAge(){
    m_age++;
    emit ageChanged(m_age);//发射信号
}
