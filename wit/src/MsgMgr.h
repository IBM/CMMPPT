//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MsgMgrH
#define MsgMgrH

//------------------------------------------------------------------------------
// Header file: "MsgMgr.h"
//
// Contains the declaration of class MsgMgr.
//------------------------------------------------------------------------------

#include <PtrVec.h>
#include <MsgFac.h>

#include <mcl/src/mcl.h>

//------------------------------------------------------------------------------
// class MsgMgr
//
// Message Manager: The internal part of a MsgFacility.
//------------------------------------------------------------------------------

class WitMsgMgr
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMsgMgr (WitMsgFacility & theMsgFac);
         //
         // myMsgFac <-- theMsgFac.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMsgMgr ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc    (WitMsgFacility &, myMsgFac)
      accessFunc    (bool,             errorsIssued)
      accessFunc    (WitApiMgr *,      myApiMgr)

      accessNonNull (WitMsg *,         issuedMsg)

      inline WitSession * mySession () const
         {
         return myMsgFac ().mySession ();
         }

      inline WitOutputDisplayer & myDisp () const
         {
         return * myDispPtr_;
         }

      inline MclFacility & myMclFacility () const
         {
         return * myMclFacilityPtr_;
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void record (WitMsg * theMsg);
         //
         // Records the fact that theMsg is being constructed.

      void issue (WitMsgID theMsgID, const MclArgList & theArgList);
         //
         // Issues the Msg matching MsgID, with theArgList.

      WitMsg * myMsg (MclMsg * theMclMsg) const;
         //
         // Returns the Msg whose externalNumber matches theMclMsg.
         // Asserts that there is one.

      WitMsg * extNumMsg (int theExtNum) const;
         //
         // Returns the Msg whose externalNumber is theExtNum, if any,
         // else NULL.

      bool issueAllowed (WitMsg * theMsg) const;
         //
         // Returns true, if it is OK to issue theMsg.

      void setCrashOnFatal (bool theValue);
         //
         // crashOnFatal_ <-- theValue

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMsgMgr);

      void resizeExtNumMsg ();
         //
         // Makes extNumMsg_ the appropriate size.

      void crashForFatal ();
         //
         // Deliberately crashes WIT, to allow easier debugging.
         // To be used optionally on a fatal error.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMsgFacility & myMsgFac_;
         //
         // The MsgFacility that owns this MsgMgr.

      WitOutputDisplayer * myDispPtr_;
         //
         // Ptr to the OutputDisplayer owned by this MsgMgr.

      MclFacility * myMclFacilityPtr_;
         //
         // Ptr to the MclFacility owned by this WitMsgMgr.

      WitApiMgr * const myApiMgr_;
         //
         // The ApiMgr for this MsgMgr.

      WitPtrVec <WitMsg> extNumMsg_;
         //
         // Data for extNumMsg.

      bool errorsIssued_;
         //
         // true, iff at least one Msg of Level >= error has been issued.

      WitMsg * issuedMsg_;
         //
         // The Msg currently being issued, if any, else NULL.
         // (The inner most Msg, if issue is called recursively.)

      bool crashOnFatal_;
         //
         // If true, WIT will deliberately crash when aborting.
         // (Useful when debugging.)
   };

#endif
