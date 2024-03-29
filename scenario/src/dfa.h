#ifndef _DFA_
#define _DFA_

// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
/* dfa.h

        Definitions for the DFA (Deterministic Finite-State Automaton)
        class. Also called a State Machine, or just machine.

        The basic function of this class is to perform specified
        actions and optionally change state, based on the input data 
        and the current state.

        Whenever a new token comes in, it is looked up in the list
        of tokens for the current state.  If it is not found, the 
        machine stays in the current state, and it waits for a new
        token.  If the token is found for the current state, several
        things can happen:
        - The machine can change to a new state;
        - If the machine stays in the same state, it can call a
          function (the accumulator function) which processes the
          current token until it is emitted (for example, accumulating
          a numeric string until it is finally emitted when a non-numeric
          character is encountered);
        - When the machine changes state, it can call an emitter
          function to perform meaningful work.  This could be to output
          the information accumulated during the previous state, or
          to do some initialization for the next state.

        TO USE THIS CLASS:

        1. Set up a transition table, consisting of an array of
        DfaTransRec structs, described below.  There must be one for 
        every possible input token, for every possible state.  Specify
        the correct values for each, so that the given token in the
        given state will cause the correct state change and other
        actions.  BE SURE to include a set of inputs for STATE_INIT,
        described below.

        2. Write any specified accumulator and emitter callback func-
        tions.

        3.  Construct a Dfa object, giving it a pointer to the transition
        table.  The new Dfa is now in STATE_INIT, and is ready to receive
        tokens.

        4.  Whenever your program gets a new input that is significant
        to its state, wrap it in a token struct and call InputToken()
        using that token.  The state machine will change state and call
        the callback actions as required.

Local Modification History:
9/29/94: by gracelin: 1. Removed include file std.h, which does not exist here.
		      2. Added RogueWave include file collect.h, and
		         changed BOOL into RWBoolean.
		      3. Changed the name dfa to LgFrDfa
10/2/94:              1. Changed the prototype of InputToken(), now it accepts
			 InputToken(int tokType) also.
		      2. Changed names of variables into the Framework style.
		      3. Changed the prototype of emitFunc and accumFunc in 
			 DfaTransRec so that they allow one argument whose
			 type is void *.
		      4. Added a varaible theObjectPtr which points to
		         the object who owns this dfa.
*/

#ifndef _STD_
// #include "std.h"
#endif


#define STATE_SAME      -1    // Means remain in the same state, do nothing
#define STATE_EOLIST    -2    // End of transition table. The last record
                              // in the transition table must have its
                              // state member set to this value.
#define STATE_INIT      0     // Initial state of machine, which is
	                      // the state before
			      // any input has been received

// forward declaration
class LgFrDfa;

/* TOKEN Structure ----------------------------------------------------

        This structure represents the input data.  It is what is
        presented to the machine when it gets a new token, and is
        what the machine stores for comparison with the input stream.

        The type field is mandatory, of type int so it can easily
        be enum'ed.  It is used to classify the input by type.  For
        example, a text parser may have a "digit" token type and an
        "alpha" token type.

        The contents field is optional.  If it is NULL, then any token
        of the type is valid, and will cause a match in the machine.
        If it is not null, only the specific combination of the type
        and its contents will cause a match, for instance, a type of
        "alpha" and specific contents of the letter "E".

        In this version, the contents pointer can only point to a
        simple data type.  Later versions could include either a
        data-type type or a callback function to do the comparison.
*/

typedef struct {
   int type;         // Token type
   void *contents;   // Specific contents to test for, if any

} Token, *TokPtr;


/* TRANSITION Structure ------------------------------------------------

        The state machine is driven by an array of these structures.
        Each one contains the current state and a token to test for
        when in that state.  If the token is found in that state,
        various state-changes and operations can occur, as detailed
        below.

        state   is the ID of the current state the machine is in at 
                the time of the input token.  It is an int so it can
                easily be enum'ed.  All DfaTransRec's with their state
                matching the current state of the machine (currState_)
                will be matched against the input token.

        token   is a token to check against for this state.

        newState is the state to change to if the input token matches
                this token.  If it equals STATE_SAME, the state does
                not change, and the accumFunc is called.  This is not
                the same as specifying the current state in the newState
                field, as in this case the accumFunc will be called.
                It is best to use STATE_SAME, and point accumFunc to
                NULL.

        stateVariant is an optional extra bit of information that can
                be used to further specify the new state.  For instance,
                a common state to change to is the "error" state, when
                input is not in expected or useful form.  You can put an
                error number in the stateVariant variable representing
                the specific error, and then when the error state is 
                emitted, the error number can identify the type of error.

        accumFunc is a call-back function to be called whenever newState
                is STATE_SAME.  If this pointer is NULL, no action will 
                occur.  This is valuable for building up an output item
                to be emitted when the state changes.  For instance, this
                function can be called while in the "number" state as
                long as the input token is a digit.  The accumulate
                function can build an integer from the digit characters.
                When some non-digit character is encountered, the state
                will change and the emitter function can output the built-
                up integer.

        emitFunc is a call-back function to be called whenever the machine
                changes state.  The state first changes by copying the
                new state into currState_, and then this function is called.

*/

typedef struct {
   int state;                      // If we're in this state...
   Token token;                    // And if current token matches this token...
   int newState;                   // Then change to this state.
   int stateVariant;               // More info about our new state (such as err no)
   void (*accumFunc)(void *); // What to do while still in the state
   void (*emitFunc) (void *); // What to do when we change states

} DfaTransRec, *DfaTransPtr;

/* DFA Class -----------------------------------------------------------

        This class encapsulates the state machine, requiring (usually)
        only repeated calls to InputToken for the machine to run.
*/

class LgFrDfa {

public:

  LgFrDfa (void * objPtr = NULL)
  :  currState_(STATE_INIT), theObjectPtr_(objPtr)
  {}

  // Constructs LgFrDfa with given transition table and object pointer
  LgFrDfa (DfaTransPtr ttbl, void * objPtr = NULL)
  :  transTable_(ttbl),  currState_(STATE_INIT), theObjectPtr_(objPtr)
  {}

  // Destructor
  ~LgFrDfa () {}

  // Selectors, modifiers

   inline int  GetState (void) const { return currState_; } 
   inline const Token* GetCurrToken (void) const { return &currToken_; }
   inline void SetTransTable (DfaTransPtr ttbl) { transTable_ = ttbl; }
 
   void InputToken (Token& t);   // Inputs the next token into the
                                 // machine, thereby advancing state (maybe)
                                 // and emitting (maybe)
   void InputToken (int tokType, void* tokContents = NULL); // Variation
                                 // if your token is not in a struct.

 protected:

   DfaTransPtr transTable_;      // Array of DfaTransRecs for this DFA
   int currState_;               // Current state of the machine
   Token currToken_;             // Current token being processed
   void * theObjectPtr_;	 // point to owner of the dfa

private:
   bool ProcessToken (Token& t, DfaTransPtr transPtr);
   void ProcessState (DfaTransPtr transPtr);

};




#endif  /* of _DFA_ */


        
        
        
