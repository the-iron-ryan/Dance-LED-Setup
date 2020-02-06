#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include "MusicFrame.h"

template <int N>
class FrameBuffer
{
    public:

        const int& size = m_size;

        FrameBuffer()
        {
            m_next_index = 0;
        }

        void push(long tick)
        {
            m_array[m_next_index] = MusicFrame(tick);

            // m_array[m_next_index].log();
            
            m_next_index += 1;
            if (m_next_index == N)
            {
                m_next_index = 0;
            }

        }

        const MusicFrame& at(int n)
        {
            return m_array[(m_next_index + N - 1 - n) % N];
        }

        const MusicFrame& operator[](int n)
        {
            return this->at(n);
        }

        const MusicFrame& first()
        {
            return this->at(0);
        }

        const MusicFrame& last()
        {
            return this->at(N - 1);
        }

    private:

        MusicFrame m_array[N];
        int m_next_index;

        int m_size = N;
};

#endif