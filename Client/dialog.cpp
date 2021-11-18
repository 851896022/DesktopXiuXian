#include "dialog.h"
#include "ui_dialog.h"
#include <QMovie>
#include <QtMath>
#include <QMessageBox>
#include "popupmanage.h"
#include "QFileDialog"
#include <QSettings>
#include <QDir>

Dialog::Dialog(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setWindowTitle("透明无边框窗口");
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::SubWindow|Qt::Popup|Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    _flags = windowFlags();
    _actionN5012346.setText("n5012346");
    _actionN5012346.setEnabled(false);
    _action52pojie.setText("52pojie");
    _action52pojie.setEnabled(false);
    _exit.setText("结束修炼");
    _deleteData.setText("自废修为");
    _rank.setText("排行榜");
    _switchSkin.setText("变身");
    _info.setText("修炼状态");
    _setTop.setText("置顶");
    _setTop.setCheckable(true);
    _setTop.setChecked(true);
    _chat.setText("华山论剑");
    //pop_menu.addAction(&_action52pojie);
    //pop_menu.addAction(&_actionN5012346);
    pop_menu.addAction(&_setTop);
    pop_menu.addAction(&_info);
    pop_menu.addAction(&_chat);
    pop_menu.addAction(&_rank);
    pop_menu.addAction(&_switchSkin);
    pop_menu.addAction(&_exit);
    pop_menu.addAction(&_deleteData);
    pop_menu.addAction(ui->_AutoStart);
    pop_menu.addAction(ui->_changeName);
    pop_menu.addAction(ui->augur);
    pop_menu.addAction(ui->feedback);
    pop_menu.addAction(ui->jianghu);


    connect(&_exit,&QAction::triggered,[](bool){
                qApp->exit(0);
            });
    connect(&_deleteData,&QAction::triggered,[](bool){
                {
                    auto re = QMessageBox::information(NULL, "自废修为", "这将会删除全部数据确定操作吗？",
                                             QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                    if(re == QMessageBox::Yes)
                    {
                        {
                            auto re = QMessageBox::information(NULL, "再次确认", "真的真的真的要删档吗？",
                                                     QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
                            if(re == QMessageBox::Yes)
                            {
                                QFile::remove(QDir::homePath()+"/AppData/Roaming/xiuxian.ini");
                                qApp->exit();
                            }
                            else
                            {
                                return;

                            }
                        }

                    }
                    else
                    {
                        return ;
                    }
                }

            });
    connect(&_rank,&QAction::triggered,this,&Dialog::showRankList);
    connect(&_switchSkin,&QAction::triggered,this,&Dialog::swicthSkin);
    connect(&_info,&QAction::triggered,[this](bool){
        QString info;
        info.append((this->_userName)+"\n");
        info.append("生命："+QString::number(_shengming.toDouble(),'e',15)+"\n");
        info.append("攻击："+QString::number(_gongji.toDouble(),'e',15)+"\n");
        info.append("防御："+QString::number(_fangyu.toDouble(),'e',15)+"\n");
        info.append("悟性："+QString::number(_wuxing.toDouble(),'e',15)+"\n");
        info.append("境界："+jieDuanNameList.at(_jingjie)+"\n");
        info.append("修为："+QString::number(_xiuwei.toDouble(),'e',15));
        QMessageBox::information(this,"修炼状态",info);
            });
    connect(&_setTop,&QAction::triggered,[this](bool){
            if(isToping)
            {
                setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::SubWindow|Qt::Popup);
                show();
                _setTop.setText("置顶");
                _setTop.setChecked(false);
                isToping = false;
            }
            else
            {
                setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::SubWindow|Qt::Popup|Qt::WindowStaysOnTopHint);
                show();
                _setTop.setText("置顶");
                _setTop.setChecked(true);
                isToping = true;
            }
            });
    connect(ui->_AutoStart,&QAction::triggered,[this](bool shate){
        QString application_name = "xiuxian";
           QSettings *settings = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat );
       if(shate)
       {
           QString application_path = QApplication::applicationFilePath();
           settings->setValue(application_name, application_path.replace("/", "\\"));

       }
       else
       {
           settings->remove(application_name);
       }
       delete settings;
       this->ui->_AutoStart->setChecked(isAppAutoRun());

            });
    this->ui->_AutoStart->setChecked(isAppAutoRun());



    connect(ui->feedback,&QAction::triggered,[this](bool){
                this->_feedback.show();
            });
    connect(ui->jianghu,&QAction::triggered,[this](bool){
                _jianghu.show();
            });

    connect(&_chat,&QAction::triggered,&_chatWindow,&ChatWindow::show);
    jieDuanNameList.append("炼体期");
    jieDuanNameList.append("练气期");
    jieDuanNameList.append("筑基期");
    jieDuanNameList.append("金丹期");
    jieDuanNameList.append("元婴期");
    jieDuanNameList.append("化神期");
    jieDuanNameList.append("炼虚期");
    jieDuanNameList.append("合体期");
    jieDuanNameList.append("大乘期");
    jieDuanNameList.append("渡劫期");

    QMovie *movie = new QMovie(":/2021_08_21_12_06_56_979.gif");
    ui->label->setMovie(movie);
    movie->start();
    /**
     * @brief 初始化在这里
     */
    initThis();


    _jianghu.setInfo(_userName,_userid);
    _chatWindow.setInfo(_userName,_userid);

    m_httpManager = new QNetworkAccessManager;

    connect(&timer1s,&QTimer::timeout,this,&Dialog::everySecond);
    connect(&timer10s,&QTimer::timeout,this,&Dialog::every10Second);
    connect(&timer60s,&QTimer::timeout,this,&Dialog::every60Second);
    timer1s.start(1000);
    timer10s.start(10000);
    timer60s.start(30000);



    //QObject::connect(&w,&Dialog::sigChatShow,&chat,&ChatWindow::show);
}

Dialog::~Dialog()
{
    delete ui;
}
#include <thread>
void sendData()
{
    //httplib::Client cli("http://xiuxian.fuh.ink:8522");
    //auto res = cli.Post("/updata", "text", "text/plain");
}
void Dialog::everySecond()
{
    addXiuwei();
    refDispaly();
    sendData();
}
void Dialog::every10Second()
{
    levelUp();
    saveData();
}


void Dialog::every60Second()
{


    QJsonObject _exampleObject;
    _exampleObject.insert("userid", _userid);
    _exampleObject.insert("username", _userName);
    _exampleObject.insert("shengming", _shengming.toDouble());
    _exampleObject.insert("gongji", _gongji);
    _exampleObject.insert("fangyu", _fangyu);
    _exampleObject.insert("wuxing", _wuxing);
    _exampleObject.insert("jingjie", _jingjie);
    _exampleObject.insert("xiuwei", _xiuwei);
    //post请求的数据是Json格式。首先创建QJsonObject对象，并插入数据。
    m_httpDocum.setObject(_exampleObject);
    m_httpData = m_httpDocum.toJson(QJsonDocument::Compact);
    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);

        QString key="c0e2fdbe"+QDateTime::currentDateTime().toString("yyyyMMdd");
        //qDebug()<<key;
        //QByteArray hashKey = QCryptographicHash::hash(key.toUtf8(), QCryptographicHash::Md5);

        QByteArray encodedText = encryption.encode(QString(m_httpData).toUtf8(), key.toLatin1());
        //qDebug()<<"key:"<<QString(m_httpData).toUtf8().toHex();
        QString str_encode_text=QString::fromLatin1(encodedText.toBase64());
        //qDebug()<<"encodedText0:"<<m_httpData;
        //qDebug()<<"encodedText2:"<<str_encode_text;

    // 转成Json格式
    QUrl _url("http://xiuxian.fuh.ink:8522/updata-new");
    m_httpRequest.setUrl(_url);
    // 设置请求的url 注意地址的正确性 之前我就受到了http和https的坑
        m_httpRequest.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("text/html; charset=utf-8"));

        // 设置请求头
    m_httpReply = m_httpManager->post(m_httpRequest, str_encode_text.toUtf8());
    // post请求
    QEventLoop _loop;
    QTimer _timer;
    connect(m_httpReply, SIGNAL(finished()), &_loop, SLOT(quit()));
    connect(&_timer, SIGNAL(timeout()), &_loop, SLOT(quit()));
    // eventloop用于阻塞等待消息，并结合timer进行超时处理
    _timer.start(1000);
    _loop.exec();
    // 这里的eventloop只是阻塞此段，并不会阻塞其他块的代码，比如我不加定时器，让这个eventloop一直exec，还是可以出发相关的槽函数
    if(_timer.isActive())
    {
        _timer.stop();
        // 处理函数
    }
    else
    {
        qDebug() <<  "time Out";
        //超时结果
    }
    _loop.deleteLater();
    _timer.deleteLater();
    // 释放资源
    m_httpReply->abort();
    // 断开连接
    return ;


}
void Dialog::initThis()
{
    QSettings configIni(QDir::homePath()+"/AppData/Roaming/xiuxian.ini", QSettings::IniFormat);
    if(configIni.status() == QSettings::NoError)
    {
        _userid = configIni.value("xiuxian/userid").toString();
        _userName = configIni.value("xiuxian/username").toString();
        {
            QString t1=configIni.value("xiuxian/shengming").toString();
            QByteArray b1 = QByteArray::fromHex(t1.toLatin1());
            QByteArray b2 = QByteArray::fromBase64(b1);
            QString t2 = QString(b2);
            _shengming =QString::number( QVariant(t2).toReal(),'g',15);
        }
        {
            QString t1=configIni.value("xiuxian/gongji").toString();
            QByteArray b1 = QByteArray::fromHex(t1.toLatin1());
            QByteArray b2 = QByteArray::fromBase64(b1);
            QString t2 = QString(b2);
            _gongji = QString::number(QVariant(t2).toReal(),'g',15);
        }
        {
            QString t1=configIni.value("xiuxian/fangyu").toString();
            QByteArray b1 = QByteArray::fromHex(t1.toLatin1());
            QByteArray b2 = QByteArray::fromBase64(b1);
            QString t2 = QString(b2);
            _fangyu = QString::number(QVariant(t2).toReal(),'g',15);
        }
        {
            QString t1=configIni.value("xiuxian/wuxing").toString();
            QByteArray b1 = QByteArray::fromHex(t1.toLatin1());
            QByteArray b2 = QByteArray::fromBase64(b1);
            QString t2 = QString(b2);
            _wuxing =QString::number(QVariant(t2).toReal(),'g',15);
        }
        {
            QString t1=configIni.value("xiuxian/jingjie").toString();
            QByteArray b1 = QByteArray::fromHex(t1.toLatin1());
            QByteArray b2 = QByteArray::fromBase64(b1);
            QString t2 = QString(b2);
            _jingjie = QVariant(t2).toInt();
        }
        {
            QString t1=configIni.value("xiuxian/xiuwei").toString();
            QByteArray b1 = QByteArray::fromHex(t1.toLatin1());
            QByteArray b2 = QByteArray::fromBase64(b1);
            QString t2 = QString(b2);
            _xiuwei = QString::number(QVariant(t2).toReal(),'g',15);
        }


        if(_userid.count() == 32)
        {

        }
        else
        {
            registerThis();
        }

    }
    else
    {
        registerThis();
    }


}
void Dialog::registerThis()
{
    bool isOK;
    QString name = MakeName::getName();
    QString text = QInputDialog::getText(NULL, "起名字",
                                                       "请输入你的名字",
                                                       QLineEdit::Normal,
                                                       name,
                                                       &isOK);
    if(!isOK || text.isEmpty())
    {
        qApp->exit(1);
    }
    else
    {
        _userName = text;
        _userid= QCryptographicHash::hash(
                    (QDateTime::currentDateTime().toString()+text+QString::number(qrand())).toLatin1(),
                    QCryptographicHash::Md5)
                    .toHex();
        _shengming =QString::number( (qrand() % 10)+95);
        _gongji=QString::number((qrand() % 10)+15.0);
        _fangyu=QString::number((qrand() % 10)+15.0);
        _wuxing=QString::number(((qrand() % 2000)/2000.0)+0.5);
        _jingjie = 0;
        _xiuwei = "0";
        saveData();
    }
}
void Dialog::refDispaly()
{
    ui->name->setText(_userName);
    ui->shengming->setText(QString::number(_shengming.toDouble(),'e',3));
    ui->gongji->setText(QString::number(_gongji.toDouble(),'e',3));
    ui->fangyu->setText(QString::number(_fangyu.toDouble(),'e',3));
    ui->wuxing->setText(QString::number(_wuxing.toDouble(),'e',3));
    ui->jingjie->setText(jieDuanNameList.at(_jingjie));
    ui->xiuwei->setText(QString::number(_xiuwei.toDouble(),'e',3));
}
void Dialog::saveData()
{

    //打开文件

    QSettings configIni(QDir::homePath()+"/AppData/Roaming/xiuxian.ini", QSettings::IniFormat);
    //写入数据，必须指定写入节点和值
    configIni.setValue("xiuxian/userid",_userid);
    configIni.setValue("xiuxian/username",_userName);
    configIni.setValue("xiuxian/shengming",QString(QVariant(_shengming.toDouble()).toString().toLatin1().toBase64().toHex()));
    configIni.setValue("xiuxian/gongji",QString(QVariant(_gongji.toDouble()).toString().toLatin1().toBase64().toHex()));
    configIni.setValue("xiuxian/fangyu",QString(QVariant(_fangyu.toDouble()).toString().toLatin1().toBase64().toHex()));
    configIni.setValue("xiuxian/wuxing",QString(QVariant(_wuxing.toDouble()).toString().toLatin1().toBase64().toHex()));
    configIni.setValue("xiuxian/jingjie",QString(QVariant(_jingjie).toString().toLatin1().toBase64().toHex()));
    configIni.setValue("xiuxian/xiuwei",QString(QVariant(_xiuwei.toDouble()).toString().toLatin1().toBase64().toHex()));
}
void Dialog::levelUp()
{

    for(int i = 0;i<10;i++)
    {
        if(_xiuwei.toDouble() >= qPow(100,i) && _jingjie <i)
        {
            _jingjie = i;
            qreal wuxingOld = _wuxing.toDouble();

            _wuxing = QString::number(MakeName::getWuxing(i),'g',15);

            qreal beilv = _wuxing.toDouble()/wuxingOld;
            _shengming = QString::number(_shengming.toDouble()*beilv,'g',15);
            _shengming = QString::number(MakeName::getRandData(_shengming.toDouble(),0.7),'g',15);
            _gongji = QString::number(_gongji.toDouble()*beilv,'g',15);
            _gongji = QString::number(MakeName::getRandData(_gongji.toDouble(),0.7),'g',15);
            _fangyu = QString::number(_fangyu.toDouble()*beilv,'g',15);
            _fangyu = QString::number(MakeName::getRandData(_fangyu.toDouble(),0.7),'g',15);
            PopupManage::getInstance()->setInfomation("恭喜", "成功晋升到"+jieDuanNameList.at(_jingjie));
        }
    }


}

void Dialog::addXiuwei()
{
    QTime current_time =QTime::currentTime();
    int hour = current_time.hour();//当前的小时

    //()<<"xia"<<_xiuwei;
    if(hour<2)
    {
        _xiuwei=QString::number(_xiuwei.toDouble()+_wuxing.toDouble()*2,'g',15);
    }
    else if(hour <4)
    {
        _xiuwei=QString::number(_xiuwei.toDouble()+_wuxing.toDouble()*5,'g',15);
    }
    else if(hour <6)
    {

        _xiuwei=QString::number(_xiuwei.toDouble()+_wuxing.toDouble()*2,'g',15);
    }
    else
    {
        _xiuwei=QString::number(_xiuwei.toDouble()+_wuxing.toDouble(),'g',15);
    }
    //qDebug()<<"shang"<<_xiuwei;

}
void Dialog::showRankList(bool)
{
    if(rank != nullptr)
    {
        rank->deleteLater();
    }
    rank = new RankList;
    rank->show();
}
void Dialog::swicthSkin(bool)
{


        QString fileName = QFileDialog::getOpenFileName(
            this,
            "选择一张图片",
            "C:/",
            "images(*.png *.jpeg *.jpg *.bmp *.gif);");
        if (fileName.isEmpty()) {
            return;
        }
        else {
            _skinPng.load(fileName);
            ui->label_skin->setPixmap(_skinPng);
            ui->label_skin->setScaledContents(true);
            ui->label_skin->show();
            this->setGeometry(x(),y(),_skinPng.width(),_skinPng.height());
            ui->label_skin->setGeometry(0,0,_skinPng.width(),_skinPng.height());
            ui->baseSkin->hide();

        }


}

void Dialog::wheelEvent(QWheelEvent*event)
{
    if (QGuiApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        // Ctrl键盘被按下
        if(event->delta() > 0) //当滚轮向上滑，远离使用者
        {
                //ui->textEdit->zoomIn(); //进行放大，textEdit的光标和字体都放大
            //_skinPng=_skinPng.scaled(_skinPng.width()*1.03,_skinPng.height()*1.03, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            //ui->label_skin->setPixmap(_skinPng);
            this->setGeometry(x(),y(),width()*1.03,height()*1.03);
            ui->label_skin->setGeometry(0,0,width(),height());
        }
        else //当滚轮向下滑，靠近使用者
        {
                //ui->textEdit->zoomOut(); //进行缩小，textEdit的光标和字体都缩小
            //_skinPng=_skinPng.scaled(_skinPng.width()*0.97,_skinPng.height()*0.97, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            //ui->label_skin->setPixmap(_skinPng);
            this->setGeometry(x(),y(),width()*0.97,height()*0.97);
            ui->label_skin->setGeometry(0,0,width(),height());
        }

    }
}
bool Dialog::isAppAutoRun()
{
    //查看注册表是否已经写入程序名称
    QSettings *reg = new QSettings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::Registry64Format);
    QString strValue = reg->value("xiuxian").toString();	//获取注册表中“YourAppName”对应的路径值
    QString path = QDir::toNativeSeparators(QCoreApplication::applicationFilePath());	//获取当前程序完整路径
    bool flag = (strValue == path);		//flag为true则表示注册表中已有此项

    return flag;
}


void Dialog::on__changeName_triggered()
{
    bool isOK;
    QString name = MakeName::getName();
    QString text = QInputDialog::getText(NULL, "起名字",
                                                       "请输入你的名字",
                                                       QLineEdit::Normal,
                                                       name,
                                                       &isOK);
    if(!isOK || text.isEmpty())
    {
        return;
    }
    else
    {
        _userName = text;
    }
}


void Dialog::on_augur_triggered()
{
    _augurWindow.show();
}

