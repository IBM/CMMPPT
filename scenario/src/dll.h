#ifndef scenario_dll_h
#define scenario_dll_h
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

/*----------------------------------------------------------------------------*/
/* When using Microsoft Visual C++ to create a dll define DECLSPEC for        */
/* importing or exporting dll symbols.                                        */
/*----------------------------------------------------------------------------*/
#if defined(_MSC_VER)
#  if defined(SCENARIO)
//   When building scenario DLL are not currently used.
#    define LGFR_DECLSPEC
#  elif defined(LU_DLLEXPORT)
#    define LGFR_DECLSPEC __declspec(dllexport)
#  else
#    define LGFR_DECLSPEC  __declspec(dllimport)
#  endif
#  define LGFR_STDCALL __stdcall
#else
#  define LGFR_DECLSPEC
#  define LGFR_STDCALL
#endif

#endif
