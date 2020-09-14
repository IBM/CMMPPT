//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PegElH
#define PegElH

//------------------------------------------------------------------------------
// Header file: "PegEl.h"
//
// Contains the declaration of the following classes:
//
//    PegEl.
//    FixedPegEl.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class PegEl.
//
// An element in a pegging.
// A PegEl is explicitly associated with a DelComp and an allocation period
// (allocPer) and implicitly associated with a Demand and shipPer. 
// The DelComp is either an Operation or a SubEntry. It stores the
// amount by which heuristic allocation increased the execVol or subVol for the
// DelComp in allocPer in order to add (to the exisiting schedule) a shipment
// for the Demand in shipPer.
//
// Implemented in Pegging.C.
//------------------------------------------------------------------------------

class WitPegEl
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPegEl (WitDelComp * theDelComp, WitPeriod);
         //
         // theDelComp must be either an Operation or a Sub.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPegEl ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void saveStateIn (WitSglSrcMgr * theSglSrcMgr);
         //
         // Saves the current state of this PegEl in theSglSrcMgr.

      void pegDeltaVol (double deltaVol);
         //
         // Increments peggedVol_ by deltaVol.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPeriod,       myExecPer)
      accessFunc (double,          peggedVol)
      accessFunc (WitFixedPegEl *, myFixedPegEl)

      WitOperation * myOpn () const;
         //
         // Returns the Operation for this PegEl, if any, else NULL.

      WitSubEntry * mySub () const;
         //
         // Returns the Sub       for this PegEl, if any, else NULL.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPegEl);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDelComp * const myDelComp_;
         //
         // The DelComp for this PegEl.

      const WitPeriod myExecPer_;
         //
         // The execPer for this PegEl.

      double peggedVol_;
         //
         // Let theOpn or theSub = myDelComp_, as appropriate.
         // peggedVol_ is the amount by which heuristic allocation increased
         // theOpn->execVol_[myExecPer_] or 
         // theSub-> subVol_[myExecPer_] in order to add a shipment for
         // this PegEl's Demand in the shipPer to the exisiting schedule. 
         // Specifically, it's the total amount since the last time the pegging
         // was cleared.

      WitFixedPegEl * myFixedPegEl_;
         //
         // The FixedPegEl owned by this PegEl.
   };

//------------------------------------------------------------------------------
// Class FixedPegEl.
//
// Similar to a PegEl, but with fixed data.
//
// Implemented in Pegging.C.
//------------------------------------------------------------------------------

class WitFixedPegEl
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitFixedPegEl (WitPegEl *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitFixedPegEl ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessWrapper (WitPeriod,      myExecPer, myPegEl_);
      accessWrapper (double,         peggedVol, myPegEl_);
      accessWrapper (WitOperation *, myOpn,     myPegEl_);
      accessWrapper (WitSubEntry *,  mySub,     myPegEl_);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitFixedPegEl);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPegEl * const myPegEl_;
         //
         // The PegEl that owns this FixedPegEl.
   };

#endif
