#include "skillbar.h"
#include "ui_skillbar.h"

SkillBar::SkillBar(QWidget *parent) :
    QPushButton(parent),
    ui(new Ui::SkillBar)
{
    ui->setupUi(this);
    connect(this,&QPushButton::clicked,this,&SkillBar::onClick);
    connect(&_refUiTimer,&QTimer::timeout,this,&SkillBar::refCD);
    _refUiTimer.start(50);
}

SkillBar::~SkillBar()
{
    delete ui;
}
void SkillBar::setRefValue(float value)
{
    if(value > 0.999)
    {
        value = 0.999;
    }
    if(value<=0)
    {
        value = 0.001;
    }


    float v1,v2;
    v1=value;
    v2=value+0.0001;
    QString style = "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:"
                    +QString::number(v1)
                    +" rgba(0, 0, 0, 149), stop:"
                    +QString::number(v2)
                    +" rgba(0, 0, 0, 0));";
    setStyleSheet(style);
}
void SkillBar::onClick()
{
    _baseTime = QDateTime::currentDateTime();
    emit damage(this->text()+"|"+killInfo.kShanghai);
}
void SkillBar::initThis(BaseKillInfo bki)
{
    this->killInfo = bki;
    setText(bki.name);
    _baseTime = QDateTime::currentDateTime();
}

void SkillBar::refCD()
{
    //计算当前时间和基础时间的差

        //如果差大于CD
            //设置按钮状态为可，刷新UI
        //如果差小于CD
            //设置按钮状态为不可，刷新UI

    QDateTime timeNow = QDateTime::currentDateTime();
    qint64 mSec= _baseTime.msecsTo(timeNow);

    if(mSec>killInfo.kCD.toInt())
    {
        setEnabled(true);
        setRefValue(0);
    }
    else
    {

        setEnabled(false);
        setRefValue(1-(mSec*1.0)/killInfo.kCD.toInt());
    }
}
