//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SubEntryH
#define SubEntryH

//------------------------------------------------------------------------------
// Header file: "SubEntry.h"
//
// Contains the declaration of the class SubEntry
//------------------------------------------------------------------------------

#include <BomEntry.h>
#include <ObjVec.h>

//------------------------------------------------------------------------------
// class SubEntry: "Substitute Bill-Of-Manufacturing Entry"
// An BillEntry representing the consumption of a Part in the execution of a
// Operation, as a substitute for the "normal" Part consumed. Each SubEntry
// theSub for BomEntry theSub->myBomEnt () represents the option to consume a
// different part, theSub->myPart (), in place of the "normal" part
// consumed theSub->myBomEnt ()->myPart (), in the execution of
// Operation theSub->myBomEnt ()->myOperation (). Typically, most BomEntries
// will have no SubEntries.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          BillEntry
//             ConsEntry
//                SubEntry
//------------------------------------------------------------------------------

class WitSubEntry: public WitConsEntry
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSubEntry (WitBomEntry * theBomEnt, WitPart * thePart);
         //
         // myBomEnt_ <-- theBomEnt.
         // ConsEntry <<< thePart.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSubEntry ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitBomEntry *,         myBomEnt)
      accessFunc (bool,                  intSubVols)
      accessFunc (bool,                  expAllowed)
      accessFunc (bool,                  netAllowed)
      accessFunc (bool,                  mrpNetAllowed)
      accessFunc (double,                expNetAversion)
      accessFunc (const WitDblFlexVec &, subVol)
      accessFunc (const WitDblFlexVec &, mrpSubVol)
      accessFunc (const WitDblFlexVec &, fssSubVol)

      inline const WitDblFlexVec & subCost () const
         {
         return subCost_.curDblFlexVec ();
         }

      inline WitOperation * myOperation ()
         {
         return myBomEnt ()->myOperation ();
         }

      inline int myBomEntIndex ()
         {
         return myBomEnt ()->localIndex ();
         }

      virtual const WitDblFlexVec & offset ();
         //
         // Override from class BillEntry.

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitSubEntry)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double defSubCost,        0.0)
      defaultValue (bool   defIntSubVols,     false)
      defaultValue (bool   defExpAllowed,     true)
      defaultValue (bool   defNetAllowed,     true)
      defaultValue (bool   defMrpNetAllowed,  false)
      defaultValue (double defExpNetAversion, 0.0)
      defaultValue (double defSubVol,         0.0)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setSubCost        (const WitIVRealArg &);
      void setIntSubVols     (bool);
      void setExpAllowed     (bool);
      void setNetAllowed     (bool);
      void setMrpNetAllowed  (bool);
      void setExpNetAversion (double);
      void setSubVol         (const WitIVRealArg &);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storeHeurSubVol (WitPeriod, double);
         //
         // Valid only when heuristic allocation is updating the solution.

      void storeOptSubVol (WitPeriod, double);
         //
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      void storeStochSubCost (const WitDblFlexVec &);
      void storeStochSubVol  (const WitDblFlexVec &);
         //
         // Valid only when the stochastic implosion subsystem is uploading
         // data.

      void storeMrpMrpSubVol (const WitTVec <double> &);
         //
         // Valid only when the MRP subsystem is uploading the MRP solution.

      void storeFssFssSubVol (const WitTVec <double> &);
         //
         // Valid only when the FSS subsystem is uploading the FSS solution.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool          inserted          ();
      virtual void          writeMultiObjVecs (WitObjective *);
      virtual void          identifyInstance  ();
      virtual void          writeSetClause    ();
      virtual void          copySelfInto      (WitCompMgr *);
      virtual void          recount           ();
      virtual void          clearOptSoln      ();
      virtual void          clearStochSoln    ();
      virtual WitSubEntry * thisSub           ();
         //
         // Override from class Component.

      virtual WitMsgFrag objTypeMsgFrag ();
      virtual WitAttr    apiObjItrState ();
         //
         // Overrides from class DelComp.

      virtual bool storesOffset ();
         //
         // Override from class BillEntry.

      virtual void writeSolnData ();
         //
         // Override from class Component.
         // Writes the section of the solution data file for this SubEntry.

      void copyAttrsFrom (WitSubEntry * theSub);
         //
         // Copies the input attributes of theSub into this SubEntry.

      void display ();
         //
         // Displays the input and preprocessing data for this SubEntry.

      void clearSubVolForHeur ();
         //
         // Clears subVol_.
         // Valid only when heuristic allocation is updating the solution.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSubEntry);

      virtual WitOperation * derivedOperation ();
         //
         // Override from class BillEntry.

      virtual WitBomEntry * derivedBomEntry ();
         //
         // Override from class ConsEntry.

      virtual void writeDataID ();
         //
         // Override from class Component.

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      virtual void findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2);
         //
         // Override from class DelComp.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data set by the constructor.
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # SubEntries constructed before this one.

      WitBomEntry * const myBomEnt_;
         //
         // The BomEntry for which this SubEntry is substituting.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      WitObjVec subCost_;
         //
         // Stores the cost coefficients of the substitution variables for this
         // Substitute.
         // Range: (-inf, +inf).

      bool intSubVols_;
         //
         // If true, then in optimizing implosion, subVol_ will be constrained
         // to take on integer values in all periods.

      bool expAllowed_;
         //
         // This SubEntry is allowed to be used for explosion by multi-route
         // implosion/allocation, iff expAllowed_ is true.

      bool netAllowed_;
         //
         // This SubEntry is allowed to be used for netting by
         // implosion/allocation, iff netAllowed_ is true.

      bool mrpNetAllowed_;
         //
         // Same as netAllowed_, but for MRP and FSS.

      double expNetAversion_;
         //
         // "Explosion / Netting Aversion":
         // SubEntries with lower expNetAversion are preferred for explosion and
         // netting over those with higher expNetAversion.

      WitDblFlexVec subVol_;
         //
         // subVol[thePer] is the amount of myOperation that was executed
         // in period thePer, using this SubEntry.
         // Normally, this is set by implosion, but it can also be set as
         // input.
         // Range: [0.0, +inf),

      //------------------------------------------------------------------------
      // Data set by MRP.
      //------------------------------------------------------------------------

      WitDblFlexVec mrpSubVol_;
         //
         // subVol for MRP.

      //------------------------------------------------------------------------
      // Data set by FSS.
      //------------------------------------------------------------------------

      WitDblFlexVec fssSubVol_;
         //
         // subVol for FSS.
   };

#endif
