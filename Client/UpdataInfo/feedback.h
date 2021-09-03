#ifndef FEEDBACK_H
#define FEEDBACK_H

#include <QDialog>
#include <QtNetwork>
namespace Ui {
class FeedBack;
}

class FeedBack : public QDialog
{
    Q_OBJECT

public:
    explicit FeedBack(QWidget *parent = nullptr);
    ~FeedBack();

private slots:
    void on_send_clicked();

private:
    Ui::FeedBack *ui;
    QNetworkAccessManager*  m_httpManager;
    QNetworkReply * m_httpReply;
    QNetworkRequest  m_httpRequest;
    QByteArray  m_httpData;
    QJsonDocument m_httpDocum;
};

#endif // FEEDBACK_H
