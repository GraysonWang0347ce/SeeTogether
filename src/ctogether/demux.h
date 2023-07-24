#pragma once
#ifndef __DEMUX__
#define __DEMUX__
#include"mainwindow.h"
#include"single_core.h"

/*
* @author Grayson_Wang
* @param core_ptr : a pointer to a single_core object
* @brief Thread ct_demux: demuxing
		1. open input from a given url
		2. get stream info
		3. find proper stream index for
		video and audio
		4. demuxing
*/
extern int ct_demux(single_core* core_ptr);

#endif // !__DEMUX__

