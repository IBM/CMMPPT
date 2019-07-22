//------------------------------------------------------------------------------
// WIT-J C++ Header File BopEntry.h.
//
// Contains the declaration of class BopEntry.
//------------------------------------------------------------------------------

#ifndef BopEntry_h
#define BopEntry_h

#include <Component.h>

//------------------------------------------------------------------------------
// Class BopEntry
//
// Represents a BOP entry in an implosion problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Component
//       BopEntry
//------------------------------------------------------------------------------

namespace WitJ
{
class BopEntry: public Component
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit BopEntry (
         Operation * theOpn, 
         int         theBopIdx,
         Part *      thePart);
      //
      // Constructor.

   ~BopEntry ();
      //
      // Destructor.

   virtual BopEntry * asBopEntry ();
      //
      // Override from class JavaAccObj.

   virtual void copyComponentData (Component *);
      //
      // Override from class Component.

   const char * getOperationName ();
      //
      // Access Wrapper from myOpn_;

   void setBopIndex (int);
      //
      // Setter.

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline int getBopIndex ()
      {
      return myBopIndex_;
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   BopEntry              (const BopEntry &);
   BopEntry & operator = (const BopEntry &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   Operation * const myOpn_;
      //
      // The producing Operation for this BopEntry.

   int myBopIndex_;
      //
      // The BOP entry index for this BopEntry.
   };
};
//
// End of namespace WitJ.

#endif
