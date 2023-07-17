#include "single_core.h"

single_core* single_core::ptr_this = nullptr;

// Free the memory allocated for the ffmpeg data structures
single_core::~single_core()
{
	if (ptr_FmtCtx)avformat_free_context(ptr_FmtCtx);
	if(ptr_frame)av_frame_free(&ptr_frame);
	if(ptr_packet)av_packet_free(&ptr_packet);
	if(ptr_VideoCodecCtx)avcodec_free_context(&ptr_VideoCodecCtx);
}

// Initialize the ffmpeg data structures
single_core* single_core::get_instance()
{
	if (ptr_this == nullptr)
	{
		ptr_this = new single_core();
		return ptr_this;
	}
		return ptr_this;
}
