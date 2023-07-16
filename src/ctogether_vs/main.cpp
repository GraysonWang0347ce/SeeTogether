#include "ctogether_vs.h"
#include <QtWidgets/QApplication>
#include"single_core.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ctogether_vs w;

    // initialize the ffmpeg data structures
    single_core* ptr_core = single_core::get_instance();

    w.show();
    return a.exec();
}
