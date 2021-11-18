#ifndef SKILLBAR_H
#define SKILLBAR_H

#include <QFrame>
#include <QPushButton>
#include <QTimer>
#include <QDateTime>
#include "jianghuglobal.h"
namespace Ui {
class SkillBar;
}
struct BaseKillInfo
{
    BaseKillInfo() {}

    QString kCD = "3000";

    QString kShanghai = "500";

    QString name = "神秘招式";
};
class SkillBar : public QPushButton
{
    Q_OBJECT

public:
    explicit SkillBar(QWidget *parent = nullptr);
    ~SkillBar();

    BaseKillInfo killInfo;
public slots:
    void setRefValue(float value);

    void initThis(BaseKillInfo);

    void refCD();

    void onClick();
private:
    Ui::SkillBar *ui;
    QTimer _refUiTimer;

    QDateTime _baseTime;
signals:
    void damage(QString);
};

#endif // SKILLBAR_H
