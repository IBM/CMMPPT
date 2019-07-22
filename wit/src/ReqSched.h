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

#ifndef ReqSchedH
#define ReqSchedH

//------------------------------------------------------------------------------
// Header file: "ReqSched.h"
//
// Contains the declaration of class ReqSched.
//------------------------------------------------------------------------------

#include <Part.h>
#include <Schedule.h>
#include <Mapping.h>

//------------------------------------------------------------------------------
// Class ReqSched
//
// The schedule of requirements for heuristic allocation.
//
// Implemented in HeurAll.C.
//------------------------------------------------------------------------------

class WitReqSched: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitReqSched (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitReqSched ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void addTo (WitPart * thePart, WitPeriod thePer, double qty);
         //
         // Adds qty to reqVol_ (thePart)[thePer].

      void clear ();
         //
         // Returns this ReqSched to its initial (all zero) state.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      inline double reqVol (WitPart * thePart, WitPeriod thePer)
         {
         return reqVol_ (thePart)[thePer];
         }

      inline WitPeriod firstPer (WitPart * thePart)
         {
         return firstPer_ (thePart);
         }
         
      inline WitPeriod lastPer (WitPart * thePart)
         {
         return lastPer_ (thePart);
         }
         
   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitReqSched);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDataRepos * myDataRepos_;
         //
         // The DataRepos owned by this ReqSched.

      WitSchedule <WitPart, double> reqVol_;
         //
         // The actual requirement schedule.

      WitMapping <WitPart, WitPeriod> firstPer_;
         //
         // firstPer_ (thePart) is the first period, thePer, in which 
         // reqVol_ (thePart)[thePer] > 0.0.

      WitMapping <WitPart, WitPeriod> lastPer_;
         //
         // lastPer_ (thePart) is the last period, thePer, in which 
         // reqVol_ (thePart)[thePer] > 0.0.
   };

#endif
