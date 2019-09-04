//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MrPtH
#define MrPtH

//------------------------------------------------------------------------------
// Header file: "MrPt.h"
//
// Contains the declaration of class MrPt.
//------------------------------------------------------------------------------

#include <DetSelPt.h>
#include <SelCand.h>

//------------------------------------------------------------------------------
// Class MrPt.
//
// "Multi-Route Point"
// Represents a location (in the BOM structure) and period at which multi-route
// selection is to be performed.
//
// Class Hierarchy:
//
// SelPt
//    DetSelPt
//       MrPt
//
// Implemented in MultiRoute.C.
//------------------------------------------------------------------------------

class WitMrPt: public WitDetSelPt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMrPt (WitMrSite *, WitPeriod);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMrPt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void select (WitRtCand * theRtCand);
         //
         // Selects theRtCand for this MrPt.

      void recoverInitState ();
         //
         // Restores the Multi-Route configuration at this MrPt to its initial
         // state.

      accessFunc (WitMrSite *, myMrSite)
      accessFunc (WitPeriod,   myPer)
      accessFunc (WitRtCand *, selRtCand)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      virtual WitSelMgr *  mySelMgr    ();
      virtual void         prtID       ();        
      virtual bool         splittable  ();   
      virtual void         getSortData (WitPart * &, WitPeriod &);
      virtual WitSelCand * newSelCand  ();
      virtual bool         sglSrcMode  ();
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

      void refreshHasResAlt ();
         //
         // Computes hasResAlt_.

      WitRtCand * findNextRtCand ();
         //
         // Finds and returns the first selectable RtCand for this MrPt
         // that is not its current selection, or NULL, if there is none.

      void printRecovery ();
         //
         // Prints the restoration of the Multi-Route configuration at this MrPt
         // to its initial state.

      FILE * msgFile ();

      inline WitSelector * mySelector ();

      noCopyCtorAssign (WitMrPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMrSite * const myMrSite_;
         //
         // The MrSite that owns this MrPt.

      const WitPeriod myPer_;
         //
         // The period in which selection is to be done.

      WitRtCand * selRtCand_;
         //
         // The RtCand currently selected for routing from this MrPt, if any;
         // otherwise, NULL.

      bool hasResAlt_;
         //
         // true, iff this MrPt currently has a selection and an 
         // shortage-resolving alternative to the current selection exists.
   };

#endif
