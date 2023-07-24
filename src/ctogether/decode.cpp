#include "decode.h"
#include"mainwindow.h"
#include"single_core.h"

void ct_decode_video(single_core* core_ptr)
{
	// if video packet queue is empty, wait for video packet
	std::unique_lock<std::mutex> lock(core_ptr->video_packet_mutex);
	while (core_ptr->video_packet_queue.empty())
	{
		core_ptr->video_packet_cv.wait(lock);
	}

	while ()
	{

	}

}

void ct_decode_audio(single_core* core_ptr)
{

}
