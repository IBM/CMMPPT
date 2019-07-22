//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

//-----------------------------------------------------------------------------
// File level.C
//
// Contains the implementation of the following classes:
//
// Level
//-----------------------------------------------------------------------------

#include <mcl/src/level.h>
#include <mcl/src/facility.h>

#include <iostream>

#include <assert.h>

//-----------------------------------------------------------------------------
// Implementation of class Level.
//-----------------------------------------------------------------------------

const MclLevel * MclLevel::   infoPtr_ = NULL;
const MclLevel * MclLevel::warningPtr_ = NULL;
const MclLevel * MclLevel::  errorPtr_ = NULL;
const MclLevel * MclLevel:: severePtr_ = NULL;
const MclLevel * MclLevel::  fatalPtr_ = NULL;

bool             MclLevel:: setUpDone_ = false;

//-----------------------------------------------------------------------------

void MclLevel::setUpClass ()
   {
   if (setUpDone_)
      return;

      infoPtr_  = new MclLevel ('I', 0);
   warningPtr_  = new MclLevel ('W', 1);
     errorPtr_  = new MclLevel ('E', 2);
    severePtr_  = new MclLevel ('S', 3);
     fatalPtr_  = new MclLevel ('F', 4);

   setUpDone_   = true;
   }

//-----------------------------------------------------------------------------

MclLevel::MclLevel (char levelFlag, int severityVal):
      flag_     (levelFlag),
      severity_ (severityVal)
   {
   }

//-----------------------------------------------------------------------------

void MclLevel::setUpNotDone ()
   {
   std::cerr <<
      "\n\n"
      "MCL CLIENT ERROR:\n\n"
      "   A Level instance access function was called when \n"
      "   the MclFacility constructor had not yet been called.\n";

   MclFacility::abortMcl ();
   }
