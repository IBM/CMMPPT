//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef SolnWritH
#define SolnWritH

//------------------------------------------------------------------------------
// Header file: "SolnWrit.h"
//
// Contains the declaration of class SolnWriter.
//------------------------------------------------------------------------------

#include <Assoc.h>

//------------------------------------------------------------------------------
// class SolnWriter.
//
// Responsible for writing solutions to files.
//------------------------------------------------------------------------------

class WitSolnWriter: public WitProbAssoc
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSolnWriter (WitProblem * theProblem);
         //
         // ProbAssoc <<< theProblem.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSolnWriter ();

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void writeCriticalList (
            const char *  fileName,
            WitFileFormat fileFormat,
            int           maxListLen);
         //
         // Writes the critical part list.

      void writeReqSched (const char * fileName, WitFileFormat fileFormat);
         //
         // Prints supply requirements schedule.

      void writeExecSched (
            const char * const fileName,
            const WitFileFormat fileFormat);
         //
         // Prints production schedule.
         // This output consists of one or two sections.
         // Each line of the first section contains the following information:
         //
         //   Operation Name
         //   Period
         //   Execution Volume
         //
         // This section is ordered by Operation and period.
         //
         // A second section is printed iff there are SubEntries. For each
         // SubEntry, it gives the execution volumes due to that SubEntry.
         //
         // After a header line for this section, each line of the second
         // section contains the following information:
         //
         //   Operation Name
         //   Name of Part Replaced
         //   BOM Entry Index
         //   Period
         //   Name of Part Consumed
         //   Substitute Index
         //   Production Volume
         //
         // This section is ordered by:
         //
         //    Operation
         //    BOM entry
         //    Period
         //    Substitute BOM entry

      void writeShipSched (
            const char * const fileName,
            const WitFileFormat fileFormat);
         //
         // Prints the shipment schedule.
         // Each line of output will contain the following information:
         //
         //    Product Name
         //    Demand Stream Name
         //    Period
         //    Shipment Volume
         //
         // Output will be ordered by demand stream and period.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSolnWriter);

      const char * quoteCstring (
            const WitString & inputString,
            WitString &       quotedString);
         //
         // return the input string with quotes added in the buffer
         // named quotedString.  Caller must insure quotedString is
         // large enough to accomodate inputString with the quotes.

      char * formattedDouble (double theDbl, char * theCstring);
         //
         // Formats theDbl for output with outputPrecision digits beyond the
         // decimal point, stores the result in theCstring and returns
         // theCstring.
   };

#endif
