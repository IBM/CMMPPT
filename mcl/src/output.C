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
// File output.C
//
// Contains the implementation of the following classes:
//
// OutputDisplayer
//    PrintfOutputDisplayer
//-----------------------------------------------------------------------------

#include <mcl/src/output.h>
#include <mcl/src/facility.h>

#include <iostream>

#include <stdlib.h>
#include <string.h>


MclOutputDisplayer::~MclOutputDisplayer()
{
}

MclPrintfOutputDisplayer::MclPrintfOutputDisplayer(
                const MclPrintfOutputDisplayer& output):
    fileName_       (output.fileName_),
    fileAccessMode_ (output.fileAccessMode_)
{
  openFile();
}

MclPrintfOutputDisplayer::MclPrintfOutputDisplayer(
               const char* fn, const char * fileAccessMode)
    :
    filePtr_        (0)
{
  MclFacility::forbidNullArg (
      fn,
     "fn",
     "MclPrintfOutputDisplayer::MclPrintfOutputDisplayer");

   MclFacility::forbidNullArg (
      fileAccessMode,
      "fileAccessMode",
     "MclPrintfOutputDisplayer::MclPrintfOutputDisplayer");

  fileName_       = fn;
  fileAccessMode_ = fileAccessMode;

  openFile();
}

MclPrintfOutputDisplayer::~MclPrintfOutputDisplayer()
{
   closeFile();
}

void MclPrintfOutputDisplayer::output(const char * finalText)
{
   
   if (filePtr_)
       {
         size_t len = strlen(finalText);
         fwrite( finalText, len, 1, filePtr_ );
       }
}

void MclPrintfOutputDisplayer::outputError(const char * finalText)
{
  if (filePtr_)
    printf("%s", finalText);
}

void MclPrintfOutputDisplayer::fileName(const char* fn)
{
  MclFacility::forbidNullArg (fn, "fn", "MclPrintfOutputDisplayer::fileName");
  fileName_ = fn;
  openFile();
}

void MclPrintfOutputDisplayer::fileAccessMode(const char* mode)
{
  MclFacility::forbidNullArg (
       mode,
      "mode",
      "MclPrintfOutputDisplayer::fileAccessMode");

  fileAccessMode_ = mode;
  openFile();
}

void MclPrintfOutputDisplayer::openFile()
{
  closeFile();
  filePtr_ = fopen( fileName_.c_str(), fileAccessMode_.c_str());
  if (!filePtr_ )
    {
      printf("can't open file %s\n", fileName_.c_str());
      exit(1);
    }
}

void MclPrintfOutputDisplayer::closeFile()
{
  if (filePtr_)
      fclose( filePtr_ );
  filePtr_ = 0;
}
