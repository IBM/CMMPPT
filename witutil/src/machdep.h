// Machine-dependent subroutines

#include <wit/src/defines.h>
#include <stdlib.h>

// Microsoft Compiler
#if defined(_MSC_VER)

   // Uses Standard C++ Library (include files do not have ".h" 
   // extension)
// #  define STD_CPP_LIB

#endif


inline
void
portableFree (void * mem)
{
   #ifdef OLD_STYLE_MALLOC_FREE

      free ((char *) mem);

   #else

      free (mem);

   #endif
}
