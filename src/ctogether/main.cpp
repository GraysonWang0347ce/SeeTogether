#include "mainwindow.h"
#include"single_core.h"
#include<iostream>
#include <QApplication>
#include<thread>
#include"demux.h"
#include"decode.h"
#include"av_queues.h"
#include"control.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	

	/* TEST CODES HERE TO BE ARCHIVED*/
	single_core* core_ptr = single_core::get_instance();
	av_queues* queues = new av_queues();
	ct_control* control = new ct_control();
	auto decode = ct_decode();
	
	std::thread demux(ct_demux,core_ptr,queues,control);
	demux.detach();

	std::thread decode_video(&ct_decode::ct_decode_video,&decode,std::ref( core_ptr),std::ref( queues),control);
	decode_video.detach();

	/* TEST CODES HERE TO BE ARCHIVED*/


	return a.exec();
}
