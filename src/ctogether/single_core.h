#ifndef __SINGLE_CORE__
#define __SINGLE_CORE__

#include"mainwindow.h"
#include<deque>
#include<condition_variable>
#include<mutex>

/*
	Preprocessing
*/
// max queue length for all queues
#define __MAX_QUEUE_LEN__ 1024

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


	// queues for video and audio packets
	std::deque<AVPacket*> video_packet_queue;
	std::deque<AVPacket*> audio_packet_queue;

	std::deque<AVFrame*> video_frame_queue;
	std::deque<AVFrame*> audio_frame_queue;


	// locks and condition variables for synchronizing queues
	std::condition_variable video_packet_cv;
	std::condition_variable video_frame_cv;
	std::condition_variable audio_packet_cv;
	std::condition_variable audio_frame_cv;

	std::mutex video_packet_mutex;
	std::mutex video_frame_mutex;
	std::mutex audio_packet_mutex;
	std::mutex audio_frame_mutex;

private:
	single_core() {};
	~single_core();
	static single_core* ptr_this;
};

enum CT_ERROR {
	NO_TRACK_FOUND=1, // cannot find any video or audio track due to given url

};

#endif // !__SINGLE_CORE__

