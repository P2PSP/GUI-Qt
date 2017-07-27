#include "sink.h"

Sink::Sink(QObject *parent) : QObject(parent)
{

}

void Sink::consume(const boost::log::record_view &rec, const std::string &str)
{
    QString newString = QString::fromStdString(str);
    std::cout<<"In consume"<<str<<std::endl;
    emit logthis(newString);
}
