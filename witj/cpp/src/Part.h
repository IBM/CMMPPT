//------------------------------------------------------------------------------
// WIT-J C++ Header File Part.h.
//
// Contains the declaration of class Part.
//------------------------------------------------------------------------------

#ifndef Part_h
#define Part_h

#include <Component.h>

#include       <wit.h>

#include    <string>

//------------------------------------------------------------------------------
// Class Part
//
// Represents a part in an implosion problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    Component
//       Part
//------------------------------------------------------------------------------

namespace WitJ
{
class Part: public Component
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit Part (
         Coordinator * theCoord,
         const char *  thePartNameCStr,
         CaseToken *   thePartCatCT);
      //
      // Constructor.

   ~Part ();
      //
      // Destructor.

   virtual Part * asPart ();
      //
      // Override from class JavaAccObj.

   void getBelowList (CompALJOR <Part> & theBelowALJOR);
      //
      // Retrieves the below list for this Part and stores it in the Java
      // ArrayList represented by theBelowALJOR.

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

   void setPartName (const char *);
      //
      // Sets the PartName of this Part within the Part itself and in WIT.

   //---------------------------------------------------------------------------
   // Accessors.
   //---------------------------------------------------------------------------

   const char * getPartName ();

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   Part              (const Part &);
   Part & operator = (const Part &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   std::string myPartName_;
      //
      // The PartName of this Part.
   };
};
//
// End of namespace WitJ.

#endif
