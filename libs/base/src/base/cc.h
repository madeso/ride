#pragma once

#include <type_traits>
#include <cassert>
#include <vector>
#include <algorithm>

// https://twitter.com/idoccor/status/1314664849276899328
// https://gist.github.com/dirocco/0e23867d0ea9ab2a23dd6dcaba065f53
template<typename E> constexpr typename std::underlying_type<E>::type cast_to_base(E e) noexcept
{
    return static_cast<typename std::underlying_type<E>::type>(e);
}

template<typename T>
T keep_within(T min, T t, T max)
{
    assert(min <= max);

    if(t < min) { return min; }
    else if(t > max) { return max; }
    else { return t; }
}


template<typename Type, typename Function>
void erase_remove_if(std::vector<Type>* v, Function fun)
{
    v->erase(std::remove_if(v->begin(), v->end(), fun), v->end());
}
