//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef ExpRestH
#define ExpRestH

//------------------------------------------------------------------------------
// Header file: "ExpRest.h"
//
// Contains the declaration of class ExpRest.
//------------------------------------------------------------------------------

#include <Part.h>
#include <GlobalComp.h>
#include <Schedule.h>
#include <Mapping.h>
#include <PairStack.h>

//------------------------------------------------------------------------------
// Class ExpRest
//
// "Explosion Restrictor"
// Responsible for the explosion restrictions of penalized execution.
//
// Implemented in PenExec.C.
//------------------------------------------------------------------------------

class WitExpRest: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitExpRest (WitHeurAtor *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitExpRest ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void placeReqs (
            WitPart * srcPart,
            WitPeriod srcPer,
            WitPart * reqPart,
            WitPeriod reqPer);
         //
         // To be called when requirements are being placed on reqPart in
         // reqPer, whose source is srcPart in srcPer.

      void recExpBeyondNet (
            WitPart * thePart, 
            WitPeriod thePer, 
            bool      netForConsEnt);
         //
         // Records the fact that restricted explosion needs to continue after
         // netting supply of thePart in fillPer. netForConsEnt should be true,
         // iff the netting was ConsEntry netting.

      void postExpBomEnt ();
         //
         // To be called just after HeurAtor::explodeBomEntry.

      bool restExpAllowed (WitBopEntry * theBopEnt, WitPeriod fillPer);
         //
         // Returns true, iff restricted explosion thru theBopEnt to fill reqs
         // in fillPer is currently allowed.

      inline bool hasPseudoSup (WitPart * thePart, WitPeriod thePer)
         {
         WitTVec <bool> & theVec = hasPseudoSup_ (thePart);

         return
            theVec.isAllocated ()?
               theVec[thePer]:
               false;
         }
         //
         // Returns true, iff thePart is considered to have pseudo-supply in 
         // thePer.

      void recordFundShortage ();
         //
         // To be called whenever a fundamental shortage occurs.

      void reqPsSupRep (WitPart * thePart, WitPeriod thePer);
         //
         // Requests replenishment of pseudo-supply of thePart in thePer.

      void postTempCommit (bool success);
         //
         // To be called at the end of each temp commit, with success indicating
         // its result.

      void updatePsSup ();
         //
         // Makes updates to pseudo-supply that were requested in the most
         // recent failing temp-commit.

      void startTempMode ();
         //
         // Puts this ExpRest into temp mode.

      void finishTempMode ();
         //
         // Takes this ExpRest out of temp mode.

      void replenishPsSups ();
         //
         // Replenishes pseudo-supplies where this was requested.

      void print ();
         //
         // Prints this ExpRest.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, failRestShortOnly)
      accessFunc (bool, tempMode)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitExpRest);

      void findPosPenalties ();
         //
         // Sets hasPosPenalty_.

      void init (WitPart * thePart);
         //
         // Initializes the pseudo-supply of thePart.

      void exhaustOtherPers (WitMaterial * theMat, WitPeriod fillPer);
         //
         // Sets hasPseudoSup to false, in the appropriate other periods, 
         // given that it's being set to false in fillPer.

      void exhaustPsSup (WitPart * thePart, WitPeriod thePer);
         //
         // Sets hasPseudoSup_ (thePart, thePer) to false.
         // In temp mode, this is done on a temp basis.

      void setUnanReqs (WitPart * thePart, WitPeriod thePer);
         //
         // Sets unanReqs_ (thePart, thePer) to true.

      void printExhRep (
            WitPart * thePart, 
            WitPeriod thePer, 
            bool      firstPer);
         //
         // Prints indicating that the pseudo-supply of thePart in thePer is
         // being exhausted or replenished. firstPer is to be true, iff thePer
         // is the first period considered by the state change.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitHeurAtor * const myHeurAtor_;
         //
         // The HeurAtor to which this ExpRest applies.

      WitMapping <WitOperation, bool> hasPosPenalty_;
         //
         // hasPosPenalty_ (theOpn) is true, iff theOperation or any of its
         // ConsEntries has a positive execPenalty.

      WitSchedule <WitPart, bool> hasPseudoSup_;
         //
         // Data for hasPseudoSup (*, *).

      WitPartPerStack curExhaustReqs_;
         //
         // The set of all (Part, fill period)s for which pseudo-supply 
         // exhaustion has been requested and not yet performed, for the current
         // temp commit.

      WitPartPerStack failExhaustReqs_;
         //
         // Same as curExhaustReqs_, but for the most recent failing temp commit

      bool curFundShort_;
         //
         // true, iff a fundamental shortage was found in the current temp 
         // commit.

      bool failRestShortOnly_;
         //
         // true, iff a restriction shortage was found in the most recent 
         // failing temp commit, but no fundamental shortage.

      WitPartPerStack repReqs_;
         //
         // The set of all (Part, fill period)s for which pseudo-supply 
         // replenishment has been requested and not yet performed.

      WitSchedule <WitPart, bool> unanReqs_;
         //
         // unanReqs_ (thePart, thePer) is true, iff some of the reqs for
         // thePart in thePer came through unanalyzed routings.

      WitPartPerStack unanReqsPairs_;
         //
         // The set of pairs (thePart, thePer) such that
         // unanReqs_ (thePart, thePer) is true.

      bool unanByConsEntNet_;
         //
         // true, iff the heuristic is currently netting / exploding a BomEntry
         // whose requirements have been made unanalyzed by ConsEntry netting.

      bool tempMode_;
         //
         // true, iff this ExpRest is being updated in temp mode.
         // (This is distinct from HeurAtor's temp mode.)

      WitDataRepos * psSupRepos_;
         //
         // DataRepos for cells of hasPseudoSup_ that have been altered during
         // temp mode.
   };

#endif
