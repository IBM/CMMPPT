//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef StrH
#define StrH

//------------------------------------------------------------------------------
// Header file: "Str.h"
//
// Contains the declaration of class String.
//------------------------------------------------------------------------------

#include <Vector.h>

//------------------------------------------------------------------------------
// class String.
// A character string.
// Implemented in terms of a Vector (char)
// Implemented in Vecs.C.
//------------------------------------------------------------------------------

class WitString
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitString ();
         //
         // Constructs a null String.

      explicit WitString (const char * theCstring);
         //
         // Constructs a copy of theCstring.

      explicit WitString (int nBlanks);
         //
         // Constructs a string containing nBlanks blanks.

      explicit WitString (const WitString &);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitString ();

      //------------------------------------------------------------------------
      // Public member operators.
      //------------------------------------------------------------------------

      inline char operator [] (int theIndex) const
            //
            // Returns the character element of this String.
            // Bounds checking is performed (by WIT), iff the compile is in
            // DEBUG  mode.
            // Returned value is not an lvalue.
         {
         return myVector_[theIndex];
         }

      inline char & operator [] (int theIndex)
            //
            // Same as the preceeding operator [], except that this Vector is
            // non-const and the returned value is an lvalue.
         {
         return myVector_[theIndex];
         }

      inline void operator = (const WitString & theString)
         {
         operator = (theString.myCstring ());
         }

      void operator = (const char * theCstring);
         //
         // Copies theCstring into this String.

      inline bool operator == (const char * theCstring) const
         {
         return equalCstrings (myCstring (), theCstring);
         }

      inline bool operator == (const WitString & theString) const
         {
         return ((* this) == theString.myCstring ());
         }

      inline bool operator != (const char *      theCstring) const
         {
         return not ((* this) == theCstring);
         }

      inline bool operator != (const WitString & theString) const
         {
         return not ((* this) == theString);
         }

      bool operator < (const char * theCstring) const;

      inline bool operator < (const WitString & theString) const
         {
         return ((* this) < theString.myCstring ());
         }

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      inline const char * myCstring () const
         {
         return myVector_.myCVec ();
         }

      inline char * myCstringForUpdate ()
            //
            // Returns the char * representation of this String, for updating
            // purposes.
            // Use with discretion.
         {
         return myVector_.myCVecForUpdate ();
         }

      inline bool isNull () const
            //
            // Returns true iff this String is a null (0 length) String.
         {
         return ((* this)[0] == '\0');
         }

      inline int length () const
            //
            // Returns the length of this String.
         {
         return strlen (myVector_.myCVec ());
         }

      void resize (int nChars);
         //
         // Resizes this String to nChars blanks.

      inline int maxLength () const
         {
         return myVector_.length () - 1;
         }

      inline static const WitString & thisString (const WitString * pTheString)
            //
            // Used for List::find ().
         {
         return * pTheString;
         }

      char * apiDupCstring () const;
         //
         // Allocates and returns a char * copy of this String for API purposes.
         // Uses malloc, so the returned value can be passed to the application
         // program.

      void copyInto (char * theCstring) const;
         //
         // Copies this String into theCstring.
         // theCstring must have enough room for the contents of this String.

   private:

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitVector <char> myVector_;
         //
         // The Vector thru which this String is implemented.
         // Its length is normally one greater than the length of this String,
         // in order to store the terminating null character.
   };

//------------------------------------------------------------------------------
// NonClass operators that use Strings.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   inline bool operator == (const char * lhs, const WitString & rhs)
      {
      return (rhs == lhs);
      }

   inline bool operator != (const char * lhs, const WitString & rhs)
      {
      return not (lhs == rhs);
      }
   };

#endif
