#ifndef MESSAGESHOW_H
#define MESSAGESHOW_H

#include <QDialog>
#include <QTimer>
#include <QDesktopWidget>
#include <QPoint>
#include <QDebug>

namespace Ui {
class MessageShow;
}

class MessageShow : public QDialog
{
    Q_OBJECT

public:
    MessageShow(QWidget *parent = 0);
    ~MessageShow();

    void setInfomation(QString titleInfo, QString msg);
    void setInfomation(QString titleInfo, QString msg, QString extraInfo);

    void updatePosition();

protected:
    virtual void enterEvent(QEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;


signals:
    void sigClickUrl(QString);
    void sigClose(MessageShow *);

private slots:
    void on_pushButton_close_clicked();

    void slotMsgMove();
    void slotMsgStay();
    void slotMsgClose();

private:
    void showMessage();
    void setUiStyle();
    inline void deleteTimer(QTimer * timer);

private:
    Ui::MessageShow *ui;

    QPoint end_showPoint;
    int m_currentHeight;

    QTimer* m_showTimer;
    QTimer* m_stayTimer;
    QTimer* m_closeTimer;

    int m_stayExeTime;
    double m_transparent;

    bool m_firstShow;
    bool m_enterEvent;
};

#endif // MESSAGESHOW_H
