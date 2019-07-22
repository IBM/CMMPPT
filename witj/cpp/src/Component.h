//------------------------------------------------------------------------------
// WIT-J C++ Header File Component.h.
//
// Contains the declaration of class Component.
//------------------------------------------------------------------------------

#ifndef Component_h
#define Component_h

#include <JavaAccObj.h>

//------------------------------------------------------------------------------
// Class Component
//
// Represents an element (i.e. data object) of an implosion problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Component
//       Problem
//       Part
//       Demand
//       Operation
//       BomEntry
//       Substitute
//       BopEntry
//------------------------------------------------------------------------------

namespace WitJ
{
class Component: public JavaAccObj
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   ~Component ();
      //
      // Destructor.

   virtual Component * asComponent ();
      //
      // Override from class JavaAccObj.

   void setComponentJOR (ComponentJOR *);
      //
      // To be called exactly once.

   virtual void setBoundSet (
         const double * hardLBCVec,
         const double * softLBCVec,
         const double * hardUBCVec);
      //
      // If this Component is an instance of a class that has a BoundSet, this
      // function calls WIT to set the value of the BoundSet to the arguments;
      // otherwise an assertion exception is thrown.

   void setBoundSetToScalars (
         double hardLBVal,
         double softLBVal,
         double hardUBVal);
      //
      // If this Component is an instance of a class that has a BoundSet, this
      // function calls WIT to set the value of the BoundSet to the arguments in
      // all periods; otherwise an assertion exception is thrown.

   virtual void getBoundSet (
         DblArrayJOR & hardLBAJOR,
         DblArrayJOR & softLBAJOR,
         DblArrayJOR & hardUBAJOR);
      //
      // If this Component is an instance of a class that has a BoundSet, this 
      // function calls WIT to set the value of the arguments to the BoundSet;
      // otherwise an assertion exception is thrown.

   virtual void copyComponentData (Component * origComp) = 0;
      //
      // Cals WIT to copy this input data from origComp'into this Component.

   void setWasDeletedInWit (bool);
      //
      // Setter.

   virtual void getPegging (
         CaseToken *          thePggCaseCT,
         int                  pegPer,
         PeggingTripleALJOR & theTripALJOR);
      //
      // Retrieves the pegging indicated by the pegging case represented by
      // thePggCaseCT for this Component in pegPer, storing it in the Java
      // ArrayList <PeggingTriple <? extends Component>> represented by
      // theTripALJOR.
      // The default implementation throws an exception.

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline ComponentJOR * getComponentJOR ()
      {
      return myComponentJOR_;
      }

   inline bool wasDeletedInWit ()
      {
      return wasDeletedInWit_;
      }

   //---------------------------------------------------------------------------
   // Protected member functions.
   //---------------------------------------------------------------------------

   protected:

   explicit Component (Coordinator * theCoord);
      //
      // Constructor

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   Component              (const Component &);
   Component & operator = (const Component &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   ComponentJOR * myComponentJOR_;
      //
      // Represents the Java Component that corresponds to this C++ Component.

   bool wasDeletedInWit_;
      //
      // True, iff the WIT element modeled by this WIT-J/C++ Component has been
      // deleted by witPurgeData.
   };
};
//
// End of namespace WitJ.

#endif
