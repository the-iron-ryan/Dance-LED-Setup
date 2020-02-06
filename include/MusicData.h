#ifndef MUSICDATA_H
#define MUSICDATA_H

#include <FrameBuffer.h>
#include <MusicFrame.h>

class MusicData
{

public:

    /**
     * The publicly-accessible singleton instance getter of the MusicData object. 
     */
    static MusicData* instance();

    /**
     * The size of the MusicData frame buffer. This many frames prior to the
     * current frame are stored in a circular buffer.
     */
    static const int FRAME_BUFFER_SIZE = 50;

    /**
     * Number of ticks (updates) that the MusicData has processed since creaion.
     */
    const int& ticks = m_ticks;

    /**
     * Updates the MusicData, adding another frame from the currently-playing
     * music and occasionally updating descriptive statistics regarding the
     * data.
     */
    void update();

    /**
     * Performs a single update.
     */
    MusicData& operator++(int n)
    {
        this->update();
        return *this;
    }

    /**
     *  Get the MusicFrame at the desired index.
     */
    const MusicFrame& at(int i)
    {
        return m_frameBuffer[i];
    }

    /**
     *  Get the MusicFrame at the desired index.
     */
    const MusicFrame& operator[](int i)
    {
        return this->at(i);
    }

    /**
     *  Get the current MusicFrame
     */
    const MusicFrame& current()
    {
        return m_frameBuffer.first();
    }

private:

    /**
     * The private instance pointer for the MusicData singleton.
     */
    static MusicData* m_instance;

    /**
     *  Number of updates this object has processed.
     */
    int m_ticks = 0;

    /**
     *  Unix time of creation.
     */
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