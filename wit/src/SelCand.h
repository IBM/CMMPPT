//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef SelCandH
#define SelCandH

//------------------------------------------------------------------------------
// Header file: "SelCand.h"
//
// Contains the declaration of class SelCand.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class SelCand
//
// "Selection Candidate"
//
// Abstract representation of a candidate for selection.
//
// Class Hierarchy:
//
// SelCand
//    MrCand
//    BaCand
//    MeCand
//    SsrCand
//    PrCand
//
// Implemented in SelSplit.C.
//------------------------------------------------------------------------------

class WitSelCand: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSelCand ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void prtID () = 0;
         //
         // Prints info identifying this SelCand.

      virtual void getMrData  (WitRtCand * &    theRtCand);
      virtual void getBaData  (WitPeriod &      expPer);
      virtual void getMeData  (WitPeriod &      execPer);
      virtual void getSsrData (bool &           srAllowedVal);
      virtual void getPoData  (bool &           activeVal,
                               bool &           potActiveVal);
      virtual void getPrData  (WitRtCandStack & theRtCands);
         //
         // Each of these functions sets it arguments to the selection state
         // data for this SelCand for a particular selection case.
         // If this SelCand does not belong to the indicated case, a fatal error
         // is issued.

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSelCand (WitProblem *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSelCand);
   };

#endif
