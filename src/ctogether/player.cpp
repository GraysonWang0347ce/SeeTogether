#include "player.h"

ct_player::ct_player()
{
	controller = new ct_control();
	core_ptr = single_core::get_instance();
	queues = new av_queues();

	decoder = new ct_decode_video(core_ptr,queues,controller);
	demuxer = new ct_demux(core_ptr, queues, controller);
	

	connect(decoder, SIGNAL(ct_image_decoded(QImage)), 
									this, SLOT(ct_player_got_image(QImage)));
}

void ct_player::run()
{
	single_core* core_ptr = single_core::get_instance();
	av_queues* queues = new av_queues();
	ct_control* control = new ct_control();
	
	decoder->start();
	demuxer->start();


	/*std::thread demux(ct_demux, core_ptr, queues, control);
	demux.detach();

	std::thread decode_video(&ct_decode::ct_decode_video, &decode, std::ref(core_ptr), std::ref(queues), control);
	decode_video.detach();*/
}

void ct_player::ct_player_got_image(QImage img)
{
	emit ct_player_emit_image(img);
}
