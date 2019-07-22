// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbBranchUser_H
#define SbbBranchUser_H

#include "SbbBranchBase.hpp"

/** Branching decision user class */

class SbbBranchUserDecision : public SbbBranchDecision {
public:
  // Default Constructor 
  SbbBranchUserDecision ();

  // Copy constructor 
  SbbBranchUserDecision ( const SbbBranchUserDecision &);

  virtual ~SbbBranchUserDecision();

 /// Clone
  virtual SbbBranchDecision * clone() const;

    /// Initialize i.e. before start of choosing at a node
  virtual void initialize(SbbModel * model);

  /** Returns nonzero if branching on first object is "better" than on
      second (if second NULL first wins).
      This is only used after strong branching.  The initial selection
      is done by infeasibility() for each SbbObject
      return code +1 for up branch preferred, -1 for down
      
 */
  virtual int betterBranch(SbbBranchingObject * thisOne,
			    SbbBranchingObject * bestSoFar,
			    double changeUp, int numberInfeasibilitiesUp,
			    double changeDown, int numberInfeasibilitiesDown);

private:
  
  /// Illegal Assignment operator 
  SbbBranchUserDecision & operator=(const SbbBranchUserDecision& rhs);

  /// data

  /// "best" so far
  double bestCriterion_;

  /// Change up for best
  double bestChangeUp_;

  /// Number of infeasibilities for up
  int bestNumberUp_;

  /// Change down for best
  double bestChangeDown_;

  /// Number of infeasibilities for down
  int bestNumberDown_;

  /// Pointer to best branching object
  SbbBranchingObject * bestObject_;

};

#endif
