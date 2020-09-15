//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
//
//     Source file:    Parlex.C
//
//     Implementation of classes Parlex and FileStackElement.
//		
//     Contains functions to manipulate global data for witParser,
//     witLexer and other auxiliary classes.
//
//------------------------------------------------------------------------------

#include <Parlex.h>

extern  FILE       *yyin;               /* current input file     */

//------------------------------------------------------------------------------
// Implementation of class FileStackElement.
//------------------------------------------------------------------------------

WitFileStackElement::WitFileStackElement ():
      inputFile_ (NULL),
      lineno_    (0),
      fileName_  ()
   {
   }

WitFileStackElement::~WitFileStackElement ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class WitParlex.
//------------------------------------------------------------------------------

// Initialize static member data
int         WitParlex::lineno_ = 1;
int         WitParlex::errcode_ = 0;
WitString & WitParlex::fileName_ = * new WitString;
int         WitParlex::stackPos_ = 0;

WitFileStackElement WitParlex::fileStack_[STACKSIZE];


// Constructor and destructor

WitParlex::WitParlex()
{
  lineno_   = 1;
  errcode_  = 0;
  fileName_ = "";
}

WitParlex::~WitParlex()
{
}

// Public operations

int WitParlex::lineno()
{ return lineno_; }

int WitParlex::errcode()
{ return errcode_; }

const char* WitParlex::fileName()
{ return fileName_.myCstring (); }

void WitParlex::setLineno(int n)
{ lineno_ = n; }

void WitParlex::setErrcode(int n)
{ errcode_ = n; }

void WitParlex::setFileName(const char *s)
{ fileName_ = s; }

// Save current input-file state and begin reading from new file
WitParseRC  WitParlex::saveFile(const char *s)
{ 
  int   yyleng;
  char  *fn;
  FILE  *fp;

  if (stackPos_+1 == STACKSIZE)
    return STACK_OVERFLOW;

  yyleng = strlen(s);                           // filename, as read
  fn = new char[yyleng-1];                      // get rid of quote marks
  strncpy(fn, s+1, yyleng-2);
  fn[yyleng-2] = '\0';
  if (!(fp = fopen(fn, "r")))
    return FOPEN_ERROR;

  fileStack_[stackPos_].fileName_ = fileName_;  // save current file name

  fileStack_[stackPos_].lineno_ = lineno_;
  fileStack_[stackPos_++].inputFile_ = yyin;
  yyin = fp;
  fileName_ = fn;
  lineno_ = 1;

  delete[] fn;

  return 0;
} 

// Close current input file and restore prvious input-file state
WitParseRC  WitParlex::restoreFile()
{
  if (stackPos_ == 0) return 1;                  // if end of original file 
  fileName_ = fileStack_[--stackPos_].fileName_; // restore lexer state
  lineno_ = fileStack_[stackPos_].lineno_;

  fclose(yyin);
  yyin = fileStack_[stackPos_].inputFile_;
  return 0;
}
