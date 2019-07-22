// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif
#include <cassert>
#include <cmath>
#include <cfloat>

#include "OsiSolverInterface.hpp"
#include "SbbModel.hpp"
#include "SbbMessage.hpp"
#include "SbbBranchUser.hpp"
#include "CoinSort.hpp"

// Default Constructor // Default Constructor 
SbbBranchUserDecision::SbbBranchUserDecision()
  :SbbBranchDecision()
{
  bestCriterion_ = 0.0;
  bestChangeUp_ = 0.0;
  bestNumberUp_ = 0;
  bestChangeDown_ = 0.0;
  bestNumberDown_ = 0;
  bestObject_ = NULL;
}

// Copy constructor 
SbbBranchUserDecision::SbbBranchUserDecision (
				    const SbbBranchUserDecision & rhs)
  :SbbBranchDecision()
{
  bestCriterion_ = rhs.bestCriterion_;
  bestChangeUp_ = rhs.bestChangeUp_;
  bestNumberUp_ = rhs.bestNumberUp_;
  bestChangeDown_ = rhs.bestChangeDown_;
  bestNumberDown_ = rhs.bestNumberDown_;
  bestObject_ = rhs.bestObject_;
}

SbbBranchUserDecision::~SbbBranchUserDecision()
{
}

// Clone
SbbBranchDecision * 
SbbBranchUserDecision::clone() const
{
  return new SbbBranchUserDecision(*this);
}

// Initialize i.e. before start of choosing at a node
void 
SbbBranchUserDecision::initialize(SbbModel * model)
{
  bestCriterion_ = 0.0;
  bestChangeUp_ = 0.0;
  bestNumberUp_ = 0;
  bestChangeDown_ = 0.0;
  bestNumberDown_ = 0;
  bestObject_ = NULL;
}


/* Returns nonzero if branching on first object is "better" than on
   second (if second NULL first wins). User can play with decision object.
   This is only used after strong branching.  The initial selection
   is done by infeasibility() for each SbbObject
   return code +1 for up branch preferred, -1 for down
   
*/
int
SbbBranchUserDecision::betterBranch(SbbBranchingObject * thisOne,
			    SbbBranchingObject * bestSoFar,
			    double changeUp, int numberInfeasibilitiesUp,
			    double changeDown, int numberInfeasibilitiesDown)
{
  bool beforeSolution = thisOne->model()->getSolutionCount()==
    thisOne->model()->getNumberHeuristicSolutions();;
  int betterWay=0;
  if (beforeSolution) {
    if (!bestObject_) {
      bestNumberUp_=INT_MAX;
      bestNumberDown_=INT_MAX;
    }
    // before solution - choose smallest number 
    // could add in depth as well
    int bestNumber = min(bestNumberUp_,bestNumberDown_);
    if (numberInfeasibilitiesUp<numberInfeasibilitiesDown) {
      if (numberInfeasibilitiesUp<bestNumber) {
	betterWay = 1;
      } else if (numberInfeasibilitiesUp==bestNumber) {
	if (changeUp<bestCriterion_)
	  betterWay=1;
      }
    } else if (numberInfeasibilitiesUp>numberInfeasibilitiesDown) {
      if (numberInfeasibilitiesDown<bestNumber) {
	betterWay = -1;
      } else if (numberInfeasibilitiesDown==bestNumber) {
	if (changeDown<bestCriterion_)
	  betterWay=-1;
      }
    } else {
      // up and down have same number
      bool better=false;
      if (numberInfeasibilitiesUp<bestNumber) {
	better=true;
      } else if (numberInfeasibilitiesUp==bestNumber) {
	if (min(changeUp,changeDown)<bestCriterion_)
	  better=true;;
      }
      if (better) {
	// see which way
	if (changeUp<=changeDown)
	  betterWay=1;
	else
	  betterWay=-1;
      }
    }
  } else {
    if (!bestObject_) {
      bestCriterion_=-1.0;
    }
    // got a solution
    if (changeUp<=changeDown) {
      if (changeUp>bestCriterion_)
	betterWay=1;
    } else {
      if (changeDown>bestCriterion_)
	betterWay=-1;
    }
  }
  if (betterWay) {
    bestCriterion_ = min(changeUp,changeDown);
    bestChangeUp_ = changeUp;
    bestNumberUp_ = numberInfeasibilitiesUp;
    bestChangeDown_ = changeDown;
    bestNumberDown_ = numberInfeasibilitiesDown;
    bestObject_=thisOne;
  }
  return betterWay;
}
