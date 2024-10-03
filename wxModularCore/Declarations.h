#ifndef _DECLARATIONS_H
#define _DECLARATIONS_H

#if defined(__WXMSW__)
#   ifdef DEMO_PLUGIN_EXPORTS
#      define DEMO_API __declspec(dllexport)
#   else
#      define DEMO_API __declspec(dllimport)
#   endif
#else
#      define DEMO_API __attribute__((visibility("default")))
#endif

#if defined(WIN32) || defined(WINDOWS)
#include <windows.h>
#include <winnt.h>
#define PLUGIN_EXPORTED_API	WXEXPORT
#else
#define PLUGIN_EXPORTED_API	extern "C"
#endif

#endif // _DECLARATIONS_H
