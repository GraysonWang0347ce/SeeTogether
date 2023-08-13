#pragma once
#ifndef __AUDIO_OUTPUT__
#define __AUDIO_OUTPUT__

#include"single_core.h"
#include"av_queues.h"
#include"ffmpeg_header.h"
#include"control.h"
#include<QThread>
#include<qaudiooutput.h>
#include<qaudioformat.h>

// AUDIO DECODE THREAD
class ct_audio_output :public QThread
{
	Q_OBJECT

public:
	ct_audio_output(single_core* o_core_ptr, av_queues* o_queues, ct_control* o_controller);
	~ct_audio_output() {};

	/*
	* @author Grayson_Wang
	* @param core_ptr : a pointer to a single_core object
	* @brief
	*/
protected:
	void run();

signals:
	void ct_audio_decoded(QByteArray audio);

private:
	single_core* core_ptr;
	av_queues* queues;
	ct_control* control;
};

#endif // !__AUDIO_OUTPUT__



