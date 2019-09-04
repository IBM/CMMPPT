//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MePtH
#define MePtH

//------------------------------------------------------------------------------
// Header file: "MePt.h"
//
// Contains the declaration of class MePt.
//------------------------------------------------------------------------------

#include <DetSelPt.h>

//------------------------------------------------------------------------------
// Class MePt
//
// "Multi-Exec Point"
// A SelPt for multi-exec.
// A MePt represents a BopEntry and explosion period at which an execution
// period must be selected.
//
// Class Hierarchy:
//
// SelPt
//    DetSelPt
//       MePt
//
// Implemented in MultiEx.C.
//------------------------------------------------------------------------------

class WitMePt: public WitDetSelPt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMePt (WitMeSitePt *, WitMeDir *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMePt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void print ();
         // 
         // Prints this MePt.

      void select (WitPeriod execPer);
         //
         // Sets execPer as the execution period currently selected by this
         // MePt.

      void recoverInitState ();
         //
         // Restores the Multi-Exec configuration at this PePt to its initial
         // state.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMeSitePt *, myMeSitePt)
      accessFunc (WitMeDir *,    myMeDir)
      accessFunc (WitPeriod,     selExecPer)

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

      void advanceSel ();
         //
         // Advances selMeCand () to the next appropriate MeCand if any;
         // otherwise NULL.

      void deSelectAsNeeded ();
         //
         // If this MePt currently selects an exec period, this function
         // causes it to stop doing so.

      void printRecovery ();
         //
         // Prints the restoration of the Multi-Exec configuration at this MePt
         // to its initial state.

      noCopyCtorAssign (WitMePt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMeSitePt * const myMeSitePt_;
         //
         // The MeSitePt that owns this MePt.

      WitMeDir * const myMeDir_;
         //
         // The MeDir associated with this MePt.

      WitPeriod selExecPer_;
         //
         // The execution period currently selected by this MePt, if any;
         // otherwise -1.
   };

#endif
