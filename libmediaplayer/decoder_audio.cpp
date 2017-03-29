#include <android/log.h>
#include "decoder_audio.h"

#define TAG "FFMpegAudioDecoder"

DecoderAudio::DecoderAudio(AVStream* stream) : IDecoder(stream)
{
}

DecoderAudio::~DecoderAudio()
{
}

bool DecoderAudio::prepare()
{
    mSamples = av_frame_alloc();
    if(mSamples == NULL) {
    	return false;
    }
    return true;
}

bool DecoderAudio::process(AVPacket *packet)
{
    int got_frame = 0;
    avcodec_decode_audio4(mStream->codec, mSamples, &got_frame, packet);

    //call handler for posting buffer to os audio driver
    if (!got_frame)
        onDecode(mSamples, mSamples->pts);

    return true;
}

bool DecoderAudio::decode(void* ptr)
{
    AVPacket        pPacket;

    __android_log_print(ANDROID_LOG_INFO, TAG, "decoding audio");

    while(mRunning)
    {
        if(mQueue->get(&pPacket, true) < 0)
        {
            mRunning = false;
            return false;
        }
        if(!process(&pPacket))
        {
            mRunning = false;
            return false;
        }
        // Free the packet that was allocated by av_read_frame
        av_packet_unref(&pPacket);
    }

    __android_log_print(ANDROID_LOG_INFO, TAG, "decoding audio ended");

    // Free audio samples buffer
    av_frame_free(&mSamples);
    return true;
}
