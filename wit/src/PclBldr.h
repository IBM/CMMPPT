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

#ifndef PclBldrH
#define PclBldrH

//------------------------------------------------------------------------------
// Header file: "PclBldr.h"
//
// Contains the declaration of class PclBldr.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class PclBldr
//
// "Pegged Critical List Builder"
// Responsible for generating the pegged critical list.
//
// Class Hierarchy:
//
// ProbAssoc
//    PclBldr
//------------------------------------------------------------------------------

class WitPclBldr: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPclBldr (WitProblem *, WitList <WitPclEl> &);

      WitPclBldr (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPclBldr ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void initSoln ();
         //
         // Clears the pegged critical list.

      void preIncAlloc (WitDemand * theDemand, WitPeriod shipPer);
         //
         // Takes action appropriate at the beginning of HeurAtor::inc (), for
         // the arguments.

      void recFundShortage (WitPart * thePart, WitPeriod thePer);
         //
         // Records a fundamental shortage of thePart in thePer.

      void recRestShortage ();
         //
         // Records the occurrance of a restriction shortage (from pen-exec).

      void postCmqs ();
         //
         // Takes action appropriate at the conclusion of
         // HeurAtor::commitMaxQtySel ().

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPclBldr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDemand * curDemand_;
         //
         // The Demand that the HeurAtor is currently working on.

      WitPeriod curShipPer_;
         //
         // The shipment period that the HeurAtor is currently working on.

      WitPart * shortPart_;
         //
         // The most recent short Part, if any; otherwise NULL.

      WitPeriod shortPer_;
         //
         // The most recent short period, if any; otherwise -1.
   };

#endif
