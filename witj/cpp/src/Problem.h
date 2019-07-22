//------------------------------------------------------------------------------
// WIT-J C++ Header File Problem.h.
//
// Contains the declaration of class Problem.
//------------------------------------------------------------------------------

#ifndef Problem_h
#define Problem_h

#include <Component.h>

//------------------------------------------------------------------------------
// Class Problem
//
// An instance of class Problem represents an implosion problem and corresponds
// to a WitRun.
// The public member functions of this class mostly correspond to public methods
// of Java class Problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Component
//       Problem
//------------------------------------------------------------------------------

namespace WitJ
{
class Problem: public Component
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit Problem (Coordinator *);
      //
      // Constructor.

   ~Problem ();
      //
      // Destructor.
   
   virtual Problem * asProblem ();
      //
      // Override from class JavaAccObj.

   virtual void copyComponentData (Component *);
      //
      // Override from class Component.

   void callSimpleWitFunc (CaseToken * theFuncCT);
      //
      // Calls the simple WIT API function indicated by theFuncCT.

   void writeSched (
         CaseToken *  theSchedIDCT,
         const char * fileName,
         CaseToken *  theFormatCT);
      //
      // Writes a schedule indicated by theSchedIDCT to a file whose name is
      // given by fileName, using the file format indicated by theFormatCT.

   void setNPeriods (int theInt);
      //
      // Wrapper for witSetNPeriods.

   int getNPeriods ();
      //
      // Returns nPeriods.

   void clearData ();
      //
      // Restores this Problem to its default state.

   void performAction (const char * theCommand);
      //
      // Performs the action indicated by theCommand.

   bool throwUnexpException ();
      //
      // Throws an exception of a type not expected by WIT-J.
      // Used for testing.
      // A special function that returns a bool is used for this case, in order
      // to test a canonical try block whose return statement returns a value.

   //---------------------------------------------------------------------------
   // Wrappers for WIT API Functions.
   //---------------------------------------------------------------------------

   void addDblCplexParSpec   (const char *            theName,
                              double                  theValue);

   void addIntCplexParSpec   (const char *            theName,
                              int                     theValue);

   void copyData             (Problem *               origProblem);

   void displayData          (const char *            fileName);

   void eqHeurAlloc          (DemandALJOR &           theDemandALJOR,
                              IntALJOR &              shipPeriodALJOR,
                              DblALJOR &              desIncVolALJOR,
                              DblALJOR &              incVolALJOR);

   void getCriticalList      (CompALJOR <Part> &      critPartALJOR,
                              IntALJOR &              critPeriodALJOR);

   void getDblCplexParSpec   (const char *            theName,
                              bool &                  dblSpecExists,
                              double &                dblValue);

   void getExpCycle          (CompALJOR <Part> &      thePartALJOR,
                              CompALJOR <Operation> & theOpnALJOR);

   void getIntCplexParSpec   (const char *            theName,
                              bool &                  intSpecExists,
                              int &                   intValue);

   void getObjectiveList     (StringALJOR &           theObjNameALJOR);

   void getOptInitMethodName (StringJOR &             theMethodNameSJOR);

   void getPgdCritList       (CompALJOR <Part> &      critPartALJOR,
                              IntALJOR &              critPeriodALJOR,
                              CompALJOR <Demand> &    theDemandALJOR,
                              IntALJOR &              shipPeriodALJOR);

   void getPipSeq            (PeggingTripleALJOR &    theDemandPTALJOR);

   void getSortedOperations  (CompALJOR <Operation> & theOpnALJOR);

   void getSortedParts       (CompALJOR <Part> &      thePartALJOR);

   void readData             (const char *            fileName);

   void setObjectiveList     (StringALJOR &           theObjNameALJOR);

   void setOptInitMethod     (CaseToken *             theMethodCT);

   void writeCriticalList    (const char *            fileName,
                              CaseToken *             theFormatCT,
                              int                     maxListLen);

   void writeData            (const char *            fileName);

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   void exhaustMemory ();
      //
      // Repeatedly allocates memory in 100MB increments until memory is
      // exhausted.
      // Used for testing.

   Problem              (const Problem &);
   Problem & operator = (const Problem &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   int nPeriods_;
      //
      // Cached copy of the value stored in WIT.
   };
};
//
// End of namespace WitJ.

#endif
