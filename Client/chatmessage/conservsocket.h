#ifndef CONSERVSOCKET_H
#define CONSERVSOCKET_H

#include <QObject>
#include <QHostInfo>
#include <QTimer>
#include <QTcpSocket>
class ConServSocket : public QObject
{
    Q_OBJECT
public:
    explicit ConServSocket(QObject *parent = nullptr);


    bool state =false;
public slots:
    /**
     * @brief receivedMessage接收Redislib返回的消息
     * @param res
     */
    void receivedMessage();
    /**
     * @brief linkToServer
     */
    void linkToServer();
    void initThis();
    void send(QByteArray);
private:

    QTcpSocket *_socket;
signals:
    void receiveMessage(QString);
    void isOpen(bool);

};

#endif // CONSERVSOCKET_H
