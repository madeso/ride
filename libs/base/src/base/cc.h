#pragma once

#include <type_traits>
#include <cassert>
#include <vector>
#include <optional>
#include <algorithm>

#include "base/c.h"

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


/* The splice() method changes the contents of an array by removing or replacing existing elements and/or adding new elements in place.
https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Global_Objects/Array/splice
*/
template<typename T>
std::vector<T> splice(std::vector<T>* vec, int at, std::optional<int> items_to_remove = {}, const std::vector<T>& insert = {})
{
    // lambda here because erasing/inserting might
    // resize the vector making the old iterator invalid
    const auto where = [at, vec]()
    {
        return at >= 0
            ? std::next(vec->begin(), at)
            : std::next(vec->begin(), C(vec->size()) + at)
            ;
    };
    
    const auto end_iterator = items_to_remove
        ? std::next(where(), *items_to_remove)
        : vec->end()
        ;
    const auto removed = std::vector<T>(where(), end_iterator);
    vec->erase(where(), end_iterator);

    vec->insert(where(), insert.begin(), insert.end());
    return removed;
}
