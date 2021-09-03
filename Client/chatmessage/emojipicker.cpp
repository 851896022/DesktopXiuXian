#include "emojipicker.h"
#include "ui_emojipicker.h"
#include <QFile>
EmojiPicker::EmojiPicker(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmojiPicker)
{
    ui->setupUi(this);
    setWindowFlag(Qt::Window);
    setWindowFlags (Qt::FramelessWindowHint);
    ui->tableWidget->setColumnCount(16);
    ui->tableWidget->setRowCount(68);
    ui->tableWidget->setItem(0,0,new QTableWidgetItem("asd"));
    QFile f(":/chatmessage/emojis.txt");
    f.open(QIODevice::ReadOnly);
    QString all = f.readAll();
    auto emojilist=all.split("\n");
    int i=0;
    for(int y=0;y<68;y++)
    {
        for(int x=0;x<16;x++)
        {
            QString str = emojilist.at(i);
            str.remove("\n");
            ui->tableWidget->setItem(y,x,new QTableWidgetItem(str));
            ui->tableWidget->item(y,x)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            i++;
        }
    }
}

EmojiPicker::~EmojiPicker()
{
    delete ui;
}

void EmojiPicker::on_tableWidget_itemClicked(QTableWidgetItem *item)
{
   emit chooseEmoji( item->text());
}

