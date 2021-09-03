#ifndef EMOJIPICKER_H
#define EMOJIPICKER_H

#include <QDialog>
#include <QTableWidgetItem>
#include <QDebug>
#include <QEvent>
namespace Ui {
class EmojiPicker;
}

class EmojiPicker : public QDialog
{
    Q_OBJECT

public:
    explicit EmojiPicker(QWidget *parent = nullptr);
    ~EmojiPicker();
    bool event(QEvent *event)
    {
        if(QEvent::WindowDeactivate == event->type())
        {
            hide(); //这里就是啦！
        }
        return QWidget::event(event);
    }
private slots:
    void on_tableWidget_itemClicked(QTableWidgetItem *item);
signals:
    void chooseEmoji(QString);
private:
    Ui::EmojiPicker *ui;
};

#endif // EMOJIPICKER_H
