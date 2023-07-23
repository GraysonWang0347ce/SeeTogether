#ifndef __SINGLE_CORE__
#define __SINGLE_CORE__

#include"mainwindow.h"
#include<deque>

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
	AVFormatContext* ptr_FmtCtx;
	AVCodecContext* ptr_VideoCodecCtx;
	AVStream* ptr_stream;
	AVPacket* ptr_packet;
	AVFrame* ptr_frame;

	std::deque<AVPacket>* video_packet_queue;
	std::deque<AVPacket>* audio_packet_queue;

	std::deque<AVFrame>* video_frame_queue;
	std::deque<AVFrame>* audio_frame_queue;

private:
	single_core() {};
	~single_core();
	static single_core* ptr_this;
};
#endif // !__SINGLE_CORE__

