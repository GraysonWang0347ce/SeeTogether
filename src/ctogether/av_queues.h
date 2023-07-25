#pragma once
#ifndef __AV_QUEUES__
#define __AV_QUEUES__
#include"mainwindow.h"
#include"single_core.h"
#include<deque>
#include<condition_variable>
#include<mutex>

#define __MAX_QUEUE_LEN__ 1024

class av_queues
{
public:
	av_queues() {};
	~av_queues() {};

public:
	// queues for video and audio packets
	std::deque<AVPacket*> video_packet_queue;
	std::deque<AVPacket*> audio_packet_queue;

	std::deque<AVFrame*> video_frame_queue;
	std::deque<AVFrame*> audio_frame_queue;

	// locks and condition variables for synchronizing queues
	std::condition_variable video_packet_cv;
	std::condition_variable video_frame_cv;
	std::condition_variable audio_packet_cv;
	std::condition_variable audio_frame_cv;

	std::mutex video_packet_mutex;
	std::mutex video_frame_mutex;
	std::mutex audio_packet_mutex;
	std::mutex audio_frame_mutex;

	template <class T>
	bool ct_is_queue_full(std::deque<T> queue)
	{
		return queue.size() >= __MAX_QUEUE_LEN__;
	}

	template <class T>
	bool ct_is_queue_empty(std::deque<T> queue)
	{
		return queue.empty();
	}

	template <class T>
	void ct_queue_pshback(std::deque<T> queue, T element)
	{
		queue.push_back(element);
	}

	template <class T>
	T ct_queue_front(std::deque<T> queue) const
	{
		return queue.front();
	}

	template <class T>
	T ct_queue_popfront(std::deque<T> queue)
	{
		return queue.pop_front();
	}

	template <class T>
	size_t ct_queue_getlength(std::deque<T> queue)
	{
		return queue.size();
	}

};


#endif // !__AV_QUEUES__


