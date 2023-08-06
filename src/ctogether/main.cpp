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
	auto decode = ct_decode();

	std::thread demux(ct_demux,core_ptr,queues);
	std::thread decode_video(&ct_decode::ct_decode_video,&decode, core_ptr, queues);
	demux.detach();
	decode_video.detach();

	/* TEST CODES HERE TO BE ARCHIVED*/


	return a.exec();
}
