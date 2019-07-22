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

#ifndef SsrPtH
#define SsrPtH

//------------------------------------------------------------------------------
// Header file: "SsrPt.h"
//
// Contains the declaration of class SsrPt.
//------------------------------------------------------------------------------

#include <DetSelPt.h>

//------------------------------------------------------------------------------
// Class SsrPt
//
// "Selective Stock Reallocation Point"
// A SelPt for Selective Stock Reallocation.
// An SsrPt represents a Material and a period from which stock reallocation is
// initially allowed, but can eventually be disallowed.
//
// Class Hierarchy:
//
// SelPt
//    DetSelPt
//       SsrPt
//
// Implemented in SelStRe.C.
//------------------------------------------------------------------------------

class WitSsrPt: public WitDetSelPt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSsrPt (WitMaterial *, WitPeriod, WitSsrMgr *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSsrPt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void defineSplit (
            double   rollVol,
            double & srAllVol,
            double & srNotAllVol);
         //
         // Sets srAllVol and srNotAllVol to be the SR-allowed-volume and the
         // SR-allowed-volume for a split explosion of rollVol units at this
         // SsrPt.

      void setSrWasDone (bool);

      void recoverInitState ();
         //
         // Restores the SSR configuration at this SsrPt to its initial state.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitSsrMgr *, mySsrMgr)
      accessFunc (bool,        srAllowed);
      accessFunc (bool,        srWasDone)
                
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

      void setSrAllowed (bool);

      void printRecovery ();
         //
         // Prints the restoration of the SSR configuration at this SsrPt to its
         // initial state.

      noCopyCtorAssign (WitSsrPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMaterial * const myMat_;
         //
         // The Material represented by this SsrPt.

      const WitPeriod myFromPer_;
         //
         // The Period represented by this SsrPt.

      WitSsrMgr * const mySsrMgr_;
         //
         // The SsrMgr for this SsrPt.

      bool srAllowed_;
         //
         // true, iff stock reallocation from myFromPer_ for myMat_ is allowed.

      bool srWasDone_;
         //
         // true, iff any of the stock of myMat_ in myFromPer_ was reallocated
         // to an earlier period during the current temp-commit.
   };

#endif
