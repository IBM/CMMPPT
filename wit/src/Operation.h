//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef OperationH
#define OperationH

//------------------------------------------------------------------------------
// Header file: "Operation.h"
//
// Contains the declaration of class Operation.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          Node
//             Operation
//------------------------------------------------------------------------------

#include <Node.h>
#include <FlexVec.h>
#include <ObjVec.h>

//------------------------------------------------------------------------------
// Class Operation.
//------------------------------------------------------------------------------

class WitOperation: public WitNode
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOperation (const char * theName, WitCompMgr * theCompMgr);
         //
         // Node <<< theName.
         // Node <<< theCompMgr.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitOperation ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitList <WitBomEntry> &, bom)
      accessFunc (const WitList <WitBopEntry> &, bop)
      accessFunc (const WitDblFlexVec &,         yieldRate)
      accessFunc (const WitDblFlexVec &,         minLotSize)
      accessFunc (const WitDblFlexVec &,         incLotSize)
      accessFunc (bool,                          twoLevelLotSizes)
      accessFunc (const WitDblFlexVec &,         lotSize2Thresh)
      accessFunc (const WitDblFlexVec &,         minLotSize2)
      accessFunc (const WitDblFlexVec &,         incLotSize2)
      accessFunc (double,                        execPenalty)
      accessFunc (bool,                          intExecVols)
      accessFunc (const WitDblFlexVec &,         execVol)
      accessFunc (WitBoundSet *,                 execBounds)
      accessFunc (bool,                          pipEnabled)
      accessFunc (int,                           pipRank)
      accessFunc (const WitTVec <bool> &,        executable)
      accessFunc (const WitDblFlexVec &,         mrpExecVol)
      accessFunc (const WitDblFlexVec &,         fssExecVol)

      inline const WitDblFlexVec & execCost () const
         {
         return execCost_.curDblFlexVec ();
         }

      inline const WitString & operationName () const
         {
         return nodeName ();
         }

      inline const WitDblFlexVec & cycleTime34 () const
         {
         witAssert (wit34Compatible ());

         return cycleTime34_;
         }

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitOperation)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double defYieldRate,        1.0)
      defaultValue (double defMinLotSize,       0.0)
      defaultValue (double defIncLotSize,       0.0)
      defaultValue (bool   defTwoLevelLotSizes, false)
      defaultValue (double defLotSize2Thresh,   0.0)
      defaultValue (double defMinLotSize2,      0.0)
      defaultValue (double defIncLotSize2,      0.0)
      defaultValue (double defExecPenalty,      0.0)
      defaultValue (double defExecCost,         0.0)
      defaultValue (bool   defIntExecVols,      false)
      defaultValue (double defCycleTime34,      0.0)
      defaultValue (double defExecVol,          0.0)
      defaultValue (bool   defPipEnabled,       false)
      defaultValue (int    defPipRank,          0)

      defaultValue (int    defYield,          100) 
         //
         // Used only by readData.

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setOperationName    (const char *);
      void setYieldRate        (const WitIVRealArg &);
      void setMinLotSize       (const WitIVRealArg &);
      void setIncLotSize       (const WitIVRealArg &);
      void setTwoLevelLotSizes (bool);
      void setLotSize2Thresh   (const WitIVRealArg &);
      void setMinLotSize2      (const WitIVRealArg &);
      void setIncLotSize2      (const WitIVRealArg &);
      void setExecPenalty      (double);
      void setExecCost         (const WitIVRealArg &);
      void setIntExecVols      (bool);
      void setExecVol          (const WitIVRealArg &);
      void setCycleTime34      (const WitIVRealArg &);
      void setPipEnabled       (bool);
      void setPipRank          (int);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePreExecutable (WitPeriod, bool);
         //
         // Valid only when preprocessing is being performed.

      void storeHeurExecVol (WitPeriod, double);
         //
         // Valid only when heuristic allocation is updating the solution.

      void storeOptExecVol (WitPeriod, double);
         //
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      void storeStochExecCost (const WitDblFlexVec &);
      void storeStochExecVol  (const WitDblFlexVec &);
         //
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      void storeMrpMrpExecVol (const WitTVec <double> &);
         //
         // Valid only when the MRP subsystem is uploading the MRP solution.

      void storeFssFssExecVol (const WitTVec <double> &);
         //
         // Valid only when the FSS subsystem is uploading the FSS solution.

      //------------------------------------------------------------------------
      // Insert functions.
      // Each of these functions does an appendUniquely on the argument for the
      // appropriate List.
      //------------------------------------------------------------------------

      void insert (WitBomEntry *);
      void insert (WitBopEntry *);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool           inserted          ();
      virtual void           writeMultiObjVecs (WitObjective *);
      virtual void           writeSolnData     ();
      virtual void           copySelfInto      (WitCompMgr * theCompMgr);
      virtual void           recount           ();
      virtual void           unpreprocess      ();
      virtual void           clearOptSoln      ();
      virtual void           clearStochSoln    ();
      virtual WitOperation * thisOpn           ();
         //
         // Overrides from class Component.

      virtual void       identifyInstance ();
      virtual void       writeSetClause   ();
      virtual WitMsgFrag objTypeMsgFrag   ();
      virtual WitAttr    apiObjItrState   ();
         //
         // Overrides from class DelComp.

      virtual int nodeIndex () const;
         //
         // Override from class Node.

      inline static WitOperation * key (WitListItr & theItr)
         {
         return reinterpret_cast <WitOperation *> (theItr.key ());
         }
         //
         // Used by forEach<object> macros.

      static bool validYieldRate (double theValue);
         //
         // Returns true, iff theValue is valid as a yieldRate.

      bool cycleTime34PreventsExec (WitPeriod t);
         //
         // Returns true, iff cycleTime34 is such has to prevent execution of
         // this operation in period t.
         // Requires that wit34Compatible is true.

      void setWit34Compatible (bool newValue);
         //
         // Makes the changes to this Operation appropriate to a change in the
         // value of wit34Compatible to newValue.

      void copyAttrsFrom (WitOperation * theOpn);
         //
         // Copies the input attributes of theOpn into this Operation.

      bool isCoproducing (WitPeriod execPer);
         //
         // Return true, iff this Operation is coproducing in execPer.
         // For more info, see Part::coproduced_.

      void display ();
         //
         // Displays the input and preprocessing data for this Operation.

      void clearExecVolForHeur ();
         //
         // Clears execVol_.
         // Valid only when heuristic allocation is updating the solution.

      double lotSizeDelta (WitPeriod thePer, double theExecVol, double delta);
         //
         // Returns delta, adjusted so that theExecVol + delta is lot-size 
         // feasible.

      void writeSoln ();
         //
         // Prints the comprehensive implosion solution for this Operation.

      void writeMrpSoln ();
         //
         // Prints the comprehensive MRP solution for this Operation.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOperation);

      virtual WitBoundSet * myBoundSet         ();
      virtual void          writeDataID        ();
      virtual void          writeDataAttrs     ();
      virtual void          purgeCompListsSpec ();
      virtual void          clearDepListsSpec  ();
         //
         // Overrides from class Component.

      bool lotSizeMode (WitPeriod thePer);
         //
         // Returns true, iff lot sizing is to be done for this Operation in
         // period thePer.

      double tllsExecVol (WitPeriod thePer, double theExecVol);
         //
         // Returns the lowest amount >= theExecVol that's lot-size feasible
         // with respect to two-level lot sizes.

      double lotSizedExecVol (
            WitPeriod             thePer,
            double                theExecVol,
            const WitDblFlexVec & mlsVec,
            const WitDblFlexVec & ilsVec);
         //
         // Returns the lowest amount >= theExecVol that's lot-size feasible
         // with respect to mls & ils.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data maintained by myCompMgr ().
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # Operations constructed before this Operation.

      WitList <WitBomEntry> bom_;
         //
         // Bill Of Manufacturing stored as list of BomEntries.

      WitList <WitBopEntry> bop_;
         //
         // Bill Of Products stored as list of BopEntries.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      WitDblFlexVec yieldRate_;
         //
         // yieldRate_[thePer] is the yield incurred when executing this
         //    Operation in thePer.
         // If yieldRate_[thePer] == 0.0, then execution is prohibited in 
         //    thePer.
         // Range: [0.01, 1.00] union {0.0}.

      WitDblFlexVec minLotSize_;
         //
         // minLotSize ()[thePer] is the minimum allowable value of
         // execVol ()[thePer], if execVol ()[thePer] > 0.0.
         // Range: [0.0, +inf).

      WitDblFlexVec incLotSize_;
         //
         // If execVol ()[thePer] > 0.0, then 
         // execVol ()[thePer] - minLotSize_[thePer]
         // will be a multiple of incLotSize ()[thePer].
         // Range: [0.0, +inf).

      bool twoLevelLotSizes_;
         //
         // true, iff two-level lot sizes are to be used for this Operation.

      WitDblFlexVec lotSize2Thresh_;
         //
         // If lotSize2Thresh_[thePer] <  0.0: minLotSize and incLotSize apply.
         // If lotSize2Thresh_[thePer] >= 0.0:
         //    If execVol_[thePer] <  lotSize2Thresh_[thePer]:
         //       minLotSize and incLotSize apply.
         //    If execVol_[thePer] >= lotSize2Thresh_[thePer]:
         //       minLotSize2 and incLotSize2 apply.
         // Range: [0.0, +inf).

      WitDblFlexVec minLotSize2_;
         //
         // Same as minLotSize, but applies when 
         // 0.0 < lotSize2Thresh_[thePer] <= execVol_[thePer]
         // Range: [0.0, +inf).

      WitDblFlexVec incLotSize2_;
         //
         // Same as incLotSize, but applies when 
         // 0.0 < lotSize2Thresh_[thePer] <= execVol_[thePer]
         // Range: [0.0, +inf).

      double execPenalty_;
         //
         // If heuristic implosion / allocation is using the penalized 
         // execution technique, execPenalty_ is the penalty that the heuristic
         // will apply to executing this Operation.
         // Range: [0.0, +inf).

      WitObjVec execCost_;
         //
         // Stores the cost coefficients of the execution variables for this
         // Operation.
         // Range: (-inf, +inf).

      bool intExecVols_;
         //
         // If true, then in optimizing implosion, execVol_ will be constrained
         // to take on integer values in all periods.

      WitDblFlexVec cycleTime34_;
         //
         // Production cycle time, expressed as a
         // function of execution (completion) period.
         // Only allocated in wit34Compatible mode.
         // Range: (-inf, +inf).

      WitDblFlexVec execVol_;
         //
         // execVol_[thePer] is the execution volume in period thePer
         //    (for implosion).
         // Normally, this is set by implosion, but it can also be set as
         // input.
         // Range: [0.0, +inf).

      WitBoundSet * execBounds_;
         //
         // Bounds on execution volumes.

      bool pipEnabled_;
         //
         // If true, PIP will compute peggings to this operation.

      int pipRank_;
         //
         // The relative rank of pegging to this Operation in PIP.
         // Higher pipRank Operations are given preference in the pegging.

      //------------------------------------------------------------------------
      // Data set by general preprocessing
      //------------------------------------------------------------------------

      WitTVec <bool> executable_;
         //
         // executable_[t] is true iff this Operation can be executed in
         // period t.

      //------------------------------------------------------------------------
      // Data set by mrp
      //------------------------------------------------------------------------

      WitDblFlexVec mrpExecVol_;
         //
         // mrpExecVol_[thePer] is the execution volume in thePer, as computed
         // by MRP.

      //------------------------------------------------------------------------
      // Data set by FSS.
      //------------------------------------------------------------------------

      WitDblFlexVec fssExecVol_;
         //
         // fssExecVol_[thePer] is the execution volume in thePer, as computed
         // by FSS.
   };

#endif
