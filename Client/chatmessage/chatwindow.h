/*-------------------------------------------------
#
# Project created by QtCreator
# Author: 沙振宇
# CreateTime: 2018-07-23
# UpdateTime: 2019-12-27
# Info: Qt5气泡式聊天框——QListWidget+QPainter实现
# Url:https://shazhenyu.blog.csdn.net/article/details/81505832
# Github:https://github.com/ShaShiDiZhuanLan/Demo_MessageChat_Qt
#
#-------------------------------------------------*/
#ifndef ChatWindow_H
#define ChatWindow_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "chatmessage/qnchatmessage.h"
//#include <QMediaPlayer>
#include <QHostInfo>
#include <QTimer>
#include <QTcpSocket>
#include "conservsocket.h"
#include "emojipicker.h"
#include "wordfilter.h"
namespace Ui {
class ChatWindow;
}

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();

    void dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time, QNChatMessage::User_Type type);
    void dealMessageTime(QString curMsgTime);
    void setInfo(QString name,QString id);
    //QMediaPlayer preview;
protected:
    void resizeEvent(QResizeEvent *event);
    bool eventFilter(QObject *target, QEvent *event);//事件过滤器
private slots:
    /**
     * @brief on_pushButton_clicked发送按钮
     */
    void on_pushButton_clicked();
    /**
     * @brief AddMessageShe
     * @param msg
     */
    void AddMessageShe(QString msg,QString name,QString head);
    /**
     * @brief AddMessageMe
     * @param msg
     */
    void AddMessageMe(QString msg,QString head);
    /**
     * @brief receivedMessage接收Redislib返回的消息
     * @param res
     */
    void receivedMessage(QString);


    void on_btn_chooseHead_clicked();

    QByteArray Image_To_Base64(QImage image);
    QPixmap Base64_To_Image(QByteArray bytearray);

    void on_Slider_A_valueChanged(int value);

    void on_checkBox_setTop_toggled(bool checked);
    void refState(bool);
    void on_chooseEmoji_clicked();
    void addEmoji(QString);
private:
    Ui::ChatWindow *ui;


    ConServSocket css;
    QString _headPng;

    QString _name = "null";
    QString _uuid = "uuid";
    EmojiPicker _emojipic;
    WordFilter _wf;
signals:
    void sendMsg(QByteArray ba);
};

#endif // ChatWindow_H
