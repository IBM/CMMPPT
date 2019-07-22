//------------------------------------------------------------------------------
// WIT-J C++ Header File ComponentJOR.h.
//
// Contains the declaration of class ComponentJOR.
//------------------------------------------------------------------------------

#ifndef ComponentJOR_h
#define ComponentJOR_h

#include <JniClient.h>

//------------------------------------------------------------------------------
// Class ComponentJOR
//
// "Component Java Object Representative"
// A C++ representation of a WIT-J/Java Component.
//
// Class Hierarchy:
//
// JniClient
//    ComponentJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class ComponentJOR: public JniClient
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit ComponentJOR (_jobject * theCompJObj, JNIEnv *);
      //
      // Constructor.

   ~ComponentJOR ();
      //
      // Destructor.

   static void setUpJni (JNIEnv *);
      //
      // Sets up the interaction between this class and JNI.

   //---------------------------------------------------------------------------
   // makeJava{Component class} functions.
   //
   // Each of these functions creates a new instance of the indicated Java
   // Component class for the Java equivalent of the arguments.
   //---------------------------------------------------------------------------

   static void makeJavaPart (
         Problem *    theProblem,
         const char * thePartNameCStr,
         bool         isMaterial);

   static void makeJavaDemand (Part * thePart, const char * theDemandNameCStr);

   static void makeJavaOperation (
         Problem *    theProblem,
         const char * theOpnNameCStr);

   static void makeJavaBomEntry   (Operation * theOpn,    Part * thePart);
   static void makeJavaSubstitute (BomEntry *  theBomEnt, Part * thePart);
   static void makeJavaBopEntry   (Operation * theOpn,    Part * thePart);

   //---------------------------------------------------------------------------

   static void setJavaNPeriods (Problem * theProblem, int theValue);
      //
      // Sets nPeriods for the Java Problem corresponding to theProblem to
      // theValue.

   void exhaustJniMemory ();
      //
      // Repeatedly has JNI allocate memory in 100MB increments until memory is
      // exhausted.
      // Used for testing.

   inline _jobject * getComponentJObj ()
      {
      return myComponentJObj_;
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   static void findAllJClasses (JNIEnv *);
      //
      // Finds and stores of the _jclass stored by this class.

   static void findAllJMethodIDs (JNIEnv *);
      //
      // Finds and stores each of the _jmethodIDs stored by this class.

   ComponentJOR              (const ComponentJOR &);
   ComponentJOR & operator = (const ComponentJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   //---------------------------------------------------------------------------
   // _jclasses
   // 
   // Each of the following data members is a _jclass that's a global reference
   // to the WIT-J/Java class indicated by the name.
   //---------------------------------------------------------------------------

   static _jclass *       ourPartJCl_;
   static _jclass *     ourDemandJCl_;
   static _jclass *  ourOperationJCl_;
   static _jclass *   ourBomEntryJCl_;
   static _jclass * ourSubstituteJCl_;
   static _jclass *   ourBopEntryJCl_;

   //---------------------------------------------------------------------------
   // _jmethodIDs.
   //
   // Each of the following data members is the _jmethodID of a WIT-J/Java
   // method whose name and class is indicated by the name of the data member.
   // For example, newInstanceDemandJMeth_ is the _jmethodID of WIT-J/Java
   // method newInstance of class Demand.
   //---------------------------------------------------------------------------

   static _jmethodID *      makeInstancePartJMeth_;
   static _jmethodID *     newInstanceDemandJMeth_;
   static _jmethodID *  newInstanceOperationJMeth_;
   static _jmethodID *   newInstanceBomEntryJMeth_;
   static _jmethodID * newInstanceSubstituteJMeth_;
   static _jmethodID *   newInstanceBopEntryJMeth_;
   static _jmethodID *           setNPeriodsJMeth_;

   //---------------------------------------------------------------------------

   _jobject * myComponentJObj_;
      //          
      // A global reference to the represented Java Component.
   };
};
//
// End of namespace WitJ.

#endif
