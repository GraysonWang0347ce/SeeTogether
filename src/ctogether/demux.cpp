#include"mainwindow.h"
#include"single_core.h"
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
int ct_demux(single_core* core_ptr)
{
	// TODO: to preserve multiple video&audio stream, use vector later
	static int idx_video_stream = -1;
	static int idx_audio_stream = -1;

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
	int ret = avformat_open_input(&ptr_fmt, path, NULL, NULL);

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

	// demuxing
	while (!av_read_frame(ptr_fmt, ptr_packet))
	{
		if (ptr_packet->stream_index == idx_video_stream)
		{
			// if video packet's size > __MAX__QUEUE_LEN__ definded, then wait
			while (core_ptr->video_packet_queue.size() >= __MAX_QUEUE_LEN__);

			// push video packet into queue
			core_ptr->video_packet_queue.push_back(ptr_packet);

			// notify video thread to decode
			core_ptr->video_packet_cv.notify_one();
			
			/*break;*/
		}
		else if (ptr_packet->stream_index == idx_audio_stream)
		{
			// if audio packet's size > __MAX__QUEUE_LEN__ definded, then wait
			while (core_ptr->audio_packet_queue.size() >= __MAX_QUEUE_LEN__);

			// push audio packet into queue
			core_ptr->audio_packet_queue.push_back(ptr_packet);

			// notify audio thread to decode
			core_ptr->audio_packet_cv.notify_one();
		}
		// TODO: preserved for AVMEDIATYPE_SUBTITLE or so
		//else if(){}
		else
		{
			// if packet is not like above, then free it
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
}