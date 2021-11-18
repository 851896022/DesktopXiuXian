#include "userinfo.h"
#include "ui_userinfo.h"

UserInfo::UserInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfo)
{
    ui->setupUi(this);
    ui->labShanghai->setText(jg->userData.uShanghai);
    ui->labBaoji->setText(jg->userData.uBaoji);
    ui->labQiXue->setText(jg->userData.uQixue);
    ui->labNeili->setText(jg->userData.uNeili);
    ui->labHuajin->setText(jg->userData.uHuajin);
    ui->labYuJin->setText(jg->userData.uYujin);

    ui->labTizhi->setText(jg->userData.uTizhi);
    ui->labGengu->setText(jg->userData.uGengu);
    ui->labShenFa->setText(jg->userData.uShenfa);


    ui->labJianggong->setText(jg->userData.uJianggong);
    ui->labWeiwang->setText(jg->userData.uWeiwang);
    ui->labJifen->setText(jg->userData.uJifen);
    ui->labXiaofeiquan->setText(jg->userData.uXiaofeiquan);
}

UserInfo::~UserInfo()
{
    delete ui;
}
