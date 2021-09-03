#include "dialog.h"

#include <QApplication>

#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include "qlog.h"
#include "chatmessage/chatwindow.h"
#include "chatmessage/emojipicker.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setQuitOnLastWindowClosed(false);
    //ChatWindow chat;
    //chat.show();
    //qInstallMessageHandler(QLog::messageHandler);
    Dialog w;
    w.show();
    //EmojiPicker w;
   // w.show();
   // QObject::connect(&w,&Dialog::sigChatShow,&chat,&ChatWindow::show);
    return a.exec();
}
