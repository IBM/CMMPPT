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

#ifndef ProblemH
#define ProblemH

//------------------------------------------------------------------------------
// Header file: "Problem.h"
//
// Contains the declaration of class Problem.
//------------------------------------------------------------------------------

#include <List.h>
#include <TVec.h>

//------------------------------------------------------------------------------
// Class Problem.
//
// Each instance of this class defines A WIT problem and its solution.
// Essentially this is all aspects of WIT that are initialized by
// witInitialize.
// It excludes any aspect of WIT that must function prior to witInitialize,
// specifically, the message facility and the API.
//------------------------------------------------------------------------------

class WitProblem
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitProblem (WitSession * theSession);
         //
         // mySession_ <-- theSession.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitProblem ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitSession *,              mySession)
      accessFunc (WitCompMgr *,              myCompMgr)
      accessFunc (WitPreprocessor *,         myPreprocessor)
      accessFunc (WitPostprocessor *,        myPostprocessor)
      accessFunc (WitHeurImploder *,         myHeurImploder)
      accessFunc (WitEqAllocator *,          myEqAllocator)
      accessFunc (WitHeurAllMgr *,           myHeurAllMgr)
      accessFunc (WitDetOptImpMgr *,         myDetOptImpMgr)
      accessFunc (WitStochImpMgr *,          myStochImpMgr)
      accessFunc (WitPipMgr *,               myPipMgr)
      accessFunc (WitOrigMrpExp *,           myOrigMrpExp)
      accessFunc (WitFSS *,                  myFSS)
      accessFunc (WitDataReader *,           myDataReader)
      accessFunc (WitDataWriter *,           myDataWriter)
      accessFunc (WitSolnWriter *,           mySolnWriter)
      accessFunc (WitMsgFacility &,          myMsgFac)
      accessFunc (WitRun *,                  myWitRun)
      accessFunc (WitApiMgr *,               myApiMgr)
      accessFunc (FILE *,                    solnFile)
      accessFunc (int,                       nPeriods)
      accessFunc (WitPeriod,                 lastPeriod)
      accessFunc (bool,                      nPeriodsWasSet)
      accessFunc (WitObjLinkMgr *,           myObjLinkMgr)
      accessFunc (WitPairLinkMgr <int> *,    myIntPairLinkMgr)
      accessFunc (WitPairLinkMgr <bool> *,   myBoolPairLinkMgr)
      accessFunc (WitPairLinkMgr <double> *, myDblPairLinkMgr)
      accessFunc (WitPairLinkMgr <void *> *, myVoidPtrPairLinkMgr)
      accessFunc (WitTripLinkMgr *,          myTripLinkMgr)
      accessFunc (WitVariant *,              optVariant)
      accessFunc (WitVariant *,              extOptVariant)
      accessFunc (WitVariant *,              heurVariant)
      accessFunc (WitVariant *,              heurOptVariant)
      accessFunc (WitVariant *,              inputVariant)
      accessFunc (WitVariant *,              currentVariant)
      accessFunc (WitFlexVAsst *,            scalarFlexVAsst)
      accessFunc (WitFlexVAsst *,            vectorFlexVAsst)

      inline WitGlobalComp * myGlobalComp ()
         {
         return myGlobalComp_;
         }

      inline WitOptComp * myOptComp ()
         {
         return myOptComp_;
         }

      bool wit34Compatible ();
         //
         // Returns myCompMgr ()->myGlobalComp ()->wit34Compatible ();

      WitHeurModifier * myHeurModifier ();
         //
         // Wrapper for myHeurAllMgr_.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void store (WitVariant * theVariant);
         //
         // Stores theVariant in this Problem.

      void setUpVariants ();
         //
         // Sets up the Variants for this Problem.

      void makeCurrent (WitVariant * theVariant);
         //
         // Sets currentVariant to theVariant.

      void allocTVecs ();
         //
         // Allocates various TVecs, if this has not already been done.
         // This is done when the first Part or Operation is constructed, at
         // which point nPeriods becomes frozen.

      void clearTVecs ();
         //
         // Clears various TVecs, if this has not already been done.
         // This is done when the # of Parts and Operations becomes 0, at which
         // point nPeriods becomes unfrozen.

      void updateNPeriods ();
         //
         // Sets nPeriods_ to the current value in myGlobalComp ().

      void displayData (const char * filename);
         //
         // Displays the input and preprocessing data of this Problem, i.e.,
         // prints it in an easy-to-read format.
         // filename is the name of the file to which it will print.

      void displayGlobalData ();
         //
         // Displays the global input and preprocessing data of this Problem.

      void displayNonGlobalData ();
         //
         // Displays the non-global input and preprocessing data of this
         // Problem.

      void display (WitMsgID theMsgID, const WitDblFlexVec & theFlexVec);
         //
         // Displays theFlexVec, issuing the indicated Msg.

      void display (WitMsgID theMsgID, const WitFlexVec <int> & theFlexVec);
         //
         // Displays theFlexVec, issuing the indicated Msg.

      void display (WitMsgID theMsgID, const WitFlexVec <bool> & theFlexVec);
         //
         // Displays theFlexVec, issuing the indicated Msg.

      void display (WitMsgID theMsgID, const WitObjVec & theObjVec);
         //
         // Displays theObjVec, issuing the indicated Msg.

      void displayBoundVec (
            WitMsgFragID          theFragID,
            const WitDblFlexVec & theFlexVec);
         //
         // Displays theFlexVec as a BoundSet vector indicated by theFragID.

      void copyProblemInto (WitProblem * theProblem);
         //
         // Copies the input state of this Problem into theProblem: the
         // Components and the input attributes. Preprocesses this Problem
         // and puts theProblem into an unpreprocessed state. Preprocessing
         // This Problem is done to check for errors. The Components of the
         // Problem are constructed in the same order as the order in which the
         // Components of this Problem were contructed.

      void prepSetUnpreAttr ();
         //
         // Takes action appropriate just before the value of an attribute is to
         // be set, thereby necessitating unpreprocessing.
         // The attribute is not allowed to be set in stochastic mode.

      void prepSetUnpostAttr ();
         //
         // Takes action appropriate just before the value of an attribute is to
         // be set, thereby necessitating unpostprocessing.
         // The attribute is not allowed to be set in stochastic mode.

      void prepSetNonUnpostAttr ();
         //
         // Takes action appropriate just before the value of an attribute is to
         // be set without necessitating unpreprocessing or unpostprocessing.
         // The attribute is not allowed to be set in stochastic mode.

      void prepSetStochModeAttr (const char * theAttName);
         //
         // Takes action appropriate just before the value of an attribute is to
         // be set, thereby necessitating unpostprocessing.
         // The attribute is allowed to be set in stochastic mode.
         // theAttName is the name of the attribute.

      void resetSoln ();
         //
         // Puts this Problem into a state reflecting that the data has been
         // changed in a way that would invalidate the current solution, in any.

      void writeSoln (const char * fileName);
         //
         // Prints the comprehensive implosion solution.
         // fileName is the name of the file to be written to.

      void writeSolnValue (double val);
         //
         // Writes val to solnFile (), or blank, if val would print as 0.

      void writeMrpSoln (const char * fileName);
         //
         // Prints the comprehensive MRP solution.
         // fileName is the name of the file to be written to.

      void computeFssIfNeeded ();
         //
         // Computed FSS it hasn't already been computed.
         // It is an error to call this function when postprocessed () is false

      void pauseForInput (const char * theText);
         //
         // Pauses execution and waits for user input.
         // Displays theText.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitProblem);

      void writeStochSoln ();
         //
         // Prints the stochastic comprehensive implosion solution.

      void writeSolnInt ();
         //
         // Prints the comprehensive implosion solution.
         // Internal version.

      void writeBoundsSoln ();
         //
         // Prints the comprehensive implosion solution for BoundSets.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSession * const mySession_;
         //
         // The Session that owns this Problem.

      WitCompMgr * myCompMgr_;
         //
         // The CompMgr owned by this Problem.

      //------------------------------------------------------------------------
      // Singleton objects.
      // Each of these objects has a one-to-one relationship with this Problem.
      // The Problem delegates some of its responsibilities to each of them.
      // It creates and destroys them when it is created and destroyed.
      //------------------------------------------------------------------------

      WitPreprocessor  * myPreprocessor_;
      WitPostprocessor * myPostprocessor_;
      WitHeurImploder  * myHeurImploder_;
      WitEqAllocator   * myEqAllocator_;
      WitHeurAllMgr    * myHeurAllMgr_;
      WitDetOptImpMgr  * myDetOptImpMgr_;
      WitStochImpMgr   * myStochImpMgr_;
      WitPipMgr        * myPipMgr_;
      WitOrigMrpExp    * myOrigMrpExp_;
      WitFSS           * myFSS_;
      WitDataReader    * myDataReader_;
      WitDataWriter    * myDataWriter_;
      WitSolnWriter    * mySolnWriter_;

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      WitMsgFacility & myMsgFac_;
         //
         // The MsgFacility for this Problem.

      WitRun * const myWitRun_;
         //
         // The WitRun for this Problem.

      WitApiMgr * const myApiMgr_;
         //
         // The ApiMgr for this Problem.

      FILE * solnFile_;
         //
         // Output file for comprehensive solution.

      WitGlobalComp * myGlobalComp_;
         //
         // Ptr to myCompMgr ()->myGlobalComp ().

      WitOptComp * myOptComp_;
         //
         // Ptr to myCompMgr ()->myOptComp ().

      int nPeriods_;
         //
         // Same value as myCompMgr ()->myGlobalComp ()->nPeriods ().

      WitPeriod lastPeriod_;
         //
         // The last period in this Problem, i.e., nPeriods_ - 1.

      bool nPeriodsWasSet_;
         //
         // true iff the value of nPeriods_ has been set.

      //------------------------------------------------------------------------
      // LinkMgrs.
      //------------------------------------------------------------------------

      WitObjLinkMgr           * myObjLinkMgr_;
      WitPairLinkMgr <int>    * myIntPairLinkMgr_;
      WitPairLinkMgr <bool>   * myBoolPairLinkMgr_;
      WitPairLinkMgr <double> * myDblPairLinkMgr_;
      WitPairLinkMgr <void *> * myVoidPtrPairLinkMgr_;
      WitTripLinkMgr          * myTripLinkMgr_;

      //------------------------------------------------------------------------
      // Variants.
      //------------------------------------------------------------------------

      WitVariant * optVariant_;
         //
         // The version of the problem solved by optimizing implosion.

      WitVariant * extOptVariant_;
         //
         // The version of the problem solved by external optimizing implosion.

      WitVariant * heurVariant_;
         //
         // The version of the problem solved by heuristic implosion.

      WitVariant * heurOptVariant_;
         //
         // The version of the problem solved by heuristic implosion when used
         // as an initial solution to optimizing implosion.

      WitVariant * inputVariant_;
         //
         // The version of the problem considered when the solution is from
         // the input data.

      WitList <WitVariant> allVariants_;
         //
         // The list of all possible Variants.

      WitVariant * currentVariant_;
         //
         // The version of the implosion problem currently under consideration.

      //------------------------------------------------------------------------
      // FlexVecAssts.
      //------------------------------------------------------------------------

      WitFlexVAsst * scalarFlexVAsst_;
      WitFlexVAsst * vectorFlexVAsst_;
         //
         // The FlexVAsst owned by this Problem for scalar and vector mode.

      //------------------------------------------------------------------------
      // Some useful vectors allocated when the first Part or Operation is
      // constructed.
      //------------------------------------------------------------------------

      WitTVec <double> dispDoubleVector_;
      WitTVec <int>    dispIntVector_;
      WitTVec <bool>   dispBoolVector_;
         //
         // TVecs used by display.
   };

//------------------------------------------------------------------------------
// forEachPeriod
//
// Expands to the header of a "for" loop causing thePer to iterate thru all the
// periods in theProblem.
//------------------------------------------------------------------------------

#define forEachPeriod(thePer, theProblem)                                      \
                                                                               \
   for (thePer = 0; thePer < (theProblem)->nPeriods (); ++ thePer)             \

#endif
