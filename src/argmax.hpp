#pragma once

#include <cmath>
#include <cassert>

namespace studd
{
    // returns an iterator to the element which maximizes the function
    template<class InputIt, class UnaryFunction,
             class T = typename std::iterator_traits<InputIt>::value_type>
    InputIt argmax(InputIt first, InputIt last, UnaryFunction f)
    {
        assert(std::distance(first, last) > 0);

        T max;
        InputIt max_it;

        for (auto it = first; it != last; ++it)
        {
            auto res = f(*it);
            if (it == first || res > max)
            {
                max = res;
                max_it = it;
            }
        }

        return max_it;
    }
}
