#ifndef UPDATAINFP_H
#define UPDATAINFP_H

#include <QDialog>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QCloseEvent>
#include <QFrame>
namespace Ui {
class UpdataInfp;
}

class UpdataInfp : public QDialog
{
    Q_OBJECT

public:
    explicit UpdataInfp(QWidget *parent = nullptr);
    ~UpdataInfp();
    void closeEvent(QCloseEvent *event)
    {
        this->hide();
        event->accept();
    }
private slots:
    void on_pushButton_clicked();
   void shwInfo(QNetworkReply *reply);
   void refData();
private:
    Ui::UpdataInfp *ui;
    QNetworkAccessManager *_accessManager;
    QString _link;
    QTimer timer;
};

#endif // UPDATAINFP_H
