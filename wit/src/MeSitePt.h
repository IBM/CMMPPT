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

#ifndef MeSitePtH
#define MeSitePtH

//------------------------------------------------------------------------------
// Header file: "MeSitePt.h"
//
// Contains the declaration of class MeSitePt.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class MeSitePt
//
// "Multi-Exec Site Point"
// Responsible for coordinating the multi-exec objects associated with a
// particular BopEntry and explosion period.
//
// Implemented in MultiEx.C.
//------------------------------------------------------------------------------

class WitMeSitePt
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMeSitePt (
            WitBopEntry * theBopEnt,
            WitPeriod     firstExecPerVal,
            WitPeriod     lastExecPerVal,
            WitMeMgr *    theMeMgr);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMeSitePt ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      WitPeriod nextExecPer (WitPeriod execPer, WitMeDir * theMeDir);
         //
         // Returns the next execution period for theMeDir and this MeSitePt
         // after execPer, if any; otherwise -1;

      void prtID ();
         //
         // Prints info identifying this MeSitePt.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc    (WitBopEntry *, myBopEnt)
      accessFunc    (WitPeriod,     firstExecPer)
      accessFunc    (WitPeriod,     lastExecPer)
      accessFunc    (WitMePt *,     myMePtForNstn)
      accessNonNull (WitMePt *,     myMePtForAsap)

      WitPeriod myExpPer ();
         //
         // Returns the explosion period associated with this MeSitePt.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMeSitePt);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitBopEntry * const myBopEnt_;
         //
         // The BopEntry associated with this MeSitePt.

      const WitPeriod firstExecPer_;
      const WitPeriod  lastExecPer_;
         //
         // The first and last execution periods corresponding to the explosion
         // period for this MeSitePt.

      WitMePt * myMePtForNstn_;
         //
         // The MePt owned by this MeSitePt for NSTN Multi-Exec.

      WitMePt * myMePtForAsap_;
         //
         // The MePt owned by this MeSitePt for ASAP Multi-Exec, in two-way
         // multi-exec mode; otherwise NULL.
   };

#endif
