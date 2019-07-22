// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbBranchActual_H
#define SbbBranchActual_H

#include "SbbBranchBase.hpp"


/// Define a clique class


class SbbClique : public SbbObject {

public:

  // Default Constructor 
  SbbClique ();

  // Useful constructor (which are integer indices)
  // slack can denote a slack in set
  SbbClique (SbbModel * model, int cliqueType, int numberMembers,
	     const int * which, const char * type,
	     int identifier,int slack=-1);
  
  // Copy constructor 
  SbbClique ( const SbbClique &);
   
  /// Clone
  virtual SbbObject * clone() const;

  // Assignment operator 
  SbbClique & operator=( const SbbClique& rhs);

  // Destructor 
  ~SbbClique ();
  
  /// Infeasibility - large is 0.5
  virtual double infeasibility(int & preferredWay, double & otherWay) const;

  /// This looks at solution and sets bounds to contain solution
  virtual void feasibleRegion();
  /// Creates a branching object
  virtual SbbBranchingObject * createBranch(int way) const;
  /// Number of members
  inline int numberMembers() const
  {return numberMembers_;};

  /// Number of Non SOS members i.e. fixing to zero is strong
  inline int numberNonSOSMembers() const
  {return numberNonSOSMembers_;};

  /// Members (indices in range 0 ... numberIntegers_-1)
  inline const int * members() const
  {return members_;};

  /** Type of each member i.e. which way is strong 0=non SOS, 1 =SOS,
      index is 0 ... numberMembers_-1 */
  inline const char type(int index) const
  {if (type_) return type_[index]; else return 1;};

  /// Clique type - 0 <=, 1 ==
  inline int cliqueType() const
  {return cliqueType_;};

private:
  /// data
  /// Number of members
  int numberMembers_;

  /// Number of Non SOS members i.e. fixing to zero is strong
  int numberNonSOSMembers_;

  /// Members (indices in range 0 ... numberIntegers_-1)
  int * members_;

  /// Type of each member 0=SOS, 1 =clique
  char * type_;

  /// Clique type - 0 <=, 1 ==
   int cliqueType_;

  /// Which one is slack (if any) sequence within this set
  int slack_;
};

/// Define a single integer class


class SbbSimpleInteger : public SbbObject {

public:

  // Default Constructor 
  SbbSimpleInteger ();

  // Useful constructor - passed integer index and model index
  SbbSimpleInteger (SbbModel * model, int sequence, int iColumn);
  
  // Copy constructor 
  SbbSimpleInteger ( const SbbSimpleInteger &);
   
  /// Clone
  virtual SbbObject * clone() const;

  // Assignment operator 
  SbbSimpleInteger & operator=( const SbbSimpleInteger& rhs);

  // Destructor 
  ~SbbSimpleInteger ();
  
  /// Infeasibility - large is 0.5
  virtual double infeasibility(int & preferredWay, double & otherWay) const;

  /// This looks at solution and sets bounds to contain solution
  virtual void feasibleRegion();

  /// Creates a branching object
  virtual SbbBranchingObject * createBranch(int way) const;

  /** Given valid solution (i.e. satisfied) and reduced costs etc
      returns a branching object which would give a new feasible
      point in direction reduced cost says would be cheaper.
      If no feasible point returns null
  */
  virtual SbbBranchingObject * preferredNewFeasible() const;
  
  /** Given valid solution (i.e. satisfied) and reduced costs etc
      returns a branching object which would give a new feasible
      point in direction opposite to one reduced cost says would be cheaper.
      If no feasible point returns null
  */
  virtual SbbBranchingObject * notPreferredNewFeasible() const ;
  
  /** Bounds may be tightened - so it may be good to reset original bounds
   */
  virtual void resetBounds();
  
  /// Sequence number
  inline int sequence() const
  {return sequence_;};

  /// Model column number
  inline int modelSequence() const
  {return columnNumber_;};

  /// Original bounds
  inline double originalLowerBound() const
  { return originalLower_;};
  inline double originalUpperBound() const
  { return originalUpper_;};


private:
  /// data

  /// Sequence
  int sequence_;
  /// Column number in model
  int columnNumber_;
  /// Original lower bound
  double originalLower_;
  /// Original upper bound
  double originalUpper_;
};


/// Simple case for integer variable

class SbbIntegerBranchingObject : public SbbBranchingObject {

public:

  // Default Constructor 
  SbbIntegerBranchingObject ();

  // Useful constructor
  SbbIntegerBranchingObject (SbbModel * model, int variable, int way , double value);
  
  // Useful constructor for fixing
  SbbIntegerBranchingObject (SbbModel * model, int variable, int way,
			     double lowerValue, double upperValue);
  
  // Copy constructor 
  SbbIntegerBranchingObject ( const SbbIntegerBranchingObject &);
   
  // Assignment operator 
  SbbIntegerBranchingObject & operator=( const SbbIntegerBranchingObject& rhs);

  /// Clone
  virtual SbbBranchingObject * clone() const;

  // Destructor 
  virtual ~SbbIntegerBranchingObject ();
  
  /// Does next branch and updates state
  virtual void branch();
private:
  /// data
  /// Lower, Upper bounds on down branch
  double down_[2];
  /// Lower, Upper bounds on up branch
  double up_[2];
};
/** Unordered Clique Branching Object class.
    These are for cliques which are long enough to make it worthwhile
    but <= 64 members.  There will also be ones for long cliques. 
    Variable is number of clique.
 */
class SbbCliqueBranchingObject : public SbbBranchingObject {

public:

  // Default Constructor 
  SbbCliqueBranchingObject ();

  // Useful constructor
  SbbCliqueBranchingObject (SbbModel * model,  const SbbClique * clique,
			    int way,
			    int numberOnDownSide, const int * down,
			    int numberOnUpSide, const int * up);
  
  // Copy constructor 
  SbbCliqueBranchingObject ( const SbbCliqueBranchingObject &);
   
  // Assignment operator 
  SbbCliqueBranchingObject & operator=( const SbbCliqueBranchingObject& rhs);

  /// Clone
  virtual SbbBranchingObject * clone() const;

  // Destructor 
  virtual ~SbbCliqueBranchingObject ();
  
  /// Does next branch and updates state
  virtual void branch();
private:
  /// data
  const SbbClique * clique_;
  /// downMask - bit set to fix to weak bounds, not set to leave unfixed
  unsigned int downMask_[2];
  /// upMask - bit set to fix to weak bounds, not set to leave unfixed
  unsigned int upMask_[2];
};
/** Unordered Clique Branching Object class.
    These are for cliques which are > 64 members
    Variable is number of clique.
 */
class SbbLongCliqueBranchingObject : public SbbBranchingObject {

public:

  // Default Constructor 
  SbbLongCliqueBranchingObject ();

  // Useful constructor
  SbbLongCliqueBranchingObject (SbbModel * model,  const SbbClique * clique,
				 int way,
			    int numberOnDownSide, const int * down,
			    int numberOnUpSide, const int * up);
  
  // Copy constructor 
  SbbLongCliqueBranchingObject ( const SbbLongCliqueBranchingObject &);
   
  // Assignment operator 
  SbbLongCliqueBranchingObject & operator=( const SbbLongCliqueBranchingObject& rhs);

  /// Clone
  virtual SbbBranchingObject * clone() const;

  // Destructor 
  virtual ~SbbLongCliqueBranchingObject ();
  
  /// Does next branch and updates state
  virtual void branch();
private:
  /// data
  const SbbClique * clique_;
  /// downMask - bit set to fix to weak bounds, not set to leave unfixed
  unsigned int * downMask_;
  /// upMask - bit set to fix to weak bounds, not set to leave unfixed
  unsigned int * upMask_;
};

/** Branching decision default class */

class SbbBranchDefaultDecision : public SbbBranchDecision {
public:
  // Default Constructor 
  SbbBranchDefaultDecision ();

  // Copy constructor 
  SbbBranchDefaultDecision ( const SbbBranchDefaultDecision &);

  virtual ~SbbBranchDefaultDecision();

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
  SbbBranchDefaultDecision & operator=(const SbbBranchDefaultDecision& rhs);

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
