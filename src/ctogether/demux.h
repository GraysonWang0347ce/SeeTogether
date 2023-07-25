#pragma once
#ifndef __DEMUX__
#define __DEMUX__
#include"mainwindow.h"
#include"single_core.h"
#include"av_queues.h"

/*
* @author Grayson_Wang
* @param core_ptr : a pointer to a single_core object
* @brief Thread ct_demux: demuxing
		1. open input from a given url
		2. get stream info
		3. find proper stream index for
		video and audio
		4. demuxing
	@return 0 on success or enum CT_ERROR(in single_core.h)
*/
extern int ct_demux(single_core* core_ptr, av_queues* queues);

#endif // !__DEMUX__

