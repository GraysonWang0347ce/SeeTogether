#pragma once
#include<mutex>
#include<semaphore>


class ct_control
{

public:

	ct_control();
	~ct_control() {};

    std::mutex mutex;
    std::condition_variable cv;

	volatile bool ready_demux;
	volatile bool audio_ready_decode;
	bool demux_finished;
	bool decode_finished;

	void wait_demux();
	void wait_demux_a();

	void notify_demux();
	void notify_demux_a();


};

