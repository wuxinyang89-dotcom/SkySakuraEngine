#pragma once

template<typename T>
class Less
{
public:
    bool operator()(T const & left, T const & right)
    {
        return left < right;
    }
};
