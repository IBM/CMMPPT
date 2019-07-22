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

#ifndef GlobalCompH
#define GlobalCompH

//------------------------------------------------------------------------------
// Header file: "GlobalComp.h"
//
// Contains the declaration of class GlobalComp.
//------------------------------------------------------------------------------

#include <Component.h>
#include <Str.h>
#include <PairStack.h>

//------------------------------------------------------------------------------
// class GlobalComp
//
// "Global Component"
// Responsible for the global attributes of a Problem,
// except those specific to optimizing implosion.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       GlobalComp
//------------------------------------------------------------------------------

class WitGlobalComp: public WitComponent
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitGlobalComp (WitCompMgr * theCompMgr);
         //
         // Component <<< theCompMgr.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitGlobalComp ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &,          title)
      accessFunc (int,                        nPeriods)
      accessFunc (bool,                       independentOffsets)
      accessFunc (bool,                       execEmptyBom)
      accessFunc (int,                        hashTableSize)
      accessFunc (bool,                       roundReqVols)
      accessFunc (int,                        outputPrecision)
      accessFunc (bool,                       computeCriticalList)
      accessFunc (bool,                       pgdCritListMode)
      accessFunc (bool,                       wit34Compatible)
      accessFunc (bool,                       wit34Allowed)
      accessFunc (bool,                       useFocusHorizons)
      accessFunc (bool,                       localBuildAhead)
      accessFunc (bool,                       multiExec)
      accessFunc (bool,                       twoWayMultiExec)
      accessFunc (bool,                       localMultiExec)
      accessFunc (int,                        equitability)
      accessFunc (bool,                       forcedMultiEq)
      accessFunc (bool,                       autoPriority)
      accessFunc (bool,                       skipFailures)
      accessFunc (bool,                       multiRoute)
      accessFunc (bool,                       truncOffsets)
      accessFunc (double,                     lotSizeTol)
      accessFunc (double,                     expCutoff)
      accessFunc (bool,                       penExec)
      accessFunc (bool,                       tieBreakPropRt)
      accessFunc (bool,                       stockRealloc)
      accessFunc (bool,                       stockReallocation)
      accessFunc (bool,                       srSelSplit)
      accessFunc (bool,                       respectStockSLBs)
      accessFunc (bool,                       prefHighStockSLBs)
      accessFunc (bool,                       perfPegging) 
      accessFunc (bool,                       selSplit)
      accessFunc (bool,                       nonMrSelSplit)
      accessFunc (bool,                       selectionRecovery)
      accessFunc (bool,                       userHeurStart)
      accessFunc (bool,                       modHeurAlloc)
      accessFunc (bool,                       pipSeqFromHeur)
      accessFunc (bool,                       newPipPggOrder) 
      accessFunc (bool,                       highPrecisionWD)
      accessFunc (bool,                       nstnResidual)
      accessFunc (bool,                       minimalExcess)
      accessFunc (bool,                       readDataMsgs)
      accessFunc (bool,                       printIncAlloc)
      accessFunc (bool,                       printEqAlloc)
      accessFunc (int,                        selPrintLevel)
      accessFunc (bool,                       twmeByDemand)
      accessFunc (double,                     splitTol)
      accessFunc (double,                     splitRes)
      accessFunc (int,                        splitItrUB)
      accessFunc (bool,                       inputPauses)
      accessFunc (bool,                       prtPipMaxRatio)
      accessFunc (bool,                       printDebug)
      accessFunc (bool,                       printExcessVol)
      accessFunc (bool,                       lotSizesExist)
      accessFunc (const WitPartPerStack &,    revCritList)
      accessFunc (const WitList <WitPclEl> &, pgdCritList)
      accessFunc (bool,                       feasible)

      inline bool oldSR ()
         {
         return stockRealloc_;
         }

      const WitString & tempPar (int index) const;
         //
         // Returns the indexed temp par.

      int nTempPars () const;
         //
         // # temporary parameters.

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (const char * defTitle,               "Untitled")
      defaultValue (int          defNPeriods,            26)
      defaultValue (bool         defIndependentOffsets,  false)
      defaultValue (bool         defExecEmptyBom,        true)
      defaultValue (int          defHashTableSize,       2000)
      defaultValue (bool         defRoundReqVols,        false)
      defaultValue (int          defOutputPrecision,     3)
      defaultValue (bool         defComputeCriticalList, false)
      defaultValue (bool         defPgdCritListMode,     false)
      defaultValue (bool         defWit34Compatible,     false)
      defaultValue (bool         defWit34Allowed,        true)
      defaultValue (bool         defUseFocusHorizons,    true)
      defaultValue (bool         defLocalBuildAhead,     false)
      defaultValue (bool         defMultiExec,           false)
      defaultValue (bool         defTwoWayMultiExec,     false)
      defaultValue (bool         defLocalMultiExec,      false)
      defaultValue (int          defEquitability,        1)
      defaultValue (bool         defForcedMultiEq,       false)
      defaultValue (bool         defAutoPriority,        false)
      defaultValue (bool         defSkipFailures,        true)
      defaultValue (bool         defMultiRoute,          false)
      defaultValue (bool         defTruncOffsets,        false)
      defaultValue (double       defLotSizeTol,          1.0E-5)
      defaultValue (double       defExpCutoff,           0.01)
      defaultValue (bool         defPenExec,             false)
      defaultValue (bool         defTieBreakPropRt,      false)
      defaultValue (bool         defStockRealloc,        false)
      defaultValue (bool         defStockReallocation,   false)
      defaultValue (bool         defSrSelSplit,          false)
      defaultValue (bool         defRespectStockSLBs,    false)
      defaultValue (bool         defPrefHighStockSLBs,   false)
      defaultValue (bool         defPerfPegging,         false)
      defaultValue (bool         defSelSplit,            false)
      defaultValue (bool         defNonMrSelSplit,       false)
      defaultValue (bool         defSelectionRecovery,   false)
      defaultValue (bool         defUserHeurStart,       false)
      defaultValue (bool         defModHeurAlloc,        false)
      defaultValue (bool         defPipSeqFromHeur,      false)
      defaultValue (bool         defNewPipPggOrder,      true)
      defaultValue (bool         defHighPrecisionWD,     false)
      defaultValue (bool         defNstnResidual,        false)
      defaultValue (bool         defMinimalExcess,       false)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setTitle               (const char *);
      void setNPeriods            (int);
      void setIndependentOffsets  (bool);
      void setExecEmptyBom        (bool);
      void setHashTableSize       (int);
      void setRoundReqVols        (bool);
      void setOutputPrecision     (int);
      void setComputeCriticalList (bool);
      void setPgdCritListMode     (bool);
      void setWit34Compatible     (bool);
      void setWit34Allowed        (bool);
      void setUseFocusHorizons    (bool);
      void setLocalBuildAhead     (bool);
      void setMultiExec           (bool);
      void setTwoWayMultiExec     (bool);
      void setLocalMultiExec      (bool);
      void setEquitability        (int);
      void setForcedMultiEq       (bool);
      void setAutoPriority        (bool);
      void setSkipFailures        (bool);
      void setMultiRoute          (bool);
      void setTruncOffsets        (bool);
      void setLotSizeTol          (double);
      void setExpCutoff           (double);
      void setPenExec             (bool);
      void setTieBreakPropRt      (bool);
      void setStockRealloc        (bool);
      void setStockReallocation   (bool);
      void setSrSelSplit          (bool);
      void setRespectStockSLBs    (bool);
      void setPrefHighStockSLBs   (bool);
      void setPerfPegging         (bool);
      void setSelSplit            (bool);
      void setNonMrSelSplit       (bool);
      void setSelectionRecovery   (bool);
      void setUserHeurStart       (bool);
      void setModHeurAlloc        (bool);
      void setPipSeqFromHeur      (bool);
      void setNewPipPggOrder      (bool);
      void setHighPrecisionWD     (bool);
      void setNstnResidual        (bool);
      void setMinimalExcess       (bool);

      //------------------------------------------------------------------------
      // Set functions for development control parameters.
      //------------------------------------------------------------------------

      void setReadDataMsgs   (bool);
      void setPrintIncAlloc  (bool);
      void setPrintEqAlloc   (bool);
      void setSelPrintLevel  (int);
      void setTwmeByDemand   (bool);
      void setSplitTol       (double);
      void setSplitRes       (double);
      void setSplitItrUB     (int);
      void setInputPauses    (bool);
      void setPrtPipMaxRatio (bool);
      void setPrintDebug     (bool);
      void setPrintExcessVol (bool);

      void setTempPar (int index, const WitString & theValue);
         //
         // Sets tempPar_ (t) to theValue.

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePreLotSizesExist (bool);
         //
         // Valid only when preprocessing is being performed.

      void storePostFeasible (bool);
         //
         // Valid only when postprocessing is being performed.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool inserted       ();
      virtual void copySelfInto   (WitCompMgr * theCompMgr);
      virtual void recount        ();
      virtual void unpreprocess   ();
      virtual void clearOptSoln   ();
      virtual void clearStochSoln ();
         //
         // Overrides from class Component.

      void display ();
         //
         // Displays the input and preprocessing data for this GlobalComp.

      bool multiExecNeeded ();
         //
         // Returns true, iff it is appropriate to use the multiple execution
         // periods technique.

      void unpostprocess ();
         //
         // Does unpostprocessing for this GlobalComp.

      void clearRevCritListForHeur ();
         //
         // Clears revCritList_.
         // Valid only when heuristic allocation is updating the solution.

      void addToRevCritListForHeur (WitPart * thePart, WitPeriod thePer);
         //
         // Adds (thePart, thePer) to revCritList_.
         // Valid only when heuristic allocation is updating the solution.

      void receiveOptRevCritList (WitPartPerStack & thePartPerStack);
         //
         // Moves the (Part, Pers) pairs from thePartPerStack into revCritList_,
         // which must be empty.
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      void writePdgCritList ();
         //
         // Writes the pegged critical list to the solution file.

      void clearPgdCritList ();
         //
         // Clears pgdCritList_.

      void appendToPgdCritListForHeur (WitPclEl * thePclEl);
         //
         // Appends thePclEl to pgdCritList_.
         // Valid only when heuristic allocation is updating the solution.

      bool tempParIsSet (int index);
         //
         // Returns true, if tempPar #index is set (to a non-empty string).

      bool tempParAsBool (int index);
         //
         // Returns tempPar #index converted to a bool.
         // The value must be "yes" to "no".

      int tempParAsInt (int index);
         //
         // Returns tempPar #index converted to an int.

      double tempParAsDouble (int index);
         //
         // Returns tempPar #index converted to a double.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitGlobalComp);

      void setNTempPars (int theValue);
         //
         // Sets # of tempPars to theValue, setting their values to the empty
         // string.

      void checkTempParIndex (int i) const;
         //
         // Checks that i is a valid tempPar index.

      virtual void writeDataID ();
         //
         // Override from class Component.

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      void copyAttrsFrom (WitGlobalComp * theGlobalComp);
         //
         // Copies the input attributes of theGlobalComp into this GlobalComp.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Private input data.
      //------------------------------------------------------------------------

      WitString title_;
         //
         // Problem title.

      int nPeriods_;
         //
         // Total number of time periods.

      bool independentOffsets_;
         //
         // If false, the offset of a subEntry cannot be set, and is
         // automatically identical to the offset of the corresponding bomEntry.
         // If true, the offset of a subEntry can be set independently of the
         // offset of the offset of the corresponding bomEntry.

      bool execEmptyBom_;
         //
         // If false, then any Operation will be prevented from executing in
         // any period in which the either of the following conditions hold:
         //
         //    * Its BOM contains to entries, or
         //    * None of its BomEntries is in effect in the period.
         //
         // If true, Operations will be allowed to execute under the above
         // condtions.
         // Defaults to true.

      int hashTableSize_;
         //
         // No longer used.

      bool roundReqVols_;
         //
         // If true, reqVols and focShortageVols will be rounded up to integer
         // values.

      int outputPrecision_;
         //
         // The number of digits to the right of the decimal point in the
         // production schedule output file and shipment schedule output file.
         // Must be >= 0.
         // Defaults to 3.

      bool computeCriticalList_;
         //
         // true, iff the critical part list is to be computed.

      bool pgdCritListMode_;
         //
         // true, iff the heuristic is to compute the pegged critical list.

      bool wit34Compatible_;
         //
         // true iff myProblem () is in WIT 3.4 compatible mode.

      bool wit34Allowed_;
         //
         // wit34Compatible is allowed to be set through the API, iff
         // wit34Allowed_ is true.

      bool useFocusHorizons_;
         //
         // If true, then before the shortage schedule is computed, the
         // fssShipVol for each Demand will be computed using the Demand's
         // focus horizon. If false, the fssShipVols will be left at their
         // current values.

      bool localBuildAhead_;
         //
         // true, iff the local build-ahead periods technique as been
         // requested.
         // This is no longer an officially documented attribute and is only
         // included for upward compatbility.

      bool multiExec_;
         //
         // true, iff the multiple execution periods technique as been
         // explicitly requested.

      bool twoWayMultiExec_;
         //
         // true, iff the multiple execution periods technique is to be used.
         // The set functions assure that whenever twoWayMultiExec_ is true, 
         // multiExec_ also true.

      bool localMultiExec_;
         //
         // true, iff the local multiple execution periods technique as been
         // requested.
         // This is no longer an officially documented attribute and is only
         // included for upward compatbility.

      int equitability_;
         //
         // Must be >= 1.
         // The precision to which equitable allocation is to be performed.
         // 1 means no equitable allocation.

      bool forcedMultiEq_;
         //
         // If true, equitable allocation will use multiple passes,
         // even if there is no tie for priority or there is only one
         // target with positive desIncVol.

      bool autoPriority_;
         //
         // If true, heuristic implosion will generate priorities automatically
         // from the objective function.

      bool skipFailures_;
         //
         // If true, heuristic allocation will not attempt to allocate to a
         // Part in a period if it previously tried and failed to allocate to
         // the Part in the period (with some exceptions).

      bool multiRoute_;
         //
         // If true, multiple routes logic will be used by heuristic allocation.

      bool truncOffsets_;
         //
         // If true, preprocessing will compute impact periods using truncated
         // offsets, i.e., whenever offset[t] > t, it will be treated as if
         // offset[t] == t.

      double lotSizeTol_;
         //
         // Rounding lot-size tolerance, relative to the incremental lot-size.
         // Range: [0.0, +inf).

      double expCutoff_;
         //
         // Smallest acceptable effProdRate for explosion.
         // Range: [1.0E-6, +inf).

      bool penExec_;
         //
         // Heuristic implosion/allocation will use penalized execution iff
         // penExec and multiRoute are both true.

      bool tieBreakPropRt_;
         //
         // If true, penalized execution overrides proportionate routing, so
         // proportionate routing functions only as a tie breaker for
         // penalized execution.
         // If false, proportionate routing overrides penalized execution.

      bool stockRealloc_;
         //
         // true, iff heuristic implosion/allocation is to perform pure
         // unconditional stock reallocation.

      bool stockReallocation_;
         //
         // true, iff heuristic implosion/allocation is to perform selective
         // stock reallocation.

      bool srSelSplit_;
         //
         // true, iff heuristic implosion/allocation is to perform selection
         // splitting for stock reallocation.

      bool respectStockSLBs_;
         //
         // true, iff heuristic allocation is to respect stock soft lower 
         // bounds.

      bool prefHighStockSLBs_;
         //
         // true, iff heuristic allocation is to give preference to stock soft
         // lower bounds on parts higher in the BOM structure over parts lower 
         // in the BOM structure.

      bool perfPegging_;
         //
         // true, iff concurrent pegging is to be performed by the heuristic.

      bool selSplit_;
         //
         // true, iff heuristic implosion/allocation is to perform all cases of
         // selection splitting.

      bool nonMrSelSplit_;
         //
         // true, iff heuristic implosion/allocation is to perform all cases of
         // selection splitting, except multi-route.

      bool selectionRecovery_;
         //
         // If true, selection re-use logic will be used by heuristic
         // allocation.

      bool userHeurStart_;
         //
         // true, iff user-specified heuristic starting solution is to be used.

      bool modHeurAlloc_;
         //
         // true, iff modifiable heuristic allocation is to be used.

      bool pipSeqFromHeur_;
         //
         // true, iff the PIP shipment sequence is to be set by the heuristic.

      bool newPipPggOrder_;
         //
         // true, iff PIP is to use the new pegging order.
         // Undocumented attribute.

      bool highPrecisionWD_;
         //
         // true, iff writeData is to use high precision when writing double
         // data.

      bool nstnResidual_;
         //
         // If true,  residualVol is to be computed on a
         //           "no sooner than necessary" basis.
         // If false, residualVol is to be computed on an
         //           "as soon as possible" basis.

      bool minimalExcess_;
         //
         // If true,  excessVol is to be minimized, given residualVol.
         // If false, excessVol is to be maximized, given residualVol.

      //------------------------------------------------------------------------
      // Development control parameters.
      //------------------------------------------------------------------------

      bool readDataMsgs_;
         //
         // If true, API informational msgs will be issued during readData.

      bool printIncAlloc_;
         //
         // If true, the heuristic allocation data is to be displayed.

      bool printEqAlloc_;
         //
         // If true, the equitable allocation Targets is to be displayed.

      int selPrintLevel_;
         //
         // Printing will be done for the multiple selections heuristic:
         //
         // 0: No printing
         // 1: Counts
         // 2: Alterations
         // 3: Everything

      bool twmeByDemand_;
         //
         // If true, then in two-way multi-exec mode, the multi-exec direction
         // for a demand will be ASAP, iff selForSel is true.

      double splitTol_;
         //
         // Tolerance for selection splitting.

      double splitRes_;
         //
         // Resolution for selection splitting.

      int splitItrUB_;
         //
         // An upper bound on the number of iterations that will be used in
         // selection splitting.

      bool inputPauses_;
         //
         // If true, WIT will pause at certain times and then wait for user
         // input before continuing.
         // Used for measuring memory utilization.

      bool prtPipMaxRatio_;
         //
         // If true, print the PIP max pegging ratio data.

      bool printDebug_;
         //
         // If true, do various debug printing.

      bool printExcessVol_;
         //
         // If true, excessVol will be printed instead of fssShortageVol in the
         // comprehensive implosion solution output.

      WitPtrVec <WitString> & tempPar_;
         //
         // A vector of temporary development control parameters.
         // Set by ParamMgr.
         // These parameters are only to be used in a local copy of WIT, i.e.,
         // code that depends on their values should never be delivered to the
         // RTC stream. This allows all WIT developers to use them at will,
         // without co-ordination.
         // tempPar_ (0) is not used.

      //------------------------------------------------------------------------
      // Data set by preprocessing.
      //------------------------------------------------------------------------

      bool lotSizesExist_;
         //
         // True, iff any minLotSize or incLotSize has a positive value.

      //------------------------------------------------------------------------
      // Data set by implosion.
      //------------------------------------------------------------------------

      WitPartPerStack revCritList_;
         //
         // The critical part list.
         // Stored in reverse order.

      //------------------------------------------------------------------------
      // Data set by heuristic allocation.
      //------------------------------------------------------------------------

      WitList <WitPclEl> pgdCritList_;
         //
         // The pegged critical list.

      //------------------------------------------------------------------------
      // Data set by postprocessing.
      //------------------------------------------------------------------------

      bool feasible_;
         //
         // true, iff postprocessed () is true and the current implosion
         // solution is feasible respect to all constraints relevant to the
         // method by which the solution was computed.
   };

#endif
