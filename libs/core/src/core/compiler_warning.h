#pragma once

// https://www.fluentcpp.com/2019/08/30/how-to-disable-a-warning-in-cpp/

#if defined(_MSC_VER)
    #define DISABLE_WARNING_PUSH __pragma(warning( push ))
    #define DISABLE_WARNING_POP __pragma(warning( pop ))

    #define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))

    #define DISABLE_WARNING_NONSTANDARD_NAMELESS_STRUCT_UNION    DISABLE_WARNING(4201)
    #define DISABLE_WARNING_OLDSTYLE_CAST
#elif defined(__GNUC__) || defined(__clang__)
    #define DISABLE_WARNING_PUSH DO_PRAGMA(GCC diagnostic push)
    #define DISABLE_WARNING_POP DO_PRAGMA(GCC diagnostic pop)

    #define DO_PRAGMA(X) _Pragma(#X)
    #define DISABLE_WARNING(warningName) DO_PRAGMA(GCC diagnostic ignored #warningName)

    #define DISABLE_WARNING_NONSTANDARD_NAMELESS_STRUCT_UNION
    #define DISABLE_WARNING_OLDSTYLE_CAST DISABLE_WARNING(-Wold-style-cast)
#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP

    #define DISABLE_WARNING_NONSTANDARD_NAMELESS_STRUCT_UNION
    #define DISABLE_WARNING_OLDSTYLE_CAST
#endif
