#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_vlcplayer.h"
namespace Ui {
	class VLCplayerClass;
}
#include "vlc/vlc.h"
class VLCplayer : public QMainWindow
{
	Q_OBJECT

public:
	VLCplayer(QWidget *parent = Q_NULLPTR);
	~VLCplayer();

private:
	Ui::VLCplayerClass ui;
	libvlc_instance_t* m_vlcApp;
	libvlc_media_t* m_vlcMedia;
	libvlc_media_player_t* m_vlcPlayer;
};
