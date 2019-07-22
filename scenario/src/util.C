#include <assert.h>

#include <scenario/src/util.h>

#include <rw/ctoken.h>


// Utility functions are contained in this file

// ---------------------------------------------------------------------
// LgFrCSV
// Convert a string of tokens to a string of Comma Seperated Variables.
// The input string tokens are identified by white space.
// Example: input  'PartA supplyVol 12.34   567'
//          result '"PartA","supplyVol",12.34,567'
// ---------------------------------------------------------------------
RWCString
LgFrCSV
( const RWCString inputString )
{
  RWCString    retVal, token;
  RWCTokenizer next(inputString);
  double       dblNum;

  // Loop once for each token
  while ( !( token=next() ) . isNull() ) {

    if ( !retVal.isNull() ) retVal.append(",");

    // Is token a number or alpha?
    if ( RWLocale::global().stringToNum( token, &dblNum ) ) {

      // token is a number
      retVal.append( token );

    }
    else {

      // token is an alpha, so enclose it in quotes
      retVal.append( "\"" + token + "\"" );

    }

  }
 return retVal;
}

#ifdef NDEBUG
#undef NDEBUG
#endif
// Test utility functions
void LgFrUtilTest()
{
  // Test LgFrCSV
  {
  assert( LgFrCSV("") == "" );
  assert( LgFrCSV("  123 45.8 9") == "123,45.8,9" );
#ifndef WIN32
  // The Microsoft VC++ compiler cannot handle the string below.
  assert( LgFrCSV("abc  123 45.8 9 def   ") == "\"abc\",123,45.8,9,\"def\"" );
#endif
  }
}

