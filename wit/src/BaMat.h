//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef BaMatH
#define BaMatH

//------------------------------------------------------------------------------
// Header file: "BaMat.h"
//
// Contains the declaration of class BaMat.
//------------------------------------------------------------------------------

#include <PtrTVec.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// Class BaMat
//
// "Build-Ahead Material"
// Represents a Material at which global build-ahead is requested.
//
// Implemented in BuildAhd.C.
//------------------------------------------------------------------------------

class WitBaMat: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBaMat (WitMaterial *, WitBaDir *, WitBaMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitBaMat ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMaterial *,                myMaterial)
      accessFunc (WitBaMgr *,                   myBaMgr)
      accessFunc (const WitPtrTVec <WitBaPt> &, myBaPt)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void findInitExpPers (WitTVec <WitPeriod> & expPerVec);
         //
         // On return, expPerVec[fillPer] is the initial explosion period to be
         // used when filling requirements for myMaterial_ in fillPer.

      void reject (WitPeriod expPer);
         //
         // Designates expPer as an unuseable explosion period for myMaterial_.

      WitPeriod nextUseableExpPer (WitPeriod fillPer);
         //
         // Returns the next useable explosion period beyond fillPer, if any;
         // otherwise -1.

      WitPeriod findInitExpPer (WitPeriod fillPer);
         //
         // Returns the initial explosion period to be used when filling
         // requirements for myMaterial_ in fillPer.

      void print ();
         //
         // Prints this BaMat.

      void recoverInitState ();
         //
         // Restores the Build-Ahead configuration at this BaPt to its initial
         // state.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void rejectUnexplodablePers ();
         //
         // Rejects each period that's not explodeable for myMaterial_.

      void deselect (WitPeriod expPer);
         //
         // Causes expPer not to be selected by any BaPt.

      void printRecovery ();
         //
         // Prints the restoration of the Build-Ahead configuration at this
         // BaMat to its initial state.

      noCopyCtorAssign (WitBaMat);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMaterial * const myMaterial_;
         //
         // The Material at which global build-ahead is to be considered.

      WitBaDir * const myBaDir_;
         //
         // The BaDir for this BaMat.

      WitBaMgr * const myBaMgr_;
         //
         // TheBaMgr that owns this BaMat.

      WitPtrTVec <WitBaPt> myBaPt_;
         //
         // myBaPt_[fillPer] is the BaPt for this BaMat in fillPer.

      WitTVec <bool> useable_;
         //
         // useable_[expPer] is true, iff expPer is currently considered to be
         // useable as an explosion period.
   };

#endif
