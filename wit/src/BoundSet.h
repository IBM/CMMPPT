//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef BoundSetH
#define BoundSetH

//------------------------------------------------------------------------------
// Header file: "BoundSet.h"
//
// Contains the declaration of class BoundSet.
//------------------------------------------------------------------------------

#include <FlexVec.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// class BoundSet
// The set of bounds on a time vector of variables.
//
// The following relationships are required:
//               0.0 <= hardLB_[thePer] <  +infinity.
//               0.0 <= softLB_[thePer] <  +infinity.
//               0.0 <= hardUB_[thePer] <= +infinity.
//   hardLB_[thePer] <= softLB_[thePer] <= hardUB_[thePer].
//
// The BoundSet member functions verify these relationships.
//
// Infinite values of hardUB are represented as negative numbers. If possible,
// code that uses BoundSets should not explicitly depend on this representation
// of infinity. Instead, to test whether or not a number x is <= the hardUB in
// thePer, use hardUBisSatisfied (thePer, x). To test whether or not the hardUB
// in thePer is finite, use hardUBIsFinite (thePer).
// Typically, BoundSets are manipulated thru pointers and not references,
// because they are used to index into schedules.
//
// Class Hierarchy:
//
// ProbAssoc
//    BoundSet
//       StockBoundSet
//       CumShipBoundSet
//       ExecBoundSet
//------------------------------------------------------------------------------

class WitBoundSet: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBoundSet (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBoundSet ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitDblFlexVec &, hardLB)
      accessFunc (const WitDblFlexVec &, softLB)
      accessFunc (const WitDblFlexVec &, hardUB)

      //------------------------------------------------------------------------
      // Mapping functions
      //------------------------------------------------------------------------

      decMappingIndex (WitBoundSet)

      static int nInstances (WitProblem *);

      //------------------------------------------------------------------------
      // Default value functions.
      //------------------------------------------------------------------------

      defaultValue (double defHardLB,  0.0)
      defaultValue (double defSoftLB,  0.0)
      defaultValue (double defHardUB, -1.0)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual double boundedVal (WitPeriod thePer) = 0;
         //
         // Returns the element indexed by thePer in the Vector bounded by this
         // BoundSet.

      virtual const char * myAttName () = 0;
         //
         // Returns the attribute name for this BoundSet.

      virtual WitDelComp * myDelComp () = 0;
         //
         // Returns the DelComp that owns this BoundSet.

      bool isAtDefault ();
         //
         // Returns true iff the vectors of this BoundSet are at their default
         // values.

      bool hardUBIsFinite (WitPeriod thePer);
         //
         // Returns true iff the hard upper bound for thePer represents a
         // finite number.

      bool hardUBisSatisfied (WitPeriod thePer, double theDbl);
         //
         // Returns true iff theDbl <= the hardUB of this BoundSet in thePer,
         // where the hardUB may be infinite.

      void set (
            const WitDblFlexVec & hardLBval,
            const WitDblFlexVec & softLBval,
            const WitDblFlexVec & hardUBval);
         //
         // Sets the vectors of this BoundSet to the arguments.

      //------------------------------------------------------------------------
      // storeStoch<Attr> functions.
      // Each of these functions sets the value of indicated attribute to the
      // value of the argument.
      // Valid only when the stochastic implosion subsystem is uploading data.
      //------------------------------------------------------------------------

      void storeStochHardLB (const WitDblFlexVec &);
      void storeStochSoftLB (const WitDblFlexVec &);
      void storeStochHardUB (const WitDblFlexVec &);

      //------------------------------------------------------------------------

      void copyAttrsFrom (WitBoundSet * theBoundSet);
         //
         // Copies the vectors of theBoundSet into this BoundSet.

      static bool seqError (
            const WitDblFlexVec & hardLBArg,
            const WitDblFlexVec & softLBArg,
            const WitDblFlexVec & hardUBArg,
            WitPeriod             thePer);
         //
         // Returns true, iff the vectors fail to satisfy the condition:
         //
         //    0.0 <= hardLBArg <= softLBArg <= hardUBArg,
         //
         // where negative values of hardUBArg are interpreted as +infinity.

      bool hasPositiveHardLB ();
         //
         // Returns true iff this BoundSet has a positive hardLB.

      void writeData ();
         //
         // Writes this BoundSet to the input data output file, if needed.

      void writeSetClause ();
         //
         // Writes a "set" clause for this BoundSet to an input data file.

      void display ();
         //
         // Displays the input data for this BoundSet.

      void writeSoln ();
         //
         // Prints the comprehensive implosion solution for this BoundSet.

      void reindex ();
         //
         // Recomputes mappingIndex for this BoundSet.
         // Valid only if myCompMgr () is reindexing this BoundSet.

      virtual WitBoundSet * myMatchIn (WitCompMgr * theCompMgr) = 0;
         //
         // Finds and returns the BoundSet in theCompMgr whose DelComp has
         // the same ID as the DelComp that owns this BoundSet.
         // Checks that such a BoundSet exists.

      virtual WitNode * stageNode () = 0;
         //
         // Returns the Node whose stage determines the stage for this BoundSet.
         // (for stochastic implosion).

      virtual void issueStage0MMCompMsg () = 0;
         //
         // Issues a msg identifying myDelComp () for a stage 0 attribute
         // mismatch situation.

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      virtual WitMsgID headingMsgID () = 0;
         //
         // Returns the MsgID to be used for displaying this BoundSet.

      virtual const char * boundedVectorName () = 0;
         //
         // Returns the attribute name of the Vector Bounded by this BoundSet.

      virtual void idObjectForSoln () = 0;
         //
         // Writes identification of the object bounded by this BoundSet to
         // myProblem ()->solnFile ().

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitBoundSet);

      bool unaccNeeded (
            const WitDblFlexVec & hardLBArg,
            const WitDblFlexVec & softLBArg);
         //
         // Returns true, if it is necessary to shut down opt implosion when
         // setting the hard and soft lower bounds to hardLBArg and softLBArg.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      int mappingIndex_;
         //
         // Mapping index: see Mapping.h.
         // == # BoundSets constructed before this BoundSet.

      WitDblFlexVec hardLB_;
         //
         // Hard lower bound vector.

      WitDblFlexVec softLB_;
         //
         // Soft lower bound vector.

      WitDblFlexVec hardUB_;
         //
         // Hard upper bound vector.
   };

#endif
