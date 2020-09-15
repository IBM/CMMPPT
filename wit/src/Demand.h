//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef DemandH
#define DemandH

//------------------------------------------------------------------------------
// Header file: "Demand.h"
//
// Contains the declaration of class Demand.
//------------------------------------------------------------------------------

#include <DelComp.h>
#include <Str.h>
#include <FlexVec.h>
#include <ObjVec.h>
#include <PtrTVec.h>

//------------------------------------------------------------------------------
// Class Demand.
//
// "Demand Stream".
// Represents a source of demand for a part.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          Demand
//------------------------------------------------------------------------------

class WitDemand: public WitDelComp
   {
   public:

      //------------------------------------------------------------------------
      // Private constructor functions.
      //------------------------------------------------------------------------

      WitDemand (const char * theName, WitPart * thePart);
         //
         // demandName_   <-- theName
         // demandedPart_ <-- thePart.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitDemand ();

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double    defDemandVol,       0.0)
      defaultValue (int       defPriority,        0)
      defaultValue (double    defSearchInc,       1.0)
      defaultValue (int       defBuildAheadUB,    0)
      defaultValue (bool      defShipLateAllowed, true)
      defaultValue (bool      defPrefBuildAhead,  false)
      defaultValue (bool      defIntShipVols,     false)
      defaultValue (double    defShipReward,      0.0)
      defaultValue (double    defCumShipReward,   0.0)
      defaultValue (double    defShipVol,         0.0)
      defaultValue (WitPeriod defFocusHorizon,    -1)

      static int defLeadTimeUB (WitProblem * theProblem);
      static int defShipLateUB (WitProblem * theProblem);
         //
         // These default value function are implemented explicitly, because the
         // values depend on theProblem.

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setDemandName      (const char *);
      void setDemandVol       (const WitIVRealArg &);
      void setPriority        (const int *);
      void setSearchInc       (double);
      void setBuildAheadUB    (const int *);
      void setLeadTimeUB      (const int *);
      void setShipLateUB      (const int *);
      void setShipLateAllowed (bool);
      void setPrefBuildAhead  (bool);
      void setIntShipVols     (bool);
      void setShipReward      (const WitIVRealArg &);
      void setCumShipReward   (const WitIVRealArg &);
      void setShipVol         (const WitIVRealArg &);
      void setFocusHorizon    (int);
      void setFssShipVol      (const WitIVRealArg &);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePostCumShipVol (const WitTVec <double> &);
         //
         // Valid only when postprocessing is being performed.

      void storeOptShipVol    (WitPeriod, double);
      void storeOptCumShipVol (WitPeriod, double);
         //
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      void storeStochDemandVol     (const WitDblFlexVec &);
      void storeStochShipReward    (const WitDblFlexVec &);
      void storeStochCumShipReward (const WitDblFlexVec &);
      void storeStochShipVol       (const WitDblFlexVec &);
      void storeStochCumShipVol    (const WitDblFlexVec &);
         //
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool        inserted          ();
      virtual void        writeMultiObjVecs (WitObjective *);
      virtual void        identifyInstance  ();
      virtual void        writeSetClause    ();
      virtual void        copySelfInto      (WitCompMgr *);
      virtual void        recount           ();
      virtual void        clearOptSoln      ();
      virtual void        clearStochSoln    ();
      virtual WitDemand * thisDemand        ();
         //
         // Overrides from class Component.

      virtual WitMsgFrag objTypeMsgFrag ();
      virtual WitAttr    apiObjItrState ();
         //
         // Overrides from class DelComp.

      void copyAttrsFrom (WitDemand * theDemand);
         //
         // Copies the input attributes of theDemand into this Demand.

      static const WitString & demandName (const WitDemand * theDemand);
         //
         // Returns theDemand->demandName ().

      void setUpPegging ();
         //
         // Takes action appropriate for when myGlobalComp ()->perfPegging_
         // has just been set to true.

      void shutDownPegging ();
         //
         // Takes action appropriate for when myGlobalComp ()->perfPegging_
         // has just been set to false.

      void extractPegEls (
            WitPeriod                shipPer,
            WitObjStack <WitPegEl> & thePegElStack);
         //
         // Extracts the PegEls for shipPer owned by this Demand and puts them
         // into thePegElStack, which must be empty.
         // Valid only when heuristic allocation is updating the solution.

      void receivePegEls (
            WitPeriod                shipPer,
            WitObjStack <WitPegEl> & thePegElStack);
         //
         // Moves the PegEls from thePegElStack into the list of PegEls owned by
         // this Demand for shipPer, which must be empty.
         // Valid only when heuristic allocation is updating the solution.

      void getMyFixedPegEls (
            WitPeriod                     shipPer,
            WitObjStack <WitFixedPegEl> & theFixedPegEls);
         //
         // On return, theFixedPegEls is a list of FixedPegEls for shipPer owned
         // by this Demand.

      void clearPegging (WitPeriod shipPer);
         //
         // Clears the pegging for this Demand in shipPer.

      double netRev () const;
         //
         // Returns the net revenue associated with shipping a unit of this
         // Demand.

      virtual void writeSolnData ();
         //
         // Override from class Component.
         // Writes the section of the solution data file for this Demand.

      void display ();
         //
         // Displays the input and preprocessing data for this Demand.

      static void genPriorities (WitProblem * theProblem);
         //
         // Automatically generates priorities from objective data.

      void clearShipVolForHeur ();
         //
         // Clears shipVol_.
         // Valid only when heuristic allocation is updating the solution.

      void incShipVolForHeur (WitPeriod thePer, double incVol);
         //
         // Increments shipVol_[thePer] by incVol.
         // Valid only when heuristic allocation is updating the solution.

      void initFssShipVolForPost ();
         //
         // Sets fssShipVol_ to its initial value.
         // Valid only when postprocessing is being performed.

      void compFssShipVol ();
         //
         // Computes fssShipVol_ in useFocusHorizons mode.

      void writeSoln ();
         //
         // Prints the comprehensive implosion solution for this Demand.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &,        demandName)
      accessFunc (WitPart *,                demandedPart)
      accessFunc (const WitDblFlexVec &,    demandVol)
      accessFunc (const WitFlexVec <int> &, priority)
      accessFunc (double,                   searchInc)
      accessFunc (const WitFlexVec <int> &, buildAheadUB)
      accessFunc (const WitFlexVec <int> &, leadTimeUB)
      accessFunc (const WitFlexVec <int> &, shipLateUB)
      accessFunc (bool,                     shipLateAllowed)
      accessFunc (bool,                     prefBuildAhead)
      accessFunc (bool,                     intShipVols)
      accessFunc (WitBoundSet *,            cumShipBounds)
      accessFunc (const WitDblFlexVec &,    shipVol)
      accessFunc (WitPeriod,                focusHorizon)
      accessFunc (const WitDblFlexVec &,    fssShipVol)
      accessFunc (const WitDblFlexVec &,    cumDemandVol)
      accessFunc (const WitDblFlexVec &,    cumShipVol)

      inline const WitDblFlexVec & shipReward () const
         {
         return shipReward_.curDblFlexVec ();
         }

      inline const WitDblFlexVec & cumShipReward () const
         {
         return cumShipReward_.curDblFlexVec ();
         }

      const WitString & demandedPartName ();

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitDemand)

      static int nInstances (WitProblem *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitDemand);

      void compCumDemandVol ();
         //
         // Computes cumDemandVol_.

      virtual WitBoundSet * myBoundSet     ();
      virtual void          writeDataID    ();
      virtual void          writeDataAttrs ();
         //
         // Overrides from class Component.

      virtual void findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2);
         //
         // Override from class DelComp.

      void buildPegEls ();
         //
         // Builds the PegEls owned by this Demand.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data set by the constructor.
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # Demands constructed before this Demand.

      WitString demandName_;
         //
         // The name of this Demand.
         // The name must be unique to this demand
         // stream among all the demand streams for the same part, but it need
         // not be unique among demand streams for other parts.

      WitPart * const demandedPart_;
         //
         // The Part demanded by this Demand.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      WitDblFlexVec demandVol_;
         //
         // Volume demanded in each period.
         // Range: [0.0, +inf).

      WitFlexVec <int> priority_;
         //
         // Used only by the heuristic; High numbers mean low priority.
         // Range: (-inf, +inf).

      double searchInc_;
         //
         // Increment for searches by the heuristic on this Demand.
         // Range: [0.001, +inf).

      WitFlexVec <int> buildAheadUB_;
         //
         // Used only by the heuristic; The heuristic will consider building
         // the part demanded up to buildAheadUB periods earlier than the
         // period in which the demandVol occurs.
         // Range: [0, nPeriods - 1].
         // Note that nPeriods - 1 implies no restriction.

      WitFlexVec <int> leadTimeUB_;
         //
         // Used only by the heuristic.
         // When the heuristic is attempting to increase the shipvol of this
         // demand in period shipPer, no Part with boundedLeadTimes_ == true may
         // be exploded in any period earlier than
         // shipPer - leadTimeUB_[shipPer].
         // Range: [0, nPeriods).

      WitFlexVec <int> shipLateUB_;
         //
         // For demand in period demPer, the heuristic will consider shipping
         // to this demand up to period demPer + shipLateUB[demPer].
         // Range: [0, nPeriods - 1].
         // Note that nPeriods - 1 implies no restriction.

      bool shipLateAllowed_;
         //
         // If false, then late shipments are not allowed for this Demand.

      bool prefBuildAhead_;
         //
         // Used only by the heuristic. 
         // If true, the heuristic will do preferential build-ahead on the 
         // demanded part, i.e., it will try to build the part as early as
         // possible.

      bool intShipVols_;
         //
         // If true, then in optimizing implosion, shipVol_ will be constrained
         // to take on integer values in all periods.

      WitBoundSet * cumShipBounds_;
         //
         // Pointer to bounds on cumulative shipment variables;

      WitObjVec shipReward_;
         //
         // Stores the shipment reward coefficients of the shipment variables
         // for this Demand.
         // Range: (-inf, +inf).

      WitObjVec cumShipReward_;
         //
         // Stores the reward coefficients on the cumulative shipment variables
         // for this Demand.
         // Range: (-inf, +inf).

      WitDblFlexVec shipVol_;
         //
         // shipVol ()[thePer] is the volume shipped to the demand stream in
         // thePer. Normally, this is set by implosion, but it can also be set
         // as input.
         // Range: [0.0, +inf).

      WitPeriod focusHorizon_;
         //
         // The last period in which the unmet demand volume of this
         // Demand will be considered when computing the shortage schedule.
         // Range: [-1, nPeriods).

      WitDblFlexVec fssShipVol_;
         //
         // fssShipVol[thePer] is the desired volume to be shipped to this
         // Demand in thePer for FSS purposes.
         // Range: [0.0, +inf).

      //------------------------------------------------------------------------
      // Data maintained automatically.
      //------------------------------------------------------------------------

      WitDblFlexVec cumDemandVol_;
         //
         // Cumulative volume demanded by each period.

      //------------------------------------------------------------------------
      // Data set by implosion.
      //------------------------------------------------------------------------

      WitDblFlexVec cumShipVol_;
         //
         // cumShipVol_[thePer] is the cumulative volume of the part shipped to
         // this Demand from periods 0 thru thePer.

      //------------------------------------------------------------------------
      // Data set by heuristic allocation.
      //------------------------------------------------------------------------

      WitPtrTVec <WitObjStack <WitPegEl> > myPegElsFor_;
         //
         // myPegElsFor_ (shipPer) is the list of all PegEl instances for
         // shipPer owned by this Demand.
   };

#endif
