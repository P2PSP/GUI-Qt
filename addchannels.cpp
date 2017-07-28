#include "addchannels.h"

addChannels::addChannels()
{

}

string addChannels::getName() const
{
    return name;
}

void addChannels::setName(const string &value)
{
    name = value;
}

string addChannels::getDescription() const
{
    return description;
}

void addChannels::setDescription(const string &value)
{
    description = value;
}

string addChannels::getThumbnailUrl() const
{
    return thumbnailUrl;
}

void addChannels::setThumbnailUrl(const string &value)
{
    thumbnailUrl = value;
}

string addChannels::getSplitterAddress() const
{
    return splitterAddress;
}

void addChannels::setSplitterAddress(const string &value)
{
    splitterAddress = value;
}

unsigned int addChannels::getSplitterPort() const
{
    return splitterPort;
}

void addChannels::setSplitterPort(unsigned int value)
{
    splitterPort = value;
}

QPixmap addChannels::getThumbnail() const
{
    return thumbnail;
}

void addChannels::setThumbnail(const QPixmap value)
{
    thumbnail = value;
}

void addChannels::read(const QJsonObject &json)
{
    name = json["name"].toString().toStdString();
    description = json["description"].toString().toStdString();
    splitterAddress = json["address"].toString().toStdString();
    splitterPort = json["port"].toInt();
    QByteArray imgbase64;
    if(json["thumbnail"].isString())
    {
        imgbase64 = json["thumbnail"].toString().toUtf8();
    }
    thumbnail.loadFromData(QByteArray::fromBase64(imgbase64), "PNG");
}

void addChannels::write(QJsonObject &json) const
{
    json["name"] = QString::fromStdString(name);
    json["description"] = QString::fromStdString(description);
    json["address"] = QString::fromStdString(splitterAddress);
    json["port"] = QString::number(splitterPort);
    QByteArray data;
    QBuffer buffer { &data };
    buffer.open(QIODevice::WriteOnly);
    thumbnail.save(&buffer, "PNG");
    auto encoded = buffer.data().toBase64();
    json["thumbnail"]=QString::fromLatin1(encoded);
}
