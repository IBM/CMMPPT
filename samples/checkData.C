//------------------------------------------------------------------------------
// C++ Program "checkData.C"
//
// This program is intended as a demonstration of how to catch an exception
// thrown by WIT.
// The program takes a file name as its command line argument and displays
// text indicating whether or not the file is a valid WIT data file.
//------------------------------------------------------------------------------

#include <wit.h>

#include <string>
#include <iostream>

//------------------------------------------------------------------------------

void invokeWit (const std::string & fileName);

void handleException (
   const std::string & fileName,
   WitException &      theWitExc);

//------------------------------------------------------------------------------

int main (int argc, char * argv[])
   {
   std::string fileName;

   if (argc != 2)
      {
      std::cerr
         << "ERROR: Exactly one argument must be passed to checkData."
         << std::endl
         << std::endl;

      exit (1);
      }

   fileName = argv[1];

   try
      {
      invokeWit (fileName);
      }

   catch (WitException & theWitExc)
      {
      handleException (fileName, theWitExc);
      }

   std::cout
      << std::endl
      << "File \""
      << fileName
      << "\" is a valid WIT data file."
      << std::endl
      << std::endl;

   exit (0);
   }

//------------------------------------------------------------------------------

void invokeWit (const std::string & fileName)
   {
   WitRun * theWitRun;

   witNewRun (& theWitRun);

   remove ("wit.msg");

   witSetMesgFileName (
      theWitRun,
      WitTRUE,
      "wit.msg");

   witSetMesgThrowException (
      theWitRun,
      WitFALSE,
      WitSEVERE_MESSAGES,
      WitTRUE);

   witSetMesgThrowException (
      theWitRun,
      WitFALSE,
      WitFATAL_MESSAGES,
      WitTRUE);

   witInitialize (theWitRun);

   witReadData (theWitRun, fileName.c_str ());

   witDeleteRun (theWitRun);
   }

//------------------------------------------------------------------------------

void handleException (
      const std::string & fileName,
      WitException &      theWitExc)
   {
   std::string throwerName;

   throwerName = theWitExc.funcName;

   if (throwerName == "witReadData")
      if (theWitExc.retCode == WitSEVERE_RC)
         {
         std::cout
            << std::endl
            << "File \""
            << fileName
            << "\" is a NOT valid WIT data file."
            << std::endl
            << std::endl;

         exit (0);
         }

   std::cerr
      << std::endl
      << "checkData is terminating due to a programming error."
      << std::endl
      << std::endl;

   exit (1);
   }
