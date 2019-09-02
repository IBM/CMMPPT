//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef InputIDH
#define InputIDH

//------------------------------------------------------------------------------
// Header file: "InputID.h"
//
// Contains the declaration of class InputID.
//------------------------------------------------------------------------------

#include <typedefs.h>

//------------------------------------------------------------------------------
// Class InputID
//
// Responsible for identifying API input.
//------------------------------------------------------------------------------

class WitInputID
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitInputID (WitApiMgr * theApiMgr);
         //
         // myApiMgr_ <-- theApiMgr.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitInputID ();

      //------------------------------------------------------------------------
      // Set functions.
      //------------------------------------------------------------------------

      void setMyAttName      (const char *);
      void setMyPart         (WitPart *);
      void setMyOperation    (WitOperation *);
      void setMyDemand       (WitDemand *);
      void setMyBomEntry     (WitBomEntry *);
      void setMySubEntry     (WitSubEntry *);
      void setMyBopEntry     (WitBopEntry *);
      void setArgName        (const char *);
      void setArgIndex       (int);
      void setDataFileName   (const char *);
      void setDataFileLineNo (int);
      void setErrorPeriod    (WitPeriod);

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void idInput ();
         //
         // Identifies the input state of the API for certain error messages.

      inline WitPeriod & errorPeriod ()
         {
         return errorPeriod_;
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitInputID);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitApiMgr * const myApiMgr_;
         //
         // The ApiMgr that owns this InputID.

      //------------------------------------------------------------------------
      // Data identifying the input state.
      //------------------------------------------------------------------------

      const char *   myAttName_;
      WitPart *      myPart_;
      WitOperation * myOperation_;
      WitDemand *    myDemand_;
      WitBomEntry *  myBomEnt_;
      WitSubEntry *  mySub_;
      WitBopEntry *  myBopEnt_;
      const char *   argName_;
      int            argIndex_;
      const char *   dataFileName_;
      int            dataFileLineNo_;
      WitPeriod      errorPeriod_;
   };

#endif
