//------------------------------------------------------------------------------
// WIT-J C++ Header File Utils.h.
//
// Contains the declarations of various global utility functions within
// namespace WitJ.
//------------------------------------------------------------------------------

#ifndef Utils_h
#define Utils_h

namespace WitJ
{
bool equal (
      const char * theCString1,
      const char * theCString2);
   //
   // Returns true, if theCString1 and theCString2 are identical c-strings.

void freeWitStrVec (char * * theWitStrVec, int theLength);
   //
   // Assumes that theWitStrVec is a vector of c-strings allocated by WIT, whose
   // length is theLength.
   // Calls witFree on the c-strings and and the c-vector.

void exceptionAssertFailure (
      const char * srcFileName,
      int          lineNo);
   //
   // Throws an PreJavaException for a C++ assertion failure.

void abortingAssertFailure (
      const char * srcFileName,
      int          lineNo);
   //
   // Issues an error message for a assertion failure and then aborts the
   // program.
};
//
// End of namespace WitJ.

#endif
