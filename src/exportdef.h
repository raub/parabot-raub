#ifndef _EXPORTDEF_HPP_
#define _EXPORTDEF_HPP_

#if defined _WIN32
	#define EXPORT __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
	#pragma comment(lib, "user32.lib")
	#pragma comment(linker, "/EXPORT:GiveFnptrsToDll=_GiveFnptrsToDll@8,@1")
#else
	#if __GNUC__ >= 4
		#define EXPORT __attribute__ ((visibility ("default")))
	#else
		#define EXPORT
	#endif
#endif

#define DLLEXPORT EXPORT
#define _DLLEXPORT EXPORT

#endif // _EXPORTDEF_HPP_
