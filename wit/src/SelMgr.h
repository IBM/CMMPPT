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

#ifndef SelMgrH
#define SelMgrH

//------------------------------------------------------------------------------
// Header file: "SelMgr.h"
//
// Contains the declaration of class SelMgr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class SelMgr
//
// Base class for classes that manage a particular kind of selection.
//
// Class Hierarchy:
//
// ProbAssoc
//    SelMgr
//       MrSelMgr
//          PmrSelMgr
//          CmrSelMgr
//       BaMgr
//       MeMgr
//       SsrMgr
//       PrSelMgr
//          PprSelMgr
//          CprSelMgr
//
// Implemented in Selection.C.
//------------------------------------------------------------------------------

class WitSelMgr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSelMgr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual WitAltPt * myAltPtFor (
            WitBopEntry *  theBopEnt,
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            WitPeriod      fillPer) = 0;
         //
         // Finds the AltPt for this SelMgr that corresponds to an explosion
         // thru theBopEnt and theConsEnt in execution period execPer to fill
         // requirements in fillPer. If such a AltPt exists, it is returned;
         // otherwise NULL is returned.

      virtual WitAltPt * myAltPtFor (WitPart * thePart, WitPeriod fillPer);
         //
         // Finds the AltPt for this SelMgr that corresponds to an explosion
         // thru thePart to fill requirements in fillPer.
         // If such a AltPt exists and is well-defined, it is returned;
         // otherwise NULL is returned.
         // The default implementation returns NULL.

      virtual bool validForNetting () = 0;
         //
         // Returns true, iff the SelPts owned by this SelMgr apply to ConsEntry
         // netting.

      virtual bool validForExecBounds () = 0;
         //
         // Returns true, iff the SelPts owned by this SelMgr apply to 
         // execBound shortages.

      virtual bool validForLTBounds () = 0;
         //
         // Returns true, iff the SelPts owned by this SelMgr apply to 
         // lead time bound shortages.

      virtual const char * selCaseName () = 0;
         //
         // Returns a string naming which case of selection is managed by this
         // SelMgr.

      void setIndex ();
         //
         // Sets myIndex_.

      bool isLowerThan (WitSelMgr * theSelMgr);
         //
         // Returns true, iff this SelMgr represents a lower aspect of the
         // explosion logic than theSelMgr.

      //------------------------------------------------------------------------
      // Selection splitting functions.
      //------------------------------------------------------------------------

      virtual bool selSplitNeeded () = 0;
         //
         // Returns true, iff the input data associated with this SelMgr 
         // includes a request for selection splitting.

      void setUpSelSplit ();
         //
         // Sets up selection splitting for this SelMgr.

      void setBoundedSplit (bool);

      virtual void postSplitCommit ();
         //
         // Takes actions appropriate at the end of a split-commit, if any.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitSelector *, mySelector)
      accessFunc (bool,          selSplit)
      accessFunc (bool,          boundedSplit)
   
   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSelMgr (WitSelector *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSelMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSelector * const mySelector_;
         //
         // The Selector that owns this SelMgr.

      int myIndex_;
         //
         // An index for sorting SelMgrs.
         // # of SelMgrs owned by mySelector_ constructed before this one, where
         // construction starts with the highest SelMgr and proceeds upward.
         // Thus higher index means lower in the explosion logic.

      bool selSplit_;
         //
         // true, iff selection splitting is be to performed.

      bool boundedSplit_;
         //
         // true, iff there is currently a bounded split explosion at this
         // SelMgr.
   };

#endif
