#include "decode.h"
#include"mainwindow.h"
#include"single_core.h"
#include"av_queues.h"

int ct_decode::ct_decode_video(single_core* core_ptr, av_queues* queues)
{
	AVFrame* ptr_frame = av_frame_alloc();
	AVPacket* ptr_packet = av_packet_alloc();
	int ret_num = 0;
	const AVCodec* codec_video;
	AVCodecContext* codec_ctx_video;
	std::unique_lock<std::mutex> lock(queues->video_packet_mutex);
	SwsContext* ptr_ctx_sws = core_ptr->ptr_sws_ctx;
	int image_size = 0;
	uint8_t* ptr_imgbuffer;
	AVFrame* ptr_frame_rgb32;

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


			ptr_frame_rgb32 = av_frame_alloc();
			// converting to RGB32 picture
			ptr_ctx_sws = sws_getContext(codec_ctx_video->width,
												codec_ctx_video->height,
												codec_ctx_video->pix_fmt,
												 codec_ctx_video->width,
												 codec_ctx_video->height,
												 AV_PIX_FMT_RGB32,
												 SWS_BICUBIC,nullptr,nullptr,nullptr);

			image_size = av_image_get_buffer_size(AV_PIX_FMT_RGB32,
																			 codec_ctx_video->width,
																			 codec_ctx_video->height,
																			 1);

			// allocate memory for image buffer
			ptr_imgbuffer = (uint8_t*)av_malloc(image_size);

			av_image_fill_arrays(ptr_frame_rgb32->data,
																	 ptr_frame_rgb32->linesize,
																	 ptr_imgbuffer,
																	 AV_PIX_FMT_RGB32,
																	 codec_ctx_video->width,
																	 codec_ctx_video->height,
																	 1);
			
			sws_scale(ptr_ctx_sws,(uint8_t const*const *)ptr_frame->data,ptr_frame->linesize,
								0,codec_ctx_video->height,ptr_frame_rgb32->data,ptr_frame_rgb32->linesize);

			// until now the complete image showed up
			QImage tmp_img(ptr_imgbuffer,
												 codec_ctx_video->width,
												 codec_ctx_video->height,
												 QImage::Format_RGB32);
			
			QImage img = tmp_img.copy();

			emit ct_image_decoded(img);

			av_frame_free(&ptr_frame_rgb32);
			av_free(ptr_imgbuffer);
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

int ct_decode::ct_decode_audio(single_core* core_ptr, av_queues* queues)
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
