//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef PreH
#define PreH

//------------------------------------------------------------------------------
// Header file: "Pre.h"
//
// Contains the declaration of class Preprocessor.
//------------------------------------------------------------------------------

#include <Assoc.h>

class WitNodeSorter;
class WitOffsetProc;
class WitBelowLister;

//------------------------------------------------------------------------------
// Class Preprocessor
//
// Responsible for performing preprocessing.
//------------------------------------------------------------------------------

class WitPreprocessor: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPreprocessor (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitPreprocessor ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (bool, preprocessing)
      accessFunc (bool, preprocessed)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void preprocess ();
         //
         // Performs preprocessing for myProblem ().

      void getExpCycle (
            WitObjStack <WitPart> &      theParts,
            WitObjStack <WitOperation> & theOpns);
         //
         // If the Problem contains at least one explodeable cycle, this
         // function retrieves one of them;
         // If not, this function empties the argument Stacks.

      void unpreprocess ();
         //
         // Does unpreprocessing for myProblem ().

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPreprocessor);

      void setUp ();
         //
         // Sets up preprocessing.

      void shutDown ();
         //
         // Shuts down preprocessing.

      void checkMaxNameLength ();
         //
         // Issues a warning is the is a name longer than the nominal limit.

      void updateLongestName (
            const WitString & theName,
            int &             maxNameLength,
            WitString &       theLongestName);
         //
         // Updates maxNameLength and theLongestName to account for theName.

      void validateBLA ();
         //
         // Verifies data for Demands with shipLateAllowed_ == false.
                                                     
      void procLotSizes ();
         //
         // Processes lot size data.

      void compHasSubsInEffect ();
         //
         // Computes hasSubsInEffect_ for all BomEntries.

      void compEffConsRates ();
         //
         // Computes effConsRates.

      void compEffConsRate (
            WitConsEntry *     theConsEnt,
            WitTVec <double> & effConsRateVec);
         //
         // Computes theConsEnt->effConsRate_.
         // Uses effConsRateVec as a working vector.

      void checkMinLotSize (WitBomEntry * theBomEnt);
         //
         // If theBomEnt->myPart () is a capacity, this function issues a
         // warning if the minLotSize of theBomEnt->myOperation () consumes
         // more than supplyVol of theBomEnt->myPart ().

      void compEffProdRates ();
         //
         // Computes effProdRate_ for each BopEntry.

      void chooseExpExecPeriods ();
         //
         // Computes expExecPeriod_ for each BopEntry.

      void checkSmallEffProdRates ();
         //
         // Finds the smallest effProdRate of an explodeable BopEntry, and if
         // it is below the default explosion cutoff, issues a warning.

      void sortSubEntries ();
         //
         // Sorts the SubEntries of each BomEntry in order of non-decreasing
         // expNetAversion.

      void sortBopEntries ();
         //
         // For each Part, thePart, sorts thePart->producingBopEntries in order
         // of non-decreasing expAversion.

      void chooseExpBopEntries ();
         //
         // Sets thePart->expBopEntry () for all Parts.

      void findExpPerGaps ();
         //
         // Sets thePart->expPerGaps () for all Parts.

      void findMandECs ();
         //
         // Determines mandEC for each Material in myProblem ().

      void setDirectMandECs (WitBomEntry * theBomEnt);
         //
         // Sets mandatory ECs directly from theBomEnt.

      void propMandECs (WitConsEntry * theConsEnt);
         //
         // Propagates the mandatory ECs from theConsEnt->myPart () to the bop
         // of theConsEnt->myOperation ().

      void setMandECs (WitOperation * theOpn, WitPeriod thePer);
         //
         // For each Material in the bop of theOpn, sets mandEC_
         // corresponding to a mandatory EC on theOpn in period t.

      accessNonNull (WitNodeSorter *,  myNodeSorter)
      accessNonNull (WitOffsetProc *,  myOffsetProc)
      accessNonNull (WitBelowLister *, myBelowLister)

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool preprocessing_;
         //
         // true, iff preprocessing is currently being performed.

      bool preprocessed_;
         //
         // true iff myProblem () is in a preprocessed state.

      WitNodeSorter * myNodeSorter_;
         //
         // The NodeSorter owned by this Preprocessor during processing;
         // otherwise NULL.

      WitOffsetProc * myOffsetProc_;
         //
         // The OffsetProc owned by this Preprocessor during processing;
         // otherwise NULL.

      WitBelowLister * myBelowLister_;
         //
         // The BelowLister owned by this Preprocessor during processing;
         // otherwise NULL.
   };

#endif
