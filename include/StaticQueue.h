#ifndef STATIC_QUEUE
#define STATIC_QUEUE

template <class T, int N>
class StaticQueue
{

public:

    StaticQueue()
    {
        this->m_array[N];
        this->m_capacity = N;
        this->m_head     = 0;
        this->m_tail     = 0;
    }

    bool empty()   { return this->m_size == 0; }
    bool full()    { return this->m_size == this->m_capacity; } 
    int size()     { return this->m_size; }
    int capacity() { return this->m_capacity; }

    void queue(T item)
    {

        // If the queue is full, the tail is about to
        // intersect with the head. Dequeue to keep the
        // queue at capacity.
        if (full())
        {
            dequeue();
        }

        this->m_array[this->m_tail] = item;
        this->m_tail = (this->m_tail + 1) % this->m_capacity;
        this->m_size++;
  
    }

    T dequeue()
    {
        T item = this->m_array[this->m_head];
        this->m_head = (this->m_head + 1) % this->m_capacity;
        this->m_size--;

        return item;
    }

    T cycle(T item)
    {
        T dequeued = dequeue();
        queue(item);
        return dequeued;
    }

    T peek()
    {
        return m_array[m_head];
    }

private:

    int m_size;
    int m_head;
    int m_tail;
    int m_capacity;

    T* m_array;

};

#endif