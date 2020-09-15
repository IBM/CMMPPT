//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef StockBSH
#define StockBSH

//------------------------------------------------------------------------------
// Header file: "StockBS.h"
//
// Contains the declaration of class StockBoundSet.
//------------------------------------------------------------------------------

#include <BoundSet.h>

//------------------------------------------------------------------------------
// class StockBoundSet
//
// The bounds on the stockVol of a Material.
//
// Class Hierarchy:
//
// ProbAssoc
//    BoundSet
//       StockBoundSet
//
// Implemented in BoundSet.C.
//------------------------------------------------------------------------------

class WitStockBoundSet: public WitBoundSet
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStockBoundSet (WitMaterial * theMat);
         //
         // myMaterial_ <-- theMat.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitStockBoundSet ();

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

      noCopyCtorAssign (WitStockBoundSet);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMaterial * const myMaterial_;
         //
         // The Material whose stockVol is bounded by this StockBoundSet.
   };

#endif
