#include "player.h"

ct_player::ct_player()
{
	
	//connect(decoder, SIGNAL(finished()), decoder, SLOT(deleteLater()));
	//connect(demuxer, SIGNAL(finished()), demuxer, SLOT(deleteLater()));

}

ct_player::~ct_player()
{
	delete decoder;
	delete demuxer;
	delete controller;
	delete queues;
    core_ptr->delete_instance();

	decoder = nullptr;
	demuxer = nullptr;
	controller = nullptr;
	queues = nullptr;
	core_ptr = nullptr;
}

void ct_player::run()
{

	controller = new ct_control();
	core_ptr = single_core::get_instance();
	queues = new av_queues();

	decoder = new ct_decode_video(core_ptr, queues, controller);
	demuxer = new ct_demux(core_ptr, queues, controller);


	connect(decoder, SIGNAL(ct_image_decoded(QImage)),
		this, SLOT(ct_player_got_image(QImage)));


	decoder->start();
	demuxer->start();

	/*std::thread demux(ct_demux, core_ptr, queues, control);
	demux.detach();

	std::thread decode_video(&ct_decode::ct_decode_video, &decode, std::ref(core_ptr), std::ref(queues), control);
	decode_video.detach();*/
	return;
}

void ct_player::ct_player_got_image(QImage imge)
{
	img = imge;
	emit ct_player_emit_image(img);
}
