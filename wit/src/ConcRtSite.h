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

#ifndef ConcRtSiteH
#define ConcRtSiteH

//------------------------------------------------------------------------------
// Header file: "ConcRtSite.h"
//
// Contains the declaration of the following classes:
//
//    ProdRtSite.
//    ConsRtSite.
//
// These are the "concrete" RtSite classes.
//------------------------------------------------------------------------------

#include <RtSite.h>

//------------------------------------------------------------------------------
// Class ProdRtSite
//
// "Production Routing Site"
// Represents a Part in its role as a site at which a production routing
// decision must be made.
//
// Class Hierarchy:
//
// ProbAssoc
//    RtSite
//       ProdRtSite
//
// Implemented in Routing.C.
//------------------------------------------------------------------------------

class WitProdRtSite: public WitRtSite
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitProdRtSite (WitPart *, WitRtMgr *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitProdRtSite ();

      //------------------------------------------------------------------------
      // Overrides from class RtSite.
      //------------------------------------------------------------------------

      virtual bool                      isForProd       ();
      virtual void                      printID         (int, int);
      virtual const char *              perCaseID       ();
      virtual bool                      explodeable     (WitPeriod);
      virtual const WitFlexVec <bool> & propRtg         ();
      virtual bool                      singleSource    ();
      virtual WitPart *                 mrSortPart      ();
      virtual WitRtCand *               mrDefaultRtCand (WitPeriod);
      virtual int                       heightIdx       ();
      virtual void                      findRtCands     (WitList <WitRtCand> &);

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitProdRtSite);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPart * const myPart_;
         //
         // The Part represented by this ProdRtSite.
   };

//------------------------------------------------------------------------------
// Class ConsRtSite
//
// "Consumption Routing Site"
// Represents a BomEntry in its role as a site at which a consumption routing
// decision must be made.
//
// Class Hierarchy:
//
// ProbAssoc
//    RtSite
//       ConsRtSite
//
// Implemented in Routing.C.
//------------------------------------------------------------------------------

class WitConsRtSite: public WitRtSite
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitConsRtSite (WitBomEntry *, WitRtMgr *);

   private:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitConsRtSite ();

      //------------------------------------------------------------------------
      // Overrides from class RtSite.
      //------------------------------------------------------------------------

      virtual bool                      isForProd       ();
      virtual void                      printID         (int, int);
      virtual const char *              perCaseID       ();
      virtual bool                      explodeable     (WitPeriod);
      virtual const WitFlexVec <bool> & propRtg         ();
      virtual bool                      singleSource    ();
      virtual WitPart *                 mrSortPart      ();
      virtual WitRtCand *               mrDefaultRtCand (WitPeriod);
      virtual int                       heightIdx       ();
      virtual void                      findRtCands     (WitList <WitRtCand> &);

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitConsRtSite);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitBomEntry * const myBomEnt_;
         //
         // The BomEntry represented by this ConsRtSite.
   };

#endif
