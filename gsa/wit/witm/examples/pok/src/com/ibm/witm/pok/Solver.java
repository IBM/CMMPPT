
////////////////////////////////////////////////////////////////////////////////
// WIT-M/POK Source File Solver.java.
////////////////////////////////////////////////////////////////////////////////

package com.ibm.witm.pok;

import com.ibm.witj.MessageGroup;
import com.ibm.witj.Problem;

import static com.ibm.witm.Embodiment.*;

import static com.ibm.witm.pok.SchemeKeeper.*;

import static com.ibm.witj.Attribute.*;

/*******************************************************************************
 * Responsible for solving an instance of the POK model. This class works with
 * the current Embodiment, which is assumed to belong to Scheme POK.<br>
 * This is a non-instantiatable class: it is to be used statically.
 *******************************************************************************
 */
final class Solver
{
////////////////////////////////////////////////////////////////////////////////
// Private Constructors
////////////////////////////////////////////////////////////////////////////////
   
/*******************************************************************************
 * (Not called)
 *******************************************************************************
 */
   private Solver ()
      {
      }
   
////////////////////////////////////////////////////////////////////////////////
// Private Static Methods
////////////////////////////////////////////////////////////////////////////////

/***********************************|********************************************
 * Builds up the WIT-J Problem.
 *******************************************************************************
 */
   private static void buildProblem ()
      {
      int nPers;

      nPers = currentGlobalItem ().get (N_PERS);

      currentProblem ().set (N_PERIODS, nPers);
      }

/***********************************|********************************************
 * Extracts the solution to the POK model instance from the WIT-J Problem.
 *******************************************************************************
 */
   private static void extractSolution ()
      {
      int nPers;
      
      nPers = currentProblem ().get (N_PERIODS);

      currentGlobalItem ().set (N_PERS2, nPers * 2);
      }

////////////////////////////////////////////////////////////////////////////////
// Package Internal Static Methods
////////////////////////////////////////////////////////////////////////////////

/*******************************************************************************
 * Solves the POK model instance.
 *******************************************************************************
 */
   static void solve ()
      {
      Problem theProblem;
      
      theProblem = Problem.newInstance ();

      currentEmbodiment ().setProblem (theProblem);

      theProblem.
         getMessageMgr ().
            setMesgTimesPrint (MessageGroup.INFORMATIONAL, 0);

      buildProblem ();

      extractSolution ();

      theProblem.deactivate ();
      }

////////////////////////////////////////////////////////////////////////////////
}
