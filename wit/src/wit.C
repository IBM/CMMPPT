//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
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
   WitRun * theWitRun;

   witNewRun       (& theWitRun);

   witExecStandAlone (theWitRun, argc, argv);

   witDeleteRun      (theWitRun);

   exit (0);
   }
