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

#ifndef ParserH
#define ParserH

//------------------------------------------------------------------------------
// Header file: "Parser.h"
//------------------------------------------------------------------------------

#include <Parlex.h>
#include <SymTable.h>

#include <string>

extern "C" int yyparse();

//------------------------------------------------------------------------------
// class Parser
//------------------------------------------------------------------------------

class WitParser: public WitParlex {
  friend int yyparse();

public:

  // Types
  // The parseState type encodes the state of the yacc parser so that better
  // error messages can be produced, and for use in situations where context-
  // sensitive behavior is required.

  enum PS {
    PS_initial, PS_add, PS_set, PS_read, PS_release, PS_part,	
      PS_operation, PS_bomEntry, PS_subEntry, PS_bopEntry, PS_demand,
      PS_partOperation, PS_nextArg, PS_attr, PS_single, PS_dense, PS_sparse,
      PS_singleParen, PS_denseParen, PS_sparseParen, PS_singleValue,
      PS_denseValue, PS_sparseValue, PS_nextAttr, PS_nextCommand, 
      PS_commandEnd, PS_period, PS_periodColon, PS_nextBound, PS_boundType,
      PS_endBounds, PS_setProblem
  };

  // Operations

  static void parse (WitProblem * theProblem, const char *, FILE *);
     //
     // Creates a unique instance of this class, uses it to parse the input,
     // and then destroys the instance.

  void prtSyntaxError(WitSymTabEntry *entryPtr, PS state, const char * token)
       const;

  void setDataFileName   () const;
  void setDataFileLineNo () const;
     //
     // Wrappers for InputID functions.

  // Accessors
  PS           parseState() const;
  void         setParseState(PS);
  WitEntity    *currentEntity() const;
  WitAttribute *currentAttr() const;

  inline static WitParser * instance ()
     {
     witAssert (instance_ != NULL);

     return instance_;
     }

  WitMsgFacility & myMsgFac () const;

  accessFunc (WitProblem *, myProblem)

private:

  // Constructor and destructor
  WitParser();
  ~WitParser();

  int  parseIt(WitProblem * theProblem, const char*, FILE*);

  WitSymTable  *theSymTable_;   // a collection of symbol table entries
  std::string   currentRule_;     // rule corresponding to the command being parsed
  std::string   previousRule_;    
  WitEntity    *currentEntity_; // the current Wit entity object
  WitAttribute *currentAttr_;   // the current Wit attribute object
  PS           parseState_;     // the state of the parser/lexer

  WitProblem * myProblem_;
     //
     // The Problem for this Parser.

  static       WitParser * instance_;
     //
     // The unique instance of this class, if any.
};

#endif
