#pragma once
#ifndef __DEMUX__
#define __DEMUX__

#include"ffmpeg_header.h"
#include"single_core.h"
#include"av_queues.h"
#include"control.h"
#include<qdebug.h>
#include<QThread>

class ct_demux :public QThread
{
	Q_OBJECT
public:
	ct_demux(single_core* o_core_ptr, av_queues* o_queues, ct_control* o_controller) ;
	~ct_demux() {};
		/*
* @author Grayson_Wang
* @param core_ptr : a pointer to a single_core object
* @brief Thread ct_demux: demuxing
		1. open input from a given url
		2. get stream info
		3. find proper stream index for
		video and audio
		4. demuxing
	@return 0 on success or enum CT_ERROR(in single_core.h)
*/
protected:
	void run();

private:
	single_core* core_ptr;
	av_queues* queues;
	ct_control* control;
};


#endif // !__DEMUX__

