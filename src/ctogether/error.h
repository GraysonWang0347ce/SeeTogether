#pragma once
#ifndef __ERROR_H__
#define __ERROR_H__

enum CT_ERROR {
	NO_TRACK_FOUND = 1, // cannot find any video or audio track due to given url

	NO_CODEC_FOUND, // cannot find any video or audio codec due to given filmlet format

	VIDEOCODEC_PARAM_COPY_FAILED, // cannot copy video codec parameters to codec context

	AUDIOCODEC_PARAM_COPY_FAILED, // cannot copy audio codec parameters to codec context

	VIDEOCODEC_BIND_FAILED, // cannot bind video codec to codec context

	AUDIOCODEC_BIND_FAILED, // cannot bind audio codec to codec context



};

#endif // !__ERROR_H__

