#pragma once
#ifndef __FFMPEG_HEADER__
#define __FFMPEG_HEADER__

extern "C" {
#pragma comment (lib, "avcodec.lib")
#pragma comment (lib, "avdevice.lib")
#pragma comment (lib, "avfilter.lib")
#pragma comment (lib, "avformat.lib")
#pragma comment (lib, "avutil.lib")
#pragma comment (lib, "swresample.lib")
#pragma comment (lib, "swscale.lib")
#include <libavutil/log.h>
#include<libavformat/avformat.h>
#include<libavutil/opt.h>
#include<libavcodec/avcodec.h>
#include<libswscale/swscale.h>
#include<libavutil/fifo.h>
#include<libswresample/swresample.h>
#include<libavutil/imgutils.h>
}


#endif // !__FFMPEG_HEADER__

