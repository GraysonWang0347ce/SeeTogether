#pragma once
#ifndef __DECODE__
#define __DECODE__

#include"single_core.h"
#include"av_queues.h"


class ct_decode:public QObject
{
	Q_OBJECT
public:
	ct_decode() {};
	~ct_decode() {};

	/*
	* @author Grayson_Wang
	* @param core_ptr : a pointer to a single_core object
	* @brief
	*/
	int ct_decode_video(single_core* core_ptr, av_queues* queues);

	int ct_decode_audio(single_core* core_ptr, av_queues* queues);

signals:
        void ct_image_decoded(QImage ptr_image);
        void ct_audio_decoded(QByteArray ptr_audio);
};

#endif // !__DECODE__
