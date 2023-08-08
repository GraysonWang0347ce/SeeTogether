#include"demux.h"

/*
* @author Grayson_Wang
* @param core_ptr : a pointer to a single_core object
* @brief Thread ct_demux: demuxing
		1. open input from a given url 
		2. get stream info
		3. find proper stream index for 
		video and audio
		4. demuxing
*/
void ct_demux::run()
{
	// TODO: to preserve multiple video&audio stream, use vector later
	static int idx_video_stream = -1;
	static int idx_audio_stream = -1;
	int ret = 0;


	//return number, 0 on success or enum CT_ERROR
	int ret_num = 0;

	/*
		To allocate avformat context
		and packet
	*/
	core_ptr->ptr_FmtCtx = avformat_alloc_context();
	core_ptr->ptr_packet = av_packet_alloc();

	/*
		using ptr_fmt instead of core_ptr->ptr_fmt
					ptr_packet instead of core_ptr->ptr_packet
	*/
	AVFormatContext* ptr_fmt = core_ptr->ptr_FmtCtx;
	AVPacket* ptr_packet = core_ptr->ptr_packet;

	// TODO: get path dynamically depending on means boosting this app
	const char* path = "D:\\Documents\\classInfoAssistant\\Ad\\ad.mp4";

	// open input from a given url
    ret = avformat_open_input(&ptr_fmt, path, NULL, NULL);

	// get stream info
	avformat_find_stream_info(ptr_fmt, nullptr);

	// find proper stream index for video and audio
	for (int i = 0; i < ptr_fmt->nb_streams; i++)
	{
		if (ptr_fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			idx_video_stream = i;
		if (ptr_fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			idx_audio_stream = i;

		// Commentable
		switch (ptr_fmt->streams[i]->codecpar->codec_type)
		{
		case AVMEDIA_TYPE_VIDEO:
			qDebug() << "Video stream bit-rate: " << ptr_fmt->streams[i]->codecpar->bit_rate;
			break;
		case AVMEDIA_TYPE_AUDIO:
			qDebug() << "Audio stream bit-rate: " << ptr_fmt->streams[i]->codecpar->bit_rate;
			break;
		default:
			break;
		}
		// Commentable
	}

	/*qDebug() << ptr_fmt->nb_streams;*/

	if (!idx_video_stream && !idx_audio_stream)
	{
		ret_num = NO_TRACK_FOUND;
		goto _OUT;
	}

	// BELOW are operates  initializing AVCodex-related data structures 
			// get the video codec according to stream info
			core_ptr->ptr_stream_video = ptr_fmt->streams[idx_video_stream];
			core_ptr->ptr_stream_audio = ptr_fmt->streams[idx_audio_stream];

			core_ptr->ptr_VideoCodec = avcodec_find_decoder(core_ptr->ptr_stream_video
																														->codecpar->codec_id);
			core_ptr->ptr_AudioCodec = avcodec_find_decoder(core_ptr->ptr_stream_audio
																														->codecpar->codec_id);

			if (!core_ptr->ptr_VideoCodec &&  !core_ptr->ptr_AudioCodec)
			{
				ret_num = NO_CODEC_FOUND;
				goto _OUT;
			}

			// allocate codec context
			core_ptr->ptr_video_codec_ctx = avcodec_alloc_context3(core_ptr->ptr_VideoCodec);
			core_ptr->ptr_audio_codec_ctx = avcodec_alloc_context3(core_ptr->ptr_AudioCodec);

			// copy codec parameters to codec context
			ret = avcodec_parameters_to_context(core_ptr->ptr_video_codec_ctx,
																				core_ptr->ptr_stream_video->codecpar);
			if (ret < 0)
			{
				ret_num = VIDEOCODEC_PARAM_COPY_FAILED;
				goto _OUT;
			}
			ret = avcodec_parameters_to_context(core_ptr->ptr_audio_codec_ctx,
																				core_ptr->ptr_stream_audio->codecpar);
			if (ret < 0)
			{
				ret_num = VIDEOCODEC_PARAM_COPY_FAILED;
				goto _OUT;
			}

			// set thread count
			core_ptr->ptr_video_codec_ctx->thread_count = std::thread::hardware_concurrency()/2;
			core_ptr->ptr_audio_codec_ctx->thread_count = std::thread::hardware_concurrency() / 2;

			if (core_ptr->ptr_VideoCodec->id == AV_CODEC_ID_H264)
			{
				av_opt_set(core_ptr->ptr_video_codec_ctx->priv_data,"preset", "slow", 0);
				av_opt_set(core_ptr->ptr_video_codec_ctx->priv_data,"tune", "zerolatency", 0);
			}

			// bind codec to codec context
			ret = avcodec_open2(core_ptr->ptr_video_codec_ctx,
												core_ptr->ptr_VideoCodec,nullptr);
			if (ret < 0)
			{
				ret_num = VIDEOCODEC_BIND_FAILED;
				goto _OUT;
			}
			ret = avcodec_open2(core_ptr->ptr_audio_codec_ctx,
														core_ptr->ptr_AudioCodec,nullptr);
			if (ret < 0)
			{
				ret_num = AUDIOCODEC_BIND_FAILED;
				goto _OUT;
			}
	 // ABOVE are operates  initializing AVCodex-related data structures 

		control->notify_demux();

	// demuxing
	while (!av_read_frame(ptr_fmt, ptr_packet))
	{
		if (ptr_packet->stream_index == idx_video_stream)
		{
			// push it into video packet queue
			queues->packet_queue[VIDEO]->ct_push_back(*ptr_packet);
	
			/*break;*/
		}
		else if (ptr_packet->stream_index == idx_audio_stream)
		{
			// push it into audio packet queue
			queues->packet_queue[AUDIO]->ct_push_back(*ptr_packet);
		}
		// TODO: preserved for AVMEDIATYPE_SUBTITLE or so
		//else if(){}
		else
		{
			av_packet_unref(ptr_packet);
		}
	}

	/*
		Garbage collection
	*/
	_OUT:
	if (ptr_fmt)
	{
		avformat_free_context(ptr_fmt);
	}
	if (ptr_packet)
	{
		av_packet_free(&ptr_packet);
	}
	return;
}

ct_demux::ct_demux(single_core* o_core_ptr, av_queues* o_queues, ct_control* o_controller)
{
	core_ptr = o_core_ptr;
	queues = o_queues;
	control = o_controller;
}
