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

#ifndef HeurCritH
#define HeurCritH

//------------------------------------------------------------------------------
// Header file: "HeurCrit.h"
//
// Contains the declaration of class HeurCritList.
//------------------------------------------------------------------------------

#include <GlobalComp.h>
#include <Schedule.h>

//------------------------------------------------------------------------------
// Class HeurCritList
//
// Responsible for the critical part list in heuristic allocation.
//
// Implemented in HeurAll.C.
//------------------------------------------------------------------------------

class WitHeurCritList: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitHeurCritList (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitHeurCritList ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void initSoln ();
         //
         // Resets the critical list.

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

      noCopyCtorAssign (WitHeurCritList);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPart * culpritPart_;
         //
         // Latest culprit found

      WitPeriod culpritPeriod_;
         //
         // Period in which culprit is short

      WitSchedule <WitPart, bool> listed_;
         //
         // thePart->listed_[t] == true iff thePart is in the critical part
         // list for period t.
   };

#endif
