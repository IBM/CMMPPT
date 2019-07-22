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
