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

#ifndef PrSelPtH
#define PrSelPtH

//------------------------------------------------------------------------------
// Header file: "PrSelPt.h"
//
// Contains the declaration of class PrSelPt.
//------------------------------------------------------------------------------

#include <SelPt.h>

//------------------------------------------------------------------------------
// Class PrSelPt
//
// "Proportionate Routing Selection Point"
// A SelPt for proportionate routing
// A PrSelPt represents an RtSite and a period at which proportionate routing is
// to be performed.
//
// Class Hierarchy:
//
// SelPt
//    PrSelPt
//
// Implemented in PropRtg.C.
//------------------------------------------------------------------------------

class WitPrSelPt: public WitSelPt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPrSelPt (WitRtSite *, WitPeriod, WitPrCoord *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPrSelPt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void print ();
         //
         // Prints the current state of this PrSelPt.

      void printAllowedRtCands ();
         //
         // Prints the RtCands for myRtSite_ that are currently allowed for
         // prop-rt in myPer_.

      void incAllowedRtCands (int delta);
         //
         // Adds delta to nAllowedRtCands_.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPeriod, myPer)
      accessFunc (int,       nAllowedRtCands)

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

      noCopyCtorAssign (WitPrSelPt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitRtSite * const myRtSite_;
         //
         // The RtSite for this PrSelPt.

      const WitPeriod myPer_;
         //
         // The period for this PrSelPt.

      WitPrCoord * const myPrCoord_;
         //
         // The PrCoord that owns this PrSelPt.

      int nAllowedRtCands_;
         //
         // # of RtCands for myRtSite_ that are currently allowed in myPer_.
   };

#endif
