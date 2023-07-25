#ifndef __SINGLE_CORE__
#define __SINGLE_CORE__

#include"mainwindow.h"
#include<deque>
#include<condition_variable>
#include<mutex>

/*
	Preprocessing
*/

/* A singleton class for ffmpeg - related data - structures 
 *  to storage and initialize. 
*/
class single_core
{
public:
	static single_core* get_instance();

	// Copy constructor and assignment operator are deleted
	single_core(single_core& other) = delete;
	single_core& operator=(single_core& other) = delete;

public:
	// compulsory data structures for ffmpeg
	AVFormatContext* ptr_FmtCtx;
	AVCodecContext* ptr_VideoCodecCtx;
	AVStream* ptr_stream_video;
	AVStream* ptr_stream_audio;
	AVPacket* ptr_packet;
	AVFrame* ptr_frame;
	const AVCodec* ptr_VideoCodec;
	const AVCodec* ptr_AudioCodec;
	AVCodecContext* ptr_video_codec_ctx;
	AVCodecContext* ptr_audio_codec_ctx;


private:
	single_core() {};
	~single_core();
	static single_core* ptr_this;
};

enum CT_ERROR {
	NO_TRACK_FOUND=1, // cannot find any video or audio track due to given url
	NO_CODEC_FOUND, // cannot find any video or audio codec due to given filmlet format
	VIDEOCODEC_PARAM_COPY_FAILED, // cannot copy video codec parameters to codec context
	AUDIOCODEC_PARAM_COPY_FAILED, // cannot copy audio codec parameters to codec context
	VIDEOCODEC_BIND_FAILED, // cannot bind video codec to codec context
	AUDIOCODEC_BIND_FAILED, // cannot bind audio codec to codec context

};

#endif // !__SINGLE_CORE__

