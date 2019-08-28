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

#ifndef mcl_defs_h
#define mcl_defs_h

//-----------------------------------------------------------------------------
// File defs.h
//
// Contains some definitions.
//-----------------------------------------------------------------------------

#ifndef TRUE

#  define TRUE  1
#  define FALSE 0

#endif

typedef int MclBoolean;

//-----------------------------------------------------------------------------
// The following macros are not used.
// They are included here for compatibility with older MCL application
// programs that may have used them.
//-----------------------------------------------------------------------------

#define DECLSPEC
#define STDCALL

#endif
