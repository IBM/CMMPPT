//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef SessionH
#define SessionH

//------------------------------------------------------------------------------
// Header file: "Session.h"
//
// Contains the declaration of class Session.
// Contains the declaration of NonClass function buildDate ().
//------------------------------------------------------------------------------

#include <Str.h>

//------------------------------------------------------------------------------
// Class Session
//
// WIT Session.
// Responsible for all non-API aspects of WIT.
// Life span: The life span of the owning WitRun.
// Implemented in Misc.C.
//------------------------------------------------------------------------------

class WitSession
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSession (WitRun * theWitRun);
         //
         // myWitRun_ <-- theWitRun.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSession ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc    (WitRun *,     myWitRun)
      accessFunc    (WitApiMgr *,  myApiMgr)
      accessNonNull (WitProblem *, myProblem)

      inline static bool devMode ()
         {
         return devMode_;
         }

      inline WitMsgFacility & myMsgFac () const
         {
         return * myMsgFacPtr_;
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void writeHeading (FILE * outFile);
         //
         // Writes a heading to outFile.

      void printHeading ();
         //
         // Issues a heading message.

      void activate ();
         //
         // Puts this session into a newly active state, i.e., one with a new
         // Problem.

      void deactivate ();
         //
         // Puts this session into an inactive state, i.e., one without a
         // Problem.

      inline bool active () const
         {
         return (myProblem_ != NULL);
         }

      void copyData (WitSession * srcSession);
         //
         // Copies the input state of srcSession into this Session.
         // No-op if srcSession is this Session.

      FILE * openFile (const char * fileName, const char * mode);
         //
         // Opens and returns a file with name filename in mode mode, with
         // error checking.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      static bool devModeValue ();
         //
         // Computes and returns the value of devMode_.

      static const char * currentTime ();
         //
         // Returns the current calendar time as a c-string.

      noCopyCtorAssign (WitSession);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      static const char * const proprietaryStmt_;
         //
         // The proprietary statement for WIT.
         // The main purpose of this data member is to cause the WIT object
         // library file (e.g., libwit.a) to contain WIT's propretary statement
         // as a sequence of text characters. It is also used in file headings.

      static const bool devMode_;
         //
         // True, iff this version of the WIT library was built in
         // "development" mode.
         // This is the case, iff the exe_type of the build was one of the
         // following:
         //    debug
         //    fast
         //    profile

      WitRun * const myWitRun_;
         //
         // The WitRun that owns this Session.

      WitApiMgr * const myApiMgr_;
         //
         // The ApiMgr for this Session.

      WitMsgFacility * myMsgFacPtr_;
         //
         // Ptr to the MsgFacility owned by this Session.

      WitString startTime_;
         //
         // Calendar time when this Session is constructed as a String.

      WitProblem * myProblem_;
         //
         // The Problem owned by this Session, when it is initialized;
         // otherwise NULL.
   };

//------------------------------------------------------------------------------
// NonClass function
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   const char * buildDate ();
      //
      // Returns the date on which the WIT library was built.
      // Implemented in BuildDate.C.
   };

#endif
