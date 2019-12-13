#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <CircularBuffer.h>
#include <MusicFrame.h>

class MusicData
{

public:

    static MusicData* instance();

    static const int FRAME_QUEUE_SIZE = 100;

    long ticks = 0;

    void update();
    MusicData& operator++(int n)
    {
        this->update();
        return *this;
    }

    const MusicFrame& at(int i)
    {
        return m_frameBuffer[i];
    }

    const MusicFrame& operator[](int i)
    {
        return this->at(i);
    }

    const MusicFrame& current()
    {
        return this->at(0);
    }

private:


    static MusicData* m_instance;

    MusicData();

    CircularBuffer<MusicFrame, G_NUM_CHANNELS> m_frameBuffer; 

};

#endif