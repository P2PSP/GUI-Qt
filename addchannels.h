#ifndef ADDCHANNELS_H
#define ADDCHANNELS_H

#include <string>
#include <iostream>
#include <QPixmap>
#include <QJsonObject>
#include <QBuffer>
#include <QJsonArray>

using namespace std;

class addChannels
{
public:
    addChannels();

    string getName() const;
    void setName(const string &value);

    string getDescription() const;
    void setDescription(const string &value);

    string getThumbnailUrl() const;
    void setThumbnailUrl(const string &value);

    string getSplitterAddress() const;
    void setSplitterAddress(const string &value);

    unsigned int getSplitterPort() const;
    void setSplitterPort(unsigned int value);

    QPixmap getThumbnail() const;
    void setThumbnail(const QPixmap value);

    void read(const QJsonObject &json);
    void write(QJsonObject &json) const;
private:
    string name;
    string description;
    string thumbnailUrl;
    string splitterAddress;
    unsigned int splitterPort;
    QPixmap thumbnail;
};

#endif // ADDCHANNELS_H
