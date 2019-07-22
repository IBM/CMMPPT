//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
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
