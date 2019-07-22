//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

#ifndef UtilH
#define UtilH

//------------------------------------------------------------------------------
// Header file: "Util.h"
//
// Contains:
//    The declaration various utility functions.
//    Some macros that refer to these functions.
//------------------------------------------------------------------------------

#include <typedefs.h>

namespace WitNonClass
   {
   //---------------------------------------------------------------------------
   // Max/Min function templates.
   //---------------------------------------------------------------------------

   template <typename Number> 
      inline Number max (Number num1, Number num2);
         //
         // Returns the maximum of num1 and num2.

   template <typename Number> 
      inline Number min (Number num1, Number num2);
         //
         // Returns the minimum of num1 and num2.

   template <typename Number> 
      inline void setToMax (Number & num1, Number num2);
         //
         // Updates num1 to be the max of num1 and num2.

   template <typename Number> 
      inline void setToMin (Number & num1, Number num2);
         //
         // Updates num1 to be the min of num1 and num2.

   //---------------------------------------------------------------------------

   bool equalCstrings (const char * lhs, const char * rhs);
      //
      // String equality.

   inline double positivePart (double theVal)
      {
      return max (0.0, theVal);
      }

   void net (double & theVal1, double & theVal2);
      //
      // Subtracts min (theVal1, theVal2) from theVal1 and from theVal2.

   int sign (int theVal);
      //
      // Returns 1, 0, or -1.

   int sign (double theVal);
      //
      // Returns 1, 0, or -1.

   void assertFunc (
         bool         assertion,
         const char * srcFileName,
         int          lineNo,
         const char * assertionString);
      //
      // Issues a fatal error, if assertion is false.
      // Invoked thru witAssert macro.

   inline void doNothing ()
      {
      }

   void nonMsgAssertFunc (
         bool         assertion,
         const char * srcFileName,
         int          lineNo,
         const char * assertionString);
      //
      // If assertion is false, issues a fatal error without using the
      // MsgFacility.
      // Invoked thru nonMsgAssert macro.

   void forbidNullArg (
         const void * theArg,
         const char * argName,
         const char * funcName);
      //
      // theArg:   A pointer that was passed to WIT as an argument to an
      //           API function.
      // argName:  The name of the argument
      // funcName: The name of the API function.
      // If theArg is NULL, this function prints an error message to
      // stderr and exists the process.
      // This function should only be used when there is no WitRun
      // available, since it does not use the message facility.

   bool isNullCstring (const char * theCstring);
      //
      // Returns true iff theCstring is the null string.

   double percentToFrac (int thePercent);
      //
      // Returns thePercent, converted to a fraction.

   int fracToPercent (double theFrac);
      //
      // Returns theFrac, converted to a percent.

   void startNonMsgSevereError ();
      //
      // Starts issuing a severe error, without the Msg Facility.

   void finishNonMsgSevereError ();
      //
      // Finishes issuing a severe error, without the Msg Facility.

   void issueNonMsgFatalError (const char * funcName);
      //
      // Issues a fatal error message without the Msg Facility.
      // funcName is the name of the WIT function in which the error occurred.

   void exitWitNonMsg (int theRc);
      //
      // Exits WIT without using the Msg Facility.
      // Uses theRc as the return code.

   bool devMode ();
      //
      // See Session::devMode_.
   };

//------------------------------------------------------------------------------
// Implementation of inlines.
//------------------------------------------------------------------------------

template <typename Number> 
      inline Number WitNonClass::max (Number num1, Number num2)
   {
   return
      (num1 > num2)?
         num1:
         num2;
   }

//------------------------------------------------------------------------------

template <typename Number> 
      inline Number WitNonClass::min (Number num1, Number num2)
   {
   return
      (num1 < num2)?
         num1:
         num2;
   }

//------------------------------------------------------------------------------

template <typename Number> 
      inline void WitNonClass::setToMax (Number & num1, Number num2)
   {
   if (num2 > num1)
      num1 = num2;
   }

//------------------------------------------------------------------------------

template <typename Number> 
      inline void WitNonClass::setToMin (Number & num1, Number num2)
   {
   if (num2 < num1)
      num1 = num2;
   }

//------------------------------------------------------------------------------
// witAssert
//
// Issues a fatal error, if assertion is false.
// No-op, if not in DEBUG mode.
//------------------------------------------------------------------------------

#ifdef DEBUG

#define witAssert(assertion)                                                   \
                                                                               \
   assertFunc ((assertion), __FILE__, __LINE__, #assertion)                    \

#else

#define witAssert(assertion)                                                   \
                                                                               \
   doNothing ()                                                                \

#endif

//------------------------------------------------------------------------------
// stronglyAssert
//
// Issues a fatal error indicating the source file name and line #, if assertion
// is false.
//------------------------------------------------------------------------------

#define stronglyAssert(assertion)                                              \
                                                                               \
   assertFunc ((assertion), __FILE__, __LINE__, #assertion)                    \

//------------------------------------------------------------------------------
// nonMsgAssert
//
// Issues a non-message fatal error, if assertion is false.
// No-op, if not in DEBUG mode.
//------------------------------------------------------------------------------

#ifdef DEBUG

#define nonMsgAssert(assertion)                                                \
                                                                               \
   nonMsgAssertFunc ((assertion), __FILE__, __LINE__, #assertion)              \

#else

#define nonMsgAssert(assertion)                                                \
                                                                               \
   doNothing ()                                                                \

#endif

#endif
