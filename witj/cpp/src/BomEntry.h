//------------------------------------------------------------------------------
// WIT-J C++ Header File BomEntry.h.
//
// Contains the declaration of class BomEntry.
//------------------------------------------------------------------------------

#ifndef BomEntry_h
#define BomEntry_h

#include <Component.h>

//------------------------------------------------------------------------------
// Class BomEntry
//
// Represents a BOM entry in an implosion problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Component
//       BomEntry
//------------------------------------------------------------------------------

namespace WitJ
{
class BomEntry: public Component
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit BomEntry (
         Operation *  theOpn, 
         int          theBomIdx,
         Part *       thePart);
      //
      // Constructor.

   ~BomEntry ();
      //
      // Destructor.

   virtual BomEntry * asBomEntry ();
      //
      // Override from class JavaAccObj.

   virtual void copyComponentData (Component *);
      //
      // Override from class Component.

   const char * getOperationName ();
      //
      // Access wrapper from myOpn_;

   void setBomIndex (int);
      //
      // Setter.

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   inline int getBomIndex ()
      {
      return myBomIndex_;
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   BomEntry              (const BomEntry &);
   BomEntry & operator = (const BomEntry &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   Operation * const myOpn_;
      //
      // The consuming Operation for this BomEntry.

   int myBomIndex_;
      //
      // The BOM entry index for this BomEntry.
      // This is the # of BomEntries for myOpn_ that were created before this
      // one and still exist.
   };
};
//
// End of namespace WitJ.

#endif
