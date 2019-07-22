// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbCompareBase_H
#define SbbCompareBase_H


//#############################################################################
/*  These are alternative strategies for node traversal.  
    They can take data etc for fine tuning 

    At present the node list is stored as a heap and the "test"
    comparison function returns true if node y is better than node x.

*/
#include "SbbNode.hpp"

class SbbModel;
class SbbCompareBase {
public:
  SbbCompareBase * test_;
  // Default Constructor 
  SbbCompareBase () {test_=NULL;};

  // This allows any method to change behavior as it is called
  // after each solution
  virtual void newSolution(SbbModel * model) {};

  // This Also allows any method to change behavior as it is called
  // after each solution
  virtual void newSolution(SbbModel * model,
			   double objectiveAtContinuous,
			   int numberInfeasibilitiesAtContinuous) {};

  // This allows any method to change behavior as it is called
  // after every 1000 nodes
  virtual void every1000Nodes(SbbModel * model,int numberNodes) {};

  virtual ~SbbCompareBase() {};

  /// This is test function
  virtual bool test (SbbNode * x, SbbNode * y) {return true;};

  bool operator() (SbbNode * x, SbbNode * y) {
    return test(x,y);
  }
};
class SbbCompare {
public:
  SbbCompareBase * test_;
  // Default Constructor 
  SbbCompare () {test_=NULL;};

  virtual ~SbbCompare() {};

  bool operator() (SbbNode * x, SbbNode * y) {
    return test_->test(x,y);
  }
};
//#############################################################################
/*  These can be alternative strategies for choosing variables
    Any descendant can be passed in by setVariableChoice
*/

class SbbChooseVariable {
public:
  // Default Constructor 
  SbbChooseVariable () {};

  virtual ~SbbChooseVariable() {};
  /** If strong branching, then only those passed in (and movement is that length)
      .  If not strong
      branching then all passed in and ignore movement.
      Returns which one chosen (or -1 if none).  way should be +1
      if branching up, -1 if down */
  virtual int chosen (const SbbModel * model,int numberToLookAt,
		      const int * which, const double * downMovement,
		      const double * upMovement, const double * solution,
		      int & way, double & value)=0;

};
#endif
