#include "logger.h"

Logger::Logger(QObject *parent):QObject(parent)
{
    player=new SimplePlayer();
    QObject::connect(this,SIGNAL(logEvent(QString)),player,SLOT(eventLog(QString)));
}

void Logger::log(int type,QString message)
{
    message=this->logMessage(type,message);
    emit logEvent(message);
}

QString Logger::logMessage(int type, QString message)
{
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString messageLog;
    switch (type) {
    case LOG_INFO:
        messageLog = QString("[" + timestamp + "] Info: " + message);
        break;
    case LOG_WARNING:
        messageLog = QString("[" + timestamp + "] Warning: " + message);
        break;
    case LOG_ERROR:
        messageLog = QString("[" + timestamp + "] Error: " + message);
        break;
    case LOG_DEBUG:
        messageLog = QString("[" + timestamp + "] Debug: " + message);
        break;
    case LOG_TRACE:
        messageLog = QString("[" + timestamp + "] Info: " + message);
        break;
    default:
        messageLog = QString("[" + timestamp + "] Error: Something want wrong");
        break;
    }
    return messageLog;
}
