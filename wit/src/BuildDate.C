//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "BuildDate.C"
//
// Contains the implementation of NonClass function buildDate ().
//------------------------------------------------------------------------------

#include <Session.h>

//------------------------------------------------------------------------------
// NonClass function buildDate ()
//
// Declared in Session.h.
//------------------------------------------------------------------------------

const char * WitNonClass::buildDate ()
   {
   return __DATE__;
   }
