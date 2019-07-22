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

#ifndef SaeMgrH
#define SaeMgrH

//------------------------------------------------------------------------------
// Header file: "SaeMgr.h"
//
// Contains the declaration of class SaeMgr.
//------------------------------------------------------------------------------

#include <Util.h>

//------------------------------------------------------------------------------
// Class SaeMgr
//
// "Stand-Alone Execution Manager"
// This class has primary responsibility for performing the actions of the WIT
// stand-alone executable.
//------------------------------------------------------------------------------

class WitSaeMgr
   {
   public:

      //------------------------------------------------------------------------
      // Public member functions.
      //------------------------------------------------------------------------

      static void execute (WitRun * theWitRun, int argc, char * argv[]);
         //
         // Uses theWitRun to perform the main actions of the WIT stand-alone
         // executable.
         // argc and argv are to be passed in from the main program.

      inline static bool standAloneMode ()
         {
         return standAloneMode_;
         }

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitSaeMgr (WitRun *, int argc, char * argv[]);
         //
         // argc and argv are to be passed in from the main program.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitSaeMgr ();

      //------------------------------------------------------------------------
      // Other private member functions.
      //------------------------------------------------------------------------

      void execute ();
         //
         // Performs main actions of the WIT stand-alone executable.

      void setUp ();
         //
         // Sets up WIT for a stand-alone run.

      void setUpMsg ();
         //
         // Sets up the message facility.

      void readInput ();
         //
         // Reads input for the problem and performs related tasks.

      //------------------------------------------------------------------------
      // Parameter value wrapper functions.
      //------------------------------------------------------------------------

      inline const WitString & stringParamVal  (const char *);
      inline const char *      cStringParamVal (const char *);
      inline int               intParamVal     (const char *);
      inline const WitString & choiceParamVal  (const char *);
      inline bool              boolParamVal    (const char *);

      //------------------------------------------------------------------------

      void preprocess ();
         //
         // Performs preprocessing and related tasks.

      void performAction ();
         //
         // Performs the action indicated by the "action" control paramater.

      void printMrp ();
         //
         // Prints the results of MRP.

      void printImplosion ();
         //
         // Prints the results of implosion.

      void doFss ();
         //
         // Does FSS for stand-alone purposes.

      void pipEnableAllOpns ();
         //
         // Sets pipEnabled to true for all Operations.

      void displayDepth ();
         //
         // Displays the Operation depth of the Problem:
         // The maximum # of Operations in any downward path through BOM
         // entries, substitutes, and explodeable BOP entries.
         // Assumes that the Problem is preprocessed.

      void makeVolServObj ();
         //
         // Turns the objective function into a volume serviceability objective.

      void clearObj ();
         //
         // Sets the objective to 0.

      void compVolServ ();
         //
         // Computes and displays the volume serviceability objective for the
         // current implosion solution.

      bool tempParIsSet (int);
         //
         // Wrapper for myProblem ()->myGlobalComp ().

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessNonNull (WitProblem *, myProblem)

      //------------------------------------------------------------------------

      noCopyCtorAssign (WitSaeMgr);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      WitRun * const myWitRun_;
         //
         // The WitRun for this SaeMgr.

      WitSession * const mySession_;
         //
         // The Session for this SaeMgr.

      WitMsgFacility & myMsgFac_;
         //
         // The MsgFac for this SaeMgr.

      WitTimer * myTimer_;
         //
         // The Timer owned by this SaeMgr.

      WitParamMgr * myParamMgr_;
         //
         // The ParamMgr owned by this SaeMgr.

      WitProblem * myProblem_;
         //
         // The Problem for this SaeMgr when one exists; otherwise NULL.

      static bool standAloneMode_;
         //
         // True, iff WIT is being used in standalone executable mode.
   };

#endif
