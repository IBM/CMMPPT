//------------------------------------------------------------------------------
// WIT-J C++ Header File Substitute.h.
//
// Contains the declaration of class Substitute.
//------------------------------------------------------------------------------

#ifndef Substitute_h
#define Substitute_h

#include <Component.h>

//------------------------------------------------------------------------------
// Class Substitute
//
// Represents a substitute BOM entry in an implosion problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Component
//       Substitute
//------------------------------------------------------------------------------

namespace WitJ
{
class Substitute: public Component
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit Substitute (
         BomEntry *   theBomEnt,
         int          theSubIdx,
         Part *       thePart);
      //
      // Constructor.

   ~Substitute ();
      //
      // Destructor.

   virtual Substitute * asSubstitute ();
      //
      // Override from class JavaAccObj.

   virtual void copyComponentData (Component *);
      //
      // Override from class Component.

   const char * getOperationName ();
      //
      // Access Wrapper from myBomEnt_.

   int getBomIndex ();
      //
      // Access Wrapper from myBomEnt_.

   void setSubIndex (int);
      //
      // Setter.

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline int getSubIndex ()
      {
      return mySubIndex_;
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   Substitute              (const Substitute &);
   Substitute & operator = (const Substitute &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   BomEntry * const myBomEnt_;
      //
      // The BomEntry for which this Substutite is substituting.

   int mySubIndex_;
      //
      // The substitute index for this Substutite.
      // This is the # of Substitutes for myBomEnt_ that were created before
      // this one and still exist.
   };
};
//
// End of namespace WitJ.

#endif
