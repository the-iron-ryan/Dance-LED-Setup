#ifndef ANALYSIS_H
#define ANALYSIS_H

#include "CircularBuffer.h"
#include "MusicFrame.h"
#include "MusicData.h"

class MusicAnalysis
{
    public:

    static int calculateBPM(CircularBuffer<MusicFrame, MusicData::FRAME_QUEUE_SIZE>& buffer);
    static int calculateRefreshRate(CircularBuffer<MusicFrame, MusicData::FRAME_QUEUE_SIZE>& buffer);

};

#endif