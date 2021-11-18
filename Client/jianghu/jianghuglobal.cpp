#include "jianghuglobal.h"
JiangHuGlobal *jg;
JiangHuGlobal::JiangHuGlobal(QObject *parent) : QObject(parent)
{
    {
        QTimer *timer = new QTimer;
        connect(timer,&QTimer::timeout,this,&JiangHuGlobal::SaveData);
        timer->start(10000);
    }
    {
        QTimer *timer = new QTimer;
        connect(timer,&QTimer::timeout,this,&JiangHuGlobal::updataRank);
        timer->start(60000);
    }

    m_httpManager = new QNetworkAccessManager;

}
/**
 * @brief 刷新用户信息
 */
void JiangHuGlobal::refUserData()
{
    userData.uTizhi=baseTizhi;
    userData.uGengu=baseGengu;
    userData.uShenfa=baseShenfa;

    {
        double tmp=baseShanghai.toDouble()+(userData.uTizhi.toDouble()*3)+(userData.uGengu.toDouble()*4);
        userData.uShanghai=QString::number(tmp,'g',15);
    }
    {
        double tmp=baseBaoji.toDouble()+(userData.uGengu.toDouble()*0.005);
        userData.uBaoji=QString::number(tmp,'g',15);
    }
    {
        double tmp=baseQiXue.toDouble()+(userData.uTizhi.toDouble()*100);
        userData.uQixue=QString::number(tmp,'g',15);
    }
    {
        double tmp=baseNeili.toDouble()+(userData.uGengu.toDouble()*5);
        userData.uNeili=QString::number(tmp,'g',15);
    }
    {
        double tmp=baseHuajin.toDouble()+(userData.uShenfa.toDouble()*5);
        userData.uHuajin=QString::number(tmp,'g',15);
    }
    {
        double tmp=baseYujin.toDouble()+(userData.uShenfa.toDouble()*0.003);
        userData.uYujin=QString::number(tmp,'g',15);
    }
}
void JiangHuGlobal::loadData()
{
    RedisClient redisCli;
    QHostInfo info = QHostInfo::fromName("chat.fuh.ink");
    if(info.addresses().count()>0)
    {
        QHostAddress addr= info.addresses().at(0);
        if(redisCli.open(addr.toString(), 8523))
        {
            qDebug()<<"redlis 连接 成功";
            if(redisCli.auth("qiandaowei"))
            {
               qDebug()<<"redlis 验证 成功";
               QByteArray data;
               if(redisCli.get(uuid,data))
               {
                   qDebug()<<"redlis 下载 成功";
                   QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);
                   QString key="c0e2adbe20210903";
                   QByteArray decodedText = encryption.decode(QByteArray::fromBase64( data), key.toLatin1());
                   qDebug()<<QString(decodedText);
                   QJsonObject obj;
                   obj = QJsonDocument::fromJson(QString(decodedText).toLatin1()).object();

                   if(obj.contains("tizhi"))
                   {
                       jg->userData.uTizhi = obj.value("tizhi").toString();
                   }
                   if(obj.contains("gengu"))
                   {
                       jg->userData.uGengu = obj.value("gengu").toString();
                   }
                   if(obj.contains("shenfa"))
                   {
                       jg->userData.uShenfa = obj.value("shenfa").toString();
                   }



                   if(obj.contains("jianggong"))
                   {
                       jg->userData.uJianggong = obj.value("jianggong").toString();
                   }
                   if(obj.contains("weiwang"))
                   {
                       jg->userData.uWeiwang = obj.value("weiwang").toString();
                   }
                   if(obj.contains("jifen"))
                   {
                       jg->userData.uJifen = obj.value("jifen").toString();
                   }
                   if(obj.contains("xiaofeiquan"))
                   {
                       jg->userData.uXiaofeiquan = obj.value("xiaofeiquan").toString();
                   }
               }
            }

        }


    }
    refUserData();

}
void JiangHuGlobal::SaveData()
{
    QString data;
    QJsonObject obj;
    obj.insert("tizhi",jg->userData.uTizhi);
    obj.insert("gengu",jg->userData.uGengu);
    obj.insert("shenfa",jg->userData.uShenfa);

    obj.insert("jianggong",jg->userData.uJianggong);
    obj.insert("weiwang",jg->userData.uWeiwang);
    obj.insert("jifen",jg->userData.uJifen);
    obj.insert("xiaofeiquan",jg->userData.uXiaofeiquan);

    data = QJsonDocument(obj).toJson(QJsonDocument::Compact);


    QAESEncryption encryption(QAESEncryption::AES_128, QAESEncryption::ECB, QAESEncryption::ZERO);
    QString key="c0e2adbe20210903";
    QByteArray encodedText = encryption.encode(QString(data).toUtf8(), key.toLatin1());
    QString str_encode_text=QString::fromLatin1(encodedText.toBase64());
    qDebug()<<str_encode_text;
    RedisClient redisCli;
    QHostInfo info = QHostInfo::fromName("chat.fuh.ink");
    if(info.addresses().count()>0)
    {
        QHostAddress addr= info.addresses().at(0);
        if(redisCli.open(addr.toString(), 8523))
        {
            qDebug()<<"redlis 连接 成功";
            if(redisCli.auth("qiandaowei"))
            {
               qDebug()<<"redlis 验证 成功";
               if(redisCli.set(uuid,str_encode_text))
               {
                   qDebug()<<"redlis 上传 成功";
               }
            }

        }


    }


}
void JiangHuGlobal::updataRank()
{
    QJsonObject _exampleObject;
    _exampleObject.insert("userid", jg->uuid);
    _exampleObject.insert("uname", jg->uName);
    _exampleObject.insert("jianggong", jg->userData.uJianggong.toInt());
    _exampleObject.insert("jifen", jg->userData.uJifen.toInt());
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
    QUrl _url("http://xiuxian.fuh.ink:8522/updata-chuangdangjianghu");
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
