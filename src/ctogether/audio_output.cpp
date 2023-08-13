#include "audio_output.h"

ct_audio_output::ct_audio_output(single_core* o_core_ptr, av_queues* o_queues, ct_control* o_controller)
{
	core_ptr = o_core_ptr;
	queues = o_queues;
	control = o_controller;
}

void ct_audio_output::run()
{

	AVFrame* ptr_frame = av_frame_alloc();
	AVPacket* ptr_packet = av_packet_alloc();
	int ret_num = 0;
	AVFrame* ptr_frame_rgb32;

	// waiting for demux thread to start
	control->wait_demux_a();

	while (ret_num != AVERROR_EOF)
	{
		// if audio packet queue is empty, wait for audio packet

		// get audio decoder(initialized at demux.cpp:81) 
		AVPacket temp = queues->packet_queue[AUDIO]->ct_pop_front();
		av_packet_move_ref(ptr_packet, &temp);
		ret_num = avcodec_send_packet(core_ptr->ptr_audio_codec_ctx, ptr_packet);

		while (ret_num >= 0)
		{
			ret_num = avcodec_receive_frame(core_ptr->ptr_audio_codec_ctx, ptr_frame);
			if (ret_num == AVERROR(EAGAIN) || ret_num == AVERROR_EOF)
			{
				break;
			}

			QAudioFormat audio_fmt;
			audio_fmt.setSampleRate(ptr_frame->sample_rate);
			audio_fmt.setSampleFormat(QAudioFormat::SampleFormat::Int16);
			audio_fmt.setChannelCount(ptr_frame->channel_layout);

		}
	}
}
