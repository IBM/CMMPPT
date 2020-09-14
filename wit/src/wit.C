//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "wit.C"
//
// The WIT Stand-Alone Executable.
//------------------------------------------------------------------------------

#include <wit.h>

#include <stdlib.h>

//------------------------------------------------------------------------------
// Main Program
// argc and argv give the command line arguments.
//------------------------------------------------------------------------------

int main (int argc, char * argv[])
   {
   WitRun *     theWitRun;
   const char * theFileName;

   theFileName = (argc > 1)? argv[1]: "";

   witNewRun       (& theWitRun);

   witExecStandAlone (theWitRun, theFileName);

   witDeleteRun      (theWitRun);

   exit (0);
   }

