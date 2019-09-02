//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MsgH
#define MsgH

//------------------------------------------------------------------------------
// Header file: "Msg.h"
//
// Contains the declaration of class Msg.
//------------------------------------------------------------------------------

#include <typedefs.h>

#include <mcl/src/mcl.h>

//------------------------------------------------------------------------------
// Class Msg
//
// WIT's specialization of class MclMsg.
//
// Implemented in MsgMgr.C.
//------------------------------------------------------------------------------

class WitMsg: public MclMsg
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMsg (
            WitMsgMgr *      theMsgMgr,
            WitMsgID         theMsgID,
            const MclLevel & theLevel,
            const char *     textVal,
            int              extNum);
         //
         // myMsgMgr_ <-- theMsgMgr.
         // MclMsg    <<< other Args.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMsg ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, delayedSevere)
      accessFunc (bool, stopRunning)
      accessFunc (bool, throwErrorExc)
      accessFunc (bool, dupToStdout)
      accessFunc (bool, issueDuringRD)
      accessFunc (bool, idInputOnIssue)
      accessFunc (bool, testable)

      //------------------------------------------------------------------------
      // Attribute setting functions.
      //------------------------------------------------------------------------

      void delayedSevere  (bool);
      void stopRunning    (bool);
      void throwErrorExc  (bool);
      void dupToStdout    (bool);
      void issueDuringRD  (bool);
      void idInputOnIssue (bool);
      void testable       (bool);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMsg);

      virtual void postIssue ();
         //
         // Override from MclMsg.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMsgMgr * const myMsgMgr_;
         //
         // The MsgMgr for this Msg.

      bool delayedSevere_;
         //
         // true, iff this Msg is a severe Msg, but WIT is to delay aborting
         // until preprocessing or the end of reading the data file.

      bool stopRunning_;
         //
         // If true, then if this Msg calls MsgMgr::abort, then program process
         // will be terminated.
         // If stopRunning_ is set to true, throwErrorExc_ is automatically
         // set to false.
         // If both stopRunning_ and throwErrorExc_ are false, then if this
         // Msg calls MsgMgr::abort, the API function called by the application
         // program immediately returns.

      bool throwErrorExc_;
         //
         // If true, then if this Msg calls MsgMgr::abort, then WIT will throw
         // an exception to be caught by the application program.
         // If throwErrorExc_ is set to true, stopRunning_ is automatically
         // set to false.

      bool dupToStdout_;
         //
         // If true, then when this Msg is issued in stand-alone mode, it will
         // also display to stdout.
         // Only allowed on info Msgs.

      bool issueDuringRD_;
         //
         // This Msg is allowed to be issued during readData, iff issueDuringRD_
         // is true or GlobalComp::readDataMsgs () is true.

      bool idInputOnIssue_;
         //
         // WitRun::idInput will be called when this Msg is issued, iff
         // idInputOnIssue_ is true.

      bool testable_;
         //
         // true, iff this Msg is to be tested by the Msg testing application
         // program.
   };

#endif
