#include "guastacked.h"
#include "ui_guastacked.h"
#include <QTimer>
#include <QEventLoop>
#include <QDebug>
GuaStacked::GuaStacked(QWidget *parent) :
    QStackedWidget(parent),
    ui(new Ui::GuaStacked)
{
    ui->setupUi(this);
    connect(&_timer,&QTimer::timeout,this,&GuaStacked::refUI);
    _timer.start(100);
}

GuaStacked::~GuaStacked()
{
    delete ui;
}

void GuaStacked::setGuaXiang(int gua)
{
    _gua=gua;
}
void GuaStacked::refUI()
{
    if(_gua<50)
    {


        if(count == 120)
        {
            _timer.stop();
            setCurrentIndex(_gua);
            emit refUiFinish();
        }
        else
        {

            setCurrentIndex(count%2);
            ++count;
            _timer.start(count);
        }


    }

}
