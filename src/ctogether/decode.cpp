#include "decode.h"

ct_decode_video::ct_decode_video(single_core* o_core_ptr, av_queues* o_queues, ct_control* o_controller)
{
	core_ptr = o_core_ptr;
	queues = o_queues;
	control = o_controller;
}


void ct_decode_video::run()
{
	AVFrame* ptr_frame = av_frame_alloc();
	AVPacket* ptr_packet = av_packet_alloc();
	int ret_num = 0;
	SwsContext* ptr_ctx_sws = core_ptr->ptr_sws_ctx;
	int image_size = 0;
	uint8_t* ptr_imgbuffer;
	AVFrame* ptr_frame_rgb32;

	// waiting for demux thread to start
	control->wait_demux();

	// start decoding
	while (true)
	{
		// if video packet queue is empty, wait for video packet

		// get video decoder(initialized at demux.cpp:81) 
	//}


		AVPacket temp = queues->packet_queue[VIDEO]->ct_pop_front();
		av_packet_move_ref(ptr_packet, &temp);
		ret_num = avcodec_send_packet(core_ptr->ptr_video_codec_ctx, ptr_packet);

		while (ret_num >= 0)
		{
			ret_num = avcodec_receive_frame(core_ptr->ptr_video_codec_ctx, ptr_frame);

			if (ret_num == AVERROR(EAGAIN) || ret_num == AVERROR_EOF)
			{
				break;
			}


			ptr_frame_rgb32 = av_frame_alloc();

			// converting to RGB32 picture
			ptr_ctx_sws = sws_getContext(core_ptr->ptr_video_codec_ctx->width,
				core_ptr->ptr_video_codec_ctx->height,
				core_ptr->ptr_video_codec_ctx->pix_fmt,
				core_ptr->ptr_video_codec_ctx->width,
				core_ptr->ptr_video_codec_ctx->height,
				AV_PIX_FMT_RGB32,
				SWS_BILINEAR, nullptr, nullptr, nullptr);

			// DEBUG
			// qDebug()<<"ptr_frame->width: "<<ptr_frame->width;
			// qDebug()<<"ptr_frame->height: "<<ptr_frame->height;

			/*image_size = av_image_get_buffer_size(AV_PIX_FMT_RGB32,
				core_ptr->ptr_video_codec_ctx->width,
				core_ptr->ptr_video_codec_ctx->height,
				1);*/

			QImage img = QImage(core_ptr->ptr_video_codec_ctx->width,
				core_ptr->ptr_video_codec_ctx->height,
				QImage::Format_RGB32);

			// pointers to img's data and linesize zone 
			uint8_t* data[1] = { reinterpret_cast<uint8_t*>(img.bits()) };
			int linesize[1] = { static_cast<int>(img.bytesPerLine()) };



			// allocate memory for image buffer
			// ptr_imgbuffer = (uint8_t*)av_malloc(image_size);

			/*av_image_fill_arrays(ptr_frame_rgb32->data,
																	 ptr_frame_rgb32->linesize,
																	 ptr_imgbuffer,
																	 AV_PIX_FMT_RGB32,
																	 core_ptr->ptr_video_codec_ctx->width,
																	 core_ptr->ptr_video_codec_ctx->height,
																	 1);*/

																	 // scale the YUV420P frame to RGB32 frame
																 /*	sws_scale(ptr_ctx_sws,(uint8_t const*const *)ptr_frame->data,ptr_frame->linesize,
																						 0,core_ptr->ptr_video_codec_ctx->height,
																						 ptr_frame_rgb32->data,
																						 ptr_frame_rgb32->linesize);*/
			sws_scale(ptr_ctx_sws,
				ptr_frame->data,
				ptr_frame->linesize,
				0,
				core_ptr->ptr_video_codec_ctx->height,
				data,
				linesize);


			// until now the complete image showed up
		/*	QImage tmp_img((uint8_t*)ptr_imgbuffer,
												 core_ptr->ptr_video_codec_ctx->width,
												 core_ptr->ptr_video_codec_ctx->height,
												 QImage::Format_RGB32);*/


			// queues->image_queue->ct_push_back(img);

			emit ct_image_decoded(img);



			av_frame_free(&ptr_frame_rgb32);
			// av_free(ptr_imgbuffer);
			if (ptr_ctx_sws)
			{
				sws_freeContext(ptr_ctx_sws);
			}
			if (ptr_frame)
			{
				av_frame_unref(ptr_frame);
			}
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
	return;
}
