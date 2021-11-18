#ifndef BATTLEAI_H
#define BATTLEAI_H

#include <QObject>
#include <QTimer>
#include "skilldata.h"
#include "QDebug"
class BattleAI : public QObject
{
    Q_OBJECT
public:
    explicit BattleAI(QObject *parent = nullptr);

public slots:
    void battleStart();
    void onTimerOut();
    void initThis();
    void beDamage(QString);
signals:
    void damage(QString);
    void nowQixue(QString);
    void baseInfo(QString);
private:
    QTimer _skillTimer;
    QString _qixue = "10000";

};

#endif // BATTLEAI_H
