#ifndef PLAYER_H
#define PLAYER_H

#include <QThread>
#include <QObject>

#include"single_core.h"
#include<thread>
#include"demux.h"
#include"decode.h"
#include"av_queues.h"
#include"control.h"

class ct_player : public QThread
{
	Q_OBJECT
public:
    ct_player();
    ~ct_player();

protected:
    void run();

private:
    ct_decode_video* decoder;
    ct_demux* demuxer;
    ct_control* controller;
    QImage Image;
    single_core* core_ptr;
    av_queues* queues;
    QImage img;

public slots:
    void ct_player_got_image(QImage imge);

signals:
    void ct_player_emit_image(QImage img);
};

#endif // PLAYER_H
