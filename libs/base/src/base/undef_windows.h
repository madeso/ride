//
// undefine macros defined by the windows api
//


///////////////////////////////////////////////////////////////////////////////
//
// CamelCase macros
//

#ifdef GetCurrentDirectory
#undef GetCurrentDirectory
#endif

#ifdef CreateWindow
#undef CreateWindow
#endif

#ifdef LoadImage
#undef LoadImage
#endif

#ifdef DrawText
#undef DrawText
#endif


///////////////////////////////////////////////////////////////////////////////
//
// CAMELCASE macros
//

#ifdef DELETE
#undef DELETE
#endif

#ifdef OUT
#undef OUT
#endif

#ifdef RGB
#undef RGB
#endif


///////////////////////////////////////////////////////////////////////////////
//
// camelcase macros
//

#ifdef near
#undef near
#endif

#ifdef far
#undef far
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif


///////////////////////////////////////////////////////////////////////////////
//
// other assorted macros that haven't caused any trouble yet...
//

#ifdef SendMessage
#undef SendMessage
#endif

#ifdef GetObject
#undef GetObject
#endif

#ifdef RegisterClass
#undef RegisterClass
#endif

#ifdef CreateFile
#undef CreateFile
#endif
