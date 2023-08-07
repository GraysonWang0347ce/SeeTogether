#pragma once
#include<mutex>
#include<semaphore>

class ct_control
{

public:

	ct_control():ready_demux(false) {};
	~ct_control() {};

	std::mutex mutex;
	std::condition_variable cv;

	bool ready_demux;

	void wait_demux();
	void notify_demux();

};

