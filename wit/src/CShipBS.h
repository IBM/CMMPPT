//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef CShipBSH
#define CShipBSH

//------------------------------------------------------------------------------
// Header file: "CShipBS.h"
//
// Contains the declaration of class CumShipBoundSet.
//
// Implemented in Demand.C.
//------------------------------------------------------------------------------

#include <BoundSet.h>

//------------------------------------------------------------------------------
// class CumShipBoundSet
//
// The bounds on the cumShipVol of a Demand.
//
// Class Hierarchy:
//
// ProbAssoc
//    BoundSet
//       CumShipBoundSet
//
// Implemented in BoundSet.C.
//------------------------------------------------------------------------------

class WitCumShipBoundSet: public WitBoundSet
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitCumShipBoundSet (WitDemand * theDemand);
         //
         // myDemand_ <-- theDemand.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitCumShipBoundSet ();

      //------------------------------------------------------------------------
      // Public virtual member functions.
      //------------------------------------------------------------------------

      virtual double        boundedVal           (WitPeriod);
      virtual const char *  myAttName            ();
      virtual WitDelComp *  myDelComp            ();
      virtual WitBoundSet * myMatchIn            (WitCompMgr *);
      virtual WitNode *     stageNode            ();
      virtual void          issueStage0MMCompMsg ();

   private:

      //------------------------------------------------------------------------
      // Private virtual member functions.
      //------------------------------------------------------------------------

      virtual WitMsgID     headingMsgID      ();
      virtual const char * boundedVectorName ();
      virtual void         idObjectForSoln   ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitCumShipBoundSet);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDemand * const myDemand_;
         //
         // The Demand whose cumShipVol is bounded by this CumShipBoundSet.
   };

#endif
