#pragma once

// a "tagged arithmetic" type so we don't accidentally mix types
// to get safety of "apps hungarian" but not using cryptic prefixes
// and let the compiler check the "types"

template<typename T, typename Tag>
struct number
{
    using type = T;
    using self = number<T, Tag>;

    T value;

    constexpr explicit number(T v) : value(v) {}

    constexpr self operator-() const
    {
        return self{-value};
    }

    self& operator+=(const self& s) { value += s.value; return *this; }
    self& operator-=(const self& s) { value -= s.value; return *this; }
    self& operator*=(      T     s) { value *= s      ; return *this; }
    self& operator/=(      T     s) { value /= s      ; return *this; }
};

#define TPL template<typename T, typename Tag>
#define NUM number  <         T,          Tag>

TPL NUM operator+(const NUM& lhs, const NUM& rhs) { return NUM{lhs.value + rhs.value}; }
TPL NUM operator-(const NUM& lhs, const NUM& rhs) { return NUM{lhs.value - rhs.value}; }
TPL NUM operator*(const NUM& lhs,       T    rhs) { return NUM{lhs.value * rhs      }; }
TPL NUM operator/(const NUM& lhs,       T    rhs) { return NUM{lhs.value / rhs      }; }
TPL NUM operator*(      T    lhs, const NUM& rhs) { return NUM{      lhs * rhs.value}; }
TPL NUM operator/(      T    lhs, const NUM& rhs) { return NUM{      lhs / rhs.value}; }
TPL T   operator/(const NUM& lhs, const NUM& rhs) { return     lhs.value / rhs.value ; }

TPL bool operator==(const NUM& lhs, const NUM& rhs) { return lhs.value == rhs.value; }
TPL bool operator!=(const NUM& lhs, const NUM& rhs) { return lhs.value != rhs.value; }
TPL bool operator<=(const NUM& lhs, const NUM& rhs) { return lhs.value <= rhs.value; }
TPL bool operator>=(const NUM& lhs, const NUM& rhs) { return lhs.value >= rhs.value; }
TPL bool operator< (const NUM& lhs, const NUM& rhs) { return lhs.value <  rhs.value; }
TPL bool operator> (const NUM& lhs, const NUM& rhs) { return lhs.value >  rhs.value; }

template<typename S, typename T, typename Tag> S& operator<<(S& s, NUM& rhs)
{
    return s << rhs.value;
}

#undef NUM
#undef TPL
