#ifndef LGFR_UTIL_H
#define LGFR_UTIL_H

#include <locale.h>

#include <rw/locale.h>
#include <rw/cstring.h>



// Utility functions are contained in this file

// ---------------------------------------------------------------------
// LgFrCSV
// Convert a string of tokens to a string of Comma Seperated Variables.
// The input string tokens are identified by white space.
// Example: input  'PartA supplyVol 12.34   567'
//          result '"PartA","supplyVol",12.34,567'
// ---------------------------------------------------------------------
RWCString
LgFrCSV ( 
  const RWCString inputString );


void LgFrUtilTest();

#endif
