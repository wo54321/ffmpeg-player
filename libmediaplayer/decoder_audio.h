#ifndef FFMPEG_DECODER_AUDIO_H
#define FFMPEG_DECODER_AUDIO_H

#include "decoder.h"

typedef void (*AudioDecodingHandler) (AVFrame*, double);

class DecoderAudio : public IDecoder
{
public:
    DecoderAudio(AVStream* stream);

    ~DecoderAudio();

    AudioDecodingHandler		onDecode;

private:

	AVFrame*					mSamples;
    bool                        prepare();
    bool                        decode(void* ptr);
    bool                        process(AVPacket *packet);
};

#endif //FFMPEG_DECODER_AUDIO_H
