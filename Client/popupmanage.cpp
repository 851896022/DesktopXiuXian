#include "PopupManage.h"

#include <QDebug>

PopupManage * PopupManage::m_popupManager = new PopupManage;
PopupManage::PopupManageGC PopupManage::popupManager_gc;

PopupManage::PopupManage()
{

}

PopupManage::~PopupManage()
{
    qDeleteAll(m_popupList);
    m_popupList.clear();
}

void PopupManage::addMessageShow(MessageShow *popup)
{
    connect(popup, &MessageShow::sigClose, this, &PopupManage::deleteMessageShow);
    connect(popup, &MessageShow::sigClickUrl, this, &PopupManage::sigClickUrl);
    m_popupList.append(popup);

    this->notifyMessageShow();
}

void PopupManage::deleteMessageShow(MessageShow *popup)
{
    disconnect(popup, &MessageShow::sigClose, this, &PopupManage::deleteMessageShow);
    disconnect(popup, &MessageShow::sigClickUrl, this, &PopupManage::sigClickUrl);
    m_popupList.removeOne(popup);
    delete popup;
    popup = nullptr;
}

void PopupManage::notifyMessageShow()
{
    for(int i = 0; i < m_popupList.length();i++){
        m_popupList.at(i)->updatePosition();
    }
}

PopupManage *PopupManage::getInstance()
{
    return m_popupManager;
}

void PopupManage::setInfomation(QString titleInfo, QString msg)
{
    MessageShow * popup = new MessageShow();
    popup->setInfomation(titleInfo, msg);
    addMessageShow(popup);
}

void PopupManage::setInfomation(QString titleInfo, QString msg, QString extraInfo)
{
    MessageShow * popup = new MessageShow();
    popup->setInfomation(titleInfo, msg, extraInfo);
    addMessageShow(popup);
}
