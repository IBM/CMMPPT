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
