#include "jhleitaiconfirm.h"
#include "ui_jhleitaiconfirm.h"

JHLeiTaiConfirm::JHLeiTaiConfirm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JHLeiTaiConfirm)
{
    ui->setupUi(this);
    connect(&_timer,&QTimer::timeout,this,&JHLeiTaiConfirm::onTimerOut);
    _timer.start(500);
}

JHLeiTaiConfirm::~JHLeiTaiConfirm()
{
    delete ui;
}

void JHLeiTaiConfirm::on_btnCancel_clicked()
{
    this->reject();
}


void JHLeiTaiConfirm::on_btnOk_clicked()
{
    this->accept();
}

void JHLeiTaiConfirm::onTimerOut()
{
    ui->progressBar->setValue(ui->progressBar->value()-1);
    if(ui->progressBar->value() == 0)
    {
        _timer.stop();
        this->reject();

    }
}
