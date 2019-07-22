#ifndef ScenarioMachdep_H
#define ScenarioMachdep_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <stdlib.h>

#include <scenario/src/dll.h>


#if defined(_MSC_VER)
//# define CMRP_DATA_PATH "..\\..\\..\\cmrpData\\"
# define CMRP_DATA_PATH "..\\..\\cmrpData\\"
#else
# define CMRP_DATA_PATH "../../cmrpData/"
#endif

#ifdef RS6K

// tempnam function supported
#define TEMPNAM_AVAILABLE 1

#endif

#ifdef SUN4
/* Sun or GNU gcc seems to not have $ NLS support in stdio print routines,
 * so NLS_DOLLAR_FORMAT_SUPPORT is left undefined */
 
/* Platform lacks ansi function strerror */
#define NO_ANSI_STRERROR
#define SPRINTF_NON_INT_RET

#ifdef __cplusplus
#define TRY_THROW_CATCH_NOT_SUPPORTED
#define OLD_STYLE_MALLOC_FREE
/* Sun C++ V2 required -DNULL=0 */
#define 0 0
#endif

// tempnam function supported
#define TEMPNAM_AVAILABLE 1

// used in difCmrpF.C
#define EXIT_FAILURE 1

#endif

//-----------------------------------------------------------------------------
// portableFree
//
// Function that hides incompatibilities between various versions of
// free()
//-----------------------------------------------------------------------------
inline
void
portableFree (void * mem) 
{
  if ( mem != 0 )
  free (
#ifdef OLD_STYLE_MALLOC_FREE
	(char *)
#endif
	         mem);
}

// Sun CC was generating the following error messages when this was inline
// "../src/timeVecs.C", line 46: warning:
//     size_tsize_tMax(const size_t&, const size_t&) not inlined,
//     called twice in an expression
// "../src/timeVecs.C", line 335: warning:
//     out-of-line copy of size_tMax() created
// "../../scenario/src/machdep.h", line 43: identifier redeclared:
//     size_tMax__FRCUiT1
// "../src/timeVecs.C", line 337: warning:
//     static function called but not defined: size_tMax__FRCUiT1()

#ifdef SUN4
static
#else
inline
#endif
size_t
size_tMax (const size_t& i, const size_t& j)
{
  return (i > j  ?  i : j);
}

#ifdef SUN4
static
#else
inline
#endif
float
LgFrFloatMax (const float i, const float j)
{
  return (i > j  ?  i : j);
}

#ifdef SUN4
static
#else
inline
#endif
size_t
size_tMin (const size_t& i, const size_t& j)
{
  return (i < j  ?  i : j);
}

#ifdef SUN4
static
#else
inline
#endif
float
LgFrFloatMin (const float i, const float j)
{
  return (i < j  ?  i : j);
}


// Get name which can be used for a temporary file
LGFR_DECLSPEC std::string LGFR_STDCALL LgFrTempFileName();

// Return true if file exists
bool LgFrDoesFileExist( const std::string& fileName );

int LgFrStrcmpcaseins( const char* s1, const char* s2 );

void LgFrMachdepTest();

#endif
