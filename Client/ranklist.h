#ifndef RANKLIST_H
#define RANKLIST_H

#include <QDialog>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QCloseEvent>
#include <QFrame>
namespace Ui {
class RankList;
}

class RankList : public QFrame
{
    Q_OBJECT

public:
    explicit RankList(QWidget *parent = nullptr);
    ~RankList();


    void closeEvent(QCloseEvent *event)
    {
        this->hide();
        event->accept();
    }


private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void showRank(QNetworkReply *reply);



    void on_btnJfen_clicked();

private:
    Ui::RankList *ui;
    QNetworkAccessManager *accessManager;
};

#endif // RANKLIST_H
