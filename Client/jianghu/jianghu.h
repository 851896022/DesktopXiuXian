#ifndef JIANGHU_H
#define JIANGHU_H

#include <QDialog>
#include "jianghuglobal.h"
#include "userinfo.h"
#include "jhleitai.h"
namespace Ui {
class JiangHu;
}

class JiangHu : public QDialog
{
    Q_OBJECT

public:
    explicit JiangHu(QWidget *parent = nullptr);
    ~JiangHu();
    /**
    * @brief setInfo设置玩家信息
    * @param userName用户名
    * @param userid uuid
    */
   void setInfo(QString userName,QString userid);
private slots:
   void on_userInfo_clicked();

   void on_JJC_clicked();

private:
    Ui::JiangHu *ui;
    UserInfo *_userInfo = nullptr;

    JHLeiTai *_leitai = nullptr;
};

#endif // JIANGHU_H
