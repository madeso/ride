#pragma once

// a "tagged arithmetic" Type so we don't accidentally mix types
// to get safety of "apps hungarian" but not using cryptic prefixes
// and let the compiler check the "types"

template<typename T, typename Tag>
struct Number
{
    using Type = T;
    using Self = Number<T, Tag>;

    T value;

    constexpr explicit Number(T v) : value(v) {}

    constexpr Self operator-() const
    {
        return Self{-value};
    }

    Self& operator+=(const Self& s) { value += s.value; return *this; }
    Self& operator-=(const Self& s) { value -= s.value; return *this; }
    Self& operator*=(      T     s) { value *= s      ; return *this; }
    Self& operator/=(      T     s) { value /= s      ; return *this; }
};

#define TPL template<typename T, typename Tag>
#define NUM Number  <         T,          Tag>

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

template<typename S, typename T, typename Tag> S& operator<<(S& s, const NUM& rhs)
{
    return s << rhs.value;
}

#undef NUM
#undef TPL
