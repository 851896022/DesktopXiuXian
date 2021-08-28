#include "dialog.h"

#include <QApplication>

#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include "qlog.h"
#include "chatmessage/chatwindow.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qApp->setQuitOnLastWindowClosed(false);
    Dialog w;
    w.show();
    return a.exec();
}
