#ifndef POPUPMANAGE_H
#define POPUPMANAGE_H

#include <QObject>
#include <QString>
#include "messageshow.h"

class PopupManage : public QObject
{
    Q_OBJECT

public:
    static PopupManage * getInstance();

    void setInfomation(QString titleInfo, QString msg);
    void setInfomation(QString titleInfo, QString msg, QString extraInfo);

private slots:
    void addMessageShow(MessageShow *popup);
    void deleteMessageShow(MessageShow *popup);
    void notifyMessageShow();

signals:
    void sigClickUrl(QString);

private:
    PopupManage();
    ~PopupManage();

    static PopupManage * m_popupManager;
    QList<MessageShow *> m_popupList;

    class PopupManageGC // 垃圾回收类
    {
    public:
        PopupManageGC(){}
        ~PopupManageGC()
        {
            if (m_popupManager != NULL)
            {
                delete m_popupManager;
                m_popupManager = NULL;
            }
        }
    };
    static PopupManageGC popupManager_gc;
};

#endif // POPUPMANAGE_H
