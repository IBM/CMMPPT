// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif

#include "SbbMessage.hpp"

typedef struct {
  SBB_Message internalNumber;
  int externalNumber; // or continuation
  char detail;
  const char * message;
} Sbb_message;
static Sbb_message us_english[]=
{
  {SBB_END_GOOD,1,1,"Search completed - best objective %g, took %d iterations and %d nodes"},
  {SBB_MAXNODES,3,1,"Exiting on maximum nodes"},
  {SBB_MAXTIME,20,1,"Exiting on maximum time"},
  {SBB_MAXSOLS,19,1,"Exiting on maximum solutions"},
  {SBB_SOLUTION,4,1,"Integer solution of %g found after %d iterations and %d nodes"},
  {SBB_END,5,1,"Partial search took %d iterations and %d nodes"},
  {SBB_INFEAS,6,1,"The LP relaxation is infeasible or too expensive"},
  {SBB_STRONG,7,3,"Strong branching on %d (%d), down %g (%d) up %g (%d) value %g"},
  {SBB_SOLINDIVIDUAL,8,2,"%d has value %g"},
  {SBB_INTEGERINCREMENT,9,1,"Objective coefficients multiple of %g"},
  {SBB_STATUS,10,1,"After %d nodes, %d on tree, %g best solution, best possible %g"},
  {SBB_GAP,11,1,"Exiting as integer gap of %g less than %g"},
  {SBB_ROUNDING,12,1,"Integer solution of %g found by rounding after %d iterations and %d nodes"},
  {SBB_ROOT,13,1,"At root node, %d cuts changed objective from %g to %g in %d passes"},
  {SBB_GENERATOR,14,1,"Cut generator %d (%s) - %d row cuts (%d active), %d column cuts - new frequency is %d"},
  {SBB_BRANCH,15,2,"Node %d Obj %g Unsat %d depth %d"},
  {SBB_STRONGSOL,16,1,"Integer solution of %g found by strong branching after %d iterations and %d nodes"},
  {SBB_NOINT,3007,0,"No integer variables - nothing to do"},
  {SBB_VUB_PASS,17,1,"%d solved, %d variables fixed, %d tightened"},
  {SBB_VUB_END,18,1,"After tightenVubs, %d variables fixed, %d tightened"},
  {SBB_DUMMY_END,999999,0,""}
};
/* Constructor */
SbbMessage::SbbMessage(Language language) :
  CoinMessages(sizeof(us_english)/sizeof(Sbb_message))
{
  language_=language;
  strcpy(source_,"Sbb");
  Sbb_message * message = us_english;

  while (message->internalNumber!=SBB_DUMMY_END) {
    CoinOneMessage oneMessage(message->externalNumber,message->detail,
		message->message);
    addMessage(message->internalNumber,oneMessage);
    message ++;
  }

  // now override any language ones

  switch (language) {

  default:
    message=NULL;
    break;
  }

  // replace if any found
  if (message) {
    while (message->internalNumber!=SBB_DUMMY_END) {
      replaceMessage(message->internalNumber,message->message);
      message ++;
    }
  }
}
