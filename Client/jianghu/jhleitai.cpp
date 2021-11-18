#include "jhleitai.h"
#include "ui_jhleitai.h"
#include "jhleitaiconfirm.h"
#include "QMessageBox"
JHLeiTai::JHLeiTai(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JHLeiTai)
{
    ui->setupUi(this);
    connect(&_timer,&QTimer::timeout,this,&JHLeiTai::onTimerOut);
    ui->stackedWidget->setCurrentIndex(0);
}

JHLeiTai::~JHLeiTai()
{
    delete ui;
}

void JHLeiTai::on_start_clicked()
{
    _waitTime.setHMS(0,0,0);
    _timer.start(1000);

    ui->labMyname->setText(jg->uName);
    ui->labMyName2->setText(jg->uName);
}

void JHLeiTai::onTimerOut()
{
    _waitTime = _waitTime.addSecs(1);
    ui->lcdNumber->display(_waitTime.toString("mm:ss"));
    //临时打电脑
    if(_waitTime.second() == 6)
    {
        ui->labYourName->setText("神秘高手");
        JHLeiTaiConfirm confirm;
        if(confirm.exec() == QDialog::Accepted)
        {
            ui->labYourName->setText("神秘高手");
            fightInit();
        }
        else
        {
            initThis();
        }
        _timer.stop();

    }
}
void JHLeiTai::initThis()
{
    _waitTime.setHMS(0,0,0);
    ui->lcdNumber->display(_waitTime.toString("mm:ss"));

}
void JHLeiTai::initSkillList()
{
    QStringList killList;
    killList<<"拳"<<"打"<<"脚"<<"踢";
    //添加技能
    for(int i=0;i<killList.count();i++)
    {
        SkillBar * bar = new SkillBar;
        BaseKillInfo killinfo;
        killinfo.kCD = QString::number(1000*i+500);
        killinfo.kShanghai = QString::number(100*i+100);
        killinfo.name = killList.at(i);
        bar->initThis(killinfo);
        ui->KillLayout->addWidget(bar);
        connect(bar,&SkillBar::damage,this,&JHLeiTai::receKillBarClick);
    }
}

void JHLeiTai::startFight()
{





}
/**
 * @brief 确定对手之后信号槽连接
 */
void JHLeiTai::connetLink()
{
    connect(_battleAI,&BattleAI::baseInfo,this,&JHLeiTai::receYourInfo);
    connect(_battleAI,&BattleAI::damage,this,&JHLeiTai::beDamage);
    connect(_battleAI,&BattleAI::nowQixue,this,&JHLeiTai::receYourQixue);


    connect(this,&JHLeiTai::damage,_battleAI,&BattleAI::beDamage);
}
/**
 * @brief 接收对方初始化信息
 */
void JHLeiTai::receYourInfo(QString data)
{
    //气血|化劲|御劲
    auto dlist=data.split('|');
    if(dlist.count()>3)
    {
        _yourName = dlist.at(0);
        _yourQixue = dlist.at(1);
        _yourQixueFull= dlist.at(1);
        _yourHuajin = dlist.at(2);
        _yourYujin = dlist.at(3);
    }

}
/**
 * @brief 当自己被攻击
 */
void JHLeiTai::beDamage(QString data)
{
    //技能编号|伤害
    auto dlist = data.split("|");
    if(dlist.count()==2)
    {
        QString info;
        info += SkillData::getSkillInfo(dlist.at(0).toInt()).split('|').at(1);
        info.replace("##",_yourName);
        info.replace("%%","你");
        ui->textBrowser->append(info);
        ui->textBrowser->append("造成了"+dlist.at(1)+"伤害");
        _myQixue =QString::number( _myQixue.toDouble() - (dlist.at(1).toDouble()));
        qDebug()<<"wo"<<_myQixue;
        emit nowQixue(_myQixue);
        int value =(_myQixue.toFloat()/jg->userData.uQixue.toInt())*1000;
        if(value<0)
        {
            value = 0;
            fightEnd();
        }
        ui->progressBar_me->setValue(value);
    }
}
/**
 * @brief 刷新对方泣血
 */
void JHLeiTai::receYourQixue(QString data)
{
    //血量
    _yourQixue = data;
    qDebug()<<"收到的AI"<<data<<_yourQixueFull;
    int value = (_yourQixue.toFloat()/_yourQixueFull.toInt())*1000;
    if(value<0)
    {
        value = 0;
        fightEnd();
    }
    ui->progressBar_your->setValue(value);


}
/**
 * @brief 接收技能按钮消息
 */
void JHLeiTai::receKillBarClick(QString data)
{
    //技能名字|伤害
    auto dlist = data.split("|");
    if(dlist.count()==2)
    {
        ui->textBrowser->append("你使出了"+dlist.at(0));
        ui->textBrowser->append("给对方造成了"+dlist.at(1)+"伤害");

        emit damage("0|"+dlist.at(1));
    }
}
/**
 * @brief 初始化战斗
 */
void JHLeiTai::fightInit()
{
    //初始化界面
    initSkillList();
    ui->stackedWidget->setCurrentIndex(1);
    ui->progressBar_me->setValue(1000);
    ui->progressBar_your->setValue(1000);
    //初始化自己信息
    _myQixue = jg->userData.uQixue;
    //初始化对方信息
    if(_battleAI)
    {
        _battleAI->deleteLater();
    }
    _battleAI = new BattleAI;
    //连接槽
    connetLink();





    //让对方发送基础信息
    _battleAI->initThis();
    //自己给对方发送基础信息


    //战斗开始
    _battleAI->battleStart();
    this->startFight();


}
/**
 * @brief 结束战斗
 */
void JHLeiTai::fightEnd()
{
    disconnect(_battleAI,&BattleAI::baseInfo,this,&JHLeiTai::receYourInfo);
    disconnect(_battleAI,&BattleAI::damage,this,&JHLeiTai::beDamage);
    disconnect(_battleAI,&BattleAI::nowQixue,this,&JHLeiTai::receYourQixue);


    disconnect(this,&JHLeiTai::damage,_battleAI,&BattleAI::beDamage);
    _battleAI->deleteLater();

    if(_yourQixue.toInt()<=0)
    {//赢了
        jg->userData.uJifen = QString::number(jg->userData.uJifen.toInt()+1);
        jg->userData.uXiaofeiquan = QString::number(jg->userData.uXiaofeiquan.toInt()+1);
        QMessageBox::information(this,"胜利","战斗胜利！\n擂台积分+1\n擂台消费券+1");
    }
    else
    {//输了
        jg->userData.uJifen = QString::number(jg->userData.uJifen.toInt()-1);
        jg->userData.uXiaofeiquan = QString::number(jg->userData.uXiaofeiquan.toInt()+1);
        QMessageBox::information(this,"失败","战斗失败！\n擂台积分-1");
    }
    close();
}

