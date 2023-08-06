#include "av_queues.h"

av_queues::av_queues()
{
	for (int i = 0; i < __NUM_PACKETS__; i++)
	{
		packet_queue[i] = new av_queue(AVPacket());
	}
	image_queue = new av_queue<QImage>(QImage());
	frame_queue = new av_queue<AVFrame>(AVFrame());
}

av_queues::~av_queues()
{
	for (int i = 0; i < __NUM_PACKETS__; i++)
	{
		packet_queue[i]->~av_queue();
		delete packet_queue[i];
	}

	image_queue->~av_queue();
	frame_queue->~av_queue();

	delete image_queue;
	delete frame_queue;
}
