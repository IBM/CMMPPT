//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ExecBSH
#define ExecBSH

//------------------------------------------------------------------------------
// Header file: "ExecBS.h"
//
// Contains the declaration of class ExecBoundSet.
//------------------------------------------------------------------------------

#include <BoundSet.h>

//------------------------------------------------------------------------------
// class ExecBoundSet
//
// The bounds on the execVol of an Operation.
//
// Class Hierarchy:
//
// ProbAssoc
//    BoundSet
//       ExecBoundSet
//
// Implemented in BoundSet.C.
//------------------------------------------------------------------------------

class WitExecBoundSet: public WitBoundSet
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitExecBoundSet (WitOperation * theOpn);
         //
         // myOperation_ <-- theOpn.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitExecBoundSet ();

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

      noCopyCtorAssign (WitExecBoundSet);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitOperation * const myOperation_;
         //
         // The Operation whose execVol is bounded by this ExecBoundSet.
   };

#endif
