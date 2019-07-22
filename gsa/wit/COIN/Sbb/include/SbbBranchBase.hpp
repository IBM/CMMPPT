// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbBranchBase_H
#define SbbBranchBase_H

#include <string>
#include <vector>

class OsiSolverInterface;

class SbbModel;
class SbbNode;
class SbbNodeInfo;
class SbbBranchingObject;

//#############################################################################

/** This is virtual to allow for any form of structure.

Any form of discontinuity is allowed.  The first "numberIntegers_"
are SbbSimpleInteger objects which are as you would expect - they are
only satisfied if they are within tolerance of an integer value.

To create a new type e.g. semi-continuous you need to provide:
   infeasibility, feasibleRegion and createBranch - see below.

At present there only is two way branching.  I don't see many problems in
extended it to multi-way branching.
 */

class SbbObject {

public:

  // Default Constructor 
  SbbObject ();

  // Useful constructor
  SbbObject (SbbModel * model);
  
  // Copy constructor 
  SbbObject ( const SbbObject &);
   
  // Assignment operator 
  SbbObject & operator=( const SbbObject& rhs);

  /// Clone
  virtual SbbObject * clone() const=0;

  /// Destructor 
  virtual ~SbbObject ();

  /** Infeasibility - large is 0.5.  This is some measure of infeasibility
      in preferred direction.  The measure of infeasibility other way
      is returned in otherWay.  This is used to prepare for strong
      branching but should also think of case when no strong branching
  */
  virtual double infeasibility(int & preferredWay, double & otherWay) const =0;

  /** This looks at solution and sets bounds to contain solution
      User should assert that solution is feasible
  */
  virtual void feasibleRegion() = 0;

  /** Creates a branching object and indicate which way first.
      The branching object has to know how to fix things on down and up branch
  */
  virtual SbbBranchingObject * createBranch(int way) const =0;
  
  /** Given valid solution (i.e. satisfied) and reduced costs etc
      returns a branching object which would give a new feasible
      point in direction reduced cost says would be cheaper.
      If no feasible point returns null
  */
  virtual SbbBranchingObject * preferredNewFeasible() const 
  { return NULL;};
  
  /** Given valid solution (i.e. satisfied) and reduced costs etc
      returns a branching object which would give a new feasible
      point in direction opposite to one reduced cost says would be cheaper.
      If no feasible point returns null
  */
  virtual SbbBranchingObject * notPreferredNewFeasible() const 
  { return NULL;};

  /** Bounds may be tightened - so it may be good to reset original bounds
   */
  virtual void resetBounds() {};
  
  /// Identifier 
  inline int id() const
  { return id_;};
  
   /// update model
  inline void setModel(SbbModel * model)
  { model_ = model;};
  
  /// Return model
  inline SbbModel * model() const
  {return  model_;};
  
protected:
  /// data

  // Model
  SbbModel * model_;
  // Id
  int id_;

};

/// This is virtual to allow for SOS 
/// and more exotic stuff.

class SbbBranchingObject {

public:

  // Default Constructor 
  SbbBranchingObject ();

  // Useful constructor
  SbbBranchingObject (SbbModel * model, int variable, int way , double value);
  
  // Copy constructor 
  SbbBranchingObject ( const SbbBranchingObject &);
   
  // Assignment operator 
  SbbBranchingObject & operator=( const SbbBranchingObject& rhs);

  /// Clone
  virtual SbbBranchingObject * clone() const=0;

  // Destructor 
  virtual ~SbbBranchingObject ();
  
  /// Number of branches to evaluate (initial)
  virtual int numberBranches() const
  {return 2;};

  /// Number of branches left to evaluate
  virtual int numberBranchesLeft() const
  {return numberBranchesLeft_;};

  /// Does next branch and updates state
  virtual void branch()=0;

  /** Branching variable (0 is first integer).
      May not be variable for some descendants */
  inline int variable() const
  {return variable_;};
  // was - Way to branch - -1 down (first), 1 up, -2 down (second), 2 up (second)
  /// Way to branch -1 down, +1 up for binary branching  
  inline int way() const
  {return way_;};
  inline void way(int way)
  {way_=way;};
  // Current value
  inline double value() const
  {return value_;};
  
  /// Return model
  inline SbbModel * model() const
  {return  model_;};
protected:
  /// data

  // Model
  SbbModel * model_;
  // Branching variable (0 is first integer)
  int variable_;
  // was - Way to branch - -1 down (first), 1 up, -2 down (second), 2 up (second)
  /* Way to branch -1 down, +1 up.  
     Use nodeInfo--.numberBranchesLeft_ to see how active
  */
  int way_;
  // Current value
  double value_;
  // Number of branches left
  int numberBranchesLeft_;

};


/** Branching decision base class */

class SbbBranchDecision {
public:
  // Default Constructor 
  SbbBranchDecision ();

  virtual ~SbbBranchDecision();

 /// Clone
  virtual SbbBranchDecision * clone() const=0;

  /// Initialize i.e. before start of choosing at a node
  virtual void initialize(SbbModel * model)=0;

  /** Returns nonzero if branching on first object is "better" than on
      second (if second NULL first wins). User can play with decision object.
      This is only used after strong branching.  The initial selection
      is done by infeasibility() for each SbbObject
      return code +1 for up branch preferred, -1 for down
      
 */
  virtual int betterBranch(SbbBranchingObject * thisOne,
			    SbbBranchingObject * bestSoFar,
			    double changeUp, int numberInfeasibilitiesUp,
			    double changeDown, int numberInfeasibilitiesDown)
    = 0;

private:
  
  /// Illegal Assignment operator 
  SbbBranchDecision & operator=(const SbbBranchDecision& rhs);
  
};

#endif
