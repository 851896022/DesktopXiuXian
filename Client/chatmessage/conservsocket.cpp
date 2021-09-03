#include "conservsocket.h"
#include <QEventLoop>
ConServSocket::ConServSocket(QObject *parent) : QObject(parent)
{

}
void ConServSocket::initThis()
{
    _socket= new QTcpSocket;
    linkToServer();
    connect(_socket,&QTcpSocket::readyRead,this,&ConServSocket::receivedMessage);
    QTimer *timer = new QTimer;
    connect(timer,&QTimer::timeout,this,&ConServSocket::linkToServer);
    timer->start(2000);
}
void ConServSocket::receivedMessage()
{

    //ID|昵称|头像|消息;
    QByteArray ba;
    QString data;
    for(int i=0;i<100;i++)
    {

        ba.append(_socket->readAll());
        if(ba.endsWith('\n'))
        {
            ba.remove(ba.count()-1,1);
            break;
        }
        {
            QEventLoop loop;
            QTimer t;
            connect(&t,SIGNAL(timeout()),&loop,SLOT(quit()));
            t.start(10);
            loop.exec();
        }

    }
    data = QByteArray::fromBase64(ba);
    emit receiveMessage(data);

}

void ConServSocket::linkToServer()
{
    if(_socket->state() != QAbstractSocket::ConnectedState)
    {

        emit isOpen( false);
    }
    else
    {

         emit isOpen( true);

    }
    if(_socket->state() != QAbstractSocket::ConnectedState )
    {
        QHostInfo info = QHostInfo::fromName("chat.fuh.ink");
        if(info.addresses().count()>0)
        {
            QHostAddress addr= info.addresses().at(0);

            _socket->connectToHost(addr,8524);


        }
    }


}
void ConServSocket::send(QByteArray data)
{
    _socket->write(data);
}
