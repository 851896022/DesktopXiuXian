#include "ranklist.h"
#include "ui_ranklist.h"
#include <QDebug>
RankList::RankList(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::RankList)
{
    ui->setupUi(this);
    accessManager = new QNetworkAccessManager(this);
    connect(accessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(showRank(QNetworkReply*)));
    on_pushButton_1_clicked();
}

RankList::~RankList()
{
    delete ui;
}

void RankList::on_pushButton_1_clicked()
{

    accessManager->get(QNetworkRequest(QUrl("http://xiuxian.fuh.ink:8522/rank/xiuwei")));

}


void RankList::on_pushButton_2_clicked()
{
    accessManager->get(QNetworkRequest(QUrl("http://xiuxian.fuh.ink:8522/rank/shengming")));
}


void RankList::on_pushButton_3_clicked()
{
    accessManager->get(QNetworkRequest(QUrl("http://xiuxian.fuh.ink:8522/rank/gongji")));
}


void RankList::on_pushButton_4_clicked()
{
    accessManager->get(QNetworkRequest(QUrl("http://xiuxian.fuh.ink:8522/rank/fangyu")));
}

void RankList::showRank(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray bytes = reply->readAll();
        ui->textBrowser->setHtml(QString(bytes));
    }
    else
    {
        qDebug() << "finishedSlot errors here";
        qDebug( "found error .... code: %d\n", (int)reply->error());
        qDebug(qPrintable(reply->errorString()));
    }
}


void RankList::on_btnJfen_clicked()
{
    accessManager->get(QNetworkRequest(QUrl("http://xiuxian.fuh.ink:8522/rank/jifen")));

}

