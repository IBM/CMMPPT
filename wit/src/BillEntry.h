//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef BillEntryH
#define BillEntryH

//------------------------------------------------------------------------------
// Header file: "BillEntry.h"
//
// Contains the declaration class BillEntry.
//------------------------------------------------------------------------------

#include <DelComp.h>
#include <FlexVec.h>

//------------------------------------------------------------------------------
// class BillEntry
// In bill entry in the complete BOM structure.
// Each BillEntry represents the consumption or production of a Part in the
// execution of an Operation.
//
// Class Hierarchy:
//
// ProbAssoc
//    Component
//       DelComp
//          BillEntry
//             ConsEntry
//                BomEntry
//                SubEntry
//             BopEntry
//------------------------------------------------------------------------------

class WitBillEntry: public WitDelComp
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBillEntry ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPart *,                   myPart)
      accessFunc (int,                         localIndex)
      accessFunc (WitPeriod,                   earliestPeriod)
      accessFunc (WitPeriod,                   latestPeriod)
      accessFunc (const WitDblFlexVec &,       routingShare)
      accessFunc (const WitTVec <WitPeriod> &, impactPeriod)

      inline WitOperation * myOperation ()
         {
         return derivedOperation ();
         }

      const WitString & myPartName ();
      const WitString & myOperationName ();

      virtual const WitDblFlexVec & offset ();
         //
         // Base class function.

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitBillEntry)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double    defOffset,         0.0)
      defaultValue (WitPeriod defEarliestPeriod, 0)
      defaultValue (double    defRoutingShare,   1.0)

      static WitPeriod defLatestPeriod (WitProblem * theProblem);
         //
         // This default value function is implemented explicitly, because the
         // value depends on theProblem.

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setEarliestPeriod (int);
      void setLatestPeriod   (int);
      void setRoutingShare   (const WitIVRealArg &);
      void setOffset         (const WitIVRealArg &);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePreImpactPeriod (WitPeriod, WitPeriod);
         //
         // Valid only when preprocessing is being performed.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void unpreprocess ();
         //
         // Overrides from class Component.

      virtual bool storesOffset ();
         //
         // Returns true, iff this BillEntry stores its own offset.

      bool withinInterval (WitPeriod thePer);
         //
         // Returns true, iff t is within the effectivity interval for this
         // BillEntry.

      inline bool inEffect (WitPeriod thePer)
            //
            // Returns true iff this BillEntry is in effect in thePer.
         {
         return impactPeriod_[thePer] >= 0;
         }

      static void getAllInstances (
            WitProblem *                 theProblem,
            WitObjStack <WitBillEntry> & theBillEntries);
         //
         // Sets theBillEntries to be the set of all BillEntries for theProblem.

      static void reLocalIndex (WitProblem * theProblem);
         //
         // Re-assigns localIndex for each BillEntry in theProblem.
         // Valid only during a purge.

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBillEntry (WitPart * thePart, int theLocalIndex);
         //
         // myPart_     <-- thePart.
         // localIndex_ <-- theLocalIndex.

      //------------------------------------------------------------------------
      // Protected member functions pertaining to offset.
      //------------------------------------------------------------------------

      inline WitDblFlexVec & offsetRef ()
         {
         return offset_;
         }
         //
         // Non-const protected access.

      void writeOffset ();
         //
         // Does writeData for offset.

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      static void preConstruct (
            WitOperation * theOpn,
            WitPart *      thePart);
         //
         // Performs various operations just prior to constructing a new
         // instance.

      void recountBillEntry ();
         //
         // Recounts this BillEntry.

      virtual void displayMandEC ();
         //
         // Base class function.
         // Displays mandEC, if this BillEntry is a BomEntry.
         // The base class implementation is a no-op.

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      void copyAttrsFrom (WitBillEntry * theBillEnt);
         //
         // Copies the input attributes of theBillEnt into this BillEntry.

      void display ();
         //
         // Displays the input and preprocessing data for this BillEntry.

      virtual WitOperation * derivedOperation () = 0;
         //
         // Base class function.
         // Returns myOperation () as deterimined by the appropriate derived
         // class.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitBillEntry);

      //------------------------------------------------------------------------
      // reLocalIndex functions.
      //
      // Each of these functions re-assigns localIndex for each BillEntry in
      // the argument List. The localIndex value will reflect the order of the
      // BillEntries in the List.
      //------------------------------------------------------------------------

      static void reLocalIndex (const WitList <WitBomEntry> &);
      static void reLocalIndex (const WitList <WitSubEntry> &);
      static void reLocalIndex (const WitList <WitBopEntry> &);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Data set by the constructor.
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # BillEntries constructed before this BillEntry.

      WitPart * const myPart_;
         //
         // The Part impacted (produced or consumed) by this BillEntry.

      int localIndex_;
         //
         // k, where this BillEntry is either:
         //    the k-th BomEntry of myOperation (), starting at 0, or
         //    the k-th SubEntry of myBomEnt (),    starting at 0, or
         //    the k-th BopEntry of myOperation (), starting at 0.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      WitDblFlexVec offset_;
         //
         // If myOperation () is executed in thePer, then myPart () is
         // produced or consumed in thePer - offset ()[thePer]. This is a double
         // because it may be the result of converting from, e.g., days to
         // weeks.
         // Range: (-inf, +inf).

      WitPeriod earliestPeriod_;
         //
         // Earliest execution Period for which this BillEntry is in effect.
         // In the case of a SubEntry, it can only be in effect if the
         // BomEntry itself is also in effect.
         // Range: [0, nPeriods].
         // Note that earliestPeriod_ == nPeriods implies that this BillEntry
         // is never in effect.

      WitPeriod latestPeriod_;
         //
         // Latest execution Period for which this BillEntry is in effect.
         // Range: [0, nPeriods).

      WitDblFlexVec routingShare_;
         //
         // If the proportionate routing technique is being used on a Part or
         // BomEntry, explosion through the BopEntries of the Part or
         // ConsEntries of the BomEntry is to be done in proportion to the
         // routingShare_ of each BillEntry.
         // Range: [1.0, +inf)

      //------------------------------------------------------------------------
      // Data set by preprocessing.
      //------------------------------------------------------------------------

      WitTVec <WitPeriod> impactPeriod_;
         //
         // Impact period: If myOperation () executes in thePer, myPart () will
         // be produced or consumed in period impactPeriod_[thePer]. A value of
         // -1 indicates that this BillEntry is not used when myOperation ()
         // executes in thePer, due to either the effectivity dates, or
         // forbidden execution of myOperation ().

      //------------------------------------------------------------------------
      // Data set by preprocessing for optimizing implosion.
      //------------------------------------------------------------------------

      WitVector <int> firstEPIndex_;
         //
         // firstEPIndex_[impactPer] is the first index into execPeriod_ for
         // impactPer.
         // firstEPIndex_[nPeriods] is the length of execPeriod_.

      WitVector <WitPeriod> execPeriod_;
         //
         // For execPerIdx = firstEPIndex_[thePer], 
         //             thru firstEPIndex_[thePer + 1] - 1
         //
         //    execPeriod_[execPerIdx] is a unique execution period execPer, 
         //    such that impactPeriod ()[execPer] == thePer.
         //    The exec periods are listed in reverse order.
   };

#endif
