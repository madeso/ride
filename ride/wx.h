#ifndef RIDE_LOCALWX_H
#define RIDE_LOCALWX_H


// hopefully this fixes this error:
// http://stackoverflow.com/questions/13219007/xcode-4-5-tr1-type-traits-file-not-found

#include <ciso646>  // detect std::lib
#ifdef _LIBCPP_VERSION
// using libc++
#ifndef HAVE_TYPE_TRAITS
#define HAVE_TYPE_TRAITS
#endif
#else
// using libstdc++
#ifndef HAVE_TR1_TYPE_TRAITS
#define HAVE_TR1_TYPE_TRAITS
#endif
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#endif  // RIDE_LOCALWX_H
