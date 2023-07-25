#include "mainwindow.h"
#include"single_core.h"
#include<iostream>
#include <QApplication>
#include<thread>
#include"demux.h"
#include"decode.h"
#include"av_queues.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();


	/* TEST CODES HERE TO BE ARCHIVED*/
	single_core* core_ptr = single_core::get_instance();
	av_queues* queues = new av_queues();
	
	std::thread demux(ct_demux,core_ptr,queues);
	std::thread decode_video(ct_decode_video, core_ptr, queues);
	demux.join();
	decode_video.join();

	
	//avformat_free_context(ptr_fmt);

	/* TEST CODES HERE TO BE ARCHIVED*/

	return a.exec();
}
