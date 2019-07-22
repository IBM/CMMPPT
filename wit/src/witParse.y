%{
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

/*------------------------------------------------------------------------------
 *
 *     Source file:    witParse.y
 *
 *     Parser for WIT 4.0 Input Data File Format
 *
 *     To create an executable parser file:
 *
 *           yacc -d  witParse.y             (creates y.tab.c and y.tab.h)
 *           mv -f y.tab.c witParse.C
 *           mv -f y.tab.h ytab.h
 *           lex  witLexer.l                 (creates lex.yy.c)    
 *           mv -f lex.yy.c witLexer.C
 *           xlC witLexer.C witParse.C -o witParse -ly -ll
 *    
 *           Notes:
 *           
 *              -d   causes y.tab.h to be created.  This is the token
 *                   name definition file.
 *
 *              -ly  causes the yacc library to be included. This 
 *                   library is in /usr/lib/liby.a
 *
 *              -ll  causes the lex library to be included. This
 *                   library is in /usr/lib/libl.a      
 *
 *------------------------------------------------------------------------------
 *
 */

#include <Parser.h>
#include <Problem.h>
#include <ApiMgr.h>
#include <InputID.h>
#include <VecBS.h>
#include <Entity.h>
#include <MsgFac.h>

#include <errno.h>

extern  FILE *yyin;

//#if defined(__GNUC__)
extern int yylex (void);
extern void yyrestart (FILE *);
//#endif

extern "C" void yyerror(const char *)
{
}

extern char yytext[];

void prterror();                     /* print line number and filename */ 

    /* variables used by the parser */
//RWCollectableString   *rulePtr;
WitSymTabEntry        *entryPtr;     /* ptr to current symbol table entry */
WitEntity::argVecDef  *argPtr;
WitVectorSpec         *vecPtr;       /* pointer to current vector format */
WitBoundSetSpec       *bndPtr;
WitParseRC            retcode;
WitEntity::eType      entityType;    /* type of the current entity */
WitAttribute::aType   attrType;      /* type of the current attribute */  
WitParser::PS         commandState;  /* current command state */
const char            *s1, *s2, *s3; /* temporary strings     */

%}

%union {
    int    intval;
    double doubleval;
    char * strval;
    bool   boolval;
    int    cval;
    int    pval;
    int    btype;
}

    /* literal keyword tokens */
%token RELEASE ADD SET READ PROBLEM
%token PART OPERATION BOM_ENTRY SUB_ENTRY BOP_ENTRY 
%token DEMAND PART_WITH_OPERATION
%token SINGLE SPARSE DENSE
%token END_BOUNDS HARDLB SOFTLB HARDUB
%token CAPACITY MATERIAL 

    /* tokens with values -- the values are returned by
       the lexer in yylval, which is a union */

%token <strval>    IDENTIFIER
%token <strval>    STRING
%token <doubleval> DOUBLE
%token <intval>    INTEGER
%token <boolval>   BOOLEAN
%token <cval>      CATEGORY
%token <strval>    EXEC_BOUNDS STOCK_BOUNDS CUM_SHIP_BOUNDS

    /* These are production names and corresponding types */
%type  <strval>   attribute_name  
%type  <strval>   bound_set_type
%type  <strval>   bound_set_attribute
%type  <cval>     category  
%type  <strval>   entity_name
%type  <pval>     simple_value
%type  <pval>     period_value
%type  <pval>     attribute_value
%type  <pval>     vector_value
%type  <pval>     argument
%type  <btype>    bound_type
%type  <btype>    bound_item
%type  <intval>   period

%%
    /* definition of grammar */

data_specification: command_list
    | release_specification command_list
    ;

command_list:  /* empty */    
    | command_list command
    ;

command: add_command
    {
      delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    }
 
    | set_command
    { 
      delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    } 
 
    | read_command
    {
      if (WitParser::instance ()->currentEntity_ != NULL)
        delete WitParser::instance ()->currentEntity_;
      WitParser::instance ()->currentEntity_ = NULL;
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ = NULL;
    }
    ;

release_specification: RELEASE
    { WitParser::instance ()->setParseState(WitParser::PS_release); }

    release_num
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }

    ';'
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
      free (yylval.strval);     
    }
    ;

release_num: STRING 
    ;

add_command: ADD  
    { 
      WitParser::instance ()->setParseState(WitParser::PS_add);
      commandState = WitParser::PS_add;     /* save command state */
      WitParser::instance ()->currentRule_ = "add";
    }

    add_clause 
    {
      /* lookup rule for this entity and save symbol table pointer */
      entryPtr = WitParser::instance ()->currentEntity_->
        lookupRule(
           WitParser::instance ()->theSymTable_,
           WitParser::instance ()->currentRule_);

      /* Issue error message if no entry found in symbol table */
      if (entryPtr == NULL) {
        switch (commandState) {
          case WitParser::PS_add:
            WitParser::instance ()->myMsgFac () ("noParamAddMsg",
              WitParser::instance ()->currentEntity_->entityID());
            break;
          case WitParser::PS_set:
            WitParser::instance ()->myMsgFac () ("noParamSetMsg",
              WitParser::instance ()->currentEntity_->entityID());
            break;
        };
        WitParser::instance ()->currentEntity_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
    }

    entity_specification ';'
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    }
    ;

add_clause: entity_name
    {
      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            entityType);
      free (yylval.strval); 
      WitParser::instance ()->currentAttr_ = NULL;
    }

    | PART_WITH_OPERATION
    {
      WitParser::instance ()->setParseState(WitParser::PS_partOperation);
      WitParser::instance ()->currentRule_ = "add_partWithOperation";

      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitEntity::partWithOperation);

      free (yylval.strval);
      WitParser::instance ()->currentAttr_ = NULL;
      entityType = WitEntity::partWithOperation;
    }
    ;

set_command : SET PROBLEM
    {
      WitParser::instance ()->setParseState(WitParser::PS_setProblem);
      commandState = WitParser::PS_set;     /* save command state */

      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitEntity::problem);
      free (yylval.strval);
      WitParser::instance ()->currentAttr_ = NULL;

      WitParser::instance ()->currentRule_ = "set_problem";
    }
 
    global_specification ';'
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    }
   
    | SET 
    { 
      WitParser::instance ()->setParseState(WitParser::PS_set);
      commandState = WitParser::PS_set;     /* save command state */
      WitParser::instance ()->currentRule_ = "set";
    }
  
    set_clause entity_specification ';'
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextCommand);
    }
    ;

set_clause: entity_name
    {
      // create a WitEntity object
      WitParser::instance ()->currentEntity_ =
         new WitEntity (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            entityType);
      free (yylval.strval);
      WitParser::instance ()->currentAttr_ = NULL;

      /* lookup rule for this entity and save symbol table pointer */
      entryPtr = WitParser::instance ()->currentEntity_->
        lookupRule (
           WitParser::instance ()->theSymTable_,
           WitParser::instance ()->currentRule_);

      /* Issue error message if no entry found in symbol table */
      if (entryPtr == NULL) {
        switch (commandState) {
          case WitParser::PS_add:
            WitParser::instance ()->myMsgFac () ("noParamAddMsg",
              WitParser::instance ()->currentEntity_->entityID());
            break;
          case WitParser::PS_set:
            WitParser::instance ()->myMsgFac () ("noParamSetMsg",
              WitParser::instance ()->currentEntity_->entityID());
            break;
        };
        WitParser::instance ()->currentEntity_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
    }
    ;

entity_name: PART
    {
      WitParser::instance ()->setParseState(WitParser::PS_part);
      WitParser::instance ()->currentRule_ += "_part";
      entityType = WitEntity::part;
    }

    | OPERATION
    {
      WitParser::instance ()->setParseState(WitParser::PS_operation);
      WitParser::instance ()->currentRule_ += "_operation";
      entityType = WitEntity::operation;
    }

    | BOM_ENTRY
    {
      WitParser::instance ()->setParseState(WitParser::PS_bomEntry);
      WitParser::instance ()->currentRule_ += "_bomEntry";
      entityType = WitEntity::bomEntry;
    } 

    | SUB_ENTRY
    {
      WitParser::instance ()->setParseState(WitParser::PS_subEntry);
      WitParser::instance ()->currentRule_ += "_subEntry";
      entityType = WitEntity::subEntry;
    }

    | BOP_ENTRY
    {
      WitParser::instance ()->setParseState(WitParser::PS_bopEntry);
      WitParser::instance ()->currentRule_ += "_bopEntry";
      entityType = WitEntity::bopEntry;
    }

    | DEMAND
    {
      WitParser::instance ()->setParseState(WitParser::PS_demand);
      WitParser::instance ()->currentRule_ += "_demand";
      entityType = WitEntity::demand;
    }
    ; 

global_specification: attribute_list
    ;

entity_specification: argument_list
    {
      /*------------------------------------------------------*
       * we have created the entity object and the argument   *
       * vector; we also know the current rule; if there is   *
       * an entry for the rule, then invoke the function.     *
       *------------------------------------------------------*/

      if (entryPtr != NULL) {
        /* before invoking the function, check the validity of the arguments */
        /* by comparing the arg. types against the types in the symbol table */

        retcode = WitParser::instance ()->currentEntity_->checkArgs(entryPtr);
        if (retcode == BAD_NUM_ARGS) {
          if (entryPtr->nparms() == 1)
            s2 = "argument";
          else s2 = "arguments";
          if (WitParser::instance ()->currentEntity()->argc() == 1)
            s3 = "was found";
          else s3 = "were found";

          s1 = "";
          if (commandState == WitParser::PS_add)
            s1 = "add "; 
          if (commandState == WitParser::PS_set)
            s1 = "set ";

          WitParser::instance ()->myMsgFac () ("incorrectNumberArgMsg",
             s1,
             WitParser::instance ()->currentEntity_->entityID(),
             entryPtr->nparms(),
             s2,
             WitParser::instance ()->currentEntity()->argc(),
             s3);

          WitParser::instance ()->currentEntity_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
        else
        if (retcode) {
          WitParser::instance ()->currentEntity_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }

        else                                    /* if arguments O.K.  */
        if (entryPtr->func() != NULL) {         /* if function exists */

          /* if entity has been invalidated because of an error,      */
          /* then, skip invocations of API functions for the entity   */
          if (!WitParser::instance ()->currentEntity_->invalid())
            (entryPtr->func())();               /* invoke function */
        }
      };  

      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");
    }

    attribute_list
    ;

category: CATEGORY
    { 
      if (WitParser::instance ()->parseState() == WitParser::PS_part)
      $$ = $1;
    }
    ; 

attribute_list:  /* empty */
    | attribute_list attribute
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextAttr);

      /* If an entry exists for the current rule, then invoke   */
      /* the function.                                          */

      if (entryPtr == NULL) {
        WitParser::instance ()->myMsgFac () ("noAttributeDefMsg",
          WitParser::instance ()->currentAttr_->attrID(),
          WitParser::instance ()->currentEntity_->entityID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
      else
      {
        retcode = WitParser::instance ()->currentAttr_->checkArgs(entryPtr);
        if (retcode == PERIOD_ERROR) {
           WitParser::instance ()->currentAttr_->setInvalid(true);
           prterror();
        }            
        else
        if (retcode) {
          WitParser::instance ()->currentAttr_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
        else 
        if (entryPtr->func() != NULL) {        /* if function exists  */

          /* if entity or attr has been invalidated because of error, */
          /* then, skip invocations of API function for attributes    */
          if (!WitParser::instance ()->currentEntity_->invalid() and
            !WitParser::instance ()->currentAttr_->invalid())
            retcode = (entryPtr->func())();    /* invoke wrapper function */
            if (retcode) {
              WitParser::instance ()->currentAttr_->setInvalid(true);
              prterror();
            }; 
        } 
      };                   

      WitParser::instance ()->currentRule_ =
         WitParser::instance ()->previousRule_;
    }  
    ;

argument_list: argument
    | argument_list argument
    ;

argument: simple_value
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextArg);
      switch($1) {
        case WitDRParmType::intScalar:
          argPtr =
             WitParser::instance ()->currentEntity_->setIntArg(yylval.intval);
          break;
        case WitDRParmType::doubleScalar:
          argPtr =
             WitParser::instance ()->currentEntity_->
                setDoubleArg(yylval.doubleval);
          break;
        case WitDRParmType::boolScalar:
          argPtr = WitParser::instance ()->currentEntity_->
            setBoolArg(yylval.boolval);
          break;
        case WitDRParmType::string:
          argPtr =
             WitParser::instance ()->currentEntity_->
                setStringArg(yylval.strval);

          /* string was created by the lexer and it's no longer needed */
          free (yylval.strval);
          break;
        default:
          return 0;
      };
      if (argPtr == NULL) {
        s1 = "";
        if (commandState == WitParser::PS_add)
          s1 = "add ";
        if (commandState == WitParser::PS_set)
          s1 = "set ";

        /* if rule is not defined in symbol table, skip message now. */
        /* However, error message will be issued after all arguments */
        /* are processed.  We don't know the correct number of args. */

        if (entryPtr != NULL) {
          if (entryPtr->nparms() == 1)
            s2 = "argument is";
          else s2 = "arguments are";

          WitParser::instance ()->myMsgFac () ("tooManyArgsMsg",
             s1,
             WitParser::instance ()->currentEntity_->entityID(),
             entryPtr->nparms(),
             s2);

          WitParser::instance ()->currentEntity_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
      }
    }

    | category
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextArg);
      argPtr = WitParser::instance ()->currentEntity_->
        setCatArg (static_cast <WitDRPartCat::Tag> (yylval.cval));
      if (argPtr == NULL) {
        s1 = "";
        if (commandState == WitParser::PS_add)
          s1 = "add ";
        if (commandState == WitParser::PS_set)
          s1 = "set ";

        /* If rule is not defined, skip message. See comment above */
        if (entryPtr != NULL) {
          if (entryPtr->nparms() == 1)
            s2 = "argument is";
          else s2 = "arguments are";

          WitParser::instance ()->myMsgFac () ("tooManyArgsMsg",
             s1,
             WitParser::instance ()->currentEntity_->entityID(),
             entryPtr->nparms(),
             s2);

          WitParser::instance ()->currentEntity_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
     }
    }
    ;

attribute: simple_attribute

    | bound_set_attribute
    ;

simple_attribute: attribute_name attribute_value
    ;

attribute_name: IDENTIFIER
    {
      WitParser::instance ()->setParseState(WitParser::PS_attr);
      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");

      WitParser::instance ()->currentRule_ += "_";
      WitParser::instance ()->currentRule_ += yylval.strval;

      /* create a new WitAttribute object */
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ =
         new WitAttribute (WitParser::instance ()->myProblem (), yylval.strval);

      free (yylval.strval);

      /* lookup rule for this attribute and save symbol table pointer */
      /* this will be helpful to check the datatype of a vector       */
      entryPtr = WitParser::instance ()->currentAttr_->
        lookupRule(WitParser::instance ()->theSymTable_,
           WitParser::instance ()->currentRule_);

      /* if attribute name is not in the symbol table, we can't       */
      /* continue because we don't know the expected argument types   */
      if (entryPtr == NULL) {
        WitParser::instance ()->myMsgFac () ("noAttributeDefMsg",
          WitParser::instance ()->currentAttr_->attrID(),
          WitParser::instance ()->currentEntity_->entityID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }

      $$ = $1;
    }
    ;

attribute_value: simple_value
    {
      switch($1) {
        case WitDRParmType::intScalar:
          argPtr =
             WitParser::instance ()->currentAttr_->setIntArg(yylval.intval);
          break;
        case WitDRParmType::doubleScalar:
          argPtr =
             WitParser::instance ()->currentAttr_->setDoubleArg(
                yylval.doubleval);
          break;
        case WitDRParmType::boolScalar:
          argPtr = WitParser::instance ()->currentAttr_->
            setBoolArg(yylval.boolval);
          break;
        case WitDRParmType::string:
          argPtr =
             WitParser::instance ()->currentAttr_->setStringArg(yylval.strval);

          /* string was created by the lexer and it's no longer needed */
          free (yylval.strval);
          break;
        default:
          return 0;
      };

      if (argPtr == NULL) {
        s1 = "";
        if (commandState == WitParser::PS_add)
          s1 = "add ";
        if (commandState == WitParser::PS_set)
          s1 = "set ";

        if (entryPtr != NULL) {
          if (entryPtr->nparms() == 1)
            s2 = "argument is";
          else s2 = "arguments are";

          WitParser::instance ()->myMsgFac () ("tooManyArgsMsg",
             s1,
             WitParser::instance ()->currentAttr_->attrID(),
             entryPtr->nparms(),
             s2);

          WitParser::instance ()->currentAttr_->setInvalid(true);
          prterror();
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
        }
      }
    }

    | vector_format
    { $$ = WitDRParmType::undefined; }
    ;

simple_value: STRING
    { $$ = WitDRParmType::string; }

    | INTEGER
    { $$ = WitDRParmType::intScalar; }

    | DOUBLE
    { $$ = WitDRParmType::doubleScalar; }

    | BOOLEAN
    { $$ = WitDRParmType::boolScalar; }
    ;

vector_format: SINGLE 
    {
      WitParser::instance ()->setParseState(WitParser::PS_single);

      // Construct a VectorSpec object
      if (WitParser::instance ()->currentAttr_->attrType() ==
          WitAttribute::simple) {
        if (entryPtr != NULL) {
          vecPtr =
             new WitVectorSpec (
                WitParser::instance ()->myProblem (),
                WitVectorSpec::single_,
                entryPtr->parmTypes()[0]);

          argPtr = WitParser::instance ()->currentAttr_->setVectorArg(vecPtr);
        }
      }
      else 
        vecPtr =
           new WitVectorSpec (
              WitParser::instance ()->myProblem (),
              WitVectorSpec::single_,
              WitDRParmType::doubleVector);
    }

    '('
    {
      WitParser::instance ()->setParseState(WitParser::PS_singleParen);
    }

    vector_value
    {
      WitParser::instance ()->setParseState(WitParser::PS_singleValue);
    }

    ')'
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }

    | SPARSE 
    {
      WitParser::instance ()->setParseState(WitParser::PS_sparse);

      /* Construct a VectorSpec object */
      if (WitParser::instance ()->currentAttr_->attrType() ==
          WitAttribute::simple) {
        if (entryPtr != NULL) {
          vecPtr =
             new WitVectorSpec (
                WitParser::instance ()->myProblem (),
                WitVectorSpec::sparse_,
                entryPtr->parmTypes()[0]);
          argPtr = WitParser::instance ()->currentAttr_->setVectorArg(vecPtr);
        }
      }
      else
         vecPtr =
            new WitVectorSpec (
               WitParser::instance ()->myProblem (),
               WitVectorSpec::sparse_,
               WitDRParmType::doubleVector);
    }

    '(' 
    {
      WitParser::instance ()->setParseState(WitParser::PS_sparseParen);
    }

    sparse_list 
    ')'
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }

    | DENSE
    {
      WitParser::instance ()->setParseState(WitParser::PS_dense);

      // Construct a VectorSpec object
      if (WitParser::instance ()->currentAttr_->attrType() ==
          WitAttribute::simple) {
        if (entryPtr != NULL) {
          vecPtr =
             new WitVectorSpec (WitParser::instance ()->myProblem (),
                WitVectorSpec::dense_,
                entryPtr->parmTypes()[0]);
          argPtr = WitParser::instance ()->currentAttr_->setVectorArg(vecPtr);
        }
      }
      else
         vecPtr =
            new WitVectorSpec (
               WitParser::instance ()->myProblem (),
               WitVectorSpec::dense_,
               WitDRParmType::doubleVector);
    }

    '(' 
    {
      WitParser::instance ()->setParseState(WitParser::PS_denseParen);
    }

    value_list 
    ')'
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }
    ;

sparse_list:   /* empty */
    | sparse_list period_value
    {
      WitParser::instance ()->setParseState(WitParser::PS_sparseValue);
    }
    ;

period_value: period 
    ':'
    {
      WitParser::instance ()->setParseState(WitParser::PS_periodColon);
    }

    vector_value
    { 
      WitParser::instance ()->setParseState(WitParser::PS_sparseParen);
      $$ = $4; 
    }
    ;

value_list: vector_value
    | value_list vector_value
    ;

vector_value: simple_value
    {
      WitParser::instance ()->setParseState(WitParser::PS_denseValue);

      if (entryPtr != NULL) {
      switch($1) {

        case WitDRParmType::intScalar:
          if (entryPtr->parmTypes()[0] == WitDRParmType::doubleVector) {
            retcode =
               vecPtr->addDoubleVal(static_cast <double> (yylval.intval));

            break;
          };
          if (entryPtr->parmTypes()[0] == WitDRParmType::intVector) {
            retcode = vecPtr->addIntVal(yylval.intval);
            break;
          };
          if (entryPtr->parmTypes()[0] == WitDRParmType::boundSet) {
            retcode =
               vecPtr->addDoubleVal(static_cast <double> (yylval.intval));
            break;
          }
          else {
            retcode = WRONG_TYPE;
            break;
          };
          
        case WitDRParmType::doubleScalar:
         if (entryPtr->parmTypes()[0] == WitDRParmType::intVector) {
            vecPtr->addIntVal(static_cast <int> (yylval.doubleval));
            retcode = SHOULD_BE_INT;
            break;
          };
          if (entryPtr->parmTypes()[0] == WitDRParmType::doubleVector) {
            retcode = vecPtr->addDoubleVal(yylval.doubleval);
            break;
          };
          if (entryPtr->parmTypes()[0] == WitDRParmType::boundSet) {
            retcode = vecPtr->addDoubleVal(yylval.doubleval);
            break;
          }
          else {
            retcode = WRONG_TYPE;
            break;
          };

        case WitDRParmType::boolScalar:
           {
           if (entryPtr->parmTypes()[0] == WitDRParmType::boolVector)
              {
              retcode = vecPtr->addBoolVal (yylval.boolval);

              break;
              };

           WitParser::instance ()->myMsgFac () ("booleanVectorMsg",
              WitParser::instance ()->currentAttr_->attrID ());

           WitParser::instance ()->currentAttr_->setInvalid (true);

           prterror ();

           retcode = 0;

           WitParser::instance ()->myMsgFac () ("parsingStopsMsg");

           break;
           }

        case WitDRParmType::string:
          WitParser::instance ()->myMsgFac () ("stringVectorMsg",
             WitParser::instance ()->currentAttr_->attrID());
          WitParser::instance ()->currentAttr_->setInvalid(true);
          prterror();
          free (yylval.strval);
          retcode = 0;
          WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
          break;
      }

      if (retcode == WRONG_TYPE) {
        WitParser::instance ()->myMsgFac () ("vecTypeNotValidMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        retcode = 0;
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
      if (retcode == SHOULD_BE_INT) {
        WitParser::instance ()->myMsgFac () ("vecValueNotIntMsg",
           WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        retcode = 0;
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
      }
    }
    ;

period: INTEGER
    {
      WitParser::instance ()->setParseState(WitParser::PS_period);

      retcode = vecPtr->addPeriodVal(yylval.intval);
      if (retcode == PERIOD_ERROR) {
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      }
      if (retcode == PERIOD_DUP) {
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      }
    }
    ;

bound_set_attribute: bound_set_type 
    {  
      /* lookup rule for this attribute and save symbol table pointer */
      entryPtr = WitParser::instance ()->currentAttr_->
         lookupRule (
            WitParser::instance ()->theSymTable_,
            WitParser::instance ()->currentRule_);

      if (entryPtr == NULL) {
        WitParser::instance ()->myMsgFac () ("noAttributeDefMsg",
          WitParser::instance ()->currentAttr_->attrID(),
          WitParser::instance ()->currentEntity_->entityID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }
    }

    bounds_specification END_BOUNDS
    {
      bndPtr = NULL;
      WitParser::instance ()->setParseState(WitParser::PS_endBounds);
    }
    ;

bound_set_type: EXEC_BOUNDS
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);

      // Construct a BoundSetSpec object
      bndPtr = new WitBoundSetSpec(WitBoundSetSpec::exec);

      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");
      WitParser::instance ()->currentRule_ += "_";
      WitParser::instance ()->currentRule_ += yylval.strval;

      /* create a new WitAttribute object */
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ =
         new WitAttribute (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitAttribute::boundSet);

      argPtr = WitParser::instance ()->currentAttr_->setBoundSetArg(bndPtr);

      free (yylval.strval);
      $$ = $1;
    }

    | STOCK_BOUNDS
    { 
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);

      // Construct a BoundSetSpec object
      bndPtr = new WitBoundSetSpec(WitBoundSetSpec::stock);

      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");
      WitParser::instance ()->currentRule_ += "_";
      WitParser::instance ()->currentRule_ += yylval.strval;

      /* create a new WitAttribute object */
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ =
         new WitAttribute (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitAttribute::boundSet);

      argPtr = WitParser::instance ()->currentAttr_->setBoundSetArg(bndPtr);

      free (yylval.strval);
      $$ = $1;
    }

    | CUM_SHIP_BOUNDS
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);

      // Construct a BoundSetSpec object
      bndPtr = new WitBoundSetSpec(WitBoundSetSpec::cumShip);

      WitParser::instance ()->previousRule_ =
         WitParser::instance ()->currentRule_.replace(0, 3, "set");
      WitParser::instance ()->currentRule_ += "_";
      WitParser::instance ()->currentRule_ += yylval.strval;

      /* create a new WitAttribute object */
      if (WitParser::instance ()->currentAttr_ != NULL)
        delete WitParser::instance ()->currentAttr_;
      WitParser::instance ()->currentAttr_ =
         new WitAttribute (
            WitParser::instance ()->myProblem (),
            yylval.strval,
            WitAttribute::boundSet);
      argPtr = WitParser::instance ()->currentAttr_->setBoundSetArg(bndPtr);

      free (yylval.strval);
      $$ = $1;
    }
    ;

bounds_specification:   /* empty */
    | bounds_specification

    bound_item
    {
      WitParser::instance ()->setParseState(WitParser::PS_nextBound);
    }
    ;

bound_item: bound_type 
    {
      WitParser::instance ()->setParseState(WitParser::PS_boundType);
    }

    vector_format
    {
       switch ($1) {
         case WitBoundSetSpec::hardlb:
           bndPtr->setHardLBVec(vecPtr);
           break;

         case WitBoundSetSpec::softlb:
           bndPtr->setSoftLBVec(vecPtr);
           break;

         case WitBoundSetSpec::hardub:
           bndPtr->setHardUBVec(vecPtr);
           break;
       }
    }
    ;

bound_type: HARDLB
    {
      if (bndPtr->hardLBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateHardLBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      $$ = WitBoundSetSpec::hardlb;
    }

    | SOFTLB
    {
      if (bndPtr->softLBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateSoftLBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      $$ = WitBoundSetSpec::softlb;
    }

    | HARDUB
    {
      if (bndPtr->hardUBVec() != NULL) {
        WitParser::instance ()->myMsgFac () ("duplicateHardUBMsg",
          WitParser::instance ()->currentAttr_->attrID());
        WitParser::instance ()->currentAttr_->setInvalid(true);
        prterror();
      };
      $$ = WitBoundSetSpec::hardub;
    }
    ;

read_command: READ 
    {
      WitParser::instance ()->setParseState(WitParser::PS_read);
      commandState = WitParser::PS_read;
      WitParser::instance ()->currentRule_ = "read";
      // printf("Current rule is: %s\n", WitParser::instance ()->currentRule_);
    }

    file_name 
    {
      WitParser::instance ()->setParseState(WitParser::PS_commandEnd);
    }

    ';'
    {
      /* save lexer state and shift input stream to new file */
      errno   = 0;
      retcode = WitParlex::saveFile(yylval.strval);
      if (retcode == FOPEN_ERROR) {
        yylval.strval[strlen(yylval.strval)-1] = '\0';
        WitParser::instance ()->myMsgFac () ("fopenSmsg",
           yylval.strval+1,
          "r",
          errno,
          strerror(errno));
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }

      if (retcode == STACK_OVERFLOW) {
        WitParser::instance ()->myMsgFac () ("stackOverflowMsg", STACKSIZE);
        prterror();
        WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
      }

//    printf("Lexer is now reading from file %s\n", WitParlex::fileName());

      WitParser::instance ()->setDataFileName   ();
      WitParser::instance ()->setDataFileLineNo ();

      //#if defined (__GNUC__)
      yyrestart(yyin);
      //#endif

      WitParser::instance ()->setParseState(WitParser::PS_initial);
      free (yylval.strval);
    }

    release_specification
    ;

file_name: STRING
    ;

%%

//------------------------------------------------------------------------------

WitParser * WitParser::instance_ = NULL;

//------------------------------------------------------------------------------

void WitParser::parse (WitProblem * theProblem, const char * fileName, FILE * f)
   {
   witAssert (instance_ == NULL);

   instance_ = new WitParser;

   instance ()->parseIt (theProblem, fileName, f);

   delete instance ();

   instance_ = NULL;
   }

//------------------------------------------------------------------------------

// accessors

WitParser::PS    WitParser::parseState() const
{ return parseState_; }

WitEntity     *WitParser::currentEntity() const
{ return currentEntity_; }

WitAttribute  *WitParser::currentAttr() const
{ return currentAttr_; }

WitMsgFacility & WitParser::myMsgFac () const
   {
   return myProblem ()->myMsgFac ();
   }

void WitParser::setParseState(PS st)
{ parseState_ = st; }


void prterror()
{
  WitParser::instance ()->myMsgFac () ("lineNumberMsg",
    reinterpret_cast <const char *> (yytext),
    WitParlex::lineno(), WitParlex::fileName());
}

//-----------------------------------------------------------------------------
// WitParser::prtSyntaxError
//
//     Implementation of syntax checker
//
//     Contains function to check the state of the parser and generate
//     a meaninful error message when a syntax error occurs.
//
// The parse states are defined in Parser.h:
//
//  PS_initial               // initial state, looking for "release"
//  PS_add                   // found "add", looking for <entity_name>
//  PS_set                   // found "set", looking for <entity_name>
//  PS_read                  // found "read", looking for <data_file>
//  PS_release               // found "release", looking for release_spec
//  PS_part                  // found "part", looking for <category>
//  PS_operation             // found "operation", looking for <arg>
//  PS_bomEntry              // found "bomEntry", looking for <arg>
//  PS_subEntry              // found "subEntry", looking for <arg>
//  PS_bopEntry              // found "bopEntry", looking for <arg>
//  PS_demand                // found "demand", looking for <arg>
//  PS_partOperation         // found "partWithOperation, looking for <arg>
//  PS_nextArg               // looking for another <arg> or a ";"
//  PS_attr                  // found <attr_name>, looking for <attr_value>
//  PS_single                // found "single", looking for "("
//  PS_dense                 // found "dense", looking for "("
//  PS_sparse                // found "sparse", looking for "("
//  PS_singleParen           // found "single (", looking for <vector_value>
//  PS_denseParen            // found "dense (", looking for <vector_value>
//  PS_sparseParen           // found "sparse (", looking for <period>
//  PS_singleValue           // found single value, looking for ")"
//  PS_denseValue            // found <value>, looking for <value> or ")"
//  PS_sparseValue           // found <period_val>, looking for <period> or ")"
//  PS_nextAttr              // looking for <attr_name> or ";"
//  PS_nextCommand           // found ";" and looking for <command>
//  PS_commandEnd            // looking for ";"
//  PS_period                // found <period>, looking for ":"
//  PS_periodColon	     // found ":", looking for <vector_value>
//  PS_nextBound             // found <bound_set_type>, looking for <bounds>
//  PS_boundType             // found <bound_type>, looking for <vector>
//  PS_endBounds             // found "endBounds", looking for ";" or next attr
//  PS_setProblem            // found "set problem", looking for
//                           //        <attribute name>
//-----------------------------------------------------------------------------

void WitParser::prtSyntaxError(WitSymTabEntry *pEntry,
                WitParser::PS parseState, const char * yytext) const
{
  const char * s1;

  // Print syntax error message, depending on current parser state
  switch (parseState) {
    case WitParser::PS_initial:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
         "\"release\"", yytext);
      break;
    case WitParser::PS_commandEnd:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg", "a \";\"", yytext);
      break;
    case WitParser::PS_nextCommand:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
      "a command name (e.g., set, add, read)", yytext);
      break;
    case WitParser::PS_add:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "the name of an addable object type after \"add\"", yytext);
      break;
    case WitParser::PS_set:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "the name of a settable object type after \"set\"", yytext);
      break;
    case WitParser::PS_setProblem:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "the name of an attribute after \"set problem\"", yytext);
      break;
    case WitParser::PS_read:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a string (the name of a file) after \"read\"", yytext);
      break;
    case WitParser::PS_release:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a quoted string (the release number) after \"release\"", yytext);
      break;
    case WitParser::PS_part:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "part", yytext);
      break;
    case WitParser::PS_operation:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "operation", yytext);
      break;
    case WitParser::PS_bomEntry:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "bomEntry", yytext);
      break;
    case WitParser::PS_subEntry:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "subEntry", yytext);
      break;
    case WitParser::PS_bopEntry:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "bopEntry", yytext);
      break;
    case WitParser::PS_demand:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "demand", yytext);
      break;
    case WitParser::PS_partOperation:
      if (pEntry->nparms() == 1)
        s1 = "argument";
      else
        s1 = "arguments";
      WitParser::instance ()->myMsgFac () ("noArgsMsg", pEntry->nparms(), s1,
        "partWithOperation", yytext);
      break;
    case WitParser::PS_nextArg:

      // Check to see if we have the correct number of argumenta.
      // If so, we are looking for an attribute or a semicolon
      if (WitParser::instance ()->currentEntity()->argc() >= pEntry->nparms()) {
        WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
          "the name of an attribute or a \";\"", yytext);
        break;
      };

      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
         "another argument",
         yytext);
      break;
    case WitParser::PS_attr:
      switch (pEntry->parmTypes()[0]) {   // what the value should be
        case WitDRParmType::intScalar:
          s1 = "integer";	
          break;
        case WitDRParmType::doubleScalar:
          s1 = "double";
          break;
        case WitDRParmType::string:
          s1 = "string";
          break;
        case WitDRParmType::boolScalar:
          s1 = "boolean";
          break;
        case WitDRParmType::partCat:
          s1 = "\"category\"";
          break;
        case WitDRParmType::boolVector:
          s1 = "\"boolean vector\"";
          break;
        case WitDRParmType::intVector:
          s1 = "\"integer vector\"";
          break;
        case WitDRParmType::doubleVector:
          s1 = "\"double vector\"";
          break;
        case WitDRParmType::boundSet:
          s1 = "\"bound set\"";
          break;
        default:
          s1 = "unknown";
          break;
      };
      WitParser::instance ()->myMsgFac () ("attrValueNotFound",
        s1, WitParser::instance ()->currentAttr()->attrID(), yytext);
      break;
    case WitParser::PS_nextAttr:
    case WitParser::PS_endBounds:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "the name of an attribute or a \";\"", yytext);
      break;
    case WitParser::PS_nextBound:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a bound set attribute (e.g., softLB, hardLB, hardUB)"
        " or \"endBounds\"", yytext);
      break;
    case WitParser::PS_boundType:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector format (e.g, single, dense, sparse)", yytext);
      break;
    case WitParser::PS_single:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \"(\" after \"single\"", yytext);
      break;
    case WitParser::PS_dense:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \"(\" after \"dense\"", yytext);
      break;
    case WitParser::PS_sparse:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \"(\" after \"sparse\"", yytext);
      break;
    case WitParser::PS_singleParen:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector element for a \"single\" vector specification",
        yytext);
      break;
    case WitParser::PS_denseParen:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector element for a \"dense\" vector specification",
        yytext);
      break;
    case WitParser::PS_sparseParen:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a period (an integer) in \"sparse\" vector specification", yytext);
      break;
    case WitParser::PS_singleValue:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \")\" after the one vector value in a \"single\" vector "
        "specification", yytext);
      break;
    case WitParser::PS_denseValue:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector value or a \")\"", yytext);
      break;
    case WitParser::PS_sparseValue:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a period (an integer) or a \")\"", yytext);
      break;
    case WitParser::PS_period:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a \":\" in \"sparse\" vector specification", yytext);
      break;
    case WitParser::PS_periodColon:
      WitParser::instance ()->myMsgFac () ("syntaxErrorMsg",
        "a vector value (a number) after the \":\"", yytext);
      break;
  }
}

//------------------------------------------------------------------------------

void WitParser::setDataFileName () const
   {
   myProblem ()->myApiMgr ()->myInputID ()->setDataFileName (fileName ());
   }

//------------------------------------------------------------------------------

void WitParser::setDataFileLineNo () const
   {
   myProblem ()->myApiMgr ()->myInputID ()->setDataFileLineNo (lineno ());
   }

//-----------------------------------------------------------------------------

// Constructor and destructor

WitParser::WitParser ():
      theSymTable_   (NULL),
      currentRule_   (),
      previousRule_  (),
      currentEntity_ (NULL),
      currentAttr_   (NULL),
      parseState_    (PS_initial),
      myProblem_     (NULL)
   {
   commandState = PS_initial;
   }

WitParser::~WitParser ()
   {
   }

//------------------------------------------------------------------------------

// Parse the input stream
int WitParser::parseIt(WitProblem * theProblem, const char* fileName, FILE* f)
{
  int   rc;

  WitParlex::setFileName (fileName);   // save current file name
  yyin = f;                            // save pointer to FILE

  myProblem_   = theProblem;
  theSymTable_ = new WitSymTable (theProblem->myWitRun ());
  rc           = theSymTable_->registerRules ();

  if (rc)
     return rc;

  rc = yyparse();

  if (rc)
  {
    WitParser::instance ()->prtSyntaxError(entryPtr,
       WitParser::instance ()->parseState(),
      reinterpret_cast <const char *> (yytext));
    WitParser::instance ()->myMsgFac () ("lineNumberMsg",
      reinterpret_cast <const char *> (yytext),
      WitParlex::lineno(), WitParlex::fileName());
    WitParser::instance ()->myMsgFac () ("parsingStopsMsg");
  }

  delete theSymTable_;
  return rc;
}
