//------------------------------------------------------------------------------
// WIT-J C++ Source File Utils.C.
//
// Contains the implementations of various global utility functions within
// namespace WitJ.
//------------------------------------------------------------------------------

#include <PreJavaException.h>

#include              <wit.h>

#include         <iostream>
#include           <stdlib.h>
#include           <string.h>

//------------------------------------------------------------------------------

bool WitJ::equal (
      const char * theCString1,
      const char * theCString2)
   {
   return strcmp (theCString1, theCString2) == 0;
   }

//------------------------------------------------------------------------------

void WitJ::freeWitStrVec (char * * theWitStrVec, int theLength)
   {
   int theIdx;

   for (theIdx = 0; theIdx < theLength; theIdx ++)
      {
      witFree (theWitStrVec[theIdx]);
      }

   witFree (theWitStrVec);
   }

//------------------------------------------------------------------------------

void WitJ::exceptionAssertFailure (
      const char * srcFileName,
      int          lineNo)
   {
   char theMessage[1000];
   
   sprintf (
      theMessage,
      "    An assertion failure was incurred by the C++ side of WIT-J:\n"
      "\n"
      "      Source File: %s\n"
      "      Line #:      %d",
      srcFileName,
      lineNo);

   throw PreJavaException ("InternalErrorException", theMessage);
   }

//------------------------------------------------------------------------------

void WitJ::abortingAssertFailure (
      const char * srcFileName,
      int          lineNo)
   {
   std::cout
      << "\n"
      << "WIT-J/C++ Internal Error:\n"
      << "\n"
      << "   Assertion failure:\n"
      << "      Source File: " << srcFileName << "\n"
      << "      Line #:      " << lineNo      << "\n"
      << "\n"
      << "Application Program aborted by WIT-J.\n";

   exit (4);
   }
