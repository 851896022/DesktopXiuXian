#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QPainter>
#include <QMouseEvent>
#include <QSettings>
#include <QVariant>
#include <QInputDialog>
#include "makename.h"
#include "QDateTime"
#include <QCryptographicHash>
#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QList>
#include <QMenu>
#include <QAction>
#include <QFile>
#include <QJsonObject>
#include <QtNetwork>
#include <QByteArray>
#include "ranklist.h"
#include <QCryptographicHash>
#include "qaesencryption.h"
#include "QPixmap"
#include <QWidget>
#include "chatmessage/chatwindow.h"
#include "UpdataInfo/updatainfp.h"
#include "Augur/augurwindow.h"
#include  "UpdataInfo/feedback.h"
#include "jianghu/jianghu.h"
QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QWidget
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();




    void paintEvent(QPaintEvent *)
    {
        QPainter painter(this);
        painter.fillRect(this->rect(), QColor(0, 0, 0, 0)); /* 设置透明颜色 */
    }

    void mousePressEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            m_startPoint = frameGeometry().topLeft() - event->globalPos();
            moveState = true;
        }
    }
    void mouseReleaseEvent(QMouseEvent *event)
    {
        if (event->button() == Qt::LeftButton) {
            moveState = false;
        }
        addXiuwei();
        refDispaly();
        event->accept();
    }
    void contextMenuEvent(QContextMenuEvent *event)
    {
        pop_menu.exec(QCursor::pos());
        event->accept();
    }
    void mouseMoveEvent(QMouseEvent *event)
    {
        if(moveState)
        {
            this->move(event->globalPos() + m_startPoint);
        }

    }
    void wheelEvent(QWheelEvent*event);
private slots:
    void everySecond();
    void every10Second();
    void every60Second();
    void initThis();
    void registerThis();
    void refDispaly();
    void saveData();
    void levelUp();
    void addXiuwei();
    void showRankList(bool);
    void swicthSkin(bool);
    bool isAppAutoRun();
    void on__changeName_triggered();

    void on_augur_triggered();

private:
    QPoint m_startPoint;
    QString _userid;
    QString _userName;
    QString _shengming;//生命
    QString _gongji;//攻击
    QString _fangyu;//防御
    QString _wuxing;//悟性
    int _jingjie;//境界
    QString _xiuwei;//修为
    QStringList jieDuanNameList;

    bool moveState = false;

    QTimer timer1s;
    QTimer timer10s;
    QTimer timer60s;

    Ui::Dialog *ui;

    QAction _actionN5012346;
    QAction _action52pojie;
    QAction _exit;
    QAction _deleteData;
    QAction _rank;
    QAction _switchSkin;
    QAction _info;
    QAction _setTop;
    QAction _chat;
    QMenu pop_menu;

    QNetworkAccessManager*  m_httpManager;
    QNetworkReply * m_httpReply;
    QNetworkRequest  m_httpRequest;
    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;

    Qt::WindowFlags _flags;
    bool isToping = true;
    QPixmap _skinPng;
    RankList *rank= nullptr;

    UpdataInfp _updataInfo;
    ChatWindow _chatWindow;
    AugurWindow _augurWindow;
    FeedBack _feedback;
    JiangHu _jianghu;

signals:
    void sigChatShow();

};
#endif // DIALOG_H
