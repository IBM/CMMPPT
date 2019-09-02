//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef MsgArgH
#define MsgArgH

//------------------------------------------------------------------------------
// Header file: "MsgArg.h"
//
// Contains the declaration of class MsgArg.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------

class MclArg;
class MclFacility;
class MclLevel;
class MclMsgFrag;

//------------------------------------------------------------------------------
// Class MsgArg
//
// Wrapper for class MclArg.
//
// Implemented in MsgFac.C.
//------------------------------------------------------------------------------

class WitMsgArg
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitMsgArg ();
      WitMsgArg (bool                       rawArg);
      WitMsgArg (int                        rawArg);
      WitMsgArg (size_t                     rawArg);
      WitMsgArg (long                       rawArg);
      WitMsgArg (double                     rawArg);
      WitMsgArg (const WitISRealArg &       rawArg);
      WitMsgArg (const char *               rawArg);
      WitMsgArg (const WitString &          rawArg);
      WitMsgArg (const WitMsgFrag &         rawArg);
      WitMsgArg (const int *                rawArg, int length = 1);
      WitMsgArg (const float *              rawArg, int length = 1);
      WitMsgArg (const double *             rawArg, int length = 1);
      WitMsgArg (const WitVector <bool> &   rawArg);
      WitMsgArg (const WitVector <int> &    rawArg);
      WitMsgArg (const WitVector <double> & rawArg);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitMsgArg ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void operator = (const WitMsgArg &);

      inline bool hasArg () const
         {
         return (myMclArgPtr_ != NULL);
         }

      noCopyCtor (WitMsgArg);
         //
         // Public, but not implemented.
         // Making the copy ctor public avoids a syntax error from the
         // MS Visual C++ compiler with the -Za flag.

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      inline const MclArg & myMclArg () const
         {
         witAssert (hasArg ());

         return * myMclArgPtr_;
         }

   private:

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      MclArg * myMclArgPtr_;
         //
         // Pointer to the MclArg wrapped by this MsgArg, if any;
         // otherwise NULL.
   };

#endif
