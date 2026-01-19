#pragma once

template <typename T>
class ControlBlock
{
public:
    ControlBlock(T data)
    {
        share_cnt_ = 0;
        weak_cnt_ = 0;
        data_ = data;
    }

    ~ControlBlock()
    {
    }

private:
    unsigned int share_cnt_;
    unsigned int weak_cnt_;
    T data_;
};
