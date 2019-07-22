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

#ifndef MrSiteH
#define MrSiteH

//------------------------------------------------------------------------------
// Header file: "MrSite.h"
//
// Contains the declaration of class MrSite.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <PtrTVec.h>

//------------------------------------------------------------------------------
// Class MrSite
//
// "Multi-Route Site"
// Represents a location in the BOM structure at which multi-route selection is
// to be performed.
//
// Implemented in MultiRoute.C.
//------------------------------------------------------------------------------

class WitMrSite: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMrSite (WitRtSite *, WitMrCoord *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMrSite ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      WitRtCand * selRtCand (WitPeriod thePer);
         //
         // Returns the RtCand selected to be used at this MrSite in
         // thePer. Invalid, if this MrSite is not explodeable in thePer.

      void printHasAlt ();
         //
         // Prints those MrPts for which hasAlt () is true.

      inline WitMrPt * myMrPtWithAlt (WitPeriod thePer)
            //
            // Returns the MrPt for this MrSite in thePer, if it has an
            // alternative to the current selection; otherwise NULL.
            //
         {
         return
            multiChoice_?
               multiChoiceMrPtWithAlt (thePer):
               NULL;
         }

      WitSelMgr * mySelMgr ();
         //
         // Returns the SelMgr for this MrSite.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitRtSite *,                  myRtSite)
      accessFunc (WitMrCoord *,                 myMrCoord)
      accessFunc (const WitPtrTVec <WitMrPt> &, myMrPt)
      accessFunc (bool,                         multiChoice)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      bool compMultiChoice ();
         //
         // Computes and returns the value to which multiChoice_ should be set.

      void buildMrPts ();
         //
         // Creates the MrPts for this MrSite.

      WitMrPt * multiChoiceMrPtWithAlt (WitPeriod thePer);
            //
            // Returns the MrPt for this MrSite in thePer, if it has an
            // alternative to the current selection; otherwise NULL.
            // multiChoice_ must be true.

      noCopyCtorAssign (WitMrSite);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitRtSite * const myRtSite_;
         //
         // The RtSite for this MrSite.

      WitMrCoord * const myMrCoord_;
         //
         // the MrCoord for this MrSite.

      WitPtrTVec <WitMrPt> myMrPt_;
         //
         // myMrPt_[thePer] is the MrPt owned by this MrSite corresponding to
         // thePer.
         // Allocated only if multiChoice_ is true.

      bool multiChoice_;
         //
         // true, iff there are at least two directions in which the routing
         // could potentially proceed downward from this MrSite.
   };

#endif
