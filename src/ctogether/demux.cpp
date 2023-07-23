#include"mainwindow.h"
#include"single_core.h"
#include"demux.h"

void ct_demux(single_core* core_ptr)
{
	static int idx_video_stream = -1;
	static int idx_audio_stream = -1;

	// To allocate avformat context
	core_ptr->ptr_FmtCtx = avformat_alloc_context();

	// using ptr_fmt instead of core_ptr->ptr_fmt
	AVFormatContext* ptr_fmt = core_ptr->ptr_FmtCtx;

	// TODO: get path dynamically depending on means boosting this app
	const char* path = "D:\\Documents\\classInfoAssistant\\Ad\\ad.mp4";

	// open input from a given url
	int ret = avformat_open_input(&ptr_fmt, path, NULL, NULL);

	// get stream info
	avformat_find_stream_info(ptr_fmt, nullptr);

	// DEBUG INFO
	for (int i = 0; i < ptr_fmt->nb_streams; i++)
	{
		if (ptr_fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
			idx_video_stream = i;
		if (ptr_fmt->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
			idx_audio_stream = i;

		switch (ptr_fmt->streams[i]->codecpar->codec_type)
		{
		case AVMEDIA_TYPE_VIDEO:
			qDebug() << ptr_fmt->streams[i]->codecpar->bit_rate;
			break;
		case AVMEDIA_TYPE_AUDIO:
			qDebug() << ptr_fmt->streams[i]->codecpar->bit_rate;
			break;
		default:
			break;
		}
	}

	qDebug() << ptr_fmt->nb_streams;

	avformat_free_context(ptr_fmt);
}