//------------------------------------------------------------------------------
// WIT-J C++ Header File Operation.h.
//
// Contains the declaration of class Operation.
//------------------------------------------------------------------------------

#ifndef Operation_h
#define Operation_h

#include <Component.h>

#include    <string>

//------------------------------------------------------------------------------
// Class Operation
//
// Represents an operation in an implosion problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Component
//       Operation
//------------------------------------------------------------------------------

namespace WitJ
{
class Operation: public Component
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit Operation (Coordinator * theCoord, const char * theOpnNameCStr);
      //
      // Constructor.

   ~Operation ();
      //
      // Destructor.

   virtual Operation * asOperation ();
      //
      // Override from class JavaAccObj.

   virtual void setBoundSet (
         const double *,
         const double *,
         const double *);
      //
      // Override from class Component.

   virtual void getBoundSet (
         DblArrayJOR & hardLBAJOR,
         DblArrayJOR & softLBAJOR,
         DblArrayJOR & hardUBAJOR);
      //
      // Override from class Component.

   virtual void copyComponentData (Component *);
      //
      // Override from class Component.

   void setOperationName (const char *);
      //
      // Sets the OperationName of this Operation within the Operation itself
      // and in WIT.

   virtual void getPegging (CaseToken *, int, PeggingTripleALJOR &);
      //
      // Override from class Component.

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   const char * getOperationName ();

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   Operation              (const Operation &);
   Operation & operator = (const Operation &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   std::string myOperationName_;
      //
      // The OperationName of this Operation.
   };
};
//
// End of namespace WitJ.

#endif
