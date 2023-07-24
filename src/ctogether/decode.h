#pragma once
#ifndef __DECODE__
#define __DECODE__
#include"mainwindow.h"
#include"single_core.h"

/*
* @author Grayson_Wang
* @param core_ptr : a pointer to a single_core object
* @brief 
*/
extern void ct_decode_video(single_core* core_ptr);

extern void ct_decode_audio(single_core* core_ptr);

#endif // !__DECODE__
