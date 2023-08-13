#include "control.h"

ct_control::ct_control()
{
	ready_demux = false;
	demux_finished = false;
	decode_finished = false;
	audio_ready_decode = false;
}

void ct_control::wait_demux()
{
	std::unique_lock<std::mutex> lock(mutex);
	while (!ready_demux)
	{
		cv.wait(lock);
	}
}

void ct_control::wait_demux_a()
{
	std::unique_lock<std::mutex> lock(mutex);
	while (!audio_ready_decode)
	{
		cv.wait(lock);
	}
}

void ct_control::notify_demux()
{
	std::unique_lock<std::mutex> lock(mutex);
	ready_demux = true;
	cv.notify_all();
}

void ct_control::notify_demux_a()
{
	std::unique_lock<std::mutex> lock(mutex);
	audio_ready_decode = true;
	cv.notify_all();
}

