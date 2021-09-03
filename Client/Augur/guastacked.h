#ifndef GUASTACKED_H
#define GUASTACKED_H

#include <QStackedWidget>
#include <QTimer>
namespace Ui {
class GuaStacked;
}

class GuaStacked : public QStackedWidget
{
    Q_OBJECT

public:
    explicit GuaStacked(QWidget *parent = nullptr);
    ~GuaStacked();
    void setGuaXiang(int gua);
    void refUI();
private:
    Ui::GuaStacked *ui;
    int _gua = 99;
    int count = 10;
    QTimer _timer;
signals:
    void refUiFinish();
};

#endif // GUASTACKED_H
