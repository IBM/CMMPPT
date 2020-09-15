//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Util.C"
//
// Contains the implementation of utility functions declared in Util.h.
//------------------------------------------------------------------------------

#include <DetOptImpMgr.h>
#include <Timing.h>
#include <MsgFac.h>
#include <Session.h>

#include <stdlib.h>

//------------------------------------------------------------------------------

bool WitNonClass::equalCstrings (const char * lhs, const char * rhs)
   {
   return (strcmp (lhs, rhs) == 0);
   }

//------------------------------------------------------------------------------

void WitNonClass::net (double & val1, double & val2)
   {
   if (val1 <= val2)
      {
      val2 -= val1;
      val1  = 0.0;
      }
   else
      {
      val1 -= val2;
      val2  = 0.0;
      }
   }

//------------------------------------------------------------------------------

int WitNonClass::sign (int theVal)
   {
   if (theVal > 0)
      return +1;

   if (theVal < 0)
      return -1;

   return 0;
   }

//------------------------------------------------------------------------------

int WitNonClass::sign (double theVal)
   {
   if (theVal > + FLOAT_EPSILON)
      return +1;

   if (theVal < - FLOAT_EPSILON)
      return -1;

   return 0;
   }

//------------------------------------------------------------------------------

void WitNonClass::assertFunc (
      bool         assertion,
      const char * srcFileName,
      int          lineNo,
      const char * assertionString)
   {
   if (not assertion)
      WitMsgFacility::issueByException ("assertFailFmsg",
         srcFileName,
         lineNo,
         assertionString);
   }

//------------------------------------------------------------------------------

void WitNonClass::nonMsgAssertFunc (
      bool         assertion,
      const char * srcFileName,
      int          lineNo,
      const char * assertionString)
   {
   if (not assertion)
      {
      fprintf (stderr,
         "\n"
         "FATAL ERROR encountered by WIT.\n"
         "\n"
         "   Non-message assertion failure:\n"
         "      Source File: %s\n"
         "      Line #:      %d\n"
         "      Assertion:   %s\n",
         srcFileName,
         lineNo,
         assertionString);

      exitWitNonMsg (4);
      }
   }

//------------------------------------------------------------------------------

void WitNonClass::forbidNullArg (
      const void * theArg,
      const char * argName,
      const char * funcName)
   {
   if (theArg != NULL)
      return;

   startNonMsgSevereError ();

   fprintf (stderr,
      "A function argument was passed as a null pointer "
      "where this in not allowed.\n"
      "\n"
      "   Argument: %s\n"
      "   Function: %s\n",
      argName, funcName);

   finishNonMsgSevereError ();
   }

//------------------------------------------------------------------------------

bool WitNonClass::isNullCstring (const char * theCstring)
   {
   return (strlen (theCstring) == 0);
   }

//------------------------------------------------------------------------------

double WitNonClass::percentToFrac (int thePercent)
   {
   return
      (thePercent == 0)?
         0.0:
         static_cast <double> (thePercent) / 100.0;
   }

//------------------------------------------------------------------------------

int WitNonClass::fracToPercent (double theFrac)
   {
   return static_cast <int> (floor (theFrac * 100.0 + 0.5));
   }

//------------------------------------------------------------------------------

void WitNonClass::startNonMsgSevereError ()
   {
   fprintf (stderr,
      "\n"
      "SEVERE ERROR encountered by WIT:\n"
      "\n");
   }

//------------------------------------------------------------------------------

void WitNonClass::finishNonMsgSevereError ()
   {
   exitWitNonMsg (3);
   }

//------------------------------------------------------------------------------

void WitNonClass::issueNonMsgFatalError (const char * funcName)
   {
   fprintf (stderr,
      "\n"
      "FATAL ERROR encountered by WIT.\n"
      "\n"
      "The error occurred in function %s.\n",
      funcName);

   exitWitNonMsg (4);
   }

//------------------------------------------------------------------------------

void WitNonClass::exitWitNonMsg (int theRc)
   {
   if (theRc == 4)
      fprintf (stderr,
         "\n"
         "The above message indicates that there is a programming error in "
         "WIT.\n");

   fprintf (stderr,
      "\n"
      "WIT is now terminating execution of the program.\n"
      "The return code is %d.\n"
      "\n",
      theRc);

   fflush (stderr);

   exit (theRc);
   }

//------------------------------------------------------------------------------

bool WitNonClass::devMode ()
   {
   return WitSession::devMode ();
   }

