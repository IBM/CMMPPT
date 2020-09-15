//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

#ifndef DataReadH
#define DataReadH

//------------------------------------------------------------------------------
// Header file: "DataRead.h"
//
// Contains the declaration of class DataReader.
//------------------------------------------------------------------------------

#include <Str.h>
#include <Assoc.h>

//------------------------------------------------------------------------------
// class DataReader
//
// Responsible for reading the input data file.
//------------------------------------------------------------------------------

class WitDataReader: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitDataReader (WitProblem *);

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitDataReader ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void readData (const char * fileName);
         //
         // Reads an input data file for myProblem ().
         // fileName is the name of the file to be written.

      bool active ();
         //
         // Returns true, iff this DataReader is currently running readData ().

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void protectedReadData (const char * fileName);
         //
         // Performs the aspects of readData () that are to be protected by a
         // try block.

      noCopyCtorAssign (WitDataReader);

      //------------------------------------------------------------------------
      // Static private member data.
      //------------------------------------------------------------------------

      static WitDataReader * activeInstance_;
         //
         // The DataReader which is currently running readData (), if any;
         // otherwise NULL.

      static bool errorHasOccurred_;
         //
         // True, iff an error has occurred during readData for any instance of
         // this class during the program process.

      //------------------------------------------------------------------------
      // Non-static private member data.
      //------------------------------------------------------------------------

      WitString inputFileName_;
         //
         // The name of inputFile_.

      FILE * inputFile_;
         //
         // The input data file being read.
   };

#endif
