#pragma once
#ifndef __AV_QUEUE__
#define __AV_QUEUE__

#define __MAX__QUEUENUM__ 2000

#include<mutex>
#include<condition_variable>
#include<typeinfo>
#include<qdebug.h>

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
	T ct_pop_front();

	/*
	@brief peek the beginning element of the queue
	@return reference to the beginning element , or nullptr if the queue is empty
	*/
	inline T& ct_peek_front();

	inline uint64_t ct_nb_data() const { return nb_data; }
	inline int64_t ct_duration() const { return duration; }
	inline uint64_t ct_size() const { return size; }

	// ffmpeg api implement of queue(bug)
	AVFifo* data;

	uint64_t nb_data;
	int64_t duration;
	uint64_t size;

	// C++ std implement of queue
	std::deque<T>* data_queue;

};

template<class T>
av_queue<T>::av_queue(T tmp)
{
#ifdef __USE_AVFIFO__
	data = av_fifo_alloc2(1, sizeof(tmp), AV_FIFO_FLAG_AUTO_GROW);
#endif // __USE_AVFIFO__

	nb_data = 0;
	duration = 0;
	size = 0;
	data_queue = new std::deque<T>();
}

template<class T>
av_queue<T>::~av_queue()
{
#ifdef __USE_AVFIFO__
	if (data)
	{
		av_fifo_freep2(&data);
		data = nullptr;
	}
#endif // __USE_AVFIFO__

	if (data_queue)
	{
		data_queue->clear();
		delete data_queue;
		data_queue = nullptr;
	}

}


/*
	@brief to push a given element to the back of the queue
	@return >=0 on success, another on error
*/
template<>
inline int av_queue<AVPacket>::ct_push_back(AVPacket& element)
{
	int ret = 0;
	AVPacket* tmp = av_packet_alloc();
	av_packet_move_ref(tmp, &element);

	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::unique_lock<std::mutex> lock(mutex);
	while (data_queue->size() >= __MAX__QUEUENUM__)
	{
		cv.wait(lock);
}

#ifdef __USE__AVFIFO__
	ret = av_fifo_write(data, &tmp, 1);
#endif // __USE__AVFIFO__

	data_queue->push_back(*tmp);

	nb_data++;
	size += sizeof(element);
	duration += element.duration ? element.duration : 0;

	cv.notify_all();

	// DEBUG
	// qDebug() << typeid(element).name() << " Packet size ++: " << nb_data;
	qDebug() << this->data_queue << "Data Queue Address :: ADDING";

	return ret;
}

template<>
inline int av_queue<AVFrame>::ct_push_back(AVFrame& element)
{
	int ret = 0;
	AVFrame* tmp = av_frame_alloc();
	av_frame_move_ref(tmp, &element);

	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::unique_lock<std::mutex> lock(mutex);
	while (data_queue->size() >= __MAX__QUEUENUM__)
	{
		cv.wait(lock);
	}
#ifdef __USE__AVFIFO__
	ret = av_fifo_write(data, &tmp, 1);
#endif // __USE__AVFIFO__

	data_queue->push_back(*tmp);

	nb_data++;
	size += sizeof(element);
	duration += element.duration ? element.duration : 0;

	cv.notify_all();

	// DEBUG
	// qDebug() << typeid(element).name() << " Packet size ++: " << nb_data;

	return ret;
}

template<>
inline int av_queue<QImage>::ct_push_back(QImage& element)
{
	int ret = 0;
	QImage tmp = element;
	//std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::unique_lock<std::mutex> lock(mutex);
	while (data_queue->size() >= __MAX__QUEUENUM__)
	{
		cv.wait(lock);
	}

#ifdef __USE_AVFIFO__
	ret = av_fifo_write(data, &tmp, 1);
#endif // __USE_AVFIFO__

	data_queue->push_back(tmp);

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
T av_queue<T>::ct_pop_front()
{
	int ret = 0;
	/*while (nb_data == 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}*/

	// DEBUG
	qDebug() << this->data_queue << "Data Queue Address :: POPPING";

	std::unique_lock<std::mutex> lock(mutex);
	while (nb_data == 0 && data_queue->size() == 0)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cv.wait(lock);
	}
	T element;

	// __USE_AVFIFO__
	//ret = av_fifo_read(data, (T*)&element, 1);

// __USE_STD__
	element = data_queue->front();
	data_queue->pop_front();

	nb_data--;
	size -= sizeof(element);
	duration -= element.duration ? element.duration : 0;
	cv.notify_all();

	// DEBUG
	qDebug() << typeid(element).name() << " Packet size -- : " << nb_data;
	qDebug() << "POP_FRONT_RET £º£º" << ret;
	return element;
}

template<>
inline QImage av_queue<QImage>::ct_pop_front()
{
	int ret = 0;
	/*while (nb_data == 0)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}*/
	std::unique_lock<std::mutex> lock(mutex);
	while (nb_data == 0)
	{
		//std::this_thread::sleep_for(std::chrono::milliseconds(10));
		cv.wait(lock);
	}
	QImage element;

	// __USE_AVFIFO__
	// ret = av_fifo_read(data, &element, 1);

	// __USE_STD__
	element = data_queue->front();
	data_queue->pop_front();

	nb_data--;
	size -= sizeof(element);
	cv.notify_all();

	return element;
}


/*
	@brief peek the beginning element of the queue
	@return reference to the beginning element , or nullptr if the queue is empty
*/
template<class T>
T& av_queue<T>::ct_peek_front()
{
	if (nb_data == 0 && data_queue->size() == 0) { return nullptr; }
	else
	{
		T element;
		std::unique_lock<std::mutex> lock(mutex);

		// __USE_AVFIFO__
		//av_fifo_peek(data, &element, 1, 0);

		// __USE_STD__
		element = data_queue->front();

		return &element;
	}
}

#endif // !__AV_QUEUE__



