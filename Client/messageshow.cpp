#include "messageshow.h"
#include "ui_messageshow.h"

#include <QDebug>

#define SHOWTIMER_EXEPERIOD 2

#define STAYTIMER_EXEPERIOD 1000
#define STAYTIMER_MAX_EXETIMES 15

#define CLOSETIMER_EXEPERIOD 100

MessageShow::MessageShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MessageShow)
  , m_currentHeight(QApplication::desktop()->height())
  , m_showTimer(new QTimer(this))
  , m_stayTimer(new QTimer(this))
  , m_closeTimer(new QTimer(this))
  , m_stayExeTime(0)
  , m_transparent(1.0)
  , m_firstShow(true)
  , m_enterEvent(false)
{
    ui->setupUi(this);

    setUiStyle();

    connect(m_showTimer, SIGNAL(timeout()),  this, SLOT(slotMsgMove()));
    connect(m_stayTimer, SIGNAL(timeout()),  this, SLOT(slotMsgStay()));
    connect(m_closeTimer, SIGNAL(timeout()), this, SLOT(slotMsgClose()));
}

MessageShow::~MessageShow()
{
    deleteTimer(m_showTimer);
    deleteTimer(m_stayTimer);
    deleteTimer(m_closeTimer);
    delete ui;
}

void MessageShow::setInfomation(QString titleInfo, QString msg)
{
    ui->label_titleInfo->setText(titleInfo);

    ui->label_msg->setText(msg);    
    ui->label_msg->setToolTip(msg);
}


void MessageShow::setInfomation(QString titleInfo, QString msg, QString extraInfo)
{
    ui->label_titleInfo->setText(titleInfo);
    ui->label_msg->setText(QString("<a style='color: gray;'href=\"")+extraInfo+QString("\">")+msg);
    connect(ui->label_msg,SIGNAL(linkActivated(QString)),this,SIGNAL(sigClickUrl(QString)));

    ui->label_msg->setToolTip(msg);
}


void MessageShow::setUiStyle()
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::SubWindow);

    ui->widget_msgtitle->setStyleSheet("border:0px solid rgb(10,10,10);"
                                        "color:#fff;"
                                        "background-color:rgb(10,10,10);"
                                        "font: 18px \"Microsoft YaHei\";");

    ui->label_msg->setStyleSheet("border:0px solid rgb(50,50,50);"
                                 "background-color:rgb(50,50,50);"
                                 "color:#fff;"
                                 "font: 24px \"Microsoft YaHei\";");
}

void MessageShow::deleteTimer(QTimer *timer)
{
    if(timer->isActive())
        timer->stop();

    delete timer;
    timer = NULL;
}

void MessageShow::on_pushButton_close_clicked()
{
    m_showTimer->stop();
    m_stayTimer->stop();
    m_closeTimer->stop();
    emit sigClose(this);
}

void MessageShow::showMessage()
{
    m_showTimer->stop();
    m_stayTimer->stop();
    m_closeTimer->stop();
    setWindowOpacity(1);

    // 获取当前桌面的位置
    QRect desk_rect = QApplication::desktop()->availableGeometry();

    // 计算最后呈现的位置
    end_showPoint.setX(desk_rect.width() - rect().width());
    end_showPoint.setY(desk_rect.height() - rect().height());

    // 先将弹窗移动到桌面外
    m_currentHeight = desk_rect.height();
    move(desk_rect.width(), desk_rect.height());

    this->open();
    m_showTimer->start(SHOWTIMER_EXEPERIOD);
}

void MessageShow::slotMsgMove()
{
    m_currentHeight--;
    move(end_showPoint.x(), m_currentHeight);
    if(m_currentHeight <= end_showPoint.y() )
    {
        m_showTimer->stop();
        if (!m_enterEvent)
            m_stayTimer->start(STAYTIMER_EXEPERIOD);
    }
}

void MessageShow::slotMsgStay()
{
    m_stayExeTime++;
    if(m_stayExeTime >= STAYTIMER_MAX_EXETIMES)
    {
        m_stayTimer->stop();
        m_closeTimer->start(CLOSETIMER_EXEPERIOD);
    }
}

void MessageShow::slotMsgClose()
{
    m_transparent -= 0.2;
    if(m_transparent <= 0.0)
    {
        m_closeTimer->stop();
        emit sigClose(this);
    }
    else
    {
        setWindowOpacity(m_transparent);
    }
}

void MessageShow::updatePosition()
{
    if(m_firstShow){
        this->showMessage();
        m_firstShow = false;
    }else{
        end_showPoint.setY(end_showPoint.y()- this->height()-2);
        m_currentHeight = m_currentHeight - this->height()-2;
        move(end_showPoint.x(), m_currentHeight);
    }
}

void MessageShow::enterEvent(QEvent *event)
{
    Q_UNUSED(event);

    m_enterEvent = true;
    m_stayTimer->stop();
    m_transparent = 1.0;
    setWindowOpacity(1.0);
    m_closeTimer->stop();
}

void MessageShow::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_enterEvent = false;

    m_stayExeTime = STAYTIMER_MAX_EXETIMES/2;
    m_stayTimer->start(STAYTIMER_EXEPERIOD);
}
