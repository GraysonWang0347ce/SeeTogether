#ifndef __SINGLE_CORE__
#define __SINGLE_CORE__

#include<deque>
#include<condition_variable>
#include<mutex>
#include"error.h"
#include"ffmpeg_header.h"

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
	AVStream* ptr_stream_video;
	AVStream* ptr_stream_audio;
	AVPacket* ptr_packet;
	AVFrame* ptr_frame;
	const AVCodec* ptr_VideoCodec;
	const AVCodec* ptr_AudioCodec;
	AVCodecContext* ptr_video_codec_ctx;
	AVCodecContext* ptr_audio_codec_ctx;

	SwsContext* ptr_sws_ctx;

private:
	single_core() {};
	~single_core();
	static single_core* ptr_this;
};



#endif // !__SINGLE_CORE__

