#include "wordfilter.h"
#include <QFile>
WordFilter::WordFilter(QObject *parent) : QObject(parent)
{
    QFile f("://SensitiveWords.txt");
    f.open(QIODevice::ReadOnly);
    while (!f.atEnd())
    {
        QString key = f.readLine();
        key.remove("\r");
        key.remove("\n");
        _keyWord.append(key);
    }

}
QString WordFilter::filter(QString msg)
{
    for(int i=0;i<_keyWord.count();i++)
    {
        msg.replace(_keyWord.at(i),"真棒",Qt::CaseInsensitive);
    }
    return msg;
}
