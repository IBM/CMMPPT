//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef ConsEntryH
#define ConsEntryH

//------------------------------------------------------------------------------
// Header file: "ConsEntry.h"
//
// Contains the declaration of class ConsEntry.
//------------------------------------------------------------------------------

#include <BillEntry.h>

//------------------------------------------------------------------------------
// Class ConsEntry
//
// "Consumption Bill Entry"
// A BillEntry representing the consumption of a Part in the execution of an
// Operation.
// The Part consumed may be either the "normal" part consumed (see BomEntry),
// or a substitute for the normal part consumed (see SubEntry).
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
//
// Implemented in Entry.C.
//------------------------------------------------------------------------------

class WitConsEntry: public WitBillEntry
   {
   public:

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitConsEntry ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitDblFlexVec &, consRate)
      accessFunc (double,                falloutRate)
      accessFunc (double,                execPenalty)
      accessFunc (const WitDblFlexVec &, effConsRate)

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitConsEntry)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double defConsRate,    1.0)
      defaultValue (double defFalloutRate, 0.0)
      defaultValue (int    defFallout,     0) // Used only by readData.
      defaultValue (double defExecPenalty, 0.0)

      //------------------------------------------------------------------------
      // "Set" functions.
      //------------------------------------------------------------------------

      void setConsRate    (const WitIVRealArg &);
      void setFalloutRate (double);
      void setExecPenalty (double);

      //------------------------------------------------------------------------
      // store<Activity><Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Each function is only valid when the indicated activity is being
      // performed.
      //------------------------------------------------------------------------

      void storePreEffConsRate (const WitTVec <double> &);
      void storePreEffConsRate (double);
         //
         // Valid only when preprocessing is being performed.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void           unpreprocess  ();
      virtual WitConsEntry * thisConsEnt   ();
         //
         // Overrides from class Component.

      void incConsVol (
                  WitTVec <double> & consVolArg,
            const WitTVec <double> & execVolArg);
         //
         // Assumes:
         //    consVolArg is a  consVol vector for myPart ().
         //    execVolArg is an execVol vector for myOperation ().
         // Increments consVolArg corresponding to consumption due to this
         // ConsEntry resulting from execVolArg.

      double effCost (WitPeriod thePer);
         //
         // Returns the objective #2 unit cost corresponding to the effective
         // rate of consumption for this ConsEntry in thePer.

      inline WitBomEntry * myBomEnt ()
         {
         return derivedBomEntry ();
         }

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitConsEntry (WitPart * thePart, int theLocalIndex);
         //
         // BillEntry <<< thePart.
         // BillEntry <<< theLocalIndex.

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      void recountConsEntry ();
         //
         // Recounts this ConsEntry.

      virtual void writeDataAttrs ();
         //
         // Override from class Component.

      void copyAttrsFrom (WitConsEntry * theConsEnt);
         //
         // Copies the input attributes of theConsEnt into this ConsEntry.

      void display ();
         //
         // Displays the input and preprocessing data for this ConsEntry.

      virtual WitBomEntry * derivedBomEntry () = 0;
         //
         // Base class function.
         // Returns myBomEnt () as deterimined by the appropriate derived class.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitConsEntry);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # ConsEnties constructed before this ConsEntry.

      //------------------------------------------------------------------------
      // Input data.
      //------------------------------------------------------------------------

      WitDblFlexVec consRate_;
         //
         // Consumption rate.
         // Range: (-inf, +inf).

      double falloutRate_;
         //
         // Fallout rate.
         // Range: [0.0, 0.99].

      double execPenalty_;
         //
         // If heuristic implosion / allocation is using the penalized execution
         // technique, execPenalty_ is the penalty that the heuristic will apply
         // to using this ConsEntry.
         // Range: [0.0, +inf).

      //------------------------------------------------------------------------
      // Data set by general preprocessing
      //------------------------------------------------------------------------

      WitDblFlexVec effConsRate_;
         //
         // effConsRate[thePer] is consRate[thePer], scaled up by 
         // 1 - falloutRate. effConsRate_[thePer] units of myPart () are
         // consumed in order to execute 1 unit of myOperation () in thePer.
         // This data should be used in place of consRate for all implosion
         // calculations.
   };

#endif
