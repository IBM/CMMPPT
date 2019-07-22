//------------------------------------------------------------------------------
// WIT-J C++ Header File AttBldr.h.
//
// Contains the declaration of class AttBldr.
//------------------------------------------------------------------------------

#ifndef AttBldr_h
#define AttBldr_h

#include <VectorAtts.h>

#include        <map>
#include     <string>

//------------------------------------------------------------------------------
// Class AttBldr
//
// "Attribute Builder"
// Reponsible for creating, temporarily holding, and then transferring ownership
// of the Atts for a Problem.
//
// Class Hierarchy:
//
// JavaAccObj
//    AttBldr
//------------------------------------------------------------------------------

namespace WitJ
{
class AttBldr: public JavaAccObj
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit AttBldr (Coordinator * theCoord);
      //
      // Constructor.

   virtual ~AttBldr ();
      //
      // Destructor.

   virtual AttBldr * asAttBldr ();
      //
      // Override from class JavaAccObj.

   void acquireAtt (Att * theAtt);
      //
      // Takes ownership of theAtt.
      // (Duplicate names are not allowed.)
      // Verifies that the AttName of theAtt is alphabetically no earlier that
      // that of the previously acquired Att.

   void dupAttError (Att * theAtt);
      //
      // Issues an error message indicating that the scoped name of theAtt
      // duplicates that of a previously acquired Att.

   void outOfOrderError (Att * theAtt);
      //
      // Issues an error message indicating that the AttName of theAtt is
      // alphabetically earlier than that of the previously acquired Att.

   bool hasAttFor (const char * scopedAttNameCStr);
      //
      // Returns true, iff this AttBldr currently owns an Att whose scoped name
      // matches scopedAttNameCStr.

   Att * handOverAttFor (const char * scopedAttNameCStr);
      //
      // If this AttBldr currently owns an Att whose scoped name matches
      // scopedAttNameCStr, that Att is returned and ownership of the Att is
      // transferred to the caller;
      // otherwise an error is issued.

   void getAllAttNames (StringJOR & theNamesJOR);
      //
      // Sets theNamesJOR to a string listing the names of all Atts owned by
      // this AttBldr.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   void buildAtts ();
      //
      // Creates the Atts.
      // The Atts are created in alphabetical order.
      // (This is verified by the code.)

   AttBldr              (const AttBldr &);
   AttBldr & operator = (const AttBldr &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   std::map <std::string, Att *> myAttMap_;
      //
      // Maps the scoped name of each Att owned by this AttBldr to the Att that
      // has that scoped name.

   std::string scopedAttName_;
      //
      // Used by acquireAtt.
      // Stores the scoped name of the attribute.

   Att * mostRecentAtt_;
      //
      // During buildOlds (), mostRecentAtt_ is the most recent Att acquired, if
      // any; otherwise NULL.
   };
};
//
// End of namespace WitJ.

#endif
