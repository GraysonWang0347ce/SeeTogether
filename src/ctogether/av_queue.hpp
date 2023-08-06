#pragma once
#ifndef __AV_QUEUE__
#define __AV_QUEUE__

#include<mutex>
#include<condition_variable>

static std::mutex mutex;
static std::condition_variable cv;

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
	inline T& ct_peek_front();

	inline uint64_t ct_nb_data() const { return nb_data; }
	inline int64_t ct_duration() const { return duration; }
	inline uint64_t ct_size() const { return size; }

	AVFifo* data;
	uint64_t nb_data;
	int64_t duration;
	uint64_t size;


};

template<class T>
av_queue<T>::av_queue(T tmp)
{
	data = av_fifo_alloc2(1, sizeof(T), AV_FIFO_FLAG_AUTO_GROW);
	nb_data = 0;
	duration = 0;
	size = 0;
	/*mutex = new std::mutex();
	cv = new std::condition_variable();
	lock = new std::unique_lock<std::mutex>(*mutex);*/
}

template<class T>
av_queue<T>::~av_queue()
{
	if (data)
	{
		av_fifo_freep2(&data);
		data = nullptr;
	}
	/*if(mutex)
	{
		delete mutex;
		mutex = nullptr;
	}
	if(cv)
	{
		delete cv;
		cv = nullptr;
	}
	if (lock)
	{
		delete lock;
		lock = nullptr;
	}*/
}


/*
	@brief to push a given element to the back of the queue
	@return >=0 on success, another on error
*/
template<class T>
int av_queue<T>::ct_push_back(T& element)
{
	int ret = 0;
	T tmp = element;
	std::unique_lock<std::mutex> lock(mutex);
	ret = av_fifo_write(data, &tmp, 1);
	lock.unlock();

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
inline int av_queue<QImage>::ct_push_back(QImage& element)
{
	int ret = 0;
	QImage tmp = element;
	std::unique_lock<std::mutex> lock(mutex);
	ret = av_fifo_write(data, &tmp, 1);
	lock.unlock();

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
	@return pointer to the beginning element , or nullptr if the queue is empty
*/
template<class T>
T* av_queue<T>::ct_pop_front()
{
	int ret = 0;
	while (nb_data == 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	std::unique_lock<std::mutex> lock(mutex);
	// while(nb_data == 0) { cv.wait(lock); }
	T element;

	ret = av_fifo_read(data, &element, 1);
	lock.unlock();
	if (ret >= 0)
	{
		nb_data--;
		size -= sizeof(element);
		duration -= element.duration ? element.duration : 0;
	}

	return &element;
}

template<>
inline QImage* av_queue<QImage>::ct_pop_front()
{
	int ret = 0;
	while (nb_data == 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
	std::unique_lock<std::mutex> lock(mutex);
	//while(nb_data == 0) { cv.wait(lock); }
	QImage element;

	lock.lock();
	ret = av_fifo_read(data, &element, 1);
	lock.unlock();
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
		std::unique_lock<std::mutex> lock(mutex);
		av_fifo_peek(data, &element, 1, 0);
		lock.unlock();
		return &element;
	}
}

#endif // !__AV_QUEUE__



