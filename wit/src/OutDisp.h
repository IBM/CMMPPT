//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef OutDispH
#define OutDispH

//------------------------------------------------------------------------------
// Header file: "OutDisp.h"
//
// Contains the declaration of class OutputDisplayer.
//------------------------------------------------------------------------------

#include <Str.h>

#include <mcl/src/mcl.h>

//------------------------------------------------------------------------------
// Class OutputDisplayer
//
// WIT's specialization of class MclOutputDisplayer.
//
// Implemented in MsgMgr.C.
//------------------------------------------------------------------------------

class WitOutputDisplayer: public MclOutputDisplayer
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitOutputDisplayer (WitMsgMgr * theMsgMgr);
         //
         // myMsgMgr_ <-- theMsgMgr.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitOutputDisplayer ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (FILE *,            myFile)
      accessFunc (const WitString &, fileName)
      accessFunc (const WitString &, accessMode)

      //------------------------------------------------------------------------
      // Data setting functions.
      //------------------------------------------------------------------------

      void setAccessMode (const char *);

      void setMyFile (FILE * theFile, const char * theName);
         //
         // myFile   <-- theFile.
         // fileName <-- theName.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      virtual void output (const char *);
         //
         // Override from class MclOutputDisplayer.

      virtual void outputError (const char *);
         //
         // Override from class MclOutputDisplayer.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitOutputDisplayer);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitMsgMgr * const myMsgMgr_;
         //
         // The MsgMgr for this OutputDisplayer.

      FILE * myFile_;
         //
         // The file to which Msgs will be displayed.

      WitString fileName_;
         //
         // The name of myFile_.

      WitString accessMode_;
         //
         // Access mode used by fopen when opening myFile_.
   };

#endif
