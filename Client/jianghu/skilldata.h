#ifndef SKILLDATA_H
#define SKILLDATA_H

#include <QObject>

class SkillData : public QObject
{
    Q_OBJECT
public:
    explicit SkillData(QObject *parent = nullptr);
    static QString getSkillInfo(int id);
signals:
private:

};

#endif // SKILLDATA_H
