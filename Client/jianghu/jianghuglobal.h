#ifndef JIANGHUGLOBAL_H
#define JIANGHUGLOBAL_H

#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QTimer>
#include <QSettings>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QHostInfo>
#include "qaesencryption.h"
#include "RedisLib/RedisClient.h"
#include <QtNetwork>
/**
 * @brief The 用户信息 struct
 */
struct UserData
{
    UserData() {}



    /**
     * @brief 体质=基础体质+装备体质
     */
    QString uTizhi ;
    /**
     * @brief 根骨=基础体质+装备体质
     */
    QString uGengu;
    /**
     * @brief 身法=基础体质+装备体质
     */
    QString uShenfa;
    /**
     * @brief 面板伤害 = (基础伤害+（体质*3）+（根骨*3）)*buff
     */
    QString uShanghai;
    /**
     * @brief 面板暴击= (基础暴击+（根骨*0.005）)*buff
     */
    QString uBaoji;
    /**
     * @brief 面板气血=基础气血+（体质*100）
     */
    QString uQixue;
    /**
     * @brief 面板内力=基础内力+（根骨*5）
     */
    QString uNeili;
    /**
     * @brief 面板化劲=基础化劲+（身法*5）
     */
    QString uHuajin;
    /**
     * @brief 面板御劲=基础御劲+（身法*0.003）
     */
    QString uYujin;
    /**
     * @brief 江湖贡献值
     */
    QString uJianggong = "0";
    /**
     * @brief 江湖威望
     */
    QString uWeiwang ="0";
    /**
     * @brief 擂台积分
     */
    QString uJifen;
    /**
     * @brief 擂台消费券
     */
    QString uXiaofeiquan;
};
Q_DECLARE_METATYPE(UserData);
class JiangHuGlobal : public QObject
{
    Q_OBJECT
public:
    explicit JiangHuGlobal(QObject *parent = nullptr);
    /**
     * @brief uuid 玩家UUID
     */
    QString uuid;
    /**
     * @brief uName 玩家名字
     */
    QString uName;
    /**
     * @brief 用户信息
     */
    UserData userData;

    /**
     * @brief 基础伤害
     */
    QString baseShanghai = "100";
    /**
     * @brief 基础暴击
     */
    QString baseBaoji = "0.1";
    /**
     * @brief 基础气血
     */
    QString baseQiXue = "5000";
    /**
     * @brief 基础内力
     */
    QString baseNeili = "2000";
    /**
     * @brief 基础化劲
     */
    QString baseHuajin = "30";
    /**
     * @brief 基础御劲
     */
    QString baseYujin = "0.05";
    /**
     * @brief 基础体质
     */
    QString baseTizhi = "10";
    /**
     * @brief 基础根骨
     */
    QString baseGengu = "10";
    /**
     * @brief 基础身法
     */
    QString baseShenfa = "10";


public slots:
    /**
     * @brief 刷新用户信息
     */
    void refUserData();
    void loadData();
    void SaveData();
    void updataRank();
signals:


private:
    QNetworkAccessManager*  m_httpManager;
    QNetworkReply * m_httpReply;
    QNetworkRequest  m_httpRequest;
    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;

};
extern JiangHuGlobal *jg;
#endif // JIANGHUGLOBAL_H
