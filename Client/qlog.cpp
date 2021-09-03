#include "qlog.h"
#include <QApplication>
#include <QMutex>
#include <QFile>
#include <QDir>
#include <QIODevice>
#include <iostream>
#include <QTextStream>
QLog::QLog(QObject *parent) : QObject(parent)
{

}

void QLog::messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &message_)
{
    static QMutex mutex;
    static QString rawTargetFilePath=qApp->applicationDirPath()+"/log/%1.log";
    static bool argDateFlag=true;

    mutex.lock();

    QString message;
    QString context_info = QString("File:(%1) Line:(%2)").arg(QString(context.file)).arg(context.line);
    QString currentTargetFilePath;

    if ( argDateFlag )
    {
        currentTargetFilePath = rawTargetFilePath.arg( ( ( argDateFlag ) ? ( QDateTime::currentDateTime().toString("yyyy_MM_dd") ) : ( "" ) ) );
    }
    else
    {
        currentTargetFilePath = rawTargetFilePath;
    }

    if ( !QFileInfo::exists( currentTargetFilePath ) )
    {
        QDir().mkpath( QFileInfo( currentTargetFilePath ).path() );
    }

    QFile file( currentTargetFilePath );
    file.open( QIODevice::WriteOnly | QIODevice::Append );
    switch ( type )
    {
        case QtDebugMsg:
        {


            message=message_;


            QTextStream textStream( &file );
            textStream << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << "->" <<context_info<<": "<<message << "\r"<<endl;
            QTextStream outStram(stdout);
            outStram << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << "->" <<context_info<<": "<<message <<endl;

            break;
        }
        case QtWarningMsg:
        {
            message.append("Warning: ");
            message.append(message_);
            QTextStream textStream( &file );
            textStream << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << "->" <<context_info<<": "<<message << "\r"<<endl;

            QTextStream outStram(stdout);
            outStram << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << "->" <<context_info<<": "<<message <<endl;
            break;
        }
        case QtCriticalMsg:
        {
            message.append("Critical: ");
            message.append(message_);
            QTextStream textStream( &file );
            textStream << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << "->" <<context_info<<": "<<message << "\r"<<endl;

            QTextStream outStram(stdout);
            outStram << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << "->" <<context_info<<": "<<message <<endl;
            break;
        }
        case QtFatalMsg:
        {
            message.append("Fatal: ");
            message.append(message_);
            QTextStream textStream( &file );
            textStream << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << "->" <<context_info<<": "<<message << "\r"<<endl;

            QTextStream outStram(stdout);
            outStram << QDateTime::currentDateTime().toString( "yyyy-MM-dd hh:mm:ss.zzz" ) << "->" <<context_info<<": "<<message <<endl;
            break;
        }
        default: { break; }
    }

    mutex.unlock();
}
