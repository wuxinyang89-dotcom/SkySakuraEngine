#pragma once
#include <cmath>
#include <type_traits>

namespace Mathf
{
    template <typename T>
    void swap(T& num1,T& num2) noexcept
    {
        T temp;
        temp = num1;
        num1 = num2;
        num2 = temp;
    }
    
    template <typename T>
    T abs(T num)
    {
        if constexpr (!std::is_floating_point_v<T>&&!std::is_integral_v<T>)
        {
            throw std::out_of_range("Absolute value should be a number");
        }
        if (num < 0)
        {
            return -num;
        }
        return num;
    }

    inline bool FloatEqual(const float num1, const float num2)
    {
        if (num1 == num2) return true;
        
        if (std::isnan(num1) || std::isnan(num2)) return false;
        
        if (std::isinf(num1) || std::isinf(num2)) {
            return num1 == num2;
        }
        
        const float abs_diff = std::fabs(num1 - num2);
        
        const float rel_diff = abs_diff / std::max(std::fabs(num1), std::fabs(num2));
        
        return abs_diff <= std::numeric_limits<float>::epsilon() * 10 ||
               rel_diff <= 1e-6f;
    }
}
