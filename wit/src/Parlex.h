//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef ParlexH
#define ParlexH

/*------------------------------------------------------------------------------
 *
 *     Source file:    Parlex.h
 *
 *     Contains global data for WitParser, WitLexer and other classes
 *     that are part of the parser for data format 4.0.
 *
 *------------------------------------------------------------------------------
*/

#include <Str.h>

// initial state of parser
#define  INITIAL    0

// error codes
#define UNDEF_ATTR      1
#define SERIOUS_ERROR   2
#define BAD_NUM_ARGS    3
#define BAD_ARG         4
#define INDEX_ERROR     5
#define PERIOD_ERROR    6
#define PERIOD_DUP      7
#define WRONG_TYPE      8
#define SHOULD_BE_INT   9
#define UNDEF_PART     10
#define FOPEN_ERROR    11
#define STACK_OVERFLOW 12

#define STACKSIZE     30             /* maximum depth of file stack */

//------------------------------------------------------------------------------
// Enum DRPartCat::Tag
//
// "Data Reader Part Category Tag".
// A tag to identify a part category for DataRead purposes.
//------------------------------------------------------------------------------

namespace WitDRPartCat
   {
   enum Tag
      {
      capacity,
      material
      };
   }

//------------------------------------------------------------------------------
// Enum DRParmType::Tag
//
// "Data Reader Parameter Type Tag".
// A tag to identify the type of an argument to an API function for DataRead
// purposes.
//------------------------------------------------------------------------------

namespace WitDRParmType
   {
   enum Tag
      {
      undefined,
      boolScalar,
      intScalar,
      doubleScalar,
      string,
      partCat,
      boolVector,
      intVector,
      doubleVector,
      boundSet
      };
   }

//------------------------------------------------------------------------------

typedef int  WitParseRC;
   //
   // Return code for parsing

struct WitFileStackElement {

       // Constructor and destructor
       //
    WitFileStackElement ();

   ~WitFileStackElement ();

    FILE *    inputFile_;
    int       lineno_;
    WitString fileName_;
  };

class WitParlex

{
public:

  // Constructor and destructor
  WitParlex();
  ~WitParlex();

  // Operations

  // Save current lexer state and begin reading from new file
  static  WitParseRC  saveFile(const char *fname);

  // Close current input file and restore prvious lexer state
  static  WitParseRC  restoreFile();

  // Accessors
  static  int     lineno();	
  static  int     errcode();
  static  const char *fileName();

  static  void    setLineno(int);
  static  void    setErrcode(int); 
  static  void    setFileName(const char *);

private:

  // static global data
  static int    lineno_;         // current line being parsed
  static int    errcode_;        // error number
  static WitString & fileName_;    // name of current input file

  // Stack to keep track of file information:
  // When a "read filename" command is encountered, the current lexer
  // state is pushed onto the stack and we continue reading from the 
  // new file.  Then, when an end-of-file occurs, yywrap() is called and
  // the stack is popped.  Reading then continues with the old file. 

  static WitFileStackElement fileStack_[];

  static int  stackPos_;                  // next free stack position

};

#endif            // ParlexH
