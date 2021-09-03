#ifndef AUGURWINDOW_H
#define AUGURWINDOW_H

#include <QDialog>
#include <QList>
#include "guastacked.h"
namespace Ui {
class AugurWindow;
}

class AugurWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AugurWindow(QWidget *parent = nullptr);
    ~AugurWindow();

private slots:
    void on_start_clicked();
    void showGuaWen();
private:
    Ui::AugurWindow *ui;
    QList<GuaStacked*> _guaList;
    QList<int> _guaXiang;
    int _m = 1 << 20;
    int _x  = 5;
    int _a = 9;  // a = 4p + 1
    int _b = 7;  // b = 2q + 1
};

#endif // AUGURWINDOW_H
