//------------------------------------------------------------------------------
// WIT-J C++ Header File Macros.h.
//
// Contains macros used by WIT-J/C++.
//------------------------------------------------------------------------------

#ifndef Macros_h
#define Macros_h

//------------------------------------------------------------------------------
// EXCEPTION_ASSERT (assertion)
//
// If assertion if false, throws a PreJavaException indicating an assertion
// failure.
//------------------------------------------------------------------------------

#define EXCEPTION_ASSERT(assertion)                                            \
                                                                               \
   if (! (assertion))                                                          \
      WitJ::exceptionAssertFailure (__FILE__, __LINE__)                        \

//------------------------------------------------------------------------------
// ABORTING_ASSERT (assertion)
//
// If assertion if false, aborts with an error.
//------------------------------------------------------------------------------

#define ABORTING_ASSERT(assertion)                                             \
                                                                               \
   if (! (assertion))                                                          \
      WitJ::abortingAssertFailure (__FILE__, __LINE__)                         \

//------------------------------------------------------------------------------

#endif
