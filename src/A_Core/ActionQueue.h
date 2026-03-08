#pragma once
#include "CoreTypes.h"

class ActionQueue
{
private:
    static const uint8_t QUEUE_SIZE = 10;
    SystemAction _buffer[QUEUE_SIZE];
    uint8_t _head = 0;
    uint8_t _tail = 0;
    uint8_t _count = 0;

public:
    bool push(SystemAction event)
    {
        if (_count >= QUEUE_SIZE)
            return false;

        _buffer[_head] = event;
        _head = (_head + 1) % QUEUE_SIZE;
        _count++;
        return true;
    }

    SystemAction pop()
    {
        if (_count == 0)
            return SystemAction();

        SystemAction event = _buffer[_tail];
        _tail = (_tail + 1) % QUEUE_SIZE;
        _count--;
        return event;
    }

    bool isEmpty() const { return _count == 0; }
};
