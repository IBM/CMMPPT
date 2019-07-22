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

#ifndef MrpExpH
#define MrpExpH

//------------------------------------------------------------------------------
// Header file: "MrpExp.h"
//
// Contains the declaration of class MrpExp.
//------------------------------------------------------------------------------

#include <Schedule.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// class MrpExp.
//
// Responsible for performing MRP explosion.
//
// This class participates in an instance of the "template method" design
// pattern. Class MrpExp is the AbstractCLass, classes OrigMrpExp and
// FssMrpExp are the ConcreteClasses and mrp is the TemplateMethod. The
// basic MRP explosion algorithm is the same in both cases and is defined in
// class MrpExp. What varies is means by which the MRP problem is defined
// and the solution is stored and these aspects are defined in the
// ConcreteClasses
//
// Class Hierarchy:
//
// ProbAssoc
//    MrpExp.
//       OrigMrpExp.
//       FssMrpExp.
//------------------------------------------------------------------------------

class WitMrpExp: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMrpExp (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMrpExp ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void mrp ();
         //
         // Performs MRP.

    protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc  (bool,                                  netSubsMode)
      accessFunc2 (const WitSchedule <WitPart, double> &, netSupplyVol)
      accessFunc2 (const WitSchedule <WitPart, double> &, myReqVol)

      //------------------------------------------------------------------------

      virtual void initDerived ();
         //
         // Initializes the derived class aspects of mrp.
         // Default: no-op.

      virtual double supplyVol (WitPart * thePart, WitPeriod thePer) = 0;
         //
         // Returns the client's supplyVol for thePart in period t for Mrp
         // purposes.

      virtual double demandVol (WitPart * thePart, WitPeriod thePer) = 0;
         //
         // Returns the client's demandVol for thePart in period t for Mrp
         // purposes.

      virtual double & execVol (WitOperation * theOpn, WitPeriod thePer) = 0;
         //
         // Returns the client's execVol vector for MRP purposes for theOpn in
         // thePer.

      virtual double & subVol (WitSubEntry * theSub, WitPeriod thePer) = 0;
         //
         // Returns the client's subVol vector for MRP purposes for theSub in
         // thePer.

      virtual void finishDerived () = 0;
         //
         // Concludes the derived aspects of mrp.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMrpExp);

      void init ();
         //
         // Initializes mrp.

      void explodeAllParts ();
         //
         // Explodes the requirements through all the Parts in downward order.

      bool explode (WitPart * thePart, WitPeriod fillPer);
         //
         // Explodes requirements for thePart that are in fillPer.
         // May choose an earlier period for the explosion.

      void explodeBomEntry (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double        qty);
         //
         // Explodes qty execVol units thru theBomEnt in execution Period
         // execPer.

      void netConsEntries (
            WitBomEntry * theBomEnt,
            WitPeriod     execPer,
            double &      qty);
         //
         // Nets theBomEnt and its SubEntries in execution period exevPer
         // prior to explosion, if appropriate.
         // Updates qty, the execution quantity.

      void netConsEntry (
            WitConsEntry * theConsEnt,
            WitPeriod      execPer,
            double &       qty);
         //
         // Nets theConsEnt in execution period execPer, if appropriate.
         // Updates qty, the execution quantity.

      void explodeBopEntry (
            WitBopEntry * theBopEnt,
            WitPeriod     execPer,
            double        qty);
         //
         // Explodes qty execVol units upward thru theBopEnt in execution
         // Period execPer.

      bool & hasSubsToNet (WitBomEntry * theBomEnt);
         //
         // hasSubsToNet (theBomEnt) is true, iff theBomEnt has at least
         // one SubEntry with mrpNetAllowed () true.

      void finish ();
         //
         // Concludes mrp.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSchedule <WitPart, double> netSupplyVol_;
         //
         // Net supply schedule.

      WitSchedule <WitPart, double> myReqVol_;
         //
         // MRP requirement schedule.

      bool netSubsMode_;
         //
         // true, iff theSub->mrpNetAllowed () is true for at least one
         // SubEntry.

      WitVector <bool> hasSubsToNetVec_;
         //
         // Data for hasSubsToNet ().
   };

#endif
