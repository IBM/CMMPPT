//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef StochAssocH
#define StochAssocH

//------------------------------------------------------------------------------
// Header file: "StochAssoc.h"
//
// Contains the declaration of class StochAssoc.
//------------------------------------------------------------------------------

#include <StochModeMgr.h>

//------------------------------------------------------------------------------
// Class StochAssoc
//
// "StochModeMgr Associate"
// This class has an association to a StochModeMgr and various member functions
// that are based on that association.
// It is to be used as a base class for a class that needs to refer to a
// StochModeMgr.
//
// Class hierarchy:
//
//    ProbAssoc
//       StochAssoc
//          StochAttMgr
//          ScenMgr
//          ScenAttMgr
//          AbsScenInputAtt
//             ScenInputAtt <Owner>
//          AbsScenSolnAtt
//             ScenSolnAtt <Owner>
//          ScenAtt <Owner>
//          StochOptMgr
//          ScenGroup
//          StochLoc
//
// Implemented in StochMode.C.
//------------------------------------------------------------------------------

class WitStochAssoc: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      accessWrapper (WitStageMgr *,   myStageMgr,    myStochModeMgr_)
      accessWrapper (WitScenMgr *,    myScenMgr,     myStochModeMgr_)
      accessWrapper (WitScenAttMgr *, myScenAttMgr,  myStochModeMgr_)
      accessWrapper (bool,            stochSolnMode, myStochModeMgr_)

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitStochModeMgr *, myStochModeMgr)

   protected:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStochAssoc (WitStochModeMgr *);
      WitStochAssoc (WitStochAssoc   *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitStochAssoc ();

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitStochAssoc);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitStochModeMgr * const myStochModeMgr_;
         //
         // The StochModeMgr for this StochAssoc.
   };

#endif
