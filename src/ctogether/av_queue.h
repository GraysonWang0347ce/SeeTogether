#pragma once
#ifndef __AV_QUEUE__
#define __AV_QUEUE__

#include"mainwindow.h"
#include<mutex>
#include<condition_variable>

template<class T>
class av_queue
{
public:
	av_queue(T tmp);
	~av_queue();

	/*
	@brief to push a given element to the back of the queue
	@return >=0 on success, another on error
	*/
	int ct_push_back(T& element);

	/*
	@brief pop the beginning element of the queue
	@return reference to the beginning element , or nullptr if the queue is empty
	*/
	T* ct_pop_front();

	/*
	@brief peek the beginning element of the queue
	@return reference to the beginning element , or nullptr if the queue is empty
	*/
	T& ct_peek_front();

	inline uint64_t ct_nb_data() const { return nb_data; }
	inline int64_t ct_duration() const { return duration; }
	inline uint64_t ct_size() const { return size; }

	AVFifo* data;
	uint64_t nb_data;
	int64_t duration;
	uint64_t size;

	std::mutex mutex;
	std::condition_variable cv;
};

template<class T>
av_queue<T>::av_queue(T tmp)
{
	data = av_fifo_alloc2(1, sizeof(T), AV_FIFO_FLAG_AUTO_GROW);
	nb_data = 0;
	duration = 0;
	size = 0;
}

template<class T>
av_queue<T>::~av_queue()
{
	if (data)
	{
		av_fifo_freep2(&data);
		data = nullptr;
	}
}


/*
	@brief to push a given element to the back of the queue
	@return >=0 on success, another on error
*/
template<class T>
int av_queue<T>::ct_push_back(T& element)
{
	int ret = 0;

	mutex.lock();
	ret = av_fifo_write(data, &element, 1);
	mutex.unlock();

	if (ret >= 0)
	{
		nb_data++;
		size += sizeof(element);
		duration += element.duration ? element.duration : 0;
		cv.notify_all();
	}

	return ret;
}

template<>
int av_queue<QImage>::ct_push_back(QImage& element)
{
	int ret = 0;

	mutex.lock();
	ret = av_fifo_write(data, &element, 1);
	mutex.unlock();

	if (ret >= 0)
	{
		nb_data++;
		size += sizeof(element);
		cv.notify_all();
	}

	return ret;
}


/*
	@brief pop the beginning element of the queue
	@return reference to the beginning element , or nullptr if the queue is empty
*/
template<class T>
T* av_queue<T>::ct_pop_front()
{
	int ret = 0;
	std::unique_lock<std::mutex> lock(mutex);
	if (nb_data == 0) { cv.wait(lock); }
	T element;

	mutex.lock();
	ret = av_fifo_read(data, &element, 1);
	mutex.unlock();
	if (ret >= 0)
	{
		nb_data--;
		size -= sizeof(element);
		duration -= element.duration ? element.duration : 0;
	}

	return &element;
}

template<>
QImage* av_queue<QImage>::ct_pop_front()
{
	int ret = 0;
	std::unique_lock<std::mutex> lock(mutex);
	if (nb_data == 0) { cv.wait(lock); }
	QImage element;

	mutex.lock();
	ret = av_fifo_read(data, &element, 1);
	mutex.unlock();
	if (ret >= 0)
	{
		nb_data--;
		size -= sizeof(element);
	}

	return &element;
}


/*
	@brief peek the beginning element of the queue
	@return reference to the beginning element , or nullptr if the queue is empty
*/
template<class T>
T& av_queue<T>::ct_peek_front()
{
	if (nb_data == 0) { return nullptr; }
	else
	{
		T element;
		mutex.lock();
		av_fifo_peek(data, &element, 1, 0);
		mutex.unlock();
		return &element;
	}
}

#endif // !__AV_QUEUE__



