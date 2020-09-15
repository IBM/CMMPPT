//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ApiMgrH
#define ApiMgrH

#include <TVec.h>
#include <wit.h>

//------------------------------------------------------------------------------
// Header file: "ApiMgr.h"
//
// Contains the declaration of class ApiMgr.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// class ApiMgr
// Responsible for handling the entry into and the return from wit API
// functions.
//------------------------------------------------------------------------------

class WitApiMgr
   {
   public:

      //------------------------------------------------------------------------
      // Public member data.
      //------------------------------------------------------------------------

      WitApiMgr (WitRun * theWitRun);
         //
         // myWitRun_ <-- theWitRun.

      //------------------------------------------------------------------------
      // Destructor functions.
      //------------------------------------------------------------------------

      ~WitApiMgr ();

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitRun *,      myWitRun)
      accessFunc (WitInputID *,  myInputID)
      accessFunc (const char *,  apiFuncName)
      accessFunc (WitReturnCode, apiRc)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static void startApiFunc (WitRun * theWitRun, const char * theFuncName);
         //
         // This function is called from the CALL_INNER_API macro
         // just prior to calling the inner API function on theWitRun.
         //
         // theFuncName: Name of the outer API function.

      void finishApiFunc ();
         //
         // This function is called from the CALL_INNER_API macro
         // just after calling the inner API function on theWitRun.

      void handleAnyExc ();
         //
         // Handles all exceptions.

      void lockMyWitRun (const char * theFuncName);
         //
         // Puts myWitRun_ into a "locked" state: No further API functions calls
         // to myWitRun_ are allowed, other than the current one.
         // theFuncName is the name of the currently called API function.

      void unlockMyWitRun ();
         //
         // Puts myWitRun_ into an "unlocked" state: Further API functions calls
         // to myWitRun_ are now allowed.

      void skipCallMsg ();
         //
         // Tells this ApiMgr not to issue the API function call Msg for the
         // current API function.

      void allowPreInit ();
         //
         // Tells this ApiMgr to allow the API current function to be called
         // when witInitialize and witCopyData have not yet been called.

      void allowExtOptActive ();
         //
         // Tells this ApiMgr to allow the API current function to be called
         // when external optimizing implosion is active.

      void startInnerFunc ();
         //
         // Takes action appropriate at the beginning of a call to an inner API
         // function.

      void start2WitRunFunc (WitApiMgr * theApiMgr2);
         //
         // Takes action as needed at the beginning of a call to an API
         // function with two WitRun arguments.
         // theApiMgr2 is the ApiMgr for the second WitRun.

      void updateApiRc (WitReturnCode theRc);
         //
         // Sets apiRc_ to theRc, if theRc is greater.

      void resetInputID ();
         //
         // Deletes myInputID () and creates a new one.

      WitTVec <WitBoolean> & reserveTempBooleanVec ();
         //
         // Same as reserveTempDblVec, but for WitBoolean.

      WitTVec <int> & reserveTempIntVec ();
         //
         // Same as reserveTempDblVec, but for int.

      WitTVec <double> & reserveTempDblVec ();
         //
         // Asserts that the temp vectors are not currently reserved,
         // reserves them, makes tempDblVec the right size, and returns it.

      void releaseTempVecs ();
         //
         // Asserts that the temp vectors are in use, then indicates that they
         // are not.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitApiMgr);

      static void setUpNewFailure ();
         //
         // Sets up WIT's response to a failure of the "new" operator,
         // if necessary.

      static int throwBadAlloc (size_t);
         //
         // Throws a std::bad_alloc exception.
         // (The argument is ignored.)

      void handleIssueMsgExc (WitIssueMsgExc & theIssueMsgExc);
         //
         // Handles theIssueMsgExc.

      void handleAbortApiExc (WitAbortApiExc & theAbortApiExc);
         //
         // Handles theAbortApiExc.

      void abortApiFunc (
            WitAbortApiExc &  theAbortApiExc,
            const WitString & theFuncName);
         //
         // Aborts the currently running API function in the manner indicated by
         // theAbortApiExc.
         // theFuncName is the name of the API function.

      inline WitMsgFacility & myMsgFac ();

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      static bool newHandlerSet_;
         //
         // True, iff the "new handler" has been set to throwBadAlloc.

      WitRun * const myWitRun_;
         //
         // The WitRun that owns this ApiMgr.

      WitInputID * myInputID_;
         //
         // The InputID owned by this ApiMgr.

      bool myWitRunIsLocked_;
         //
         // True, iff a call to an API function for myWitRun_ is currently in
         // progress.

      const char * apiFuncName_;
         //
         // The name of the current outer API function, if any; otherwise NULL.

      bool startInnerNeeded_;
         //
         // True, iff there is currently a need to call startInnerFunc ().

      bool mustSkipCallMsg_;
         //
         // True, iff this ApiMgr is not to issue the API function call Msg for
         // the current API function.

      bool preInitAllowed_;
         //
         // True, iff the current API function is allowed to be called when
         // witInitialize and witCopyData have not yet been called.
         // Only valid between a call to startApiFunc (...) and        
         // startInnerFunc (...).

      bool extOptActAllowed_;
         //
         // True, iff the current API function is allowed to be called when
         // external optimizing implosion is currently active.
         // Only valid between a call to startApiFunc (...) and        
         // startInnerFunc (...).

      WitReturnCode apiRc_;
         //
         // Return code value which api function will return to caller

      bool prevApiAbort_;
         //
         // True, iff at least one previous call to an API function for
         // myWitRun_ aborted with a severe or fatal return code.

      WitApiMgr * myApiMgr2_;
         //
         // During a call to an API function with two WitRun arguments, if the
         // two WitRuns are distinct, myApiMgr2_ is the ApiMgr for the second
         // WitRun; otherwise NULL.

      WitTVec <int> tempIntVec_;
         //
         // Temporary int vector to be used by some API "set" compatibility
         // functions.

      WitTVec <double> tempDblVec_;
         //
         // Temporary double vector to be used by some API "set" compatibility
         // functions.

      bool tempVecsReserved_;
         //
         // true, iff the temp vectors are currently reserved.
   };

//------------------------------------------------------------------------------

namespace WitNonClass
   {
   bool asaBool (WitBoolean theBoolean);
      //
      // Converts theBoolean to a bool.
   };

#endif
