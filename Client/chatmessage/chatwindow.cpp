
#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDateTime>
#include <QDebug>
#include<QKeyEvent>
#include <QFileDialog>
#include <QBuffer>
#include <QSettings>
#include <QThread>

ChatWindow::ChatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->textEdit->installEventFilter(this);//设置完后自动调用其eventFilter函数
    resize(600, 800);
    setWindowFlag(Qt::Window);

    //头像
    QSettings configIni(QDir::homePath()+"/AppData/Roaming/xiuxian_chat.ini", QSettings::IniFormat);
    ui->btn_chooseHead->setIcon((Base64_To_Image(_headPng.toLatin1())));
    if(configIni.status() == QSettings::NoError)
    {
        _headPng = configIni.value("xiuxian/head").toString();
        ui->btn_chooseHead->setIcon((Base64_To_Image(_headPng.toLatin1())));
    }
    //QTimer *timer = new QTimer;
    //connect(timer,&QTimer::timeout,this,&ChatWindow::refState);
    //timer->start(2000);

    //socket
    QThread *th = new QThread;
    css.moveToThread(th);
    connect(th,&QThread::started,&css,&ConServSocket::initThis);
    connect(&css,&ConServSocket::receiveMessage,this,&ChatWindow::receivedMessage);
    connect(this,&ChatWindow::sendMsg,&css,&ConServSocket::send);
    connect(&css,&ConServSocket::isOpen,this,&ChatWindow::refState);
    th->start();

     connect(&_emojipic,&EmojiPicker::chooseEmoji,this,&ChatWindow::addEmoji);


}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_pushButton_clicked()
{

    QString msg = ui->textEdit->toPlainText();
    msg.remove("\r");
    msg.remove("\n");
    msg.remove(" ");
    msg.remove("\t");
    if(msg.isEmpty())
    {
        return;
    }
    if(msg.count()>100)
    {
        msg.chop(msg.count()-100);
    }
    //ai.WriteMessage(msg);
    ui->textEdit->setText("");
    //AddMessageMe(msg);
    //ID|昵称|头像|消息;

    msg = _uuid+'|'+_name+'|'+_headPng+'|'+_wf.filter(msg);
    qDebug()<<msg.count();
    QByteArray ba= QByteArray(msg.toStdString().data()).toBase64();
    ba.append('\n');
            emit sendMsg( ba);




}
void ChatWindow::receivedMessage(QString msg)
{
    qDebug()<<msg;
    auto list = msg.split('|');

    if(list.count()>3)
    {
        if(list.at(3).count()>110)
        {
            return;
        }
        QApplication::alert(this);
        if(list.at(0) == _uuid)
        {
            AddMessageMe(list.at(3),list.at(2));
        }
        else
        {
            AddMessageShe(list.at(3),list.at(1),list.at(2));
        }
    }
}

void ChatWindow::AddMessageShe(QString msg,QString name,QString head)
{
    //preview.play();
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

    if(msg != "") {
        dealMessageTime(time);

        QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget(),name,head);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        dealMessage(messageW, item, msg, time, QNChatMessage::User_She);
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);

}
void ChatWindow::AddMessageMe(QString msg,QString head)
{
    QString time = QString::number(QDateTime::currentDateTime().toTime_t()); //时间戳

    bool isSending = false; // 发送中
    if(isSending) {
        dealMessageTime(time);

        QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget(),_name,head);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
    }
    else
    {
        bool isOver = true;
        //for(int i = ui->listWidget->count() - 1; i > 0; i--) {
        //   QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        //    if(messageW->text() == msg)
        //    {
        //        isOver = false;
        //        messageW->setTextSuccess();
        //    }
        //}

        if(isOver)
        {
            dealMessageTime(time);

            QNChatMessage* messageW = new QNChatMessage(ui->listWidget->parentWidget(),_name,head);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, msg, time, QNChatMessage::User_Me);
            messageW->setTextSuccess();
        }
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);

}

void ChatWindow::dealMessage(QNChatMessage *messageW, QListWidgetItem *item, QString text, QString time,  QNChatMessage::User_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setText(text, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}

void ChatWindow::dealMessageTime(QString curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime.toInt();
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        QNChatMessage* messageTime = new QNChatMessage(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        messageTime->setText(curMsgTime, curMsgTime, size, QNChatMessage::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}

void ChatWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);


    ui->textEdit->resize(this->width() - 20, ui->widget->height() - 20);
    ui->textEdit->move(10, 10);

    ui->pushButton->move(ui->textEdit->width()+ui->textEdit->x() - ui->pushButton->width() - 10,
                         ui->textEdit->height()+ui->textEdit->y() - ui->pushButton->height() - 10);

    ui->chooseEmoji->move(ui->textEdit->width()+ui->textEdit->x() - ui->chooseEmoji->width() - 10,
                          ui->pushButton->y() - ui->chooseEmoji->height() - 10);




    ui-> state->move(ui->textEdit->width()+ui->textEdit->x() - ui->pushButton->width() - ui-> state->width() - 20,
                       ui->textEdit->height()+ui->textEdit->y() - ui->pushButton->height() - 10);

    ui->checkBox_setTop->move(ui->textEdit->width()+ui->textEdit->x() -ui->pushButton->width()- ui-> state->width()- ui->checkBox_setTop->width()- 20 ,
                              ui->textEdit->y() + ui->textEdit->height() - ui->checkBox_setTop->height() - 10
                              );

    ui->btn_chooseHead->move(ui->textEdit->x()+10,
                             ui->textEdit->y() + ui->textEdit->height() - ui->btn_chooseHead->height() - 10);



    int SliderW= (ui->checkBox_setTop->x()-10) - (ui->btn_chooseHead->x()+ui->btn_chooseHead->width() +10);

    ui->Slider_A->setGeometry(ui->btn_chooseHead->x() +ui->btn_chooseHead->width()+10,
                              ui->textEdit->height()+ui->textEdit->y() - ui->Slider_A->height() - 10,
                              SliderW,
                              ui->Slider_A->height()
                              );

    for(int i = 0; i < ui->listWidget->count(); i++) {
        QNChatMessage* messageW = (QNChatMessage*)ui->listWidget->itemWidget(ui->listWidget->item(i));
        QListWidgetItem* item = ui->listWidget->item(i);

        dealMessage(messageW, item, messageW->text(), messageW->time(), messageW->userType());
    }
}
bool ChatWindow::eventFilter(QObject *target, QEvent *event)
{

    if(1||target == ui->textEdit)
    {
        if(event->type() == QEvent::KeyPress)//回车键
        {
             QKeyEvent *k = static_cast<QKeyEvent *>(event);
             if(k->key() == Qt::Key_Return)
             {
                 on_pushButton_clicked();
                 return true;
             }
        }
    }
    return QWidget::eventFilter(target,event);
}

void ChatWindow::setInfo(QString name,QString id)
{
    _name=name;
    _uuid=id;
}

void ChatWindow::on_btn_chooseHead_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        "选择一张图片",
        "C:/",
        "images(*.png *.jpeg *.jpg *.bmp *.gif);*.*");
    if (fileName.isEmpty()) {
        return;
    }
    else {

        QPixmap pix_head(fileName);
       pix_head = pix_head.scaled (50, 50);

        QByteArray ba_head =  Image_To_Base64(pix_head.toImage());
        _headPng = QString(ba_head);

         ui->btn_chooseHead->setIcon((Base64_To_Image(_headPng.toLatin1())));
         //打开文件

         QSettings configIni(QDir::homePath()+"/AppData/Roaming/xiuxian_chat.ini", QSettings::IniFormat);
         //写入数据，必须指定写入节点和值
         configIni.setValue("xiuxian/head",_headPng);


    }
}
QByteArray ChatWindow::Image_To_Base64(QImage image)
{

    qDebug()<<image.size()<<image.sizeInBytes();
    QByteArray ba;
    QBuffer buf(&ba);
    image.save(&buf,"PNG",20);
    QByteArray hexed = ba.toBase64();
    buf.close();
    return hexed;
}
QPixmap ChatWindow::Base64_To_Image(QByteArray bytearray)
{
    QByteArray Ret_bytearray;
    Ret_bytearray = QByteArray::fromBase64(bytearray);
    QBuffer buffer(&Ret_bytearray);
    buffer.open(QIODevice::WriteOnly);
    QPixmap imageresult;
    imageresult.loadFromData(Ret_bytearray);
    return imageresult;
}


void ChatWindow::on_Slider_A_valueChanged(int value)
{
    setWindowOpacity(value/100.0);
}


void ChatWindow::on_checkBox_setTop_toggled(bool checked)
{
    if(checked)
    {
        setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);
        show();
    }
    else
    {
        setWindowFlags(windowFlags() &  ~Qt::WindowStaysOnTopHint);
        show();
    }
}
void ChatWindow::refState(bool state)
{
    if(!state)
    {


         ui-> state->setStyleSheet("background-color: rgb(255, 100, 100);");
        ui-> state->setText("连接断开");
    }
    else
    {
       ui-> state->setStyleSheet("background-color: rgb(100, 255, 100);");
        ui-> state->setText("连接正常");
    }
}

void ChatWindow::on_chooseEmoji_clicked()
{
    ;
    _emojipic.show();
    _emojipic.move(QCursor::pos());
}
void ChatWindow::addEmoji(QString emoji)
{

    ui->textEdit->setText(ui->textEdit->toPlainText()+ emoji);
}
