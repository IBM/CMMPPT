//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef BomEntryH
#define BomEntryH

//------------------------------------------------------------------------------
// Header file: "BomEntry.h"
//
// Contains the declaration of the class BomEntry
//------------------------------------------------------------------------------

#include <ConsEntry.h>

//------------------------------------------------------------------------------
// class BomEntry: "Bill-Of-Manufacturing Entry"
// An BillEntry representing the consumption of a Part in the execution of an
// Operation.
// The part consumed is considered to be the "normal" part consumed.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          BillEntry
//             ConsEntry
//                BomEntry
//------------------------------------------------------------------------------

class WitBomEntry: public WitConsEntry
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBomEntry (WitOperation * theOpn, WitPart * thePart);
         //
         // myOperation_ <-- theOpn.
         // ConsEntry    <<< thePart.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBomEntry ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOperation *,                myOperation)
      accessFunc (const WitList <WitSubEntry> &, mySubEntries)
      accessFunc (bool,                          mandEC)
      accessFunc (bool,                          singleSource)
      accessFunc (const WitFlexVec <bool> &,     propRtg)
      accessFunc (const WitFlexVec <bool> &,     hasSubsInEffect)

      const WitDblFlexVec & nonSubVol ();

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitBomEntry)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (bool defMandEC,       false)
      defaultValue (bool defSingleSource, false)
      defaultValue (bool defPropRtg,      false)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setMandEC       (bool);
      void setSingleSource (bool);
      void setPropRtg      (const WitBoolean *);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePreMySubEntries    (const WitList <WitSubEntry> &);
      void storePreHasSubsInEffect (const WitTVec <bool> &);
         //
         // Valid only when preprocessing is being performed.

      void storePostNonSubVol (const WitTVec <double> &);
         //
         // Valid only when postprocessing is being performed.

      void storeOptNonSubVol (WitPeriod, double);
         //
         // Valid only when the optimizing implosion subsystem is uploading the
         // solution.

      //------------------------------------------------------------------------
      // Insert functions.
      // Each of these functions does an appendUniquely on the argument for the
      // appropriate List.
      //------------------------------------------------------------------------

      void insert (WitSubEntry *);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual bool          inserted         ();
      virtual void          identifyInstance ();
      virtual void          writeSetClause   ();
      virtual void          copySelfInto     (WitCompMgr * theCompMgr);
      virtual void          recount          ();
      virtual void          unpreprocess     ();
      virtual WitBomEntry * thisBomEnt       ();
         //
         // Overrides from class Component.

      virtual WitMsgFrag objTypeMsgFrag ();
      virtual WitAttr    apiObjItrState ();
         //
         // Override from class DelComp.

      void copyAttrsFrom (WitBomEntry * theBomEnt);
         //
         // Copies the input attributes of theBomEnt into this BomEntry.

      inline static WitBomEntry * key (WitListItr & theItr)
         {
         return reinterpret_cast <WitBomEntry *> (theItr.key ());
         }
         //
         // Used by forEach<object> macros.

      void setUsageTime34 (double theValue);
         //
         // Assigns a value to offset that emulates the WIT 3.4
         // attribute of usageTime == theValue, given the current value of
         // myOperation ()->cycleTime34 ().
         // Issues a fatal error, if not in wit34Compatible mode.

      double usageTime34 (WitPeriod thePer);
         //
         // Computes and returns a value of the WIT 3.4 usageTime attribute
         // corresponding to the current values of offset ()[thePer] and
         // myOperation ()->cycleTime34 ()[thePer].
         // Issues a fatal error, if not in wit34Compatible mode.

      double usageTime34 ();
         //
         // Returns usageTime34 (0), which is the convention for determining
         // usageTime from offset and cycleTime34 ().

      void adjustOffset (const WitIVRealArg & newCycleTime34);
         //
         // Adjusts offset () so that usageTime34 () will give the same result
         // after myOperation ()->cycleTime34 () is changed from its current
         // value to newCycleTime34.

      void display ();
         //
         // Displays the input and preprocessing data for this BomEntry.

      void writeSubSoln ();
         //
         // Prints the comprehensive implosion solution for the SubEntries
         // for this BomEntry.

      void writeMrpSubSoln ();
         //
         // Prints the comprehensive MRP solution for the SubEntries
         // for this BomEntry.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitBomEntry);

      virtual WitOperation * derivedOperation ();
         //
         // Override from class BillEntry.

      virtual void displayMandEC ();
         //
         // Override from class BillEntry.
         // Displays mandEC.

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

      virtual void purgeCompListsSpec ();
         //
         // Override from class Component.

      virtual void clearDepListsSpec ();
         //
         // Override from class Component.

      void writeOffset34 ();
         //
         // Writes offset to the WIT data file in 3.4 compatible mode. 
         // In this case, offset must be written unconditionally, because it's
         // initialized to cycleTime34 rather than defOffset ().

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data set by the constructor.
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # BomEntries constructed before this one.

      WitOperation * const myOperation_;
         //
         // The Operation to which this BomEntry belongs.

      //------------------------------------------------------------------------
      // Data maintained by myCompMgr ().
      //------------------------------------------------------------------------

      WitList <WitSubEntry> mySubEntries_;
         //
         // The list of substitutes for the BomEntry.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      bool mandEC_;
         //
         // true, iff effectivity dates reflect a mandatory engineering change.

      bool singleSource_;
         //
         // The single-source technique is to be applied to this BomEntry, iff
         // singleSource_ is true.

      WitFlexVec <bool> propRtg_;
         //
         // The proportionate routing technique is to be applied to this
         // BomEntry in thePer, iff propRtg_[thePer] is true.

      //------------------------------------------------------------------------
      // Data set by general preprocessing.
      //------------------------------------------------------------------------

      WitFlexVec <bool> hasSubsInEffect_;
         //
         // hasSubsInEffect_[execPer] is true, iff this BomEntry has at least
         // one SubEntry in effect in execPer.

      //------------------------------------------------------------------------
      // Data set by postprocessing.
      //------------------------------------------------------------------------

      WitDblFlexVec nonSubVol_;
         //
         // nonSubVol_[thePer] is the amount of 
         // myOperation ()->execVol ()[thePer]
         // that is not due to substitution of this BomEntry.
         // Not used if mySubEntries () is empty.
   };

#endif
