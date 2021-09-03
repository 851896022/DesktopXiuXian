#ifndef MAKENAME_H
#define MAKENAME_H

#include <QObject>
#include <QTime>
#include <QList>
class MakeName : public QObject
{
    Q_OBJECT
public:
    explicit MakeName(QObject *parent = nullptr);
    static QString getName();
    static qreal getWuxing(int jieduan);
    static qreal getRandData(qreal data,qreal fanwei = 0.9);
signals:

};

#endif // MAKENAME_H
