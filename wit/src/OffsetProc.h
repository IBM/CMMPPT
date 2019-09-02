//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OffsetProcH
#define OffsetProcH

//------------------------------------------------------------------------------
// Header file: "OffsetProc.h"
//------------------------------------------------------------------------------

#include <Schedule.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class OffsetProc
// "Offset preprocessor"
//
// Responsible for computing:
//    WitOperation::executable_
//    WitBillEntry::impactPeriod_
// as part of preprocessing.
//
// Implemented in PreHelp.C.
//------------------------------------------------------------------------------

class WitOffsetProc: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOffsetProc (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitOffsetProc ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void roundOffsets ();
         //
         // Computes impact periods by applying a special rounding algorithm on
         // offsets, designed to avoid cumulative round-off errors.
         // Data computed:
         //    impactPeriod for each BomEntry and each BopEntry.
         //    executable for each Operation.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void truncateOffsets ();
         //
         // Computes truncOffset_;

      void truncateOffset (WitBillEntry * theBillEnt);
         //
         // Computes truncOffset_ (theBillEnt).

      bool roundOffsets (WitOperation * theOpn, WitPeriod thePer);
         //
         // Computes impactPeriods for the BomEntries and BopEntries of
         // theOpn in period t.
         // Also checks for the various conditions under which execution of
         // theOpn is to be forbidden in period t.
         // If any such condition occurs, computation is terminated and false
         // is returned, indicating forbidden execution.
         // Otherwise true is returned.

      bool compImpactPeriod (WitConsEntry * theConsEnt, WitPeriod thePer);
         //
         // Computes theConsEnt->impactPeriod_[thePer].
         // If this functions discovers that execution of
         // theConsEnt->myOperation () in thePer is to be forbidden, the
         // computation is aborted and the function returns false.
         // Otherwise returns true.

      void copyImpactPeriod (WitSubEntry * theSub, WitPeriod thePer);
         //
         // Copies impactPeriod to theSub from its BomEntry.

      bool compImpactPeriod (WitBopEntry * theBopEnt, WitPeriod thePer);
         //
         // CompImpactPeriod for a BopEntry.

      static int ltRoundDown (double leadTime);
         //
         // Returns leadTime rounded down to an int with some numerical fuzz.

      void resetExec (WitOperation * theOpn, WitPeriod thePer);
         //
         // Turns off impactPeriods and dblCumLead given theOpn cannot
         // be executed in period t.

      void updateProdCumLeads (
            WitOperation *     theOpn,
            WitTVec <double> & leadToImpact,
            WitTVec <bool> &   hasImpact);
         //
         // Updates dblCumLead_ for the Parts produced by theOpn.
         // For a given BopEntry, and Period thePer:
         //    hadImpact[thePer] is true, iff the BopEntry has an 
         //       impactPeriod == thePer
         //    leadToImpact[thePer] is the double cum lead time for impactPeriod
         //       thePer, if any.

      double workOffset (WitBillEntry * theBillEnt, WitPeriod thePer);
         //
         // Returns the "working offset" of theBillEnt in thePer, i.e., either
         // the actual offset or the truncated offset, as appropriate.

      noCopyCtorAssign (WitOffsetProc);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSchedule <WitBillEntry, double> truncOffset_;
         //
         // If truncOffsets is true,
         //    truncOffset_ (theBillEnt)[thePer] is 
         //    theBillEnt->offset ()[thePer], truncated to avoid a negative
         //    impact Period.
         // Otherwise not allocated.

      WitSchedule <WitNode, double> dblCumLead_;
         //
         // dblCumLead_ (theNode)[thePer] is the double cumulative lead time of
         // theNode for execution in thePer, if theNode is an Operation or
         // production in thePer, if theNode is a Part.
   };

#endif
