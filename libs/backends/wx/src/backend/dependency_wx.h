#pragma once

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

#include "wx/wxprec.h"
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
