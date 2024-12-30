#ifndef RIDE_WX_H_
#define RIDE_WX_H_

// headerlint: disable

// hopefully this fixes this error:
// http://stackoverflow.com/questions/13219007/xcode-4-5-tr1-type-traits-file-not-found

#include <ciso646>	// detect std::lib
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

#include <wx/wxprec.h>	// NOLINT header order is b/c compiler fixes
#ifndef WX_PRECOMP
	#include <wx/wx.h>	// NOLINT header order is b/c compiler fixes
#endif

#endif	// RIDE_WX_H_
