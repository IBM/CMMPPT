//------------------------------------------------------------------------------
// WIT-J C++ Header File Coordinator.h.
//
// Contains the declaration of class Coordinator.
//------------------------------------------------------------------------------

#ifndef Coordinator_h
#define Coordinator_h

#include <JavaAccObj.h>

#include     <string>
#include        <map>

//------------------------------------------------------------------------------
// Class Coordinator
//
// Facilitates interactions between the objects of a Problem.
// Owns the WitRun for a Problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Coordinator
//------------------------------------------------------------------------------

namespace WitJ
{
class Coordinator: public JavaAccObj
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit Coordinator ();
      //
      // Constructor.

   ~Coordinator ();
      //
      // Destructor.
   
   virtual Coordinator * asCoordinator ();
      //
      // Override from class JavaAccObj.

   void store (Problem * theProblem);
      //
      // Stores theProblem as myProblem_.
      // To be called exactly once.

   void mapNameToCaseToken (const char * theName, CaseToken * theCaseToken);
      //
      // Maps theName to theCaseToken.

   void setUpCaseTokens ();
      //
      // Sets the data for each CaseToken.
      // Clears the CaseToken map.

   void recordCreatePartOpn ();
      //
      // Takes action appropriate when a Part or an Operation is created.

   void recordDeletePartOpn ();
      //
      // Takes action appropriate when a Part or an Operation is deleted.

   void extract (bool forReadData);
      //
      // Extracts the state of the WIT problem from the WitRun associated with
      // this Coordinator and copies it into the WIT-J Problem associated with
      // this Coordinator on both the Java side and the C++ side.
      // forReadData is true, iff this is being done for readData.

   void restoreAfterPurge ();
      //
      // Updates the state of WIT-J/C++ to match that of WIT just after a call
      // to witPurgeData.

   void clearData ();
      //
      // Restores this Coordinator to its default state.

   //---------------------------------------------------------------------------
   // populate functions
   //
   // Each of these functions populates the Java ArrayList represented by the
   // CompALJOR with the Components identified by the subsequent arguments, 
   // where theLength is taken to be the length of the list.
   //---------------------------------------------------------------------------

   void populate (
         CompALJOR <Part> &   thePartALJOR,
         const char * const * partNameCVec,
         int                  theLength);

   void populate (
         CompALJOR <Demand> & theDemandALJOR,
         const char * const * partNameCVec,
         const char * const * demandNameCVec,
         int                  theLength);

   void populate (
         CompALJOR <Operation> & theOpnALJOR,
         const char * const *    opnNameCVec,
         int                     theLength);

   //---------------------------------------------------------------------------
   // find{Component Class} functions
   //
   // Each of these functions:
   // * Finds and returns the instance of the indicated Component Class that's
   //   identified by the arguments.
   // * Has WIT verify that such an instance exists.
   //---------------------------------------------------------------------------

   Part *       findPart       (const char * thePartName);
   Demand *     findDemand     (const char * thePartName, 
                                const char * theDemandName);
   Operation *  findOperation  (const char * theOpnName);
   BomEntry *   findBomEntry   (const char * theOpnName,
                                int          theBomIndex);
   Substitute * findSubstitute (const char * theOpnName,
                                int          theBomIndex,
                                int          theSubIndex);
   BopEntry *   findBopEntry   (const char * theOpnName,
                                int          theBopIndex);

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline WitRun * getWitRun ()
      {
      return myWitRun_;
      }

   inline Problem * getProblem ()
      {
      return myProblem_;
      }

   inline PggHandler * getPggHandler ()
      {
      return myPggHandler_;
      }

   inline CTVecSupply * getCTVecSupply ()
      {
      EXCEPTION_ASSERT (myCTVecSupply_ != NULL);

      return myCTVecSupply_;
      }

   inline bool extractInProgress ()
      {
      return extractInProgress_;
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   void setDemPggFunc (const char * theTokenName, DemPggFunc theFunc);
      //
      // Sets the DemPggFunc of the CaseToken whose name matches theTokenName
      // to theFunc.

   void setOpnPggFunc (const char * theTokenName, OpnPggFunc theFunc);
      //
      // Sets the OpnPggFunc of the CaseToken whose name matches theTokenName
      // to theFunc.

   void verifyGlobalUTF8 ();
      //
      // Verifies that all global string attributes are UFT-8.

   void extractGlobalData ();
      //
      // Sets nPeriods as stored by both sides of WIT-J to the value stored in
      // the WitRun.

   void extractNonGlobalComps ();
      //
      // For each non-global Component in the WitRun, creates a matching
      // Component on both sides of WIT-J.

   //---------------------------------------------------------------------------
   // extract{Component Class} functions
   //
   // Each of these functions:
   // * Assumes that object iteration for the WitRun is currently positioned
   //   at an instance of the indicated Component Class.
   // * Creates an instance of the indicated Component Class on the Java side of
   //   of WIT-J and an instance on the C++ side, both matching the WIT object
   //   at which object iteration is currently positioned.
   //---------------------------------------------------------------------------

   void extractPart       ();
   void extractDemand     ();
   void extractOperation  ();
   void extractBomEntry   ();
   void extractSubstitute ();
   void extractBopEntry   ();

   //---------------------------------------------------------------------------
   // restore{Component Class} functions
   //
   // Each of these functions:
   // * Assumes that object iteration for the WitRun is currently positioned
   //   at an instance of the indicated Component Class.
   // * Updates the state of the Component to as appropriate just after a call
   //   to witPurgeData.
   //---------------------------------------------------------------------------

   void restorePart       ();
   void restoreDemand     ();
   void restoreOperation  ();
   void restoreBomEntry   ();
   void restoreSubstitute ();
   void restoreBopEntry   ();

   //---------------------------------------------------------------------------

   void verifyUTF8 (const char * theCString);
      //
      // Verifies that theCString is UFT-8 and throws a PreJavaException if not.
      // The verification is only done during extraction for readData.

   Coordinator              (const Coordinator &);
   Coordinator & operator = (const Coordinator &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   WitRun * myWitRun_;
      //
      // The WitRun owned by this Coordinator.

   Problem * myProblem_;
      //
      // The Problem for this Coordinator.

   PggHandler * myPggHandler_;
      //
      // The PggHandler owned by this Coordinator.

   std::map <std::string, CaseToken *> myCaseTokenMap_;
      //
      // Maps the name of each CaseToken for the Problem to the CaseToken.

   int nPartsOpns_;
      //
      // # Parts + # Operations in the Problem for this Coordinator.

   CTVecSupply * myCTVecSupply_;
      //
      // The CTVecSupply that belongs to this Coordinator, if any;
      // otherwise NULL.
      // Specifically, myCTVecSupply_ is non-NULL, iff at least one Part or
      // Operation exists in the Problem.

   bool extractInProgress_;
      //
      // True, iff an extraction is currently in progress, i.e., iff the
      // extract () function is currently executing.

   bool extractForReadData_;
      //
      // True,  iff an extraction for readData is currently in progress
   };
};
//
// End of namespace WitJ.

#endif
