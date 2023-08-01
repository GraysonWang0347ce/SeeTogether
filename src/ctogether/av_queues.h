#pragma once
#ifndef __AV_QUEUES__
#define __AV_QUEUES__
#include"mainwindow.h"
#include"single_core.h"
#include<deque>
#include<condition_variable>
#include<mutex>
#include<qimage.h>
#include"av_queue.h"

#define __MAX_QUEUE_LEN__ 128
#define __NUM_PACKETS__ 2

enum packets {
	VIDEO = 1,
	AUDIO
};

class av_queues
{
public:
	av_queues() ;
	~av_queues() ;

public:
	av_queue<AVPacket>* packet_queue[__NUM_PACKETS__];
	av_queue<QImage>* image_queue;
	av_queue<AVFrame>* frame_queue;
};


#endif // !__AV_QUEUES__


