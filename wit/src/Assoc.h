//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef AssocH
#define AssocH

//------------------------------------------------------------------------------
// Header file: "Assoc.h"
//
// Contains the declaration of class ProbAssoc.
//------------------------------------------------------------------------------

#include <Problem.h>

//------------------------------------------------------------------------------
// Class ProbAssoc
//
// "Problem Associate"
// A ProbAssoc is simply an object that's associated with a particular Problem.
// This class provides a number of convenient wrappers around Problem functions.
//
// Implemented in Misc.C.
//
// Class Hierarchy:
//
// ProbAssoc
//    <Many Derived Classes>
//------------------------------------------------------------------------------

class WitProbAssoc
   {
   public:

      //-----------------------------------------------------------------------
      // Data access functions.
      //-----------------------------------------------------------------------

      accessFunc (WitProblem *, myProblem)

      //------------------------------------------------------------------------
      // Wrappers for class Problem.
      //------------------------------------------------------------------------

      accessWrapper (WitCompMgr *,       myCompMgr,       myProblem_)
      accessWrapper (WitGlobalComp *,    myGlobalComp,    myProblem_)
      accessWrapper (WitOptComp *,       myOptComp,       myProblem_)
      accessWrapper (WitRun *,           myWitRun,        myProblem_)
      accessWrapper (WitApiMgr *,        myApiMgr,        myProblem_)
      accessWrapper (WitMsgFacility &,   myMsgFac,        myProblem_)
      accessWrapper (WitPreprocessor *,  myPreprocessor,  myProblem_)
      accessWrapper (WitPostprocessor *, myPostprocessor, myProblem_)
      accessWrapper (WitDetOptImpMgr *,  myDetOptImpMgr,  myProblem_)
      accessWrapper (WitHeurAllMgr *,    myHeurAllMgr,    myProblem_)
      accessWrapper (WitStochImpMgr *,   myStochImpMgr,   myProblem_)
      accessWrapper (WitOrigMrpExp *,    myOrigMrpExp,    myProblem_)
      accessWrapper (WitFSS *,           myFSS,           myProblem_)
      accessWrapper (WitDataWriter *,    myDataWriter,    myProblem_)
      accessWrapper (FILE *,             solnFile,        myProblem_)
      accessWrapper (int,                nPeriods,        myProblem_)
      accessWrapper (WitPeriod,          lastPeriod,      myProblem_)
      accessWrapper (bool,               wit34Compatible, myProblem_)

      inline void writeSolnValue (double val) const
         {
         myProblem_->writeSolnValue (val);
         }

      //-----------------------------------------------------------------------
      // Other wrappers.
      //-----------------------------------------------------------------------

      FILE * msgFile () const;
         //
         // Wrapper for myMsgFac ().

      bool tempParIsSet (int);
         //
         // Wrapper for myGlobalComp ().

      FILE * openFile (const char * fileName, const char * mode);
         //
         // Wrapper for myProblem_->mySession ().

      //-----------------------------------------------------------------------
      // Other public member functions.
      //-----------------------------------------------------------------------

      bool fssComputed () const;
         //
         // Returns true, iff the FSS has been computed.

      bool selPrintLevelAt (int theLevel);
         //
         // Returns true, iff myGlobalComp ()->selPrintLevel () >= theLevel.

   protected:

      //-----------------------------------------------------------------------
      // Constructor functions.
      //-----------------------------------------------------------------------

      WitProbAssoc (WitProblem *);

      WitProbAssoc (const WitProbAssoc *);

      //-----------------------------------------------------------------------
      // Destructor function.
      //-----------------------------------------------------------------------

      ~WitProbAssoc ();

   private:

      //-----------------------------------------------------------------------
      // Private member functions.
      //-----------------------------------------------------------------------

      noCopyCtorAssign (WitProbAssoc);

      //-----------------------------------------------------------------------
      // Private member data.
      //-----------------------------------------------------------------------

      WitProblem * myProblem_;
         //
         // The Problem with which this ProbAssoc is associated.
   };

#endif
