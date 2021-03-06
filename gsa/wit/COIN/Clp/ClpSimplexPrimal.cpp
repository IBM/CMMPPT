// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.


/* Notes on implementation of primal simplex algorithm.

   When primal feasible(A):

   If dual feasible, we are optimal.  Otherwise choose an infeasible
   basic variable to enter basis from a bound (B).  We now need to find an 
   outgoing variable which will leave problem primal feasible so we get
   the column of the tableau corresponding to the incoming variable
   (with the correct sign depending if variable will go up or down).

   We now perform a ratio test to determine which outgoing variable will
   preserve primal feasibility (C).  If no variable found then problem
   is unbounded (in primal sense).  If there is a variable, we then 
   perform pivot and repeat.  Trivial?

   -------------------------------------------

   A) How do we get primal feasible?  All variables have fake costs
   outside their feasible region so it is trivial to declare problem
   feasible.  OSL did not have a phase 1/phase 2 approach but
   instead effectively put an extra cost on infeasible basic variables
   I am taking the same approach here, although it is generalized
   to allow for non-linear costs and dual information.

   In OSL, this weight was changed heuristically, here at present
   it is only increased if problem looks finished.  If problem is
   feasible I check for unboundedness.  If not unbounded we 
   could play with going into dual.  As long as weights increase
   any algorithm would be finite.
   
   B) Which incoming variable to choose is a virtual base class.
   For difficult problems steepest edge is preferred while for
   very easy (large) problems we will need partial scan.

   C) Sounds easy, but this is hardest part of algorithm.
      1) Instead of stopping at first choice, we may be able
      to allow that variable to go through bound and if objective
      still improving choose again.  These mini iterations can
      increase speed by orders of magnitude but we may need to
      go to more of a bucket choice of variable rather than looking
      at them one by one (for speed).
      2) Accuracy.  Basic infeasibilities may be less than
      tolerance.  Pivoting on these makes objective go backwards.
      OSL modified cost so a zero move was made, Gill et al
      modified so a strictly positive move was made.
      The two problems are that re-factorizations can
      change rinfeasibilities above and below tolerances and that when
      finished we need to reset costs and try again.
      3) Degeneracy.  Gill et al helps but may not be enough.  We
      may need more.  Also it can improve speed a lot if we perturb
      the rhs and bounds significantly.  

  References:
     Forrest and Goldfarb, Steepest-edge simplex algorithms for
       linear programming - Mathematical Programming 1992
     Forrest and Tomlin, Implementing the simplex method for
       the Optimization Subroutine Library - IBM Systems Journal 1992
     Gill, Murray, Saunders, Wright A Practical Anti-Cycling
       Procedure for Linear and Nonlinear Programming SOL report 1988


  TODO:
 
  a) Better recovery procedures.  At present I never check on forward
     progress.  There is checkpoint/restart with reducing 
     re-factorization frequency, but this is only on singular 
     factorizations.
  b) Fast methods for large easy problems (and also the option for
     the code to automatically choose which method).
  c) We need to be able to stop in various ways for OSI - this
     is fairly easy.

 */


#include "CoinPragma.hpp"

#include <math.h>

#include "CoinHelperFunctions.hpp"
#include "ClpSimplexPrimal.hpp"
#include "ClpFactorization.hpp"
#include "ClpNonLinearCost.hpp"
#include "CoinPackedMatrix.hpp"
#include "CoinIndexedVector.hpp"
#include "CoinWarmStartBasis.hpp"
#include "ClpPrimalColumnPivot.hpp"
#include "ClpMessage.hpp"
#include <cfloat>
#include <cassert>
#include <string>
#include <stdio.h>
#include <iostream>
// primal 
int ClpSimplexPrimal::primal (int ifValuesPass )
{

  /*
      Method

     It tries to be a single phase approach with a weight of 1.0 being
     given to getting optimal and a weight of infeasibilityCost_ being
     given to getting primal feasible.  In this version I have tried to
     be clever in a stupid way.  The idea of fake bounds in dual
     seems to work so the primal analogue would be that of getting
     bounds on reduced costs (by a presolve approach) and using
     these for being above or below feasible region.  I decided to waste
     memory and keep these explicitly.  This allows for non-linear
     costs!

     The code is designed to take advantage of sparsity so arrays are
     seldom zeroed out from scratch or gone over in their entirety.
     The only exception is a full scan to find incoming variable for 
     Dantzig row choice.  For steepest edge we keep an updated list 
     of dual infeasibilities (actually squares).  
     On easy problems we don't need full scan - just
     pick first reasonable.

     One problem is how to tackle degeneracy and accuracy.  At present
     I am using the modification of costs which I put in OSL and which was
     extended by Gill et al.  I am still not sure of the exact details.

     The flow of primal is three while loops as follows:

     while (not finished) {

       while (not clean solution) {

          Factorize and/or clean up solution by changing bounds so
	  primal feasible.  If looks finished check fake primal bounds.
	  Repeat until status is iterating (-1) or finished (0,1,2)

       }

       while (status==-1) {

         Iterate until no pivot in or out or time to re-factorize.

         Flow is:

         choose pivot column (incoming variable).  if none then
	 we are primal feasible so looks as if done but we need to
	 break and check bounds etc.

	 Get pivot column in tableau

         Choose outgoing row.  If we don't find one then we look
	 primal unbounded so break and check bounds etc.  (Also the
	 pivot tolerance is larger after any iterations so that may be
	 reason)

         If we do find outgoing row, we may have to adjust costs to
	 keep going forwards (anti-degeneracy).  Check pivot will be stable
	 and if unstable throw away iteration and break to re-factorize.
	 If minor error re-factorize after iteration.

	 Update everything (this may involve changing bounds on 
	 variables to stay primal feasible.

       }

     }

     At present we never check we are going forwards.  I overdid that in
     OSL so will try and make a last resort.

     Needs partial scan pivot in option.

     May need other anti-degeneracy measures, especially if we try and use
     loose tolerances as a way to solve in fewer iterations.

     I like idea of dynamic scaling.  This gives opportunity to decouple
     different implications of scaling for accuracy, iteration count and
     feasibility tolerance.

  */

  algorithm_ = +1;

  // save data
  ClpDataSave data = saveData();


    // initialize - maybe values pass and algorithm_ is +1
    if (!startup(ifValuesPass)) {

    int lastCleaned=0; // last time objective or bounds cleaned up
    
    // Progress indicator
    ClpSimplexProgress progress(this);
    
    // Say no pivot has occurred (for steepest edge and updates)
    pivotRow_=-2;
    
    // This says whether to restore things etc
    int factorType=0;
    /*
      Status of problem:
      0 - optimal
      1 - infeasible
      2 - unbounded
      -1 - iterating
      -2 - factorization wanted
      -3 - redo checking without factorization
      -4 - looks infeasible
      -5 - looks unbounded
    */
    while (problemStatus_<0) {
      int iRow,iColumn;
      // clear
      for (iRow=0;iRow<4;iRow++) {
	rowArray_[iRow]->clear();
      }    
      
      for (iColumn=0;iColumn<2;iColumn++) {
	columnArray_[iColumn]->clear();
      }    
      
      // give matrix (and model costs and bounds a chance to be
      // refreshed (normally null)
      matrix_->refresh(this);
      // If getting nowhere - why not give it a kick
#if 0
      // primal perturbation not coded yet
      if (perturbation_<101&&numberIterations_>2*(numberRows_+numberColumns_)) 
	perturb();
#endif
      // If we have done no iterations - special
      if (lastGoodIteration_==numberIterations_)
	factorType=3;
      // may factorize, checks if problem finished
      statusOfProblemInPrimal(lastCleaned,factorType,progress);
      
      // Say good factorization
      factorType=1;
      
      // Say no pivot has occurred (for steepest edge and updates)
      pivotRow_=-2;

      // exit if victory declared
      if (problemStatus_>=0)
	break;
      
      // Iterate
      whileIterating();
    }
  }
  // if infeasible get real values
  if (problemStatus_==1) {
    infeasibilityCost_=0.0;
    createRim(7);
    nonLinearCost_->checkInfeasibilities(true);
    sumPrimalInfeasibilities_=nonLinearCost_->sumInfeasibilities();
    numberPrimalInfeasibilities_= nonLinearCost_->numberInfeasibilities();
    // and get good feasible duals
    computeDuals(NULL);
  }
  // clean up
  unflag();
  finish();
  restoreData(data);
  return problemStatus_;
}
/*
  Reasons to come out:
  -1 iterations etc
  -2 inaccuracy 
  -3 slight inaccuracy (and done iterations)
  -4 end of values pass and done iterations
  +0 looks optimal (might be infeasible - but we will investigate)
  +2 looks unbounded
  +3 max iterations 
*/
int
ClpSimplexPrimal::whileIterating()
{

  // Say if values pass
  int ifValuesPass=(firstFree_>=0) ? 1 : 0;
  int returnCode=-1;
  // status stays at -1 while iterating, >=0 finished, -2 to invert
  // status -3 to go to top without an invert
  while (problemStatus_==-1) {
#ifdef CLP_DEBUG
    {
      int i;
      // not [1] as has information
      for (i=0;i<4;i++) {
	if (i!=1)
	  rowArray_[i]->checkClear();
      }    
      for (i=0;i<2;i++) {
	columnArray_[i]->checkClear();
      }    
    }      
#endif
#if CLP_DEBUG>2
    // very expensive
    if (numberIterations_>0&&numberIterations_<-2534) {
      handler_->setLogLevel(63);
      double saveValue = objectiveValue_;
      double * saveRow1 = new double[numberRows_];
      double * saveRow2 = new double[numberRows_];
      memcpy(saveRow1,rowReducedCost_,numberRows_*sizeof(double));
      memcpy(saveRow2,rowActivityWork_,numberRows_*sizeof(double));
      double * saveColumn1 = new double[numberColumns_];
      double * saveColumn2 = new double[numberColumns_];
      memcpy(saveColumn1,reducedCostWork_,numberColumns_*sizeof(double));
      memcpy(saveColumn2,columnActivityWork_,numberColumns_*sizeof(double));
      createRim(7);
      gutsOfSolution(rowActivityWork_,columnActivityWork_,NULL,NULL);
      printf("xxx %d old obj %g, recomputed %g, sum primal inf %g\n",
	     numberIterations_,
	     saveValue,objectiveValue_,sumPrimalInfeasibilities_);
      memcpy(rowReducedCost_,saveRow1,numberRows_*sizeof(double));
      memcpy(rowActivityWork_,saveRow2,numberRows_*sizeof(double));
      memcpy(reducedCostWork_,saveColumn1,numberColumns_*sizeof(double));
      memcpy(columnActivityWork_,saveColumn2,numberColumns_*sizeof(double));
      delete [] saveRow1;
      delete [] saveRow2;
      delete [] saveColumn1;
      delete [] saveColumn2;
      objectiveValue_=saveValue;
    }
#endif
    if (!ifValuesPass) {
      // choose column to come in
      // can use pivotRow_ to update weights
      // pass in list of cost changes so can do row updates (rowArray_[1])
      // NOTE rowArray_[0] is used by computeDuals which is a 
      // slow way of getting duals but might be used 
      primalColumn(rowArray_[1],rowArray_[2],rowArray_[3],
		   columnArray_[0],columnArray_[1]);
    } else {
      // in values pass
      int sequenceIn=nextSuperBasic();
      if (sequenceIn<0) {
	// end of values pass - initialize weights etc
	handler_->message(CLP_END_VALUES_PASS,messages_)
	  <<numberIterations_;
	primalColumnPivot_->saveWeights(this,5);
	problemStatus_=-2; // factorize now
	pivotRow_=-1; // say no weights update
	returnCode=-4;
	break;
      } else {
	// normal
	sequenceIn_ = sequenceIn;
	valueIn_=solution_[sequenceIn_];
	lowerIn_=lower_[sequenceIn_];
	upperIn_=upper_[sequenceIn_];
	dualIn_=dj_[sequenceIn_];
      }
    }
    pivotRow_=-1;
    sequenceOut_=-1;
    rowArray_[1]->clear();
    if (sequenceIn_>=0) {
      // we found a pivot column
#ifdef CLP_DEBUG
      if ((handler_->logLevel()&32)) {
	char x = isColumn(sequenceIn_) ? 'C' :'R';
	std::cout<<"pivot column "<<
	  x<<sequenceWithin(sequenceIn_)<<std::endl;
      }
#endif
      // do second half of iteration
      returnCode = pivotResult(ifValuesPass);
      if (returnCode<-1&&returnCode>-5) {
	problemStatus_=-2; // 
      } else if (returnCode==-5) {
	// something flagged - continue;
      } else if (returnCode==2) {
	problemStatus_=-5; // looks unbounded
      } else if (returnCode==4) {
	problemStatus_=-2; // looks unbounded but has iterated
      } else if (returnCode!=-1) {
	assert(returnCode==3);
	problemStatus_=3;
      }
    } else {
      // no pivot column
#ifdef CLP_DEBUG
      if (handler_->logLevel()&32)
	printf("** no column pivot\n");
#endif
      if (nonLinearCost_->numberInfeasibilities())
	problemStatus_=-4; // might be infeasible 
      returnCode=0;
      break;
    }
  }
  return returnCode;
}
/* Checks if finished.  Updates status */
void 
ClpSimplexPrimal::statusOfProblemInPrimal(int & lastCleaned,int type,
			       ClpSimplexProgress &progress)
{
  if (type==2) {
    // trouble - restore solution
    memcpy(status_ ,saveStatus_,(numberColumns_+numberRows_)*sizeof(char));
    memcpy(rowActivityWork_,savedSolution_+numberColumns_ ,
	   numberRows_*sizeof(double));
    memcpy(columnActivityWork_,savedSolution_ ,
	   numberColumns_*sizeof(double));
    forceFactorization_=1; // a bit drastic but ..
    pivotRow_=-1; // say no weights update
    changeMade_++; // say change made
  }
  int saveThreshold = factorization_->sparseThreshold();
  int tentativeStatus = problemStatus_;
  if (problemStatus_>-3||problemStatus_==-4) {
    // factorize
    // later on we will need to recover from singularities
    // also we could skip if first time
    // do weights
    // This may save pivotRow_ for use 
    primalColumnPivot_->saveWeights(this,1);

    if (type) {
      // is factorization okay?
      if (internalFactorize(1)) {
	if (solveType_==2) {
	  // say odd
	  problemStatus_=5;
	  return;
	}
#if 1
	internalFactorize(0);
#else

	// no - restore previous basis
	assert (type==1);
	memcpy(status_ ,saveStatus_,(numberColumns_+numberRows_)*sizeof(char));
	memcpy(rowActivityWork_,savedSolution_+numberColumns_ ,
	       numberRows_*sizeof(double));
	memcpy(columnActivityWork_,savedSolution_ ,
	       numberColumns_*sizeof(double));
	forceFactorization_=1; // a bit drastic but ..
	type = 2;
	assert (internalFactorize(1)==0);
#endif
	changeMade_++; // say change made
      }
    }
    if (problemStatus_!=-4)
      problemStatus_=-3;
  }
  // at this stage status is -3 or -5 if looks unbounded
  // get primal and dual solutions
  // put back original costs and then check
  createRim(4);
  gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);
  // Check if looping
  int loop;
  if (type!=2) 
    loop = progress.looping();
  else
    loop=-1;
  if (loop>=0) {
    problemStatus_ = loop; //exit if in loop
    return ;
  } else if (loop<-1) {
    // something may have changed
    gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);
  }
  // really for free variables in
  //if((progressFlag_&2)!=0)
  //problemStatus_=-1;;
  progressFlag_ = 0; //reset progress flag

  handler_->message(CLP_SIMPLEX_STATUS,messages_)
    <<numberIterations_<<objectiveValue();
  handler_->printing(sumPrimalInfeasibilities_>0.0)
    <<sumPrimalInfeasibilities_<<numberPrimalInfeasibilities_;
  handler_->printing(sumDualInfeasibilities_>0.0)
    <<sumDualInfeasibilities_<<numberDualInfeasibilities_;
  handler_->printing(numberDualInfeasibilitiesWithoutFree_
		     <numberDualInfeasibilities_)
		       <<numberDualInfeasibilitiesWithoutFree_;
  handler_->message()<<CoinMessageEol;
  assert (primalFeasible());
  // we may wish to say it is optimal even if infeasible
  bool alwaysOptimal = (specialOptions_&1)!=0;
  // give code benefit of doubt
  if (sumOfRelaxedDualInfeasibilities_ == 0.0&&
      sumOfRelaxedPrimalInfeasibilities_ == 0.0) {
    // say optimal (with these bounds etc)
    numberDualInfeasibilities_ = 0;
    sumDualInfeasibilities_ = 0.0;
    numberPrimalInfeasibilities_ = 0;
    sumPrimalInfeasibilities_ = 0.0;
  }
  // had ||(type==3&&problemStatus_!=-5) -- ??? why ????
  if (dualFeasible()||problemStatus_==-4) {
    
    if (nonLinearCost_->numberInfeasibilities()&&!alwaysOptimal) {
      //may need infeasiblity cost changed
      // we can see if we can construct a ray
      // make up a new objective
      double saveWeight = infeasibilityCost_;
      // save nonlinear cost as we are going to switch off costs
      ClpNonLinearCost * nonLinear = nonLinearCost_;
      // do twice to make sure Primal solution has settled
      // put non-basics to bounds in case tolerance moved
      // put back original costs
      createRim(4);
      nonLinearCost_->checkInfeasibilities(true);
      gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);

      infeasibilityCost_=1.0e100;
      // put back original costs
      createRim(4);
      nonLinearCost_->checkInfeasibilities(true);
      // may have fixed infeasibilities - double check
      if (nonLinearCost_->numberInfeasibilities()==0) {
	// carry on
	problemStatus_ = -1;
	infeasibilityCost_=saveWeight;
      } else {
	nonLinearCost_=NULL;
	// scale
	int i;
	for (i=0;i<numberRows_+numberColumns_;i++) 
	  cost_[i] *= 1.0e-100;
	gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);
	nonLinearCost_=nonLinear;
	infeasibilityCost_=saveWeight;
	if ((infeasibilityCost_>=1.0e18||
	     numberDualInfeasibilities_==0)&&perturbation_==101) {
	  unPerturb(); // stop any further perturbation
	  numberDualInfeasibilities_=1; // carry on
	}
	if (infeasibilityCost_>=1.0e20||
	    numberDualInfeasibilities_==0) {
	  // we are infeasible - use as ray
	  delete [] ray_;
	  ray_ = new double [numberRows_];
	  memcpy(ray_,dual_,numberRows_*sizeof(double));
	  // and get feasible duals
	  infeasibilityCost_=0.0;
	  createRim(4);
	  nonLinearCost_->checkInfeasibilities(true);
	  gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);
	  // so will exit
	  infeasibilityCost_=1.0e30;
	  // reset infeasibilities
	  sumPrimalInfeasibilities_=nonLinearCost_->sumInfeasibilities();;
	  numberPrimalInfeasibilities_=nonLinearCost_->numberInfeasibilities();;
	}
	
	if (infeasibilityCost_<1.0e20) {
	  infeasibilityCost_ *= 5.0;
	  changeMade_++; // say change made
	  handler_->message(CLP_PRIMAL_WEIGHT,messages_)
	    <<infeasibilityCost_
	    <<CoinMessageEol;
	  // put back original costs and then check
	  createRim(4);
	  nonLinearCost_->checkInfeasibilities(true);
	  gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);
	  problemStatus_=-1; //continue
	} else {
	  // say infeasible
	  problemStatus_ = 1;
	}
      }
    } else {
      // may be optimal
      if (perturbation_==101) {
	unPerturb(); // stop any further perturbation
	lastCleaned=-1; // carry on
      }
      if ( lastCleaned!=numberIterations_||unflag()) {
	handler_->message(CLP_PRIMAL_OPTIMAL,messages_)
	  <<primalTolerance_
	  <<CoinMessageEol;
	if (numberTimesOptimal_<4) {
	  numberTimesOptimal_++;
	  changeMade_++; // say change made
	  if (numberTimesOptimal_==1) {
	    // better to have small tolerance even if slower
	    factorization_->zeroTolerance(1.0e-15);
	  }
	  lastCleaned=numberIterations_;
	  handler_->message(CLP_PRIMAL_ORIGINAL,messages_)
	    <<CoinMessageEol;
	  primalTolerance_=dblParam_[ClpPrimalTolerance];
	  
	  // put back original costs and then check
	  createRim(4);
	  nonLinearCost_->checkInfeasibilities(true);
	  gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);
	  problemStatus_ = -1;
	} else {
	  problemStatus_=0; // optimal
	  if (lastCleaned<numberIterations_) {
	    handler_->message(CLP_SIMPLEX_GIVINGUP,messages_)
	      <<CoinMessageEol;
	  }
	}
      } else {
	problemStatus_=0; // optimal
      }
    }
  } else {
    // see if looks unbounded
    if (problemStatus_==-5) {
      if (nonLinearCost_->numberInfeasibilities()) {
	if (infeasibilityCost_>1.0e18&&perturbation_==101) {
	  // back off weight
	  infeasibilityCost_ = 1.0e13;
	  unPerturb(); // stop any further perturbation
	}
	//we need infeasiblity cost changed
	if (infeasibilityCost_<1.0e20) {
	  infeasibilityCost_ *= 5.0;
	  changeMade_++; // say change made
	  handler_->message(CLP_PRIMAL_WEIGHT,messages_)
	    <<infeasibilityCost_
	    <<CoinMessageEol;
	  // put back original costs and then check
	  createRim(4);
	  gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);
	  problemStatus_=-1; //continue
	} else {
	  // say unbounded
	  problemStatus_ = 2;
	}
      } else {
	// say unbounded
	problemStatus_ = 2;
      } 
    } else {
      if(type==3&&problemStatus_!=-5)
	unflag(); // odd
      // carry on
      problemStatus_ = -1;
    }
  }
  if (type==0||type==1||(type==3&&!numberIterations_)) {
    if (!type||(type==3&&!numberIterations_)) {
      // create save arrays
      delete [] saveStatus_;
      delete [] savedSolution_;
      saveStatus_ = new unsigned char [numberRows_+numberColumns_];
      savedSolution_ = new double [numberRows_+numberColumns_];
    }
    // save arrays
    memcpy(saveStatus_,status_,(numberColumns_+numberRows_)*sizeof(char));
    memcpy(savedSolution_+numberColumns_ ,rowActivityWork_,
	   numberRows_*sizeof(double));
    memcpy(savedSolution_ ,columnActivityWork_,numberColumns_*sizeof(double));
  }
  // restore weights (if saved) - also recompute infeasibility list
  if (tentativeStatus>-3) 
    primalColumnPivot_->saveWeights(this,(type <2) ? 2 : 4);
  else
    primalColumnPivot_->saveWeights(this,3);
  if (problemStatus_<0&&!changeMade_) {
    problemStatus_=4; // unknown
  }
  if (saveThreshold) {
    // use default at present
    factorization_->sparseThreshold(0);
    factorization_->goSparse();
  }
  lastGoodIteration_ = numberIterations_;
}
/* 
   Row array has pivot column
   This chooses pivot row.
   For speed, we may need to go to a bucket approach when many
   variables go through bounds
   On exit rhsArray will have changes in costs of basic variables
*/
void 
ClpSimplexPrimal::primalRow(CoinIndexedVector * rowArray,
			    CoinIndexedVector * rhsArray,
			    CoinIndexedVector * spareArray,
			    CoinIndexedVector * spareArray2,
			    int valuesPass)
{
  if (valuesPass) {
    dualIn_ = cost_[sequenceIn_];

    double * work=rowArray->denseVector();
    int number=rowArray->getNumElements();
    int * which=rowArray->getIndices();

    int iIndex;

    for (iIndex=0;iIndex<number;iIndex++) {
      
      int iRow = which[iIndex];
      double alpha = work[iRow];
      int iPivot=pivotVariable_[iRow];
      dualIn_ -= alpha*cost(iPivot);
    }
    // determine direction here
    if (dualIn_<-dualTolerance_) {
      directionIn_=1;
    } else if (dualIn_>dualTolerance_) {
      directionIn_=-1;
    } else {
      // towards nearest bound
      if (valueIn_-lowerIn_<upperIn_-valueIn_) {
	directionIn_=-1;
	dualIn_=dualTolerance_;
      } else {
	directionIn_=1;
	dualIn_=-dualTolerance_;
      }
    }
  }

  // sequence stays as row number until end
  pivotRow_=-1;
  int numberSwapped=0;
  int numberRemaining=0;

  int numberThru =0; // number gone thru a barrier
  int lastThru =0; // number gone thru a barrier on last time
  
  double totalThru=0.0; // for when variables flip
  double acceptablePivot=1.0e-7;
  if (factorization_->pivots())
    acceptablePivot=1.0e-5; // if we have iterated be more strict
  double bestEverPivot=acceptablePivot;
  int lastPivotRow = -1;
  double lastPivot=0.0;
  double lastTheta=1.0e50;
  int lastNumberSwapped=0;

  // use spareArrays to put ones looked at in
  // First one is list of candidates
  // We could compress if we really know we won't need any more
  // Second array has current set of pivot candidates
  // with a backup list saved in double * part of indexed vector

  // for zeroing out arrays after
  int maximumSwapped=0;
  // pivot elements
  double * spare;
  // indices
  int * index, * indexSwapped;
  int * saveSwapped;
  spareArray->clear();
  spareArray2->clear();
  spare = spareArray->denseVector();
  index = spareArray->getIndices();
  saveSwapped = (int *) spareArray2->denseVector();
  indexSwapped = spareArray2->getIndices();

  // we also need somewhere for effective rhs
  double * rhs=rhsArray->denseVector();

  /*
    First we get a list of possible pivots.  We can also see if the
    problem looks unbounded.

    At first we increase theta and see what happens.  We start
    theta at a reasonable guess.  If in right area then we do bit by bit.
    We save possible pivot candidates

   */

  // do first pass to get possibles 
  // We can also see if unbounded
  // We also re-compute reduced cost

  dualIn_ = cost_[sequenceIn_];

  double * work=rowArray->denseVector();
  int number=rowArray->getNumElements();
  int * which=rowArray->getIndices();

  // we need to swap sign if coming in from ub
  double way = directionIn_;
  double maximumMovement;
  if (way>0.0) 
    maximumMovement = min(1.0e30,upperIn_-valueIn_);
  else
    maximumMovement = min(1.0e30,valueIn_-lowerIn_);

  double tentativeTheta = maximumMovement;
  double upperTheta = maximumMovement;

  int iIndex;

  for (iIndex=0;iIndex<number;iIndex++) {

    int iRow = which[iIndex];
    double alpha = work[iRow];
    int iPivot=pivotVariable_[iRow];
    dualIn_ -= alpha*cost(iPivot);
    alpha *= way;
    double oldValue = solution(iPivot);
    // get where in bound sequence
    if (alpha>0.0) {
      // basic variable going towards lower bound
      double bound = lower(iPivot);
      oldValue -= bound;
    } else if (alpha<0.0) {
      // basic variable going towards upper bound
      double bound = upper(iPivot);
      oldValue = bound-oldValue;
    }
    double value = oldValue-tentativeTheta*fabs(alpha);
    assert (oldValue>=-primalTolerance_*1.0001);
    if (value<-primalTolerance_) {
      // add to list
      spare[numberRemaining]=alpha;
      rhs[iRow]=oldValue;
      index[numberRemaining++]=iRow;
      double value=oldValue-upperTheta*fabs(alpha);
      if (value<-primalTolerance_&&fabs(alpha)>=acceptablePivot)
	upperTheta = (oldValue+primalTolerance_)/fabs(alpha);
    }
  }

  // we need to keep where rhs non-zeros are
  int numberInRhs=numberRemaining;
  memcpy(rhsArray->getIndices(),index,numberInRhs*sizeof(int));
  rhsArray->setNumElements(numberInRhs);

  theta_=maximumMovement;

  double dualCheck = fabs(dualIn_);
  // but make a bit more pessimistic
  dualCheck=max(dualCheck-100.0*dualTolerance_,0.99*dualCheck);

  bool goBackOne = false;

  if (numberRemaining) {

    // looks like pivoting
    // now try until reasonable theta
    tentativeTheta = max(10.0*upperTheta,1.0e-7);
    tentativeTheta = min(tentativeTheta,maximumMovement);
    
    // loops increasing tentative theta until can't go through
    
    while (tentativeTheta <= maximumMovement) {
      double thruThis = 0.0;
      
      double bestPivot=acceptablePivot;
      pivotRow_ = -1;
      
      numberSwapped = 0;
      
      upperTheta = maximumMovement;
      
      for (iIndex=0;iIndex<numberRemaining;iIndex++) {

	int iRow = index[iIndex];
	double alpha = spare[iIndex];
	double oldValue = rhs[iRow];
	double value = oldValue-tentativeTheta*fabs(alpha);

	if (value<-primalTolerance_) {
	  // how much would it cost to go thru
	  thruThis += alpha*
	    nonLinearCost_->changeInCost(pivotVariable_[iRow],alpha);
	  // goes on swapped list (also means candidates if too many)
	  indexSwapped[numberSwapped++]=iRow;
	  if (fabs(alpha)>bestPivot) {
	    bestPivot=fabs(alpha);
	    pivotRow_ = iRow;
	    theta_ = oldValue/bestPivot;
	  }
	} else {
	  value = oldValue-upperTheta*fabs(alpha);
	  if (value<-primalTolerance_ && fabs(alpha)>=acceptablePivot) 
	    upperTheta = (oldValue+primalTolerance_)/fabs(alpha);
	}
      }
      
      maximumSwapped = max(maximumSwapped,numberSwapped);

      if (totalThru+thruThis>=dualCheck) {
	// We should be pivoting in this batch
	// so compress down to this lot

	int saveNumber = numberRemaining;
	numberRemaining=0;
	for (iIndex=0;iIndex<numberSwapped;iIndex++) {
	  int iRow = indexSwapped[iIndex];
	  spare[numberRemaining]=way*work[iRow];
	  index[numberRemaining++]=iRow;
	}
	memset(spare+numberRemaining,0,
	       (saveNumber-numberRemaining)*sizeof(double));
	int iTry;
#define MAXTRY 100
	// first get ratio with tolerance
	for (iTry=0;iTry<MAXTRY;iTry++) {
	  
	  upperTheta=maximumMovement;
	  numberSwapped = 0;
	  
	  for (iIndex=0;iIndex<numberRemaining;iIndex++) {
	    
	    int iRow = index[iIndex];
	    double alpha = fabs(spare[iIndex]);
	    double oldValue = rhs[iRow];
	    double value = oldValue-upperTheta*alpha;
	    
	    if (value<-primalTolerance_ && alpha>=acceptablePivot) 
	      upperTheta = (oldValue+primalTolerance_)/alpha;
	    
	  }
	  
	  // now look at best in this lot
	  bestPivot=acceptablePivot;
	  pivotRow_=-1;
	  for (iIndex=0;iIndex<numberRemaining;iIndex++) {
	    
	    int iRow = index[iIndex];
	    double alpha = spare[iIndex];
	    double oldValue = rhs[iRow];
	    double value = oldValue-upperTheta*fabs(alpha);
	    
	    if (value<=0) {
	      // how much would it cost to go thru
	      totalThru += alpha*
		nonLinearCost_->changeInCost(pivotVariable_[iRow],alpha);
	      // goes on swapped list (also means candidates if too many)
	      indexSwapped[numberSwapped++]=iRow;
	      if (fabs(alpha)>bestPivot) {
		bestPivot=fabs(alpha);
		theta_ = oldValue/bestPivot;
		pivotRow_=iRow;
	      }
	    } else {
	      value = oldValue-upperTheta*fabs(alpha);
	      if (value<-primalTolerance_ && fabs(alpha)>=acceptablePivot) 
		upperTheta = (oldValue+primalTolerance_)/fabs(alpha);
	    }
	  }
	  
	  maximumSwapped = max(maximumSwapped,numberSwapped);
	  if (bestPivot<0.1*bestEverPivot&&
	      bestEverPivot>1.0e-6&&bestPivot<1.0e-3) {
	    // back to previous one
	    goBackOne = true;
	    break;
	  } else if (pivotRow_==-1&&upperTheta>largeValue_) {
	    if (lastPivot>acceptablePivot) {
	      // back to previous one
	      goBackOne = true;
	      //break;
	    } else {
	      // can only get here if all pivots so far too small
	    }
	    break;
	  } else if (totalThru>=dualCheck) {
	    break; // no point trying another loop
	  } else {
	    // skip this lot
	    nonLinearCost_->goThru(numberSwapped,way,indexSwapped, work,rhs);
	    lastPivotRow=pivotRow_;
	    lastTheta = theta_;
	    lastThru = numberThru;
	    numberThru += numberSwapped;
	    lastNumberSwapped = numberSwapped;
	    memcpy(saveSwapped,indexSwapped,lastNumberSwapped*sizeof(int));
	    if (bestPivot>bestEverPivot)
	      bestEverPivot=bestPivot;
	  }
	}
	break;
      } else {
	// skip this lot
	nonLinearCost_->goThru(numberSwapped,way,indexSwapped, work,rhs);
	lastPivotRow=pivotRow_;
	lastTheta = theta_;
	lastThru = numberThru;
	numberThru += numberSwapped;
	lastNumberSwapped = numberSwapped;
	memcpy(saveSwapped,indexSwapped,lastNumberSwapped*sizeof(int));
	if (bestPivot>bestEverPivot)
	  bestEverPivot=bestPivot;
	totalThru += thruThis;
	tentativeTheta = 2.0*upperTheta;
      }
    }
    // can get here without pivotRow_ set but with lastPivotRow
    if (goBackOne||(pivotRow_<0&&lastPivotRow>=0)) {
      // back to previous one
      pivotRow_=lastPivotRow;
      theta_ = lastTheta;
	    // undo this lot
      nonLinearCost_->goBack(lastNumberSwapped,saveSwapped,rhs);
      memcpy(indexSwapped,saveSwapped,lastNumberSwapped*sizeof(int));
      numberSwapped = lastNumberSwapped;
    }
  }

  if (pivotRow_>=0) {
    
    alpha_ = work[pivotRow_];
    // translate to sequence
    sequenceOut_ = pivotVariable_[pivotRow_];
    valueOut_ = solution(sequenceOut_);
    lowerOut_=lower_[sequenceOut_];
    upperOut_=upper_[sequenceOut_];
#define MINIMUMTHETA 1.0e-12
    // Movement should be minimum for anti-degeneracy - unless
    // fixed variable out
    double minimumTheta;
    if (upperOut_>lowerOut_)
      minimumTheta=MINIMUMTHETA;
    else
      minimumTheta=0.0;
    // will we need to increase tolerance
#ifdef CLP_DEBUG
    bool found=false;
#endif
    double largestInfeasibility = primalTolerance_;
    if (theta_<minimumTheta) {
      theta_=minimumTheta;
      for (iIndex=0;iIndex<numberSwapped;iIndex++) {
	int iRow = indexSwapped[iIndex];
#ifdef CLP_DEBUG
	if (iRow==pivotRow_)
	  found=true;
#endif
	largestInfeasibility = max (largestInfeasibility,
				    -(rhs[iRow]-fabs(work[iRow])*theta_));
      }
#ifdef CLP_DEBUG
      assert(found);
      if (largestInfeasibility>primalTolerance_&&(handler_->logLevel()&32))
	printf("Primal tolerance increased from %g to %g\n",
	       primalTolerance_,largestInfeasibility);
#endif
      primalTolerance_ = max(primalTolerance_,largestInfeasibility);
    }

    if (way<0.0) 
      theta_ = - theta_;
    double newValue = valueOut_ - theta_*alpha_;
    if (alpha_*way<0.0) {
      directionOut_=-1;      // to upper bound
      if (fabs(theta_)>1.0e-6)
	upperOut_ = nonLinearCost_->nearest(sequenceOut_,newValue);
      else
	upperOut_ = newValue;
    } else {
      directionOut_=1;      // to lower bound
      if (fabs(theta_)>1.0e-6)
	lowerOut_ = nonLinearCost_->nearest(sequenceOut_,newValue);
      else
	lowerOut_ = newValue;
    }
    dualOut_ = reducedCost(sequenceOut_);
  } else if (maximumMovement<1.0e20) {
    // flip
    pivotRow_ = -2; // so we can tell its a flip
    sequenceOut_ = sequenceIn_;
    valueOut_ = valueIn_;
    dualOut_ = dualIn_;
    lowerOut_ = lowerIn_;
    upperOut_ = upperIn_;
    alpha_ = 0.0;
    if (way<0.0) {
      directionOut_=1;      // to lower bound
      theta_ = lowerOut_ - valueOut_;
    } else {
      directionOut_=-1;      // to upper bound
      theta_ = upperOut_ - valueOut_;
    }
  }

  // clear arrays

  memset(spare,0,numberRemaining*sizeof(double));
  memset(saveSwapped,0,maximumSwapped*sizeof(int));

  // put back original bounds etc
  nonLinearCost_->goBackAll(rhsArray);

  rhsArray->clear();

}
/* 
   Chooses primal pivot column
   updateArray has cost updates (also use pivotRow_ from last iteration)
   Would be faster with separate region to scan
   and will have this (with square of infeasibility) when steepest
   For easy problems we can just choose one of the first columns we look at
*/
void 
ClpSimplexPrimal::primalColumn(CoinIndexedVector * updates,
			       CoinIndexedVector * spareRow1,
			       CoinIndexedVector * spareRow2,
			       CoinIndexedVector * spareColumn1,
			       CoinIndexedVector * spareColumn2)
{
  sequenceIn_ = primalColumnPivot_->pivotColumn(updates,spareRow1,
					       spareRow2,spareColumn1,
					       spareColumn2);
  if (sequenceIn_>=0) {
    valueIn_=solution_[sequenceIn_];
    dualIn_=dj_[sequenceIn_];
    if (nonLinearCost_->lookBothWays()) {
      // double check 
      ClpSimplex::Status status = getStatus(sequenceIn_);
      
      switch(status) {
      case ClpSimplex::atUpperBound:
	if (dualIn_<0.0) {
	  // move to other side
	  printf("For %d U (%g, %g, %g) dj changed from %g",
		 sequenceIn_,lower_[sequenceIn_],solution_[sequenceIn_],
		 upper_[sequenceIn_],dualIn_);
	  dualIn_ -= nonLinearCost_->changeUpInCost(sequenceIn_);
	  printf(" to %g\n",dualIn_);
	  nonLinearCost_->setOne(sequenceIn_,upper_[sequenceIn_]+2.0*currentPrimalTolerance());
	  setStatus(sequenceIn_,ClpSimplex::atLowerBound);
	}
	break;
      case ClpSimplex::atLowerBound:
	if (dualIn_>0.0) {
	  // move to other side
	  printf("For %d L (%g, %g, %g) dj changed from %g",
		 sequenceIn_,lower_[sequenceIn_],solution_[sequenceIn_],
		 upper_[sequenceIn_],dualIn_);
	  dualIn_ -= nonLinearCost_->changeDownInCost(sequenceIn_);
	  printf(" to %g\n",dualIn_);
	  nonLinearCost_->setOne(sequenceIn_,lower_[sequenceIn_]-2.0*currentPrimalTolerance());
	  setStatus(sequenceIn_,ClpSimplex::atUpperBound);
	}
	break;
      default:
	break;
      }
    }
    lowerIn_=lower_[sequenceIn_];
    upperIn_=upper_[sequenceIn_];
    if (dualIn_>0.0)
      directionIn_ = -1;
    else 
      directionIn_ = 1;
  } else {
    sequenceIn_ = -1;
  }
}
/* The primals are updated by the given array.
   Returns number of infeasibilities.
   After rowArray will have list of cost changes
*/
int 
ClpSimplexPrimal::updatePrimalsInPrimal(CoinIndexedVector * rowArray,
		  double theta,
		  double & objectiveChange)
{
  double * work=rowArray->denseVector();
  int number=rowArray->getNumElements();
  int * which=rowArray->getIndices();

  int newNumber = 0;

  nonLinearCost_->setChangeInCost(0.0);
  int iIndex;

  for (iIndex=0;iIndex<number;iIndex++) {

    int iRow = which[iIndex];
    double alpha = work[iRow];
    int iPivot=pivotVariable_[iRow];
    double & value = solutionAddress(iPivot);
    double change = theta*alpha;
    value -= change;

    if (change>0.0) {
      // going down
      if (value<=lower(iPivot)+primalTolerance_) {
	double difference = nonLinearCost_->setOne(iPivot,value);
	work[iRow] = difference;
	if (difference) {
	  //change reduced cost on this
	  reducedCostAddress(iPivot) = -difference;
	  which[newNumber++]=iRow;
	}
      } else {
	work[iRow]=0.0;
      }
    } else {
      // going up
      if (value>=upper(iPivot)-primalTolerance_) {
	double difference = nonLinearCost_->setOne(iPivot,value);
	work[iRow] = difference;
	if (difference) {
	  //change reduced cost on this
	  reducedCostAddress(iPivot) = -difference;
	  which[newNumber++]=iRow;
	}
      } else {
	work[iRow]=0.0;
      }
    }
  }
  objectiveChange += nonLinearCost_->changeInCost();
  rowArray->setNumElements(newNumber);
  return 0;
}
// Perturbs problem
void 
ClpSimplexPrimal::perturb()
{
  if (perturbation_>100)
    return; //perturbed already
  int i;
  // primal perturbation
  double perturbation=1.0e-20;
  // maximum fraction of rhs/bounds to perturb
  double maximumFraction = 1.0e-4;
  if (perturbation_>=50) {
    perturbation = 1.0e-4;
    for (i=0;i<numberColumns_+numberRows_;i++) {
      if (upper_[i]>lower_[i]+primalTolerance_) {
	double lowerValue, upperValue;
	if (lower_[i]>-1.0e20)
	  lowerValue = fabs(lower_[i]);
	else
	  lowerValue=0.0;
	if (upper_[i]<1.0e20)
	  upperValue = fabs(upper_[i]);
	else
	  upperValue=0.0;
	double value = max(lowerValue,upperValue);
	perturbation = max(perturbation,value);
      }
    }
    perturbation *= 1.0e-8;
  } else if (perturbation_<100) {
    perturbation = pow(10.0,perturbation_);
    // user is in charge
    maximumFraction = 1.0e100;
  }
  // modify bounds
  handler_->message(CLP_SIMPLEX_PERTURB,messages_)
    <<perturbation
    <<CoinMessageEol;
  for (i=0;i<numberColumns_+numberRows_;i++) {
    double lowerValue=lower_[i], upperValue=upper_[i];
    if (upperValue>lowerValue+primalTolerance_) {
      double value = CoinDrand48()*perturbation;
      if (lowerValue>-1.0e20)
	lowerValue -= value * (max(1.0,1.0e-3*fabs(lowerValue))); 
      if (upperValue<1.0e20)
	upperValue += value * (max(1.0,1.0e-3*fabs(upperValue))); 
    }
    lower_[i]=lowerValue;
    upper_[i]=upperValue;
  }
  // say perturbed
  perturbation_=101;
}
// un perturb
void
ClpSimplexPrimal::unPerturb()
{
  // put back original bounds and costs
  createRim(7);
  // unflag
  unflag();
  // get a valid nonlinear cost function
  delete nonLinearCost_;
  nonLinearCost_= new ClpNonLinearCost(this);
  perturbation_ = 102; // stop any further perturbation
  // move non basic variables to new bounds
  nonLinearCost_->checkInfeasibilities(true);
  gutsOfSolution(rowActivityWork_, columnActivityWork_,NULL,NULL);
}
// Unflag all variables and return number unflagged
int 
ClpSimplexPrimal::unflag()
{
  int i;
  int number = numberRows_+numberColumns_;
  int numberFlagged=0;
  for (i=0;i<number;i++) {
    if (flagged(i)) {
      clearFlagged(i);
      numberFlagged++;
    }
  }
  return numberFlagged;
}
// Do not change infeasibility cost and always say optimal
void 
ClpSimplexPrimal::alwaysOptimal(bool onOff)
{
  if (onOff)
    specialOptions_ |= 1;
  else
    specialOptions_ &= ~1;
}
bool 
ClpSimplexPrimal::alwaysOptimal() const
{
  return (specialOptions_&1)!=0;
}
/*
  Reasons to come out (normal mode/user mode):
  -1 normal
  -2 factorize now - good iteration/ NA
  -3 slight inaccuracy - refactorize - iteration done/ same but factor done
  -4 inaccuracy - refactorize - no iteration/ NA
  -5 something flagged - go round again/ pivot not possible
  +2 looks unbounded
  +3 max iterations (iteration done)
*/
int
ClpSimplexPrimal::pivotResult(int ifValuesPass)
{

  bool roundAgain=true;
  int returnCode=-1;

  // loop round if user setting and doing refactorization
  while (roundAgain) {
    roundAgain=false;
    returnCode=-1;
    pivotRow_=-1;
    sequenceOut_=-1;
    rowArray_[1]->clear();
    // we found a pivot column
    // update the incoming column
    unpack(rowArray_[1]);
    // save reduced cost
    double saveDj = dualIn_;
    factorization_->updateColumn(rowArray_[2],rowArray_[1],true);
    // do ratio test and re-compute dj
    primalRow(rowArray_[1],rowArray_[3],rowArray_[2],rowArray_[0],
	      ifValuesPass);
    if (ifValuesPass) {
      saveDj=dualIn_;
      if (pivotRow_==-1||(pivotRow_>=0&&fabs(alpha_)<1.0e-5)) {
	if(fabs(dualIn_)<1.0e2*dualTolerance_) {
	  // try other way
	  directionIn_=-directionIn_;
	  primalRow(rowArray_[1],rowArray_[3],rowArray_[2],rowArray_[0],
		    0);
	}
	if (pivotRow_==-1||(pivotRow_>=0&&fabs(alpha_)<1.0e-5)) {
	  if (solveType_==1) {
	    // reject it
	    char x = isColumn(sequenceIn_) ? 'C' :'R';
	    handler_->message(CLP_SIMPLEX_FLAG,messages_)
	      <<x<<sequenceWithin(sequenceIn_)
	      <<CoinMessageEol;
	    setFlagged(sequenceIn_);
	    lastBadIteration_ = numberIterations_; // say be more cautious
	    rowArray_[1]->clear();
	    pivotRow_=-1;
	  }
	  returnCode=-5;
	  break;
	}
      }
    }
    if (solveType_==1&&((saveDj*dualIn_<1.0e-20&&!ifValuesPass)||
	fabs(saveDj-dualIn_)>1.0e-3*(1.0+fabs(saveDj)))) {
      handler_->message(CLP_PRIMAL_DJ,messages_)
	<<saveDj<<dualIn_
	<<CoinMessageEol;
      if(lastGoodIteration_ != numberIterations_) {
	rowArray_[1]->clear();
	pivotRow_=-1; // say no weights update
	returnCode=-4;
	if(lastGoodIteration_+1 == numberIterations_) {
	  // not looking wonderful - try cleaning bounds
	  // put non-basics to bounds in case tolerance moved
	  nonLinearCost_->checkInfeasibilities(true);
	}
	sequenceOut_=-1;
	break;
      } else {
	// take on more relaxed criterion
	if (saveDj*dualIn_<1.0e-20||
	    fabs(saveDj-dualIn_)>1.0e-1*(1.0+fabs(dualIn_))) {
	  // need to reject something
	  char x = isColumn(sequenceIn_) ? 'C' :'R';
	  handler_->message(CLP_SIMPLEX_FLAG,messages_)
	    <<x<<sequenceWithin(sequenceIn_)
	    <<CoinMessageEol;
	  setFlagged(sequenceIn_);
	  lastBadIteration_ = numberIterations_; // say be more cautious
	  rowArray_[1]->clear();
	  pivotRow_=-1;
	  returnCode=-5;
	  sequenceOut_=-1;
	  break;
	}
      }
    }
    if (pivotRow_>=0) {
      if (solveType_==2) {
	// **** Coding for user interface
	// do ray
	primalRay(rowArray_[1]);
	// update duals
	if (pivotRow_>=0) {
	  alpha_ = rowArray_[1]->denseVector()[pivotRow_];
	  assert (fabs(alpha_)>1.0e-8);
	  double multiplier = dualIn_/alpha_;
	  rowArray_[0]->insert(pivotRow_,multiplier);
	  factorization_->updateColumnTranspose(rowArray_[2],rowArray_[0]);
	  // put row of tableau in rowArray[0] and columnArray[0]
	  matrix_->transposeTimes(this,-1.0,
				  rowArray_[0],columnArray_[1],columnArray_[0]);
	  // update column djs
	  int i;
	  int * index = columnArray_[0]->getIndices();
	  int number = columnArray_[0]->getNumElements();
	  double * element = columnArray_[0]->denseVector();
	  for (i=0;i<number;i++) {
	    int ii = index[i];
	    dj_[ii] += element[ii];
	    element[ii]=0.0;
	  }
	  columnArray_[0]->setNumElements(0);
	  // and row djs
	  index = rowArray_[0]->getIndices();
	  number = rowArray_[0]->getNumElements();
	  element = rowArray_[0]->denseVector();
	  for (i=0;i<number;i++) {
	    int ii = index[i];
	    dj_[ii+numberColumns_] += element[ii];
	    dual_[ii] = dj_[ii+numberColumns_];
	    element[ii]=0.0;
	  }
	  rowArray_[0]->setNumElements(0);
	  // check incoming
	  assert (fabs(dj_[sequenceIn_])<1.0e-6);
	}
      }
      // if stable replace in basis
      int updateStatus = factorization_->replaceColumn(rowArray_[2],
						       pivotRow_,
						       alpha_);
      // if no pivots, bad update but reasonable alpha - take and invert
      if (updateStatus==2&&
	  lastGoodIteration_==numberIterations_&&fabs(alpha_)>1.0e-5)
	updateStatus=4;
      if (updateStatus==1||updateStatus==4) {
	// slight error
	if (factorization_->pivots()>5||updateStatus==4) {
	  returnCode=-3;
	}
      } else if (updateStatus==2) {
	// major error
	// better to have small tolerance even if slower
	factorization_->zeroTolerance(1.0e-15);
	int maxFactor = factorization_->maximumPivots();
	if (maxFactor>10) {
	  if (forceFactorization_<0)
	    forceFactorization_= maxFactor;
	  forceFactorization_ = max (1,(forceFactorization_>>1));
	} 
	// later we may need to unwind more e.g. fake bounds
	if(lastGoodIteration_ != numberIterations_) {
	  rowArray_[1]->clear();
	  pivotRow_=-1;
	  if (solveType_==1) {
	    returnCode=-4;
	    break;
	  } else {
	    // user in charge - re-factorize
	    int lastCleaned;
	    ClpSimplexProgress dummyProgress;
	    if (saveStatus_)
	      statusOfProblemInPrimal(lastCleaned,1,dummyProgress);
	    else
	      statusOfProblemInPrimal(lastCleaned,0,dummyProgress);
	    roundAgain=true;
	    continue;
	  }
	} else {
	  // need to reject something
	  if (solveType_==1) {
	    char x = isColumn(sequenceIn_) ? 'C' :'R';
	    handler_->message(CLP_SIMPLEX_FLAG,messages_)
	      <<x<<sequenceWithin(sequenceIn_)
	      <<CoinMessageEol;
	    setFlagged(sequenceIn_);
	  }
	  lastBadIteration_ = numberIterations_; // say be more cautious
	  rowArray_[1]->clear();
	  pivotRow_=-1;
	  sequenceOut_=-1;
	  returnCode = -5;
	  break;

	}
      } else if (updateStatus==3) {
	// out of memory
	// increase space if not many iterations
	if (factorization_->pivots()<
	    0.5*factorization_->maximumPivots()&&
	    factorization_->pivots()<200)
	  factorization_->areaFactor(
				     factorization_->areaFactor() * 1.1);
	returnCode =-2; // factorize now
      } else if (updateStatus==5) {
	problemStatus_=-2; // factorize now
      }
      // here do part of steepest - ready for next iteration
      primalColumnPivot_->updateWeights(rowArray_[1]);
    } else {
      if (pivotRow_==-1) {
	// no outgoing row is valid
	rowArray_[0]->clear();
	if (!factorization_->pivots()) {
	  returnCode = 2; //say looks unbounded
	  // do ray
	  primalRay(rowArray_[1]);
	} else if (solveType_==2) {
	  // refactorize
	  int lastCleaned;
	  ClpSimplexProgress dummyProgress;
	  if (saveStatus_)
	    statusOfProblemInPrimal(lastCleaned,1,dummyProgress);
	  else
	    statusOfProblemInPrimal(lastCleaned,0,dummyProgress);
	  roundAgain=true;
	  continue;
	} else {
	  returnCode = 4; //say looks unbounded but has iterated
	}
	break;
      } else {
	// flipping from bound to bound
      }
    }

    
    // update primal solution
    
    double objectiveChange=0.0;
    // Cost on pivot row may change - may need to change dualIn
    double oldCost=0.0;
    if (pivotRow_>=0)
      oldCost = cost(pivotVariable_[pivotRow_]);
    // rowArray_[1] is not empty - used to update djs
    updatePrimalsInPrimal(rowArray_[1],theta_, objectiveChange);
    if (pivotRow_>=0)
      dualIn_ += (oldCost-cost(pivotVariable_[pivotRow_]));
    double oldValue = valueIn_;
    if (directionIn_==-1) {
      // as if from upper bound
      if (sequenceIn_!=sequenceOut_) {
	// variable becoming basic
	valueIn_ -= fabs(theta_);
      } else {
	valueIn_=lowerIn_;
      }
    } else {
      // as if from lower bound
      if (sequenceIn_!=sequenceOut_) {
	// variable becoming basic
	valueIn_ += fabs(theta_);
      } else {
	valueIn_=upperIn_;
      }
    }
    objectiveChange += dualIn_*(valueIn_-oldValue);
    // outgoing
    if (sequenceIn_!=sequenceOut_) {
      if (directionOut_>0) {
	valueOut_ = lowerOut_;
      } else {
	valueOut_ = upperOut_;
      }
      double lowerValue = lower_[sequenceOut_];
      double upperValue = upper_[sequenceOut_];
      assert(valueOut_>=lowerValue-primalTolerance_&&
	     valueOut_<=upperValue+primalTolerance_);
      // may not be exactly at bound and bounds may have changed
      if (valueOut_<=lowerValue+primalTolerance_) {
	directionOut_=1;
      } else if (valueOut_>=upperValue-primalTolerance_) {
	directionOut_=-1;
      } else {
	printf("*** variable wandered off bound %g %g %g!\n",
	       lowerValue,valueOut_,upperValue);
	if (valueOut_-lowerValue<=upperValue-valueOut_) {
	  valueOut_=lowerValue;
	  directionOut_=1;
	} else {
	  valueOut_=upperValue;
	  directionOut_=-1;
	}
      }
      solution_[sequenceOut_]=valueOut_;
      nonLinearCost_->setOne(sequenceOut_,valueOut_);
    }
    // change cost and bounds on incoming if primal
    nonLinearCost_->setOne(sequenceIn_,valueIn_); 
    int whatNext=housekeeping(objectiveChange);
    
    if (whatNext==1) {
	returnCode =-2; // refactorize
    } else if (whatNext==2) {
      // maximum iterations or equivalent
      returnCode=3;
    } else if(numberIterations_ == lastGoodIteration_
	      + 2 * factorization_->maximumPivots()) {
      // done a lot of flips - be safe
      returnCode =-2; // refactorize
    }
  }
  if (solveType_==2&&(returnCode == -2||returnCode==-3)) {
    // refactorize here
    int lastCleaned;
    ClpSimplexProgress dummyProgress;
    if (saveStatus_)
      statusOfProblemInPrimal(lastCleaned,1,dummyProgress);
    else
      statusOfProblemInPrimal(lastCleaned,0,dummyProgress);
    if (problemStatus_==5) {
      printf("Singular basis\n");
      problemStatus_=-1;
      returnCode=5;
    }
  }
#ifdef CLP_DEBUG
  {
    int i;
    // not [1] as may have information
    for (i=0;i<4;i++) {
      if (i!=1)
	rowArray_[i]->checkClear();
    }    
    for (i=0;i<2;i++) {
      columnArray_[i]->checkClear();
    }    
  }      
#endif
  return returnCode;
}
// Create primal ray
void 
ClpSimplexPrimal::primalRay(CoinIndexedVector * rowArray)
{
  delete [] ray_;
  ray_ = new double [numberColumns_];
  ClpFillN(ray_,numberColumns_,0.0);
  int number=rowArray->getNumElements();
  int * index = rowArray->getIndices();
  double * array = rowArray->denseVector();
  double way=-directionIn_;
  int i;
  double zeroTolerance=1.0e-12;
  if (sequenceIn_<numberColumns_)
    ray_[sequenceIn_]=directionIn_;
  for (i=0;i<number;i++) {
    int iRow=index[i];
    int iPivot=pivotVariable_[iRow];
    double arrayValue = array[iRow];
    if (iPivot<numberColumns_&&fabs(arrayValue)>=zeroTolerance)
      ray_[iPivot] = way* array[iRow];
  }
}
  

