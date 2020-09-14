//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef BaPtH
#define BaPtH

//------------------------------------------------------------------------------
// Header file: "BaPt.h"
//
// Contains the declaration of class BaPt.
//------------------------------------------------------------------------------

#include <DetSelPt.h>

//------------------------------------------------------------------------------
// Class BaPt
//
// "Build-Ahead Point"
// A SelPt for build-ahead.
// A BaPt represents a Material and fill period at which an explosion period
// must be selected.
//
// Class Hierarchy:
//
// SelPt
//    DetSelPt
//       BaPt
//
// Implemented in BuildAhd.C.
//------------------------------------------------------------------------------

class WitBaPt: public WitDetSelPt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBaPt (WitBaMat *, WitPeriod fillPer);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitBaPt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      static void setInitSelExpPers (WitBaMat * theBaMat);
         //
         // Sets the selected explosion period for each BaPt for theBaMat to its
         // initial value.

      void advanceSel ();
         //
         // Advances selExpPer_ to the next useable explosion period, if any;
         // otherwise, sets selExpPer_ to -1.

      void select (WitPeriod expPer);
         //
         // Sets the explosion period currently selected by this BaPt to expPer.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitBaMat *, myBaMat)
      accessFunc (WitPeriod,  selExpPer)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      virtual WitSelMgr *  mySelMgr    ();
      virtual void         prtID       ();        
      virtual bool         splittable  ();   
      virtual void         getSortData (WitPart * &, WitPeriod &);
      virtual WitSelCand * newSelCand  ();
         //
         // Overrides from class SelPt.

      virtual bool hasResAlt       ();
      virtual void alterSelection  ();
      virtual void storeRecoveryPt ();
      virtual void tempAlterSel    ();
      virtual void cancelTempAlt   ();
      virtual void printAlteration ();
         //
         // Overrides from class DetSelPt.

      noCopyCtorAssign (WitBaPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitBaMat * const myBaMat_;
         //
         // The BaMat that owns this BaPt.

      const WitPeriod myFillPer_;
         //
         // The fill period associated with this BaPt.

      WitPeriod selExpPer_;
         //
         // The explosion period currently selected by this BaPt, if any,
         // otherwise -1.
   };

#endif
