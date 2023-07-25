#pragma once
#ifndef __DECODE__
#define __DECODE__
#include"mainwindow.h"
#include"single_core.h"
#include"av_queues.h"

/*
* @author Grayson_Wang
* @param core_ptr : a pointer to a single_core object
* @brief 
*/
extern int ct_decode_video(single_core* core_ptr, av_queues* queues);

extern int ct_decode_audio(single_core* core_ptr, av_queues* queues);

#endif // !__DECODE__
