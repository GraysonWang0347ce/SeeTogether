#pragma once
#ifndef __DECODE__
#define __DECODE__

#include"single_core.h"
#include"av_queues.h"
#include"ffmpeg_header.h"
#include"control.h"
#include<QThread>

class ct_decode_video:public QThread
{
	Q_OBJECT

public:
	ct_decode_video(single_core* o_core_ptr, av_queues* o_queues, ct_control* o_controller);
	~ct_decode_video() {};

	/*
	* @author Grayson_Wang
	* @param core_ptr : a pointer to a single_core object
	* @brief
	*/
protected:
	void run();

	//int ct_decode_audio(single_core* core_ptr, av_queues* queues, ct_control* control);

signals:
        void ct_image_decoded(QImage image);
        void ct_audio_decoded(QByteArray audio);

private:
	single_core* core_ptr;
	av_queues* queues;
	ct_control* control;
};

#endif // !__DECODE__
