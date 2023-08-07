#include "control.h"

void ct_control::wait_demux()
{
	std::unique_lock<std::mutex> lock(mutex);
	while (!ready_demux)
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
