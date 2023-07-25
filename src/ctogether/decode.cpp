#include "decode.h"
#include"mainwindow.h"
#include"single_core.h"
#include"av_queues.h"

int ct_decode_video(single_core* core_ptr , av_queues* queues)
{
	// if video packet queue is empty, wait for video packet
	std::unique_lock<std::mutex> lock(queues->video_packet_mutex);
	while (queues->ct_is_queue_empty(queues->video_packet_queue))
	{
		queues->video_packet_cv.wait(lock);
	}

	// find video decoder
	

	/*while ()
	{

	}*/
	return 0;
}

int ct_decode_audio(single_core* core_ptr, av_queues* queues)
{
	return 0;
}
