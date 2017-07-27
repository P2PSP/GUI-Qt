#ifndef SIMPLEPLAYER_H_
#define SIMPLEPLAYER_H_

#include <QMainWindow>
#include <QFileDialog>
#include <QInputDialog>
#include <QApplication>

#include <add_dialog.h>
#include <addchannels.h>
#include <rundialog.h>
#include <helper.h>
#include <iostream>
#include <vlc/vlc.h>
#include "peerthread.h"
#include "sink.h"
#include "rundialogstorage.h"
#include <util/trace.h>
namespace Ui {
    class SimplePlayer;
}

//class EqualizerDialog;
class runDialog;
class PeerThread;

class SimplePlayer : public QMainWindow
{
    Q_OBJECT
public:
    explicit SimplePlayer(QWidget *parent = 0);
    ~SimplePlayer();
    void closeWindow();

private slots:
//    void openLocal();
//    void openUrl();

    void playFile(QString file);

    void changeVolume(int newVolume);

    void on_checkBox_clicked();

    void on_fullscreen_clicked();

    void on_pause_clicked();

    void addChannelSlot(addChannels);

    void runPeerClicked();

    void on_volume_valueChanged(int value);

    void on_volumeButton_clicked();

    void stats(int download,int upload,int peerSize);

    void receivePeerParameters(runDialogStorage runParameters);

    void on_stop_clicked();

    void logit(QString text);
signals:

    void sendChannels(vector<addChannels>);

    void runPeer(runDialogStorage runParameters);
private:
    Ui::SimplePlayer *ui;

    bool _isPlaying;
    libvlc_instance_t *_vlcinstance;
    libvlc_media_player_t *_mp;
    libvlc_media_t *_m;

    void play();
    void pause();
    void resume();

//    QString downloadSpeed;
//    QString uploadSpeed;
//    QString peerSize;
    helper *help1;
    add_dialog *channel1;
    runDialog *rundialogobj;
    PeerThread *peerthreadobj;
    vector<addChannels> channels;
    Sink *si;
    //For playing the vlc stream
    void openUrl();
    //Log Method
    void logInit();
};

#endif // SIMPLEPLAYER_H_
