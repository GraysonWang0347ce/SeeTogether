#pragma once
#include"mainwindow.h"

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

private:
	single_core() {};
	~single_core();
	static single_core* ptr_this;
};

