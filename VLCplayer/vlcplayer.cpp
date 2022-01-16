#pragma execution_character_set("utf-8")
#include "vlcplayer.h"
#include <QFileDialog>
#pragma comment(lib, "libvlc")
#pragma comment(lib, "libvlccore")

VLCplayer::VLCplayer(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // 1. 创建VLC Application的实例
    m_vlcApp = libvlc_new(0, NULL);
    m_vlcMedia = NULL;
    m_vlcPlayer = NULL;

    setWindowFlag(Qt::Window, false);
    // 2. 视频地址
    connect(ui.btnOpen, &QAbstractButton::clicked, [=]() {
        QString fileName = QFileDialog::getOpenFileName(this, "选择视频文件", "E:\\videos", "*.mp4");
        if (fileName.isEmpty())
            return;
#ifdef Q_OS_WIN
        fileName = fileName.replace("/", "\\");
#endif
        if (m_vlcMedia) {
            libvlc_media_release(m_vlcMedia);
            m_vlcMedia = NULL;
        }
        m_vlcMedia = libvlc_media_new_path(m_vlcApp, fileName.toStdString().c_str());
        if (m_vlcMedia) {
            // 3. 创建VLC Player
            if (m_vlcPlayer)
                return;
            m_vlcPlayer = libvlc_media_player_new_from_media(m_vlcMedia);

            // 4. 设置视频目标显示窗口
#ifdef Q_OS_UNIX
            libvlc_media_player_set_xwindow(m_vlcPlayer, ui.videobox->winId());
#else
            libvlc_media_player_set_hwnd(m_vlcPlayer, (HWND)ui.videobox->winId());
#endif

        }
    });
    connect(ui.btnPlay, &QAbstractButton::clicked, [=]() {
        if (!m_vlcPlayer)
            return;
        if (libvlc_media_player_is_playing(m_vlcPlayer))
            libvlc_media_player_pause(m_vlcPlayer);
        else
            libvlc_media_player_play(m_vlcPlayer);
    });
}

VLCplayer::~VLCplayer()
{
    libvlc_media_player_release(m_vlcPlayer);
    libvlc_media_release(m_vlcMedia);
    libvlc_release(m_vlcApp);
}
