%{
//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

/*------------------------------------------------------------------------------
 *
 *     Source file:    witLexer.l
 *
 *     Lexer for WIT 4.0 Input Data File Format
 *
 *     To create an executable lexer file:
 *
 *             lex witLexer.l                (creates lex.yy.c)
 *             mv -f lex.yy.c witLexer.C
 *             mv y.tab.h ytab.h
 *             xlC witLexer.C -o filename -ll  
 *
 *     Notes:  ytab.h is the token name definition file created by yacc.
 *             -ll causes the lex library to be included.
 *
 *------------------------------------------------------------------------------
 */

#define  LINESIZE 128

//#if defined (__GNUC__)
#define YY_SKIP_YYWRAP
//#endif
#define YY_NEVER_INTERACTIVE 1

#include <ytab.h>                     /* token name definition file */
#include <Parlex.h>
#include <SymTable.h>
#include <VecBS.h>
#include <Entity.h>
#include <Parser.h>
#include <iostream>
#include <ApiMgr.h>
#include <WitRun.h>
#include <MsgFac.h>

#if _WIN32
#include <io.h>
#define isatty _isatty
#define fileno _fileno
#endif

//------------------------------------------------------------------------------
// The following code permits input lines of length <= 1000.
//------------------------------------------------------------------------------

#undef  YYLMAX
#define YYLMAX 1002

//------------------------------------------------------------------------------

int    newstr = 1;                     /* new-string state       */
int    extraquote = 0;                 
char   *strbuf;                        /* current-string buffer  */         
int    bufsize = 0;                    /* allocated size of string buffer */

char   *yytext_;                       /* local copy of yytext   */
int    yyleng_;                        /* local copy of yyleng   */

static void  yyerror(const char *);
char*  stradd(const char *s);          /* adds a string to the str buffer */


/* yywrap() is redefined as a macro in order to compile on the sun4 platform */
/* note: yywrap is defined using the -D option in appl.mk                    */
/* #define  yywrap() 1 */
#undef yywrap

int yywrap()
{
  WitParseRC retcode;

  retcode = WitParlex::restoreFile();
  if (retcode == 1) return 1;

//printf("Lexer is now reading from file %s\n", WitParlex::fileName());

  WitParser::instance ()->setDataFileName   ();
  WitParser::instance ()->setDataFileLineNo ();

  return 0;
}

%}

%array

%%

release           { return RELEASE; }
add               { return ADD; }
set               { return SET; }
read              { return READ; }
problem           { yylval.strval = dupCString (yytext);
                    return PROBLEM; }                                 
part              { yylval.strval = dupCString (yytext);
                    return PART; }
operation         { yylval.strval = dupCString (yytext);
                    return OPERATION; }
bomEntry          { yylval.strval = dupCString (yytext);
                    return BOM_ENTRY; }
subEntry          { yylval.strval = dupCString (yytext);
                    return SUB_ENTRY; }
bopEntry          { yylval.strval = dupCString (yytext);
                    return BOP_ENTRY; }
demand            { yylval.strval = dupCString (yytext);
                    return DEMAND; }
partWithOperation { yylval.strval = dupCString (yytext);
                    return PART_WITH_OPERATION; }      
single            { return SINGLE; }
sparse            { return SPARSE; }
dense             { return DENSE; }
execBounds        { yylval.strval = dupCString (yytext);
                    return EXEC_BOUNDS; }
stockBounds       { yylval.strval = dupCString (yytext);
                    return STOCK_BOUNDS; }
cumShipBounds     { yylval.strval = dupCString (yytext);
                    return CUM_SHIP_BOUNDS; }
endBounds         { return END_BOUNDS; }
hardLB            { return HARDLB; }
softLB            { return SOFTLB; }
hardUB            { return HARDUB; }
capacity          { yylval.cval = static_cast <int> (WitDRPartCat::capacity);
                    return CATEGORY; }
material          { yylval.cval = static_cast <int> (WitDRPartCat::material);
                    return CATEGORY; }

[();:]            { return yytext[0]; }

-?[0-9]+          { yylval.intval =
                       atoi(reinterpret_cast <const char *> (&yytext[0])); 
                    return INTEGER; }

-?[0-9]*"."[0-9]+([eE][-+]?[0-9]+)? |
-?[0-9]+"."([eE][-+]?[0-9]+)? |
-?[0-9]+[eE][-+]?[0-9]+ {
                    yylval.doubleval =
                       atof(reinterpret_cast <const char *> (&yytext[0]));
                    return DOUBLE; }

true              { yylval.boolval = true;
                    return BOOLEAN; }

false             { yylval.boolval = false;
                    return BOOLEAN; }

\"[^\"\n\\]*[\"\\] { yytext_ = static_cast <char *> (yytext);
                    yyleng_ = yyleng;
                    if (newstr) {
                      strbuf = static_cast <char *> (malloc(LINESIZE + 1));
                      bufsize = LINESIZE + 1;
                      strbuf[0] = '\0';
                      newstr = 0;
                    };
            
                    char c1 = yytext[yyleng-1];  /* last character read */
//#if defined (__GNUC__)
                    char c2 = yyinput();           /*(for GNU) peek at next char */  
//#else
//                    char c2 = input();           /* peek at next char   */  
//#endif
                    unput(c2); 

                    if (extraquote) {
                      yytext_++;                 /* get rid of extra quote */
                      yyleng_--;
                      extraquote = 0;
                    };
  
                    if (c1 == '\\') {
                      if (c2 == '\"') {
                        stradd(yytext_);         /* get rid of extra \ */
                        strbuf[strlen(strbuf)-1] = '\0';
                      }  
                      else
                      if (c2 == '\\') {
                        stradd(yytext_);
//#if defined (__GNUC__)
                        yyinput();           /* (for GNU) get rid of extra \  */  
//#else
//                        input();             /* get rid of extra \  */  
//#endif
                        unput('\"');             /* and fake a "       */
                        extraquote = 1;          /* then continue scan */
                      }
                      else
                         {
                         char msgString[2] = " ";

                         msgString[0] = c2;

                         WitParser::instance ()->myMsgFac () (
                            "invalidStringSmsg",
                            msgString);

                         WitParser::instance ()->myMsgFac () (
                            "lineNumberMsg",
                            yytext, 
                            WitParlex::lineno(), 
                            WitParlex::fileName());

                         WitParser::instance ()->myMsgFac () (
                            "parsingStopsMsg");
                         }
                    }  
                     
                    else 
                    {
                      yylval.strval = stradd(yytext_);
                      bufsize = 0;
                      newstr = 1;                /* return string and     */
                      return STRING;             /* get ready for another */
                    }
                  }

\"[^\"\n\\]*$     { yytext_ = static_cast <char *> (yytext);
                    yyleng_ = yyleng; 
                    if (newstr) {
                      strbuf = static_cast <char *> (malloc(LINESIZE + 1));
                      bufsize = LINESIZE + 1;
                      strbuf[0] = '\0';
                      newstr = 0;
                    };

                    if (extraquote) {
                      yytext_++;                 /* get rid of extra quote */
                      yyleng_--;
                      extraquote = 0;
                    };

                    WitParser::instance ()->myMsgFac () (
                       "unterminatedStringMsg");
                    stradd(yytext_);
                    WitParser::instance ()->myMsgFac () ("lineNumberMsg",
                      strbuf, WitParlex::lineno(), WitParlex::fileName());
                    stradd("\"");
                    yylval.strval = strbuf;
                    bufsize = 0;
                    newstr = 1;
                    return STRING; 
                  } 

[A-Za-z][A-Za-z0-9_]* {
                    yylval.strval = dupCString (yytext);
                    return IDENTIFIER; }

\n                { WitParlex::setLineno(WitParlex::lineno()+1);
                    WitParser::instance ()->setDataFileLineNo ();
                             /* set lineno for API functions */
                  }

[ \t\r]+          ;                      /* white space */

"//".*            ;                      /* comments */

.                 { WitParser::instance ()->myMsgFac () ("invalidCharacterMsg");
                    WitParser::instance ()->myMsgFac () ("lineNumberMsg",
                      yytext, WitParlex::lineno(), WitParlex::fileName());
                  }                      /* anything else */

%%

void yyerror(const char *s)
{
  printf("%s (%c) at line number %-d in file \n%s\n",
    s, yytext[0], WitParlex::lineno(), WitParlex::fileName());
}

char * stradd (const char * s)	
{
  char  *newbuf;
 
  // reallocate the string buffer if necessary
  if (strlen(strbuf) + strlen(s) + 1 > bufsize) {
    newbuf = static_cast <char *> (malloc(strlen(strbuf) + strlen(s) + 1));
    strcpy(newbuf, strbuf);
    strcat(newbuf, s);
    bufsize = strlen(newbuf) + 1;
    free (strbuf);
    strbuf = newbuf;
  }
  else
    strcat(strbuf, s);
  return (strbuf);
}
