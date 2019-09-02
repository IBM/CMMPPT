//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SelPtH
#define SelPtH

//------------------------------------------------------------------------------
// Header file: "SelPt.h"
//
// Contains the declaration of class SelPt.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class SelPt
//
// "Selection Point"
// Represents a point in the problem at which a selection must be made.
//
// Class Hierarchy:
//
// SelPt
//    DetSelPt
//       MrPt
//       BaPt
//       MePt
//       SsrPt
//    PrSelPt
//
// Implemented in Selection.C.
//------------------------------------------------------------------------------

class WitSelPt
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      void attachSplitPt (WitSplitPt * theSplitPt);
         //
         // Associates theSplitPt with this SelPt.

      void detachSplitPt ();
         //
         // Makes this SelPt unassociated with any SplitPt.

      void recordFlowSS (double incFlowVol);
         //
         // See WitFlowMonitor::recordFlow.

      virtual WitSelMgr * mySelMgr () = 0;
         //
         // Returns the SelMgr for this SelPt.

      virtual void prtID () = 0;
         //
         // Prints info identifying this SelPt.

      virtual bool splittable () = 0;
         //
         // Returns true, iff selection splitting is to be performed at this
         // SelPt.

      virtual void getSortData (WitPart * & thePart, WitPeriod & thePer) = 0;
         //
         // Sets the args to the Part and Period associated with this SelPt, for
         // determining the lowest ZeroPt in sel-split.

      virtual WitSelCand * newSelCand () = 0;
         //
         // Constructs and returns a new instance of a class derived from
         // SelCand, where the derived class corresponds to the derived class of
         // which this SelPt is an instance.
         // Sets the SelCand to represent the current selection at this SelPt.
         // Ownership of the SelCand is transfered to the calling function.
         // This is an application of the "factory method" design pattern.

      virtual bool sglSrcMode ();
         //
         // Returns true, iff this SelPt is currently in sgl-src mode.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessNonNull (WitSplitPt *, mySplitPt)

      inline bool isSplit ()
         {
         return (mySplitPt_ != NULL);
         }

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSelPt ();

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSelPt ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      virtual void recSplit ();
         //
         // Records the fact that the selection at this SelPt is splitting,
         // if the derived class needs to do this.
         // The default behavior is no-op.

      virtual void recUnsplit ();
         //
         // Records the fact that the selection at this SelPt is unsplitting,
         // if the derived class needs to do this.
         // The default behavior is no-op.

      noCopyCtorAssign (WitSelPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSplitPt * mySplitPt_;
         //
         // If the selection for this SelPt is currently split, mySplitPt_ is
         // the SplitPt that represents the split; otherwise mySplitPt_ is NULL.
   };

#endif
