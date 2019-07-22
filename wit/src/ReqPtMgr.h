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

#ifndef ReqPtMgrH
#define ReqPtMgrH

//------------------------------------------------------------------------------
// Header file: "ReqPtMgr.h"
//
// Contains the declaration of class ReqPtMgr. 
//------------------------------------------------------------------------------

#include <ObjStack.h>
#include <PtrSched.h>

//------------------------------------------------------------------------------
// Class ReqPtMgr
//
// ReqPt Manager.
// Responsible for managing the ReqPts used by a Selector.
//
// Implemented in Selection.C.
//------------------------------------------------------------------------------

class WitReqPtMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitReqPtMgr (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitReqPtMgr ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      WitReqPt * myReqPt (WitPart * thePart, WitPeriod thePer);
         //
         // myReqPt (thePart, thePer) is the ReqPt for thePart in thePer,
         // constructing it, if necessary.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void reset ();
         //
         // Deletes all ReqPts.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitReqPtMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPtrSched <WitPart, WitReqPt> myReqPts_;
         //
         // myReqPts_.myPtrAt (thePart, thePer) is the ReqPt for thePart in 
         // thePer, or NULL, if there is none.

      WitObjStack <WitReqPt *> allocedSlots_;
         //
         // Non-NULL slots in myReqPts_.

      WitObjStack <WitReqPt> unusedPoints_;
         //
         // The ReqPts not currently in use.
   };

#endif
