#include "augurwindow.h"
#include "ui_augurwindow.h"
#include <QDebug>
#include <QDateTime>
AugurWindow::AugurWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AugurWindow)
{
    ui->setupUi(this);
    for(int i=0;i<6;i++)
    {
        GuaStacked *gua = new GuaStacked;
        ui->verticalLayout->addWidget(gua);
        _guaList.append(gua);
    }
    connect(_guaList.at(5),&GuaStacked::refUiFinish,this,&AugurWindow::showGuaWen);

}

AugurWindow::~AugurWindow()
{
    delete ui;
}
#include <QSettings>
#include <QDir>
void AugurWindow::on_start_clicked()
{
    int userid =2222;

    QSettings configIni(QDir::homePath()+"/AppData/Roaming/xiuxian.ini", QSettings::IniFormat);
    if(configIni.status() == QSettings::NoError)
    {
        auto str = configIni.value("xiuxian/userid").toString();
        QByteArray ba = str.toLatin1();
        for(int i = 0 ;i<ba.count();i++)
        {
            userid=userid+ba.at(i);
        }
    }
    qsrand(QDateTime::currentDateTime().toString("yyyyMMdd").toInt()+userid);
    for(int i=0;i<6;i++)
    {
        int guaXiang =qrand() % 2;
        _guaXiang.append(guaXiang);
        _guaList.at(i)->setGuaXiang(guaXiang);
        {
            QEventLoop loop;
            QTimer t;
            connect(&t,SIGNAL(timeout()),&loop,SLOT(quit()));
            t.start(2000);
            loop.exec();
        }
    }

}
#include <QFile>
void AugurWindow::showGuaWen()
{
    QString fileName="i_";
    for(int i=0;i<6;i++)
    {
        fileName.append(QString::number(_guaXiang.at(i)));
    }
    qDebug()<<":/gua_ci/"+fileName;
    QFile f(":/gua_ci/"+fileName);
    if(f.open(QIODevice::ReadOnly))
    {
        QString data;
        {
            QString tmp = f.readLine();
            tmp.remove("原文");
            data.append("<h1>"+tmp+"</h1>");
        }
        {
            QString tmp = f.readLine();
            data.append("<h3>"+tmp+"</h3>");

        }
        {
            QString tmp = f.readLine();
            if(tmp.startsWith("象"))
            {
                data.append(tmp);
            }
        }
        bool isUseful = false;
        while(!f.atEnd())
        {

            QString tmp = f.readLine();
            if(tmp.startsWith("传"))
            {
                isUseful = true;
                tmp.remove("传统");
                data.append("<h3>"+tmp+"</h3>");
                continue;
            }
            if(tmp.startsWith("台"))
            {
                isUseful = false;
            }
            if(isUseful)
            {
                data.append(tmp+"<br>");
            }
        }
        ui->textBrowser->setText(data);
    }


}




