//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef PipPggH
#define PipPggH

//------------------------------------------------------------------------------
// Header file: "PipPgg.h"
//
// Contains the declaration of class PipPgg.
//------------------------------------------------------------------------------

#include <Assoc.h>
#include <List.h>

//------------------------------------------------------------------------------
// Class PipPgg
//
// "PIP Pegging"
// Represents the pegging for the Post-Implosion Pegging Subsystem.
//
// Implemented in PipPgg.C.
//------------------------------------------------------------------------------

class WitPipPgg: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPipPgg (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPipPgg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void consolidatePeggings ();
         //
         // Consolidates each of the attribute peggings.

      void print ();
         //
         // Prints the pegging.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPipAttPgg <WitOperation> *, execVolPgg)
      accessFunc (WitPipAttPgg <WitBopEntry>  *, coExecVolPgg)
      accessFunc (WitPipAttPgg <WitSubEntry>  *, subVolPgg)
      accessFunc (WitPipAttPgg <WitPart>      *, supplyVolPgg)
      accessFunc (WitPipAttPgg <WitPart>      *, prodVolPgg)
      accessFunc (WitPipAttPgg <WitPart>      *, consVolPgg)
      accessFunc (WitPipAttPgg <WitPart>      *, sideVolPgg)

      accessFunc (WitPipBuilder *,               myPipBuilder)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void countTrips ();
         //
         // Counts and displays the total # of pegging triples in this pegging.

      noCopyCtorAssign (WitPipPgg);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPipAttPgg <WitOperation> * execVolPgg_;
      WitPipAttPgg <WitBopEntry>  * coExecVolPgg_;
      WitPipAttPgg <WitSubEntry>  * subVolPgg_;
      WitPipAttPgg <WitPart>      * supplyVolPgg_;
      WitPipAttPgg <WitPart>      * prodVolPgg_;
      WitPipAttPgg <WitPart>      * consVolPgg_;
      WitPipAttPgg <WitPart>      * sideVolPgg_;
         //
         // The PipAttPggs owned by this PipPgg for the following attributes:
         //    execVol
         //    coExecVol
         //    subVol
         //    supplyVol
         //    prodVol
         //    consVol
         //    sideVol

      WitPipBuilder * myPipBuilder_;
         //
         // While the pegging is being built, this is the PipBuilder that's
         // building it.
         // Otherwise NULL.
   };

#endif
