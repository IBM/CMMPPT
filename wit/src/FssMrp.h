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

#ifndef FssMrpH
#define FssMrpH

//------------------------------------------------------------------------------
// Header file: "FssMrp.h"
//
// Contains the declaration of class FssMrpExp.
//------------------------------------------------------------------------------

#include <MrpExp.h>

//------------------------------------------------------------------------------
// class FssMrpExp.
//
// Responsible for performing MRP explosion for FSS.
// Implemented in FSS.C.
//------------------------------------------------------------------------------

class WitFssMrpExp: public WitMrpExp
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitFssMrpExp (
            WitProblem *,
            WitFSS *,
            bool &                               uploadingSolnArg,
            WitSchedule <WitOperation, double> & fssExecVolArg,
            WitSchedule <WitSubEntry,  double> & fssSubVolArg);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitFssMrpExp ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitFssMrpExp);

      virtual double   supplyVol (WitPart      *, WitPeriod);
      virtual double   demandVol (WitPart      *, WitPeriod);
      virtual double & execVol   (WitOperation *, WitPeriod);
      virtual double & subVol    (WitSubEntry  *, WitPeriod);

      virtual void finishDerived ();

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitFSS * const myFSS_;
         //
         // The FSS that uses this FssMrpExp.

      bool & uploadingSoln_;
         //
         // True, iff the FSS subsystem is currently uploading the FSS solution.

      //------------------------------------------------------------------------
      // References to FSS solution Schedules.
      //------------------------------------------------------------------------

      WitSchedule <WitOperation, double> & fssExecVol_;
      WitSchedule <WitSubEntry,  double> & fssSubVol_;
   };

#endif
