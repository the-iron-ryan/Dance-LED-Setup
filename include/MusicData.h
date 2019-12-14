#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <CircularBuffer.h>
#include <MusicFrame.h>

class MusicData
{

public:

    static MusicData* instance();

    static const int FRAME_QUEUE_SIZE = 100;

    const long& ticks = m_ticks;
    const int&  refreshRate = m_refreshRate;

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
        return this->at(m_frameBuffer.size() - 1);
    }

    void analyze();

private:


    static MusicData* m_instance;

    long m_ticks = 0;
    int m_refreshRate = 0;

    MusicData();

    CircularBuffer<MusicFrame, G_NUM_CHANNELS> m_frameBuffer; 

};

#endif