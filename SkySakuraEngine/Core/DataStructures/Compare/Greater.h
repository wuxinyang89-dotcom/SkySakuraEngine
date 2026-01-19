#pragma once

template<typename T>
class Greater
{
public:
    bool operator()(T const & left, T const & right)
    {
        return left > right;
    }
};
