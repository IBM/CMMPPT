//==============================================================================
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//==============================================================================

#ifndef mcl_output_h
#define mcl_output_h

//-----------------------------------------------------------------------------
// File output.h
//
// Contains the declaration of the following classes:
//
// OutputDisplayer
//    PrintfOutputDisplayer
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Notes on Implementation:
//
// * This is the class that serves as an interface between the Message Class
//   Library and the output buffer -- {file,screen}; It writes all output.
//
// * Though there are public methods that allow for manual issuing of
//   of messages, this is not necessary if used with the MclFacility. This
//   is all done internally within MclFacility class.
//
// * Note: the base class MclOutputDisplayer does not have a public
//         constructor. you must either use the MclPrintfOutputDisplayer class
//         or create another derived class of MclOutputDisplayer.
//
// * Also: the MclPrintfOutputDisplayer is missing a default constructor
//         like the MclFacility. if you haven't read the Notes on Imp. for
//         the MclFacility class (in file: facility.h), read them.
//
//-----------------------------------------------------------------------------

#include <mcl/src/defs.h>
#include <string>
#include <stdio.h>

class MclOutputDisplayer
{
public:
  virtual ~MclOutputDisplayer();
  virtual void output(const char* finalText) = 0;
  virtual void outputError(const char * ) {}
protected:
  MclOutputDisplayer(){}
  
};
                                     

class MclPrintfOutputDisplayer : public MclOutputDisplayer
{
public:
  MclPrintfOutputDisplayer(const MclPrintfOutputDisplayer& output);
  MclPrintfOutputDisplayer(const char* fn, const char * fileAccessMode);
  virtual ~MclPrintfOutputDisplayer();

  virtual void output(const char * finalText);
  virtual void outputError(const char * finalText);
  virtual void fileName(const char* fn);
  virtual void fileAccessMode(const char* mode);


// ......
  
protected:
  std::string fileName_;
  std::string fileAccessMode_;
  FILE*         filePtr_;

  void openFile();
  void closeFile();
  
  //...
};

#endif
