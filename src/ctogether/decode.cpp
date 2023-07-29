#include "decode.h"
#include"mainwindow.h"
#include"single_core.h"
#include"av_queues.h"

int ct_decode_video(single_core* core_ptr, av_queues* queues)
{
	AVFrame* ptr_frame = av_frame_alloc();
	AVPacket* ptr_packet = av_packet_alloc();
	int ret_num = 0;
	const AVCodec* codec_video;
	AVCodecContext* codec_ctx_video;
	std::unique_lock<std::mutex> lock(queues->video_packet_mutex);

	// start decoding
	while (true)
	{
		// if video packet queue is empty, wait for video packet
		while (queues->ct_is_queue_empty(queues->video_packet_queue))
		{
			queues->video_packet_cv.wait(lock);
			// get video decoder(initialized at demux.cpp:81) 
		}

		codec_video = core_ptr->ptr_VideoCodec;
		codec_ctx_video = core_ptr->ptr_video_codec_ctx;

		ptr_packet = queues->ct_queue_front(queues->video_packet_queue);
		ret_num = avcodec_send_packet(codec_ctx_video,ptr_packet);
		queues->ct_queue_popfront(queues->video_packet_queue);

		while (ret_num>=0)
		{
			ret_num = avcodec_receive_frame(codec_ctx_video, ptr_frame);
			
			if (ret_num == AVERROR(EAGAIN) || ret_num ==  AVERROR_EOF)
			{
				break;
			}

			qDebug() << ptr_frame->linesize[1];
			exit(-10);

		}

	}

_OUT:
	if (ptr_frame)
	{
		av_frame_free(&ptr_frame);
	}
	if (ptr_packet)
	{
		av_packet_free(&ptr_packet);
	}
	return ret_num;
}

int ct_decode_audio(single_core* core_ptr, av_queues* queues)
{
	// mian loop
	while (true)
	{
		// if audio packet queue is empty, wait for audio packet
		std::unique_lock<std::mutex> lock(queues->audio_packet_mutex);
		while (queues->ct_is_queue_empty(queues->audio_packet_queue))
		{
			queues->audio_packet_cv.wait(lock);
		}

		// getaudio decoder(initialized at demux.cpp:81) 
		const AVCodec* codec_audio = core_ptr->ptr_AudioCodec;
		AVCodecContext* codec_ctx_audio = core_ptr->ptr_audio_codec_ctx;



	}
	return 0;
}
