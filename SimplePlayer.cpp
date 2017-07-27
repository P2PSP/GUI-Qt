#include "SimplePlayer.h"
#include "ui_SimplePlayer.h"

SimplePlayer::SimplePlayer(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::SimplePlayer)
{
    ui->setupUi(this);
    help1 = new helper();
    channel1 = new add_dialog();
    rundialogobj = new runDialog();
    peerthreadobj = new PeerThread();

    _isPlaying=false;

    //Sink Method Initialization
    si=new Sink();
    QObject::connect(si,SIGNAL(logthis(QString)),this,SLOT(logit(QString)));
    //log Initialization
    logInit();
    //create a new libvlc instance
    _vlcinstance=libvlc_new(0, NULL);

    // Create a media player playing environement
    _mp = libvlc_media_player_new (_vlcinstance);

    ui->volume->setSliderPosition(50);
    ui->volume->hide();

    connect(ui->volume, SIGNAL(sliderMoved(int)), this, SLOT(changeVolume(int)));
    //connect(ui->stop, &QPushButton::clicked, this, SLOT(stop()));
    //This Button openUrl will be removed in future releases
    connect(ui->openUrl, &QPushButton::clicked, this, &SimplePlayer::openUrl);

    ui->groupBox->setVisible(false);
    ui->checkBox->hide();//hidden for this release
    ui->openUrl->hide();//should be removed in future
    QObject::connect(ui->actionHelp,SIGNAL(triggered()),help1,SLOT(exec()));
    QObject::connect(ui->checkBox, SIGNAL(clicked(bool)), ui->groupBox, SLOT(setVisible(bool)));
    QObject::connect(ui->actionAdd,SIGNAL(triggered()),channel1,SLOT(exec()));
    QObject::connect(ui->actionRunPeer,SIGNAL(triggered()),this,SLOT(runPeerClicked()));
    QObject::connect(this,SIGNAL(sendChannels(vector<addChannels>)),rundialogobj,SLOT(receiveChannels(vector<addChannels>)));
    QObject::connect(rundialogobj,SIGNAL(runPeer(runDialogStorage)),this,SLOT(receivePeerParameters(runDialogStorage)));
    QObject::connect(this,SIGNAL(runPeer(runDialogStorage)),peerthreadobj,SLOT(Play(runDialogStorage)));
    QObject::connect(peerthreadobj,SIGNAL(stats(int,int,int)),this,SLOT(stats(int,int,int)));
    QObject::connect(channel1,SIGNAL(addChannelSignal(addChannels)),this,SLOT(addChannelSlot(addChannels)));
}
SimplePlayer::~SimplePlayer()
{
    if(_mp)
    {
        /* Stop playing */
        libvlc_media_player_stop (_mp);

        /* Free the media_player */
        libvlc_media_player_release (_mp);

        libvlc_release (_vlcinstance);
    }

    delete ui;
}

void SimplePlayer::logInit()
{
    typedef sinks::synchronous_sink<Sink> sink_t;
    boost::shared_ptr<sink_t> sink (new sink_t());
    boost::shared_ptr<boost::log::core> logc = boost::log::core::get();
    logc->add_sink (sink);
}

void SimplePlayer::stats(int download, int upload, int peerSize)
{
    ui->downloadSpeedValue->setText(QString::number(download));
    ui->uploadSpeedValue->setText(QString::number(upload));
    ui->usersOnlineValue->setText(QString::number(peerSize));
}

void SimplePlayer::closeWindow()
{
    this->window()->close();
}

void SimplePlayer::changeVolume(int newVolume)
{
    if(!_mp)
    {
        return;
    }
    libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
    if (curMedia == NULL)
        return;

    libvlc_audio_set_volume (_mp,newVolume);
}

//void SimplePlayer::eventLog(QString message)
//{
//    ui->log->append(message);
//}

void SimplePlayer::runPeerClicked()
{
    rundialogobj->show();
    emit sendChannels(channels);
}

void SimplePlayer::playFile(QString file)
{
    if(file.mid(0,4)=="http")
    {
         _m = libvlc_media_new_location(_vlcinstance, file.toLatin1());
    }
    else
    {
         _m = libvlc_media_new_path(_vlcinstance, file.toLatin1());
    }

    libvlc_media_player_set_media (_mp, _m);

    int windid = ui->video->winId();
    libvlc_media_player_set_xwindow (_mp, windid );

    play();
}

void SimplePlayer::play()
{
    if(!_mp)
    {
        return;
    }
    /* Play */
    libvlc_media_player_play (_mp);

    ui->pause->setChecked(true);

    _isPlaying=true;
}

void SimplePlayer::pause()
{
    if (!_mp)
        return;

    if (libvlc_media_player_can_pause(_mp))
    {
        libvlc_media_player_set_pause(_mp, true);
        _isPlaying=false;
    }
}

void SimplePlayer::resume()
{
    if (!_mp)
        return;

    if (libvlc_media_player_can_pause(_mp))
    {
        libvlc_media_player_set_pause(_mp, false);
        _isPlaying=true;
    }
}

void SimplePlayer::receivePeerParameters(runDialogStorage runParameters)
{
    emit runPeer(runParameters);
    QString url = "http://localhost:";
    if(runParameters.getPlayerPort()!=0)
    {
        url+=QString::number(runParameters.getPlayerPort());
    }
    else
    {
        url+="9999";
    }
    this->playFile(url);
}

void SimplePlayer::addChannelSlot(addChannels chnnl)
{
    channels.push_back(chnnl);
}

//void SimplePlayer::openLocal()
//{
//    QString file =
//            QFileDialog::getOpenFileName(this, tr("Open file"),
//                                         QDir::homePath(),
//                                         tr("Multimedia files(*)"));

//    if (file.isEmpty())
//        return;

//    _media = new VlcMedia(file, true, _instance);

//    _player->open(_media);
//}

void SimplePlayer::openUrl()
{
    QString url =
            QInputDialog::getText(this, tr("Open Url"), tr("Enter the URL you want to play"));

    if (url.isEmpty())
        return;

    this->playFile(url);
}

void SimplePlayer::on_checkBox_clicked()
{
    ui->checkBox->setCheckable(true);
    if(ui->checkBox->isChecked()){
        ui->checkBox->setText("Hide Channels");
    }
    else{
        ui->checkBox->setText("Show Channels");
    }

}

void SimplePlayer::on_fullscreen_clicked()
{
    isFullScreen() ? showNormal() : showFullScreen();
    QIcon ico;
    ico.addPixmap(QPixmap("://icons/Fit to Width-50.png"),QIcon::Normal,QIcon::On);
    ico.addPixmap(QPixmap("://icons/Normal Screen Filled-50.png"),QIcon::Normal,QIcon::Off);
    ui->fullscreen->setIcon(ico);
    ui->fullscreen->setCheckable(true);
}

void SimplePlayer::on_pause_clicked()
{
    if(ui->pause->isChecked()){
            this->pause();
        }
        else{
            this->resume();
        }
}


void SimplePlayer::on_volume_valueChanged(int value)
{
    QIcon icon;
    if(value>=1&&value<=30)
    {
        icon.addPixmap(QPixmap("://icons/Low Volume-64.png"),QIcon::Normal,QIcon::On);
    }
    else if(value>30&&value<60)
    {
        icon.addPixmap(QPixmap("://icons/Medium Volume-64.png"),QIcon::Normal,QIcon::On);
    }
    else if(value>=60)
    {
        icon.addPixmap(QPixmap("://icons/High Volume-64.png"),QIcon::Normal,QIcon::On);
    }
    else if(value==0)
    {
        icon.addPixmap(QPixmap("://icons/Mute-64.png"),QIcon::Normal,QIcon::On);
    }
    ui->volumeButton->setIcon(icon);
}

void SimplePlayer::on_volumeButton_clicked()
{
    if(ui->volume->isHidden())
    {
        ui->volume->show();
    }
    else
    {
        ui->volume->hide();
    }
}

void SimplePlayer::on_stop_clicked()
{
    if(!_mp)
    {
        return;
    }

    libvlc_media_player_stop(_mp);

    _isPlaying=false;
}

void SimplePlayer::logit(QString text)
{
    ui->log->append(text);
}
