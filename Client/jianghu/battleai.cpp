#include "battleai.h"

BattleAI::BattleAI(QObject *parent) : QObject(parent)
{
    connect(&_skillTimer,&QTimer::timeout,this,&BattleAI::onTimerOut);
}
void BattleAI::battleStart()
{

    _skillTimer.start(qrand()%3000+1000);

}
void BattleAI::initThis()
{
    _qixue = "10000";
    emit baseInfo("神秘高手|10000|0.5|0.03");
}
void BattleAI::onTimerOut()
{
    _skillTimer.stop();
    //技能编号|伤害
    emit damage(QString::number(qrand()%101)+"|"+QString::number (qrand()%700));
    _skillTimer.start(qrand()%3000+1000);
}
void BattleAI::beDamage(QString data)
{
    auto dlist = data.split("|");
    if(dlist.count()==2)
    {
        qDebug()<<"ai"<<_qixue;
        _qixue =QString::number( _qixue.toDouble() - (dlist.at(1).toDouble()));
        emit nowQixue(_qixue);
    }
}
