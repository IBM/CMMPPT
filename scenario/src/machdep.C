// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <iostream>
#include <stdio.h>

#if defined (__OS2__) || defined (WIN32) || defined (__WINDOWS__)
#include <io.h>
#else
#include <unistd.h>
#include <strings.h> // for strcasecmp()
#endif

#include <assert.h>

#include <scenario/src/machdep.h>


// Machine dependant code is contained in this file.


// Determine if file exists.
// Return true if file exits, othewise false.
bool
LgFrDoesFileExist(
   const std::string& fileName )
{
  // See if file exists
#if defined (__OS2__) || defined (WIN32) || defined (__WINDOWS__)
  if ( _access( fileName.c_str(), 00 ) == -1 ) return false;
#else
  if ( access( fileName.c_str(), F_OK ) == -1 ) return false;
#endif
  return true;
}

int LgFrStrcmpcaseins( const char* s1, const char* s2 )
{
#if defined (__OS2__) || defined (WIN32) || defined (__WINDOWS__)
    return stricmp( s1, s2 );
#else
    return strcasecmp( s1, s2 );
#endif    
}



// Get a name for a temporary file.
LGFR_DECLSPEC std::string  LGFR_STDCALL
LgFrTempFileName()
{
#if TEMPNAM_AVAILABLE == 1
   char * cName = tempnam( 0, "LgFr" );
   if (cName == 0) {
      perror ("WITUSE: machdep.C, LgFrTempFileName, tempnam error: ");
      exit (1);
   }
   std::string retVal( cName );
   portableFree( cName );
   return retVal;
#else
   char * cName = tmpnam( 0 );
   if (cName == 0) {
      perror ("WITUSE: machdep.C, LgFrTempFileName, tmpnam error: ");
      exit (1);
   }
   std::string retVal( cName );
   return retVal;
#endif
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// Test machine dependant functions
void LgFrMachdepTest()
{
   // assert ( 1 == 0 );         // Test workaround for NDEBUG.  It worked, 8/29/94
   // Test doesFileExist function
   assert( 
         LgFrDoesFileExist("../../../../wit/wit-data/examples/diner/wit.data" ) 
         );
   assert( 
         !LgFrDoesFileExist("../../../../wit/wit-data/examples/diner/XXX.yyyy" ) 
         );

   // Test tempFileName()
   // cout <<LgFrTempFileName() <<endl;
}

