#include"mainwindow.h"
#include"single_core.h"
#include<iostream>
#include<QApplication>
#include<thread>
#include"demux.h"
#include"decode.h"
#include"av_queues.h"
#include"control.h"

int main(int argc, char* argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	

	/* TEST CODES HERE TO BE ARCHIVED*/
	

	/* TEST CODES HERE TO BE ARCHIVED*/


	return a.exec();
}
