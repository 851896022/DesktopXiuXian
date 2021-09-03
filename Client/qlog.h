#ifndef QLOG_H
#define QLOG_H

#include <QObject>
#include <QDateTime>
#include <QTimer>
class QLog : public QObject
{
    Q_OBJECT
public:
    explicit QLog(QObject *parent = nullptr);
signals:

public slots:
    static void messageHandler(QtMsgType type, const QMessageLogContext &, const QString &message_);
};

#endif // QLOG_H
