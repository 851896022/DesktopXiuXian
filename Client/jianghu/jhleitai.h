#ifndef JHLEITAI_H
#define JHLEITAI_H

#include <QDialog>
#include "jianghuglobal.h"
#include <QTime>
#include <QTimer>
#include <QList>
#include "skillbar.h"
#include "battleai.h"
#include "skilldata.h"
namespace Ui {
class JHLeiTai;
}

class JHLeiTai : public QDialog
{
    Q_OBJECT

public:
    explicit JHLeiTai(QWidget *parent = nullptr);
    ~JHLeiTai();

public slots:
    /**
     * @brief 开始匹配按钮
     */
    void on_start_clicked();
    /**
     * @brief 匹配界面计时响应
     */
    void onTimerOut();
    /**
     * @brief 初始化类
     */
    void initThis();
    /**
     * @brief 初始化技能列表
     */
    void initSkillList();
    /**
     * @brief 匹配确认之后执行
     */
    void startFight();
    /**
     * @brief 确定对手之后信号槽连接
     */
    void connetLink();
    /**
     * @brief 当自己被攻击
     */
    void beDamage(QString);
    /**
     * @brief 刷新对方泣血
     */
    void receYourQixue(QString);
    /**
     * @brief 接收对方初始化信息
     */
    void receYourInfo(QString);
    /**
     * @brief 初始化战斗
     */
    void fightInit();
    /**
     * @brief 接收技能按钮消息
     */
    void receKillBarClick(QString);
    /**
     * @brief 结束战斗
     */
    void fightEnd();
signals:
    /**
     * @brief 发出攻击技能号|伤害值
     */
    void damage(QString);
    /**
     * @brief 刷新当前剩余血量
     */
    void nowQixue(QString);
    /**
     * @brief 初始化时的基础信息 气血|化劲|御劲
     */
    void baseInfo(QString);

private:
    Ui::JHLeiTai *ui;

    QTimer _timer;

    QTime _waitTime;

    QList<SkillBar*> skillBarList;

    BattleAI *_battleAI = nullptr;

    QString _yourQixue;
    QString _yourQixueFull;
    QString _yourHuajin;
    QString _yourYujin;
    QString _yourName;

    QString _myQixue;

};

#endif // JHLEITAI_H
