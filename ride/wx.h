#ifndef RIDE_LOCALWX_H
#define RIDE_LOCALWX_H


// hopefully this fixes this error:
// http://stackoverflow.com/questions/13219007/xcode-4-5-tr1-type-traits-file-not-found

#include <ciso646>  // detect std::lib
#ifdef _LIBCPP_VERSION
// using libc++
#define HAVE_TYPE_TRAITS
#else
// using libstdc++
#define HAVE_TR1_TYPE_TRAITS
#endif

#endif  // RIDE_LOCALWX_H
