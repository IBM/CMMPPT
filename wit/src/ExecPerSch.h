//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef ExecPerSchH
#define ExecPerSchH

//------------------------------------------------------------------------------
// Header file: "ExecPerSch.h"
//
// Contains the declaration of class template ExecPerSched <Entry>.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <Schedule.h>

//------------------------------------------------------------------------------
// Class template ExecPerSched <Entry>
//
// "Execution Period Schedule"
// Entry is either BopEntry for BillEntry.
// For each Entry and each period, responsible for indicating the range of
// execution periods for which the Entry has an impactPeriod matching the
// period.
//
// Implemented in Misc.C.
//------------------------------------------------------------------------------

template <typename Entry>
      class WitExecPerSched: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitExecPerSched (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitExecPerSched ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void getExecPerRange (
            Entry *     theEntry,
            WitPeriod   impactPer,
            WitPeriod & execPerFirst,
            WitPeriod & execPerLast);
         //
         // Sets execPerFirst and execPerLast to be the first and last execution
         // periods, execPer, for which
         // theEntry->impactPeriod ()[execPer] == impactPer.
         //
         // If there are no such execution periods, sets:
         //    firstExecPer = -1
         //    lastExecPer  = -2

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitExecPerSched);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSchedule <Entry, WitPeriod> firstExecPer_;
         //
         // firstExecPer_ (theEntry)[impactPer] is the first period execPer
         // such that theEntry->impactPeriod ()[execPer] == impactPer.
         // or -1 if there is none.

      WitSchedule <Entry, WitPeriod> lastExecPer_;
         //
         // lastExecPer_ (theEntry)[impactPer] is the last period execPer
         // such that theEntry->impactPeriod ()[execPer] == impactPer.
         // or -2 if there is none.
   };

#endif
