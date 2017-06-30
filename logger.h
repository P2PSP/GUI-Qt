#ifndef LOGGER_H
#define LOGGER_H

#include <QtCore>
#include <QObject>
#include "SimplePlayer.h"
#define LOG_INFO 0
#define LOG_WARNING 1
#define LOG_ERROR 2
#define LOG_DEBUG 3
#define LOG_TRACE 4

class SimplePlayer;

class Logger : public QObject
{
    Q_OBJECT
signals:
    void logEvent(QString);

public:
    explicit Logger(QObject *parent=0);
    void log(int type,QString message);
    QString logMessage(int type, QString message);

private:
    SimplePlayer *player;
};

#endif // LOGGER_H
