#ifndef JHLEITAICONFIRM_H
#define JHLEITAICONFIRM_H

#include <QDialog>
#include <QTimer>
namespace Ui {
class JHLeiTaiConfirm;
}

class JHLeiTaiConfirm : public QDialog
{
    Q_OBJECT

public:
    explicit JHLeiTaiConfirm(QWidget *parent = nullptr);
    ~JHLeiTaiConfirm();

private slots:
    void on_btnCancel_clicked();

    void on_btnOk_clicked();

    void onTimerOut();
private:
    Ui::JHLeiTaiConfirm *ui;

    QTimer _timer;
};

#endif // JHLEITAICONFIRM_H
