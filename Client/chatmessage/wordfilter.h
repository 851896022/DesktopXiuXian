#ifndef WORDFILTER_H
#define WORDFILTER_H

#include <QObject>

class WordFilter : public QObject
{
    Q_OBJECT
public:
    explicit WordFilter(QObject *parent = nullptr);
    QString filter(QString);
signals:
private:
    QStringList _keyWord;
};

#endif // WORDFILTER_H
