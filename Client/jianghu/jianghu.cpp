#include "jianghu.h"
#include "ui_jianghu.h"

JiangHu::JiangHu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JiangHu)
{
    ui->setupUi(this);
}

JiangHu::~JiangHu()
{
    delete ui;
}
void JiangHu::setInfo(QString userName,QString userid)
{
    jg->uName = userName;
    jg->uuid = userid;
}

void JiangHu::on_userInfo_clicked()
{
    if(_userInfo)
    {
        _userInfo->deleteLater();
    }
    _userInfo = new UserInfo;
    _userInfo->show();
}


void JiangHu::on_JJC_clicked()
{
    if(_leitai)
    {
        _leitai->deleteLater();
    }
    _leitai = new JHLeiTai;
    _leitai->show();
}

