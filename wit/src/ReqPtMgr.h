//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
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
