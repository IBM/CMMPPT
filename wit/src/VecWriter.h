//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef VecWriterH
#define VecWriterH

//------------------------------------------------------------------------------
// Header file: "VecWriter.h"
//
// Contains the declaration of class template VecWriter <ArgVec, ArgScal>.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// Class template VecWriter <ArgVec, ArgScal>
//
// Responsible for writing vectors of some type into the input data file.
//
// Template arguments:
//    ArgVec:  The type of vector to be written
//    ArgScal: The underlying scalar type for ArgVec.
//
// Class Hierarchy:
//
// ProbAssoc
//    VecWriter <*, *>
//------------------------------------------------------------------------------

template <typename ArgVec, typename ArgScal>
      class WitVecWriter: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitVecWriter (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitVecWriter ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void writeDblVector (
            const char *   attrName,
            const ArgVec & theArgVec,
            ArgScal        defaultVal);
         //
         // Same as below, but uses MsgIDs specific to data of type double and
         // distinguishes between the the high and low precision cases.

      void writeVector (
            const char *   attrName,
            const ArgVec & theArgVec,
            ArgScal        defaultVal,
            WitMsgID       singleMsgID,
            WitMsgID       sparseMsgID,
            WitMsgID       denseMsgID);
         //
         // Writes the value of theArgVec to the input data
         // output file, or no-op, if the attribute is at its default value.
         // attrName is the name of the attribute.
         // defaultVal is the default value of the attribute.
         // The last 3 args indicate which Msgs are to be issued.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void writeSingle (
            const ArgVec & theArgVec,
            WitMsgID       theMsgID,
            bool &         vecWritten);
         //
         // Writes the value of theVector to the input data output file, if it
         // is a single replicated value, using theMsgID, else no-op.
         // On return, vecWritten is true, iff the write happened.

      void writeSparse (
            const ArgVec & theArgVec,
            ArgScal        defaultVal,
            WitMsgID       theMsgID,
            bool &         vecWritten);
         //
         // Writes the value of theVector to the input data output file, if it
         // is sparse relative to defaultVal, using theMsgID, else no-op.
         // On return, vecWritten is true, iff the write happened.

      void writeDense (const ArgVec & theArgVec, WitMsgID theMsgID);
         //
         // Writes the value of theVector to the input data output file, in
         // dense format, using theMsgID.

      noCopyCtorAssign (WitVecWriter);
   };

#endif
