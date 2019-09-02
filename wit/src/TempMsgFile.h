//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef TempMsgFileH
#define TempMsgFileH

//------------------------------------------------------------------------------
// Header file: "TempMsgFile.h"
//
// Contains the declaration of class TempMsgFile.
//------------------------------------------------------------------------------

#include <Str.h>

//------------------------------------------------------------------------------
// class TempMsgFile.
// Implements a temporary assignment of the message file to a different file.
// An instance of this class fulfils the following responsibility:
//    * When it is constructed, it assigns the message file to a specified
//      temporary file.
//    * When it is destructed, it restores the message file back to the
//      file to which it was assigned just prior to construction of the
//      WitTempMsgFile.
//
// An instance of this class is intended to be constructed (on the stack) near
// the beginning of any function that has an output file name parameter, where
// the output file is to be written to using the message facility. At the end
// of the function, the instance will be automatically destructed, thereby
// restoring the message file.
//
// Implemented in MsgFac.C.
//------------------------------------------------------------------------------

class WitTempMsgFile
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitTempMsgFile (WitMsgFacility & theMsgFac, const char * theFileName);
         //
         // myMsgFac_ <-- theMsgFac
         // fileName is the name of the file to which the message file is to
         // be temporarily assigned.
         // The body of this ctor performs the temporary assignment of the
         // message file.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitTempMsgFile ();
         //
         // The body of this dtor restores the message file and closes the
         // temporary file.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitTempMsgFile);

      accessFunc (WitMsgFacility &, myMsgFac)

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMsgFacility & myMsgFac_;
         //
         // The MsgFacility for this TempMsgFile.

      bool active_;
         //
         // true, iff a temporary file was actually used.

      FILE * prevFile_;
         //
         // myMsgFac ().myFile () before the temp file was used.

      WitString prevFileName_;
         //
         // myMsgFac ().fileName () before the temp file was used.
   };

#endif
