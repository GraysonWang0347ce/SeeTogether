#pragma once
#ifndef __AV_QUEUES__
#define __AV_QUEUES__
#include"mainwindow.h"
#include"single_core.h"
#include<deque>
#include<condition_variable>
#include<mutex>

#define __MAX_QUEUE_LEN__ 128

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
	bool ct_is_queue_full(std::deque<T>& queue) const
	{
		return queue.size() >= __MAX_QUEUE_LEN__;
	}

	template <class T>
	bool ct_is_queue_empty(std::deque<T>& queue) const
	{
		return queue.empty();
	}


	inline void ct_queue_pshback_v(std::deque<AVPacket*>& queue, AVPacket* element)
	{
		AVPacket* temp = av_packet_clone(element);
		video_packet_mutex.lock();
		queue.emplace_back((AVPacket*)temp);
		video_packet_mutex.unlock();
		return;
	}

	inline void ct_queue_pshback_v(std::deque<AVFrame*>& queue, AVFrame* element)
	{
		AVFrame* temp = av_frame_clone(element);
		video_frame_mutex.lock();
		queue.emplace_back((AVFrame*)temp);
		video_frame_mutex.unlock();
		return;
	}

	inline void ct_queue_pshback_a(std::deque<AVPacket*>& queue, AVPacket* element)
	{
		AVPacket* temp = av_packet_clone(element);
		audio_packet_mutex.lock();
		queue.emplace_back((AVPacket*)temp);
		audio_packet_mutex.unlock();
		return;
	}

	inline void ct_queue_pshback_a(std::deque<AVFrame*>& queue, AVFrame* element)
	{
		AVFrame* temp = av_frame_clone(element);
		audio_frame_mutex.lock();
		queue.emplace_back((AVFrame*)temp);
		audio_frame_mutex.unlock();
		return;
	}

	template <class T>
	T& ct_queue_front(std::deque<T>& queue) 
	{
		auto temp = queue.front();
		return temp;
	}


	void ct_queue_popfront(std::deque<AVPacket*>& queue)
	{
		av_packet_free(&queue.front());
		queue.pop_front();
		return;
	}

	void ct_queue_popfront(std::deque<AVFrame*>& queue)
	{
		av_frame_free(&queue.front());
		queue.pop_front();
		return;
	}

	template <class T>
	size_t ct_queue_getlength(std::deque<T>& queue)
	{
		return queue.size();
	}

};


#endif // !__AV_QUEUES__


