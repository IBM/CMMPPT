//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef ApiCallH
#define ApiCallH

//------------------------------------------------------------------------------
// Header file: "ApiCall.h"
//
// Contains the declaration of the following classes:
//
//    ApiCall
//    GlobalCompApiCall
//    OptCompApiCall
//    StochApiCall
//    PartApiCall
//    MaterialApiCall
//    DemandApiCall
//    OperationApiCall
//    BomEntryApiCall
//    SubEntryApiCall
//    BopEntryApiCall
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class ApiCall
//
// Responsible for setting up an API function call and cleaning up afterwards.
//
// ProbAssoc
//    ApiCall
//       GlobalCompApiCall
//       OptCompApiCall
//       StochApiCall
//       PartApiCall
//       MaterialApiCall
//       DemandApiCall
//       OperationApiCall
//       BomEntryApiCall
//       SubEntryApiCall
//       BopEntryApiCall
//------------------------------------------------------------------------------

class WitApiCall: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      inline bool argsOK ()
         {
         witAssert (prepSetCalled_);

         return argsOK_;
         }

      //------------------------------------------------------------------------
      // prep<AttributeKind> functions.
      //
      // Each of these functions takes the appropriate API action in preparation
      // for setting the value of a specific kind of attribute.
      // Typically, a prep function has the following arguments:
      // * theAttName (usually)
      // * oldValue (sometimes)
      // * theValue
      //
      // * Sets myInputID ()->myAttName_ to theAttName.
      // * On return, sets myInputID ()->myAttName_ to NULL.
      // * Verifies that theValue is a valid value for the attribute.
      // * If theValue is valid:
      //      Issues an attribute-setting info msg.
      // * If theValue is not valid:
      //      Issues a severe Msg (possibly severe-after-input).
      // oldValue is the old value of the attribute, which is used in the
      // attribute-setting msg.
      //------------------------------------------------------------------------

      void prepBool (
            const char * theAttName,
            bool         oldValue,
            WitBoolean   theValue);
         //
         // Prep for a Bool attribute.

      void prepInt (
            const char * theAttName,
            int          oldValue,
            int          theValue);
         //
         // Prep for an int attribute.

      void prepNonNegInt (
            const char * theAttName,
            int          oldValue,
            int          theValue);
         //
         // Prep for a non-negative int attribute.

      void prepImmPosInt (
            const char * theAttName,
            int          oldValue,
            int          theValue);
         //
         // Prep for a positive int attribute.
         // Violation causes immediate abort.

      void prepRangedInt (
            const char * theAttName,
            int          oldValue,
            int          theValue,
            int          lb,
            int          ub);
         //
         // Prep for an int attribute in the range [lb, ub].

      void prepImmRangedInt (
            const char * theAttName,
            int          oldValue,
            int          theValue,
            int          lb,
            int          ub);
         //
         // Prep for an int attribute in the range [lb, ub].
         // Violation causes immediate abort.

      void prepDouble (
            const char * theAttName,
            double       oldValue,
            double       theValue);
         //
         // Prep for an unconstrained double attribute.

      void prepNonNegDouble (
            const char * theAttName,
            double       oldValue,
            double       theValue);
         //
         // Prep for a non-negative double attribute.

      void prepLBDouble (
            const char * theAttName, 
            double       oldValue, 
            double       theValue,
            double       lb,
            double       msgLB);
         //
         // Prep for a double attribute that must be >= lb.
         // msgLB is the lower bound to be displayed in any error message.

      void prepNonNegUBDouble (
            const char * theAttName, 
            double       oldValue, 
            double       theValue,
            double       ub,
            double       msgUB);
         //
         // Prep for a double attribute that must be in the range [0, ub].
         // msgUB is the upper bound to be displayed in any error message.

      void prepPosDouble (
            const char * theAttName,
            double       oldValue,
            double       theValue);
         //
         // Prep for a positive double attribute.

      void prepBoolVec (
            const char *       theAttName,
            const WitBoolean * theValue);
         //
         // Prep for a bool vector attribute.

      void prepIntVec (
            const char * theAttName,
            const int *  theValue);
         //
         // Prep for an unconstrained int vector attribute.

      void prepDblVec (
            const char *         theAttName,
            const WitIVRealArg & theValue);
         //
         // Prep for an unconstrained double vector attribute.

      void prepNonNegDblVec (
            const char *         theAttName,
            const WitIVRealArg & theValue);
         //
         // Prep for a non-negative double vector attribute.

      void prepBuildAheadUB (const int * theValue);
         //
         // Prep for a buildAheadUB attribute.

      void prepLeadTimeUB (const int * theValue);
         //
         // Prep for a leadTimeUB attribute.

      void prepShipLateUB (const int * theValue);
         //
         // Prep for a shipLateUB attribute.

      void prepString (
            const char *      theAttName,
            const WitString & oldValue,
            const char *      theValue);
         //
         // Prep for a String attribute.

      //------------------------------------------------------------------------
      // Attribute setting functions.
      //
      // Each of these functions sets an attribute to the given value for API
      // purposes. Generally, these functions are created for attributes of 
      // base classes, so they can be used multiple times.
      // A fatal error is issued, if this ApiCall is not associated with a
      // Component that has the attribute.
      //------------------------------------------------------------------------

      void setAppData        (void *);
      void setSelForDel      (WitBoolean);
      void setEarliestPeriod (int);
      void setLatestPeriod   (int);
      void setRoutingShare   (const WitIVRealArg &);
      void setOffset         (const WitIVRealArg &);
      void setObjectStage    (int);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void getObjectStage (int * objectStage);
         //
         // apiGet of objectStage for a Node.

      void setBoundSet (
            WitBoundSet *        theBoundSet,
            const WitIVRealArg & hardLBArg,
            const WitIVRealArg & softLBArg,
            const WitIVRealArg & hardUBArg);
         //
         // Sets theBoundSet to the args.
         // Similar to a "prep" functions, but sets the value as well.

      void reqNoPartsOrOpns (const char * theAttName);
         //
         // Issues a severe error if this WitRun contains a Part or Operations.
         // theAttName is the name of the attribute being set.

      void issueSetDblVecMsg (
            const char *  theAttName,
            const float * theFltCVec);
         //
         // Issues the message for setting a double vector attribute.
         // theAttName is the name of the attribute.
         // theFltCVec is the vector to which the attribute is to be set.

      void issueSetDblVecMsg (
            const char *   theAttName,
            const double * theDblCVec);
         //
         // Same as above.

      void issueBoundVecMsg (
            WitBoundSet * theBoundSet,
            const float * theFltCVec,
            WitMsgFragID  theFragID);
         //
         // Issues the message for setting one of the vectors of theBoundSet to
         // theFragID, where theFragID indicates a message fragment indicating
         // the vector being set.

      void issueBoundVecMsg (
            WitBoundSet *  theBoundSet,
            const double * theDblCVec,
            WitMsgFragID   theFragID);
         //
         // Same as above.

   protected:

      //------------------------------------------------------------------------
      // Protected member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitApiCall (WitRun * theWitRun);
         //
         // ProbAssoc <<< theWitRun->myProblem ().

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitApiCall ();

      //------------------------------------------------------------------------
      // Associated Component functions.
      //
      // Each of these functions returns the Component of the indicated class
      // associated with this ApiCall, if any.
      // Issues a fatal error if there is none.
      // The base class implementation issues the error.
      //------------------------------------------------------------------------

      virtual WitComponent * myComponent () = 0;
      virtual WitDelComp *   myDelComp   ();
      virtual WitBillEntry * myBillEntry ();
      virtual WitConsEntry * myConsEntry ();

      //------------------------------------------------------------------------
      // Other protected member functions.
      //------------------------------------------------------------------------

      inline WitInputID * myInputID ();
         //
         // Implemented in the .C file.

      void forbidNullSetArg (const void * theArg);
         //
         // theArg: A pointer that was passed to WIT as an argument to a
         //         API function for setting the value of a vector attribute.
         // If theArg is NULL, this function issues a severe error.

      void errorFound (WitPeriod thePer);
         //
         // Records the fact that an error has been found in thePer.

      void errorFound ();
         //
         // Records the fact that an error has been found.

      void prepSetAttr (const char * theAttName);
         //
         // To be called just prior to setting the value of any non-msg
         // attribute.

      void prepGetPip (int rcpPeriod, const char * theArgName);
         //
         // Prepares a post-implosion pegging API function call.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      static WitProblem * theProblemFor (WitRun * theWitRun);
         //
         // Calls myApiMgr ()->startInnerFunc () and then returns
         // theWitRun->myProblem ().

      void checkPeriodIntVec (const int * theValue);
         //
         // Error checking for an int vector in the range [0, nPeriods - 1].

      void mergeBoundVecs (
            WitDblFlexVec &       mergedVec,
            const WitDblFlexVec & oldVec,
            const WitIVRealArg &  theIVRealArg);
         //
         // Sets mergedVec by merging oldVec and newCVec as BoundSet vectors.

      void prepSetAttrStochMode (const char * theAttName);
         //
         // Called by prepSetAttr in stochastic mode.

      noCopyCtorAssign (WitApiCall);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      bool argsOK_;
         //
         // true, iff no errors have been found in the arguments.

      bool prepSetCalled_;
         //
         // In DEBUG mode, true, iff prepSetAttr has been called.
   };

//------------------------------------------------------------------------------
// class GlobalCompApiCall
//
// Responsible for setting up an API call for a GlobalComp and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       GlobalCompApiCall
//------------------------------------------------------------------------------

class WitGlobalCompApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitGlobalCompApiCall (WitRun * theWitRun);
         //
         // Sets up for an API call for the GlobalComp of theWitRun.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitGlobalCompApiCall ();
         //
         // Cleans up after an API call for the GlobalComp of myWitRun ().

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitGlobalCompApiCall);

      virtual WitComponent * myComponent ();
         //
         // Override from class ApiCall.
   };

//------------------------------------------------------------------------------
// class OptCompApiCall
//
// Responsible for setting up an API call for an OptComp and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       OptCompApiCall
//------------------------------------------------------------------------------

class WitOptCompApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOptCompApiCall (WitRun * theWitRun);
         //
         // Sets up for an API call for the OptComp of theWitRun.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitOptCompApiCall ();
         //
         // Cleans up after an API call for the OptComp of myWitRun ().

      //------------------------------------------------------------------------
      // Other public member functions
      //------------------------------------------------------------------------

      void reqMultiObjMode (const char * theAttName);
         //
         // Issues a severe error if this WitRun is not in multi-obj mode.
         // theAttName is the name of the global attribute being set or
         // retrieved.

      WitMultiObjMgr *   myMultiObjMgr    ();
      WitObjective *     currentObjective ();
      WitCpxParSpecMgr * myCpxParSpecMgr  ();
         //
         // Access wrappers.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOptCompApiCall);

      virtual WitComponent * myComponent ();
         //
         // Override from class ApiCall.
   };

//------------------------------------------------------------------------------
// class StochApiCall
//
// Responsible for setting up an API call for stochastic implosion and cleaning
// up afterwards.
//
// ProbAssoc
//    ApiCall
//       StochApiCall
//------------------------------------------------------------------------------

class WitStochApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitStochApiCall (WitRun * theWitRun);
         //
         // Sets up for an API call for stochastic implosion.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitStochApiCall ();
         //
         // Cleans up after an API call for stochastic implosion.

      //------------------------------------------------------------------------
      // Attribute setting functions.
      //------------------------------------------------------------------------

      void setPeriodStage (const int *);

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitStochApiCall);

      virtual WitComponent * myComponent ();
         //
         // Override from class ApiCall.
   };

//------------------------------------------------------------------------------
// class PartApiCall
//
// Responsible for setting up an API call for a Part and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       PartApiCall
//------------------------------------------------------------------------------

class WitPartApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitPartApiCall (WitRun * theWitRun, const char * partName);
         //
         // Sets up for an API call for a Part in theWitRun identified by
         // partName.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitPartApiCall ();
         //
         // Cleans up after an API call for a Part in myWitRun ().

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitPart *, myPart)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitPartApiCall);

      virtual WitComponent * myComponent ();
      virtual WitDelComp *   myDelComp ();
         //
         // Override from class ApiCall.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitPart * const myPart_;
         //
         // The Part matching the API call.
   };

//------------------------------------------------------------------------------
// class MaterialApiCall
//
// Responsible for setting up an API call for a Material and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       MaterialApiCall
//------------------------------------------------------------------------------

class WitMaterialApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMaterialApiCall (
            WitRun *     theWitRun, 
            const char * partName,
            const char * theAttName);
         //
         // Sets up for an API call for a Material in theWitRun identified by
         // partName and for theAttName.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitMaterialApiCall ();
         //
         // Cleans up after an API call for a Material in myWitRun ().

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void prepBa (
            const char * theAttName,
            bool         oldValue,
            WitBoolean   theValue,
            const char * otherAttr);
         //
         // Prep for buildNstn or buildAsap.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMaterial *, myMaterial)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitMaterialApiCall);

      virtual WitComponent * myComponent ();
      virtual WitDelComp *   myDelComp ();
         //
         // Override from class ApiCall.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMaterial * const myMaterial_;
         //
         // The Material matching the API call.
   };

//------------------------------------------------------------------------------
// class DemandApiCall
//
// Responsible for setting up an API call for a Demand and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       DemandApiCall
//------------------------------------------------------------------------------

class WitDemandApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDemandApiCall (
            WitRun *     theWitRun,
            const char * demandedPartName,
            const char * demandName);
         //
         // Sets up for an API call for a Demand in theWitRun identified by
         // demandedPartName and demandName.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitDemandApiCall ();
         //
         // Cleans up after an API call for a Demand in myWitRun ().

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void prepGetPip (int shipPeriod);
         //
         // Prepares a post-implosion pegging API function call on myDemand.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitDemand *, myDemand)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitDemandApiCall);

      virtual WitComponent * myComponent ();
      virtual WitDelComp *   myDelComp ();
         //
         // Override from class ApiCall.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitDemand * const myDemand_;
         //
         // The Demand matching the API call.
   };

//------------------------------------------------------------------------------
// class OperationApiCall
//
// Responsible for setting up an API call for a Operation and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       OperationApiCall
//------------------------------------------------------------------------------

class WitOperationApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOperationApiCall (WitRun * theWitRun, const char * operationName);
         //
         // Sets up for an API call for a Operation in theWitRun identified by
         // operationName.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitOperationApiCall ();
         //
         // Cleans up after an API call for a Operation in myWitRun ().

      //------------------------------------------------------------------------
      // Attribute setting functions.
      //------------------------------------------------------------------------

      void setYieldRate   (const WitIVRealArg &);
      void setCycleTime34 (const WitIVRealArg &);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void prepGetPip (int execPeriod);
         //
         // Prepares a post-implosion pegging API function call on myOperation.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitOperation *, myOperation)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOperationApiCall);

      virtual WitComponent * myComponent ();
      virtual WitDelComp *   myDelComp ();
         //
         // Override from class ApiCall.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitOperation * const myOperation_;
         //
         // The Operation matching the API call.
   };

//------------------------------------------------------------------------------
// class BomEntryApiCall
//
// Responsible for setting up an API call for a BomEntry and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       BomEntryApiCall
//------------------------------------------------------------------------------

class WitBomEntryApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBomEntryApiCall (
            WitRun *     theWitRun,
            const char * consumingOperationName,
            int          bomEntryIndex);
         //
         // Sets up for an API call for a BomEntry in theWitRun identified by
         // consumingOperationName and bomEntryIndex.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBomEntryApiCall ();
         //
         // Cleans up after an API call for a BomEntry in myWitRun ().

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitBomEntry *, myBomEnt)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitBomEntryApiCall);

      virtual WitComponent * myComponent ();
      virtual WitDelComp *   myDelComp ();
      virtual WitBillEntry * myBillEntry ();
      virtual WitConsEntry * myConsEntry ();
         //
         // Override from class ApiCall.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitBomEntry * myBomEnt_;
         //
         // The BomEntry matching the API call.
   };

//------------------------------------------------------------------------------
// class SubEntryApiCall
//
// Responsible for setting up an API call for a SubEntry and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       SubEntryApiCall
//------------------------------------------------------------------------------

class WitSubEntryApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSubEntryApiCall (
            WitRun *     theWitRun,
            const char * consumingOperationName,
            int          bomEntryIndex,
            int          subEntryIndex);
         //
         // Sets up for an API call for a SubEntry in theWitRun identified by
         // consumingOperationName, bomEntryIndex, and subEntryIndex.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitSubEntryApiCall ();
         //
         // Cleans up after an API call for a SubEntry in myWitRun ().

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitSubEntry *, mySub)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSubEntryApiCall);

      virtual WitComponent * myComponent ();
      virtual WitDelComp *   myDelComp ();
      virtual WitBillEntry * myBillEntry ();
      virtual WitConsEntry * myConsEntry ();
         //
         // Override from class ApiCall.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitSubEntry * mySub_;
         //
         // The SubEntry matching the API call.
   };

//------------------------------------------------------------------------------
// class BopEntryApiCall
//
// Responsible for setting up an API call for a BopEntry and cleaning up
// afterwards.
//
// ProbAssoc
//    ApiCall
//       BopEntryApiCall
//------------------------------------------------------------------------------

class WitBopEntryApiCall: public WitApiCall
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitBopEntryApiCall (
            WitRun *     theWitRun,
            const char * producingOperationName,
            int          bopEntryIndex);
         //
         // Sets up for an API call for a BopEntry in theWitRun identified by
         // producingOperationName and bopEntryIndex.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      virtual ~WitBopEntryApiCall ();
         //
         // Cleans up after an API call for a BopEntry in myWitRun ().

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitBopEntry *, myBopEnt)

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitBopEntryApiCall);

      virtual WitComponent * myComponent ();
      virtual WitDelComp *   myDelComp ();
      virtual WitBillEntry * myBillEntry ();
         //
         // Override from class ApiCall.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitBopEntry * myBopEnt_;
         //
         // The BopEntry matching the API call.
   };

#endif
