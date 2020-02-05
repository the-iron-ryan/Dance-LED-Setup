#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <FrameBuffer.h>
#include <MusicFrame.h>

class MusicData
{

public:

    static MusicData* instance();

    static const int FRAME_BUFFER_SIZE = 50;

    const long& ticks = m_ticks;

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
        return m_frameBuffer.first();
    }

private:


    static MusicData* m_instance;

    int m_ticks = 0;
    int m_creationTime;

    /**
     * Array for storing rolling averages for each audio channel
     */
    float channelMeans[G_NUM_CHANNELS];

    /**
     * Array for storing rolling variance for each audio channel
     */
    float channelEnergyVariances[G_NUM_CHANNELS];

    /**
     * Private constructor for Singleton pattern.
     */
    MusicData();

    /**
     * Buffer for MusicFrame data.
     */
    FrameBuffer<FRAME_BUFFER_SIZE> m_frameBuffer; 

};

#endif