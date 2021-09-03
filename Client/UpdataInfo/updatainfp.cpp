#include "updatainfp.h"
#include "ui_updatainfp.h"
#include <QDebug>
UpdataInfp::UpdataInfp(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UpdataInfp)
{
    ui->setupUi(this);
    _accessManager = new QNetworkAccessManager(this);
    connect(_accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(shwInfo(QNetworkReply*)));
    refData();
    connect(&timer,&QTimer::timeout,this,&UpdataInfp::refData);
    timer.start(3600*1000);
}

UpdataInfp::~UpdataInfp()
{
    delete ui;
}
#include <QDesktopServices>
#include <QUrl>
void UpdataInfp::on_pushButton_clicked()
{
    QDesktopServices::openUrl(QUrl(_link));
}

void UpdataInfp::shwInfo(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        auto list = QString(bytes).split('|');
        if(list.count()>2)
        {
            ui->open->setHtml(list.at(2));
            if(qApp->applicationVersion() != list.at(0))
            {
                show();
            }
            _link = list.at(1);

        }

    }
    else
    {
        qDebug() << "finishedSlot errors here";
        qDebug( "found error .... code: %d\n", (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
    }
}
void UpdataInfp::refData()
{
    _accessManager->get(QNetworkRequest(QUrl("http://xiuxian.fuh.ink:8522/lastVersion")));


}
