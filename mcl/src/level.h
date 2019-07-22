//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

#ifndef mcl_level_h
#define mcl_level_h

//-----------------------------------------------------------------------------
// File level.h
//
// Contains the declaration of the following classes:
//
// Level
//-----------------------------------------------------------------------------

#include <mcl/src/defs.h>

#include <stddef.h>

//-----------------------------------------------------------------------------
// Forward declarations.
//-----------------------------------------------------------------------------

class MclFacility;

//-----------------------------------------------------------------------------
// class Level.
//
// The severity level of a Msg.
// The ctor for this class is declared private: The only instances of this
// class that are allowed to exist are created by the Level class itself and
// accessable thru the "Level instance access functions".
//-----------------------------------------------------------------------------

class MclLevel
   {
   public:

      //-----------------------------------------------------------------------
      // Access functions
      //-----------------------------------------------------------------------

      inline char flag () const
         {
         return flag_;
         }

      inline int severity () const
         {
         return severity_;
         }

      //-----------------------------------------------------------------------
      // Level instance access functions.
      // Each of these functions returns a reference to one of the Level
      // instances defined by this class.
      //-----------------------------------------------------------------------

      inline static const MclLevel & info ()
         {
         reqSetUpDone ();

         return * infoPtr_;
         }

      inline static const MclLevel & warning ()
         {
         reqSetUpDone ();

         return * warningPtr_;
         }

      inline static const MclLevel & error ()
         {
         reqSetUpDone ();

         return * errorPtr_;
         }

      inline static const MclLevel & severe ()
         {
         reqSetUpDone ();

         return * severePtr_;
         }

      inline static const MclLevel & fatal ()
         {
         reqSetUpDone ();

         return * fatalPtr_;
         }

      //-----------------------------------------------------------------------
      // Comparison operators.
      //-----------------------------------------------------------------------

      inline MclBoolean operator== (const MclLevel & theLevel) const
         {
         return (severity_ == theLevel.severity_);
         }

      inline MclBoolean operator!= (const MclLevel & theLevel) const
         {
         return (severity_ != theLevel.severity_);
         }

      inline MclBoolean operator<= (const MclLevel & theLevel) const
         {
         return (severity_ <= theLevel.severity_);
         }

      inline MclBoolean operator>= (const MclLevel & theLevel) const
         {
         return (severity_ >= theLevel.severity_);
         }

      inline MclBoolean operator< (const MclLevel & theLevel) const
         {
         return (severity_ < theLevel.severity_);
         }

      inline MclBoolean operator> (const MclLevel & theLevel) const
         {
         return (severity_ > theLevel.severity_);
         }
     
      //-----------------------------------------------------------------------

      static void setUpClass ();
         //
         // Does initial set up of this class the first time it is called
         // during the program process.

   private:

      MclLevel             (const MclLevel &);
      MclLevel & operator= (const MclLevel &);
         //
         // Copy ctor and assignment are declared private and not implemented
         // so as to prevent inadvertant copying and assignment.

      MclLevel (char levelFlag, int severityVal);
         //
         // Constructor.
         // flag_     <-- levelFlag
         // severity_ <-- severityVal

      ~MclLevel ();
         //
         // Destructor
         // It is not implemented, because the instances of this class are never
         // destructed.

      inline static void reqSetUpDone ()
         {
         if (! setUpDone_)
            setUpNotDone ();
         }

      static void setUpNotDone ();
         //
         // Issues a client error indicating that a Level instance access
         // function was called when the Facility has not yet been called.

      //-----------------------------------------------------------------------
      // Data.
      //-----------------------------------------------------------------------

      const char flag_;
         //
         // This is the severity flag for that level

      const int severity_;
         //
         // Severity number.

      //-----------------------------------------------------------------------
      // Pointers to the only instances of class Level that are allowed to
      // exist.
      // They are constructed by the first call to setUpClass and never
      // destructed.
      //-----------------------------------------------------------------------

      static const MclLevel *    infoPtr_;
      static const MclLevel * warningPtr_;
      static const MclLevel *   errorPtr_;
      static const MclLevel *  severePtr_;
      static const MclLevel *   fatalPtr_;

      //-----------------------------------------------------------------------

      static bool setUpDone_;
         //
         // True, iff setUpClass () has been called at least once during the
         // program process.
   };

#endif
