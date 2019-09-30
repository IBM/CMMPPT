//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Session.C"
//
// Contains the implementation of class Session
//
// Note:
//    This source file needs to be compiled with the flags:
//       -DEXE_TYPE={value}
//       -DPLATFORM={value}
//    This is handled in wit/appl.mk by defining a special target to compile
//    Session.C.
//------------------------------------------------------------------------------

#include <Session.h>
#include <OpSolverIf.h>
#include <ApiMgr.h>
#include <WitRun.h>
#include <FSS.h>
#include <MsgFac.h>
#include <MsgFrag.h>

#include <time.h>
#include <errno.h>

//------------------------------------------------------------------------------
// WIT Release number.
//------------------------------------------------------------------------------

#define RELEASE_NO "8.0"

//------------------------------------------------------------------------------
// Macro TO_STRING (arg)
//
// Does macro expansion on arg and then turns the result into a quoted string.
//------------------------------------------------------------------------------

#define INNER_STRING(arg) #arg

#define TO_STRING(arg)    INNER_STRING (arg)

//------------------------------------------------------------------------------
// Macro EXE_TYPE_STRING.
// Identifies the exe_type used for the compile.
// This matches the "exe_type" macro defined in the WIT makefiles.
//------------------------------------------------------------------------------

#ifndef EXE_TYPE
#define EXE_TYPE unknown
#endif

#define EXE_TYPE_STRING TO_STRING (EXE_TYPE)

//------------------------------------------------------------------------------
// Macro PLATFORM_STRING.
// Identifies the platform used for the compile.
// This matches the "platform" macro defined in the WIT makefiles.
//
// Note that the symbol 'linux' is #defined as 1 on some Linux compilers, and
// this needs to be undone here.
//------------------------------------------------------------------------------

#ifndef PLATFORM
#define PLATFORM unknown
#endif

#undef  linux

#define PLATFORM_STRING TO_STRING (PLATFORM)

//------------------------------------------------------------------------------
// NO_TIME_T
// 
// Defining NO_TIME_T will cause wit to be built without using the 
// ANSI c data structure time_t.  It will also cause the string "Unknown"
// to be printed in messages which determine the time from time_t.
// This is being done to support ITS's desire to build the wit library 
// on a back level of AIX which has a different sizeof (time_t) when
// compared to the current level.
//------------------------------------------------------------------------------

// #define NO_TIME_T

//------------------------------------------------------------------------------
// Implementation of class Session.
//------------------------------------------------------------------------------

const char * const WitSession::proprietaryStmt_ =
   "\n"
   "=============================================================\n"
   "Licensed Materials - Property of IBM\n"
   "\n"
   "Constrained Materials Management and Production Planning Tool\n"
   "\n"
   "(C) Copyright IBM Corp. 1993, 2012  All Rights Reserved\n"
   "\n"
   "US Government Users Restricted Rights -\n"
   "Use, duplication or disclosure restricted by\n"
   "GSA ADP Schedule Contract with IBM Corp.\n"
   "=============================================================\n";

//------------------------------------------------------------------------------

const bool WitSession::devMode_ = devModeValue ();

//------------------------------------------------------------------------------

WitSession::WitSession (WitRun * theWitRun):

      myWitRun_    (theWitRun),
      myApiMgr_    (theWitRun->myApiMgr ()),
      myMsgFacPtr_ (NULL),
      startTime_   (currentTime ()),
      myProblem_   (NULL)
   {
   myMsgFacPtr_ = new WitMsgFacility (this);
   }

//------------------------------------------------------------------------------

WitSession::~WitSession ()
   {
   delete myProblem_;

   myProblem_ = NULL;

   delete myMsgFacPtr_;
   }

//------------------------------------------------------------------------------

void WitSession::writeHeading (FILE * outFile)
   {
   fprintf (outFile,
      "%s\n"
      "Constrained Materials Management and Production Planning Tool\n"
      "Release         %s\n"
      "Build Date:     %s\n"
      "Build Type:     %s\n"
      "Platform:       %s\n"
      "CPLEX Embedded? %s\n"
      "Run Started At: %s\n",
      proprietaryStmt_,
      RELEASE_NO,
      buildDate (),
      EXE_TYPE_STRING,
      PLATFORM_STRING,
      WitOpSolverIf::cplexEmbedded ()? "Yes": "No",
      startTime_.myCstring ());
   }

//------------------------------------------------------------------------------

void WitSession::printHeading ()
   {
   const char * embFragID;

   embFragID = WitOpSolverIf::cplexEmbedded ()? "yesFrag": "noFrag";

   myMsgFac () ("headingMsg",
      proprietaryStmt_,
      RELEASE_NO,
      buildDate (),
      EXE_TYPE_STRING,
      PLATFORM_STRING,
      myMsgFac ().myFrag (embFragID),
      startTime_);
   }

//------------------------------------------------------------------------------

void WitSession::activate ()
   {
   deactivate ();

   printHeading ();

   myProblem_ = new WitProblem (this);
   }

//------------------------------------------------------------------------------

void WitSession::deactivate ()
   {
   delete myProblem_;

   myProblem_ = NULL;

   myApiMgr ()->resetInputID ();
   }

//------------------------------------------------------------------------------

void WitSession::copyData (WitSession * srcSession)
   {
   if (srcSession == this)
      return;

   activate ();

   srcSession->myProblem ()->copyProblemInto (myProblem ());
   }

//------------------------------------------------------------------------------
// openFile (...)
//
// Note that errno is global and therefore its value is not completely reliable
// in a multi-threaded environment. However, the risk of it having the wrong
// value seems to be minimal.
//------------------------------------------------------------------------------

FILE * WitSession::openFile (const char * fileName, const char * mode)
   {
   FILE * openedFile;

   errno      = 0;

   openedFile = fopen (fileName, mode);

   if (not openedFile)
      myMsgFac () (
         "fopenSmsg",
         fileName,
         mode,
         errno,
         strerror (errno));

   return openedFile;
   }

//------------------------------------------------------------------------------

bool WitSession::devModeValue ()
   {
   WitString theExeType;

   theExeType = EXE_TYPE_STRING;

   return
         (theExeType == "debug")
      || (theExeType == "fast")
      || (theExeType == "profile");
   }

//------------------------------------------------------------------------------

#if defined(NO_TIME_T)

const char * WitSession::currentTime ()
   {
   return "<Date Unknown>\n";
   }

#else

const char * WitSession::currentTime ()
   {
   time_t startTime_t;

   startTime_t = time (NULL);

   return ctime (& startTime_t);
   }

#endif
