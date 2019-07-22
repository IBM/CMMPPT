// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbCompareActual_H
#define SbbCompareActual_H


//#############################################################################
/*  These are alternative strategies for node traversal.  
    They can take data etc for fine tuning 

    At present the node list is stored as a heap and the "test"
    comparison function returns true if node y is better than node x.

*/
#include "SbbNode.hpp"
#include "SbbCompareBase.hpp"

// This is default before first solution
class SbbCompareDepth : public SbbCompareBase{
public:
  // Default Constructor 
  SbbCompareDepth () {test_=this;};

  ~SbbCompareDepth() {};
  // This returns true if the depth of node y is greater than depth of node x
  virtual bool test (SbbNode * x, SbbNode * y) {
    return x->depth() < y->depth();
  }
};
class SbbCompareObjective  : public SbbCompareBase {
public:
  // Default Constructor 
  SbbCompareObjective () {test_=this;};

  ~SbbCompareObjective() {};

  /* This returns true if objective value of node y is less than
     objective value of node x */
  virtual bool test (SbbNode * x, SbbNode * y) {
    return x->objectiveValue() > y->objectiveValue();
  }
};
/* This is an example of a more complex rule with data
   It is default after first solution
   If weight is 0.0 then it is computed to hit first solution
   less 2%
*/
class SbbCompareDefault  : public SbbCompareBase {
public:
  // Weight for each infeasibility
  double weight_;
  // Default Constructor 
  SbbCompareDefault () : weight_(0.0) {test_=this;};
  // Constructor with weight
  SbbCompareDefault (double weight) : weight_(weight) {test_=this;};

  ~SbbCompareDefault() {};
  /* This returns true if weighted value of node y is less than
     weighted value of node x */
  virtual bool test (SbbNode * x, SbbNode * y) {
    return x->objectiveValue()+ weight_*x->numberUnsatisfied() > 
      y->objectiveValue() + weight_*y->numberUnsatisfied();
  }
  double getWeight() const
  { return weight_;};
  void setWeight(double weight)
  { weight_ = weight;};
};

/* This is when rounding is being done
*/
class SbbCompareEstimate  : public SbbCompareBase {
public:
  // Default Constructor 
  SbbCompareEstimate () {test_=this;};

  ~SbbCompareEstimate() {};
  virtual bool test (SbbNode * x, SbbNode * y) {
    return x->guessedObjectiveValue() >  y->guessedObjectiveValue() ;
  }
};

#endif
