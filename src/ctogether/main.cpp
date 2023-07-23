#include "mainwindow.h"
#include"single_core.h"
#include<iostream>
#include <QApplication>
#include<thread>
#include"demux.h"


int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	MainWindow w;
	w.show();


	/* TEST CODES HERE TO BE ARCHIVED*/
	single_core* core_ptr = single_core::get_instance();

	
	std::thread demux(ct_demux,core_ptr);
	demux.join();

	
	//avformat_free_context(ptr_fmt);

	/* TEST CODES HERE TO BE ARCHIVED*/

	return a.exec();
}
