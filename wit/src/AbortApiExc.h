//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef AbortApiExcH
#define AbortApiExcH

#include <typedefs.h>

//------------------------------------------------------------------------------
// Header file: "AbortApiExc.h"
//
// Contains the declaration of class AbortApiExc.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Class AbortApiExc
//
// "Abort API Exception"
// Exceptions of this type are caught at the end of the outer API function that
// was called by the application program and cause it to either:
//   Terminate execution of the program, or
//   Throw a WitException, or
//   Return execution to the application program.
//
// Implemented in MsgFac.C.
//------------------------------------------------------------------------------

class WitAbortApiExc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitAbortApiExc (WitMsg *);

      WitAbortApiExc (const WitAbortApiExc &);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitAbortApiExc ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      int myMsgNum ();
         //
         // The number of the message that caused this exception to be thrown.

      bool stopRunning ();
         //
         // Returns true, iff program process is to be terminated when this
         // exception has been thrown.

      bool throwErrorExc ();
         //
         // Returns true, iff WIT is to throw an ErrorExc when this exception
         // has been thrown.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      WitMsgArg & operator = (const WitMsgArg &);
         //
         // Not implemented.

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMsg * const myMsg_;
         //
         // The message that caused this exception to be thrown.
   };

#endif
