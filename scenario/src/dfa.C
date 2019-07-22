/* DFA.CPP

   Implementation of Deterministic Finite Automaton class.

   See DFA.H for details.

Local Modification History:
9/29/94: gracelin: removed include file std.h, which does not exists here.
		   change type BOOL into RWBoolean.

*/
#include <stddef.h>
//#include "std.h"

//#include "dfa.h"
#include <scenario/src/dfa.h>




void LgFrDfa :: InputToken (Token& t)
{

   DfaTransPtr transPtr = transTable_;      // Start at beginning of table

   currToken_.type = t.type;
   currToken_.contents = t.contents;

   while (transPtr->state != STATE_EOLIST) {
      if (transPtr->state == currState_) {
         if (ProcessToken (t, transPtr)) {
            return;
         }
      }
      transPtr++;
   }
}


void LgFrDfa :: InputToken (int tokType, void* tokContents)
{
   Token t;

   t.type = tokType;
   t.contents = tokContents;
   InputToken (t);
}



RWBoolean LgFrDfa :: ProcessToken (Token& t, DfaTransPtr transPtr)
{
   if (transPtr->token.type == t.type) {
      if (transPtr->token.contents == NULL) {
         // Matches token type, no specific contents
         ProcessState (transPtr);
   }
   else {
      // Check for specific match on token contents
      if (*(char*)t.contents ==
          *(char*)transPtr->token.contents) {
         ProcessState (transPtr);
         }
      }
      return TRUE;   // Token was found, end search
   }
   return FALSE;     // Token was not found, continue search
}



void LgFrDfa :: ProcessState (DfaTransPtr transPtr)
{                       
   if (transPtr->newState == STATE_SAME) {
      if (transPtr->accumFunc != NULL) {
         (*transPtr->accumFunc)(theObjectPtr_);
      }
   }
   else {   // State has changed
      if (transPtr->emitFunc != NULL) {
         (*transPtr->emitFunc)(theObjectPtr_);
      }
      currState_ = transPtr->newState;
   }
}
                
        
