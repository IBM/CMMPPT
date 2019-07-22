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

//------------------------------------------------------------------------------
// Source file: "OptMisc.C"
//
// Implementation of the following classes:
//
//    BoundPair
//    Coeff
//    OptVC
//    OptVar
//    OptCon
//------------------------------------------------------------------------------

#include <Coeff.h>
#include <OptVar.h>
#include <OptCon.h>
#include <OptProblem.h>
#include <OptComp.h>
#include <Timing.h>

#include <float.h>

//------------------------------------------------------------------------------
// Implementation of class BoundPair.
//------------------------------------------------------------------------------

WitBoundPair::WitBoundPair ():

      lower_ (0.0),
      upper_ (0.0)
   {
   }

//------------------------------------------------------------------------------

WitBoundPair::WitBoundPair (double lowerInit, double upperInit):
      lower_ (lowerInit),
      upper_ (upperInit)
   {
   }

//------------------------------------------------------------------------------

void WitBoundPair::operator = (const WitBoundPair & theBoundPair)
   {
   lower () = theBoundPair.lower ();
   upper () = theBoundPair.upper ();
   }

//------------------------------------------------------------------------------

WitBoundPair::~WitBoundPair ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class Coeff.
//------------------------------------------------------------------------------

WitCoeff::WitCoeff (WitOptCon * theCon, WitOptVar * theVar, double theValue):

      myCon_   (theCon),
      myVar_   (theVar),
      myValue_ (theValue)
   {
   }

//------------------------------------------------------------------------------

WitCoeff::~WitCoeff ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class OptVC.
//------------------------------------------------------------------------------

WitOptVC::WitOptVC (WitOptProblem * theOptProblem, int theIndex):

      myCoeffVec_ (),
      nCoeffs_    (0),
      bounds_     (0.0, 0.0),
      index_      (theIndex)
   {
   }

//------------------------------------------------------------------------------

WitOptVC::~WitOptVC ()
   {
   }

//------------------------------------------------------------------------------

void WitOptVC::writeExtReal (
      FILE * outFile,
      const char * fFormat,
      const char * sFormat,
      double val)
   {
   if      (val == + DBL_MAX)
      fprintf (outFile, sFormat, "+Infty");

   else if (val == - DBL_MAX)
      fprintf (outFile, sFormat, "-Infty");

   else
      fprintf (outFile, fFormat, val);
   }

//------------------------------------------------------------------------------

void WitOptVC::allocMyCoeffs (int nCoeffsForOptVC)
   {
   myCoeffVec_.resize (nCoeffsForOptVC);
   }

//------------------------------------------------------------------------------

void WitOptVC::clearMyCoeffs ()
   {
   nCoeffs_ = 0;
   }

//------------------------------------------------------------------------------

void WitOptVC::store (WitCoeff * theCoeff)
   {
   myCoeffVec_[nCoeffs_] = theCoeff;

   nCoeffs_ ++;
   }

//------------------------------------------------------------------------------

void WitOptVC::compBounds ()
   {
   bounds_.upper () = upperBoundVal ();
   bounds_.lower () = lowerBoundVal ();
   }

//------------------------------------------------------------------------------

void WitOptVC::printID ()
   {
   fprintf (myOptProblem ()->optProbFile (), "%s #%-7d %-12s",
      kindText (), index (), classText ());

   printSource ();
   }

//------------------------------------------------------------------------------

void WitOptVC::printBounds (const WitBoundPair & preScreenBounds)
   {
   writeExtReal (myOptProblem ()->optProbFile (), " %7.1f", " %7s",
      bounds ().lower ());

   writeExtReal (myOptProblem ()->optProbFile (), " %7.1f", " %7s",
      bounds ().upper ());

   if (preScreenBounds.lower () == bounds ().lower ())
      fprintf (myOptProblem ()->optProbFile (), " %7s", "");
   else
      writeExtReal (myOptProblem ()->optProbFile (), " %7.1f", " %7s",
         preScreenBounds.lower ());

   if (preScreenBounds.upper () == bounds ().upper ())
      fprintf (myOptProblem ()->optProbFile (), " %7s", "");
   else
      writeExtReal (myOptProblem ()->optProbFile (), " %7.1f", " %7s",
         preScreenBounds.upper ());

   fprintf (myOptProblem ()->optProbFile (), "\n");
   }

//------------------------------------------------------------------------------

void WitOptVC::attachCoeffItr (WitCoeffItr & theItr)
   {
   myCoeffVec_.attachItr (theItr);
   }

//------------------------------------------------------------------------------

WitProblem * WitOptVC::myProblem ()
   {
   return myOptProblem ()->myProblem ();
   }

//------------------------------------------------------------------------------
// Implementation of class OptVar.
//------------------------------------------------------------------------------

WitOptVar::WitOptVar (WitOptProblem * theOptProblem):

      WitOptVC  (theOptProblem, theOptProblem->nOptVars ()),
      objCoeff_ (0.0)
   {
   theOptProblem->store (this);
   }

//------------------------------------------------------------------------------

WitOptVar::~WitOptVar ()
   {
   }

//------------------------------------------------------------------------------

double WitOptVar::solnValue ()
   {
   return primalValue ();
   }

//------------------------------------------------------------------------------

double WitOptVar::indepInitValue ()
   {
   return 0.0;
   }

//------------------------------------------------------------------------------

bool WitOptVar::isAnIntVar ()
   {
   return false;
   }

//------------------------------------------------------------------------------

const char * WitOptVar::lexObjElemName ()
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

void WitOptVar::reportLexUnbounded ()
   {
   stronglyAssert (false);
   }

//------------------------------------------------------------------------------

WitDetVar * WitOptVar::mandDetVar ()
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

WitStochVar * WitOptVar::mandStochVar ()
   {
   stronglyAssert (false);

   return NULL;
   }

//------------------------------------------------------------------------------

void WitOptVar::incCoeff (double delta)
   {
   witAssert (this != NULL);

   myOptProblem ()->incCoeff (this, delta);
   }

//------------------------------------------------------------------------------

void WitOptVar::generateObjCoeff ()
   {
   if (myOptProblem ()->myOptComp ()->multiObjMode ())
      {
      objCoeff_ = 0.0;
      }
   else
      objCoeff_ = compObjCoeff ();
   }

//------------------------------------------------------------------------------

void WitOptVar::print ()
   {
   printID ();

   fprintf (myOptProblem ()->optProbFile (), 
      " %1s %8.1f",
      isAnIntVar ()? "I": " ",
      objCoeff ());

   printBounds (myOptProblem ()->preScreenBounds (this));
   }

//------------------------------------------------------------------------------

double WitOptVar::reducedReward ()
   {
   double      rrVal;
   WitCoeffItr theItr;
   WitCoeff *  theCoeff;

   rrVal = objCoeff ();

   attachCoeffItr (theItr);

   while (theItr.advance (theCoeff))
      rrVal -= theCoeff->myCon ()->dualValue () * theCoeff->myValue ();

    return rrVal;
   }

//------------------------------------------------------------------------------

void WitOptVar::setPrimalValue (double theValue)
   {
   primalValue_ = theValue;
   }

//------------------------------------------------------------------------------

const char * WitOptVar::kindText ()
   {
   return "Var";
   }

//------------------------------------------------------------------------------
// Implementation of class OptCon.
//------------------------------------------------------------------------------

WitOptCon::WitOptCon (WitOptProblem * theOptProblem):

      WitOptVC   (theOptProblem, theOptProblem->nOptCons ()),

      dualValue_ (0.0)
   {
   theOptProblem->store (this);
   }

//------------------------------------------------------------------------------

WitOptCon::~WitOptCon ()
   {
   WitCoeffItr theItr;
   WitCoeff *  theCoeff;

   attachCoeffItr (theItr);

   while (theItr.advance (theCoeff))
      delete theCoeff;
   }

//------------------------------------------------------------------------------

double WitOptCon::solnValue ()
   {
   return dualValue ();
   }

//------------------------------------------------------------------------------
// screenable
//
// Returns true, iff this OptCon can be screened.
//
// It is assumed that all variables that intersect this OptCon are
// constrained (implicitly or explicitly) to be nonnegative.
// The following conditions are tested:
//
// A. The upper bound on this OptCon is zero.
// B. Each variable whose Coeff in this OptCon is negative has an upper bound
//    of zero.
// C. Each variable whose Coeff in this OptCon is non-zero has a nonpositive
//    lower bound and a nonnegative upper bound.
//
// When A and B hold, it follows that any variable with non-zero Coeff in
// this OptCon must be zero to be feasible. If, in addition, C fails to hold,
// then this OptCon causes an infeasibility and so it should not be screened.
// Otherwise, if A, B, and C hold, then this OptCon should be screened.
//------------------------------------------------------------------------------

bool WitOptCon::screenable ()
   {
   WitCoeffItr theItr;
   WitCoeff *  theCoeff;

   if (fabs (bounds ().upper ()) > FLOAT_EPSILON)
      return false;
         //
         // A fails.

   attachCoeffItr (theItr);

   while (theItr.advance (theCoeff))
      if (theCoeff->myValue () < - FLOAT_EPSILON)
         if (fabs (theCoeff->myVar ()->bounds ().upper ()) > FLOAT_EPSILON)
            return false;
               //
               // B fails.

   attachCoeffItr (theItr);

   while (theItr.advance (theCoeff))
      {
      if (fabs (theCoeff->myValue ()) <= FLOAT_EPSILON)
         continue;

      if (theCoeff->myVar ()->bounds ().lower () > + FLOAT_EPSILON)
         return false;
            //
            // C fails.

      if (theCoeff->myVar ()->bounds ().upper () < - FLOAT_EPSILON)
         return false;
            //
            // C fails.
      }

   return true;
   }

//------------------------------------------------------------------------------

void WitOptCon::print ()
   {
   WitCoeffItr theItr;
   WitCoeff *  theCoeff;

   fprintf     (myOptProblem ()->optProbFile (), "\n");
   printID     ();
   printBounds (myOptProblem ()->preScreenBounds (this));

   attachCoeffItr (theItr);

   while (theItr.advance (theCoeff))
      {
      theCoeff->myVar ()->printID ();

      fprintf (myOptProblem ()->optProbFile (),
         " %7.1f\n",
         theCoeff->myValue ());
      }
   }

//------------------------------------------------------------------------------

void WitOptCon::setDualValue (double theValue)
   {
   dualValue_ = theValue;
   }

//------------------------------------------------------------------------------

const char * WitOptCon::kindText ()
   {
   return "Con";
   }
