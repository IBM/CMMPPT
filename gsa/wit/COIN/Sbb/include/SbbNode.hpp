// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbNode_H
#define SbbNode_H

#include <string>
#include <vector>

#include "CoinWarmStartBasis.hpp"
#include "SbbBranchBase.hpp"

class OsiSolverInterface;

class OsiCuts;
class OsiRowCut;
class OsiRowCutDebugger;
class CoinWarmStartBasis;
class SbbCountRowCut;
class SbbModel;
class SbbNode;

//#############################################################################
/* Node information base class. 
   This may be a complete description of status of model, or it could
   just be a difference between this and parent. */

class SbbNodeInfo {
public:
  // Default Constructor 
  SbbNodeInfo ();

  // Constructor given parent
  SbbNodeInfo (SbbNodeInfo * parent);
   
  // Constructor given parent and owner
  SbbNodeInfo (SbbNodeInfo * parent, SbbNode * owner);
   
  virtual ~SbbNodeInfo();
  /// returns NULL or previous one to apply (if mode ==0 )
  virtual SbbNodeInfo * applyToModel(SbbModel * model, int mode,
				     CoinWarmStartBasis & basis,
				     SbbCountRowCut ** addCuts, int & currentNumberCuts ) const = 0;
  /** Builds up row basis backwards (until original model).
      Returns NULL or previous one to apply .
      Depends on Free being 0 and impossible for cuts
  */
  virtual SbbNodeInfo * buildRowBasis(CoinWarmStartBasis & basis) const = 0;
  /// Clone
  virtual SbbNodeInfo * clone() const = 0;

  /// Increment number of references
  inline void increment(int amount=1)
  {numberPointingToThis_+=amount;};

  /// Decrement number of references and return number left
  inline int decrement(int amount=1)
  {numberPointingToThis_-=amount;return numberPointingToThis_;};

  /// Initialize node - amount will depend on branching object
  inline void initializeInfo(int number)
  {numberPointingToThis_=number;numberBranchesLeft_=number;};

  /// Return number of branches left in object
  inline int numberBranchesLeft() const
  {return numberBranchesLeft_;};

  /// Return number of objects pointing to this
  inline int numberPointingToThis() const
  {return numberPointingToThis_;};

  /// Say one branch taken 
  inline int branchedOn()
  {numberPointingToThis_--;numberBranchesLeft_--;return numberBranchesLeft_;};

  /// Say thrown away
  inline void throwAway()
  {numberPointingToThis_-=numberBranchesLeft_;numberBranchesLeft_=0;};

  /// Parent of this
  SbbNodeInfo * parent() const
  {return parent_;};

  void addCuts(OsiCuts & cuts,int numberToBranch, int * whichGenerator);
  void addCuts(int numberCuts, SbbCountRowCut ** cuts,int numberToBranch);
  /** Delete cuts (decrements counts)
      Slow unless cuts in same order as saved
  */
  void deleteCuts(int numberToDelete,SbbCountRowCut ** cuts);
  void deleteCuts(int numberToDelete,int * which);

  /// Really delete a cut
  void deleteCut(int whichOne);

  /// Decrement active cut counts
  void decrementCuts(int change=1);

  /// Decrement all active cut counts in chain starting at parent
  void decrementParentCuts(int change=1);

  /// Increment all active cut counts in parent chain
  void incrementParentCuts(int change=1);

  /// Array of pointers to cuts
  inline SbbCountRowCut ** cuts() const
  {return cuts_;};

  /// Number of row cuts (this node)
  inline int numberCuts() const
  {return numberCuts_;};
  inline void setNumberCuts(int value)
  {numberCuts_=value;};

  /// Set owner null
  inline void nullOwner()
  { owner_=NULL;};
protected:

  /// Number of other SbbNodeInfos pointing to this
  int numberPointingToThis_;

  /// parent
  SbbNodeInfo * parent_;

  /// Owner
  SbbNode * owner_;

  /// Number of row cuts (this node)
  int numberCuts_;

  /// Array of pointers to cuts
  SbbCountRowCut ** cuts_;

  /** Number of rows in problem (before these cuts).  This
      means that for top of chain it must be rows at continuous */
  int numberRows_;

  /** Number of branches left to do on this node */
  int numberBranchesLeft_;
      
private:
  
  /// Illegal Assignment operator 
  SbbNodeInfo & operator=(const SbbNodeInfo& rhs);
  
};

/* Node information complete class. 
   This is a complete description of status of model and it includes 
   all cuts so far*/

class SbbFullNodeInfo : public SbbNodeInfo {
public:

  virtual SbbNodeInfo * applyToModel(SbbModel * model,int mode,
				     CoinWarmStartBasis & basis,
				     SbbCountRowCut ** addCuts, 
				     int & currentNumberCuts ) const ;
  /** Builds up row basis backwards (until original model).
      Returns NULL or previous one to apply .
      Depends on Free being 0 and impossible for cuts
  */
  virtual SbbNodeInfo * buildRowBasis(CoinWarmStartBasis & basis) const ;
  // Default Constructor 
  SbbFullNodeInfo ();

  /** Constructor from continuous or satisfied
  */
  SbbFullNodeInfo (SbbModel * model,
		   int numberRowsAtContinuous);
  
  // Copy constructor 
  SbbFullNodeInfo ( const SbbFullNodeInfo &);
   
  // Destructor 
  ~SbbFullNodeInfo ();
  
  /// Clone
  virtual SbbNodeInfo * clone() const;
protected:
  // Data
  // Full basis 
  CoinWarmStartBasis basis_;
  int numberIntegers_;
  // Bounds stored in full
  double * lower_;
  double * upper_;
private:
  /// Illegal Assignment operator 
  SbbFullNodeInfo & operator=(const SbbFullNodeInfo& rhs);
};

/* If going virtual - could specialize - for now just one partial */

/* Node information class when words change in basis and
    bounds change
 */

class SbbPartialNodeInfo : public SbbNodeInfo {
public:

  virtual SbbNodeInfo * applyToModel(SbbModel * model,int mode,
				     CoinWarmStartBasis & basis,
				     SbbCountRowCut ** addCuts, 
				     int & currentNumberCuts ) const ;
  /** Builds up row basis backwards (until original model).
      Returns NULL or previous one to apply .
      Depends on Free being 0 and impossible for cuts
  */
  virtual SbbNodeInfo * buildRowBasis(CoinWarmStartBasis & basis ) const ;
  // Default Constructor 
  SbbPartialNodeInfo ();

  // Constructor from current state 
  SbbPartialNodeInfo (SbbNodeInfo * parent, SbbNode * owner,
		int numberChangedBounds,const int * variables,
		const double * boundChanges, int numberChanged,
		const unsigned int * whichBasis,
		const unsigned int * changeBasis);
  
  // Copy constructor 
  SbbPartialNodeInfo ( const SbbPartialNodeInfo &);
   
  // Destructor 
  ~SbbPartialNodeInfo ();
  
  /// Clone
  virtual SbbNodeInfo * clone() const;
private:
  // Data
  // Which parts of basis (top bit set for rows)
  unsigned int * which_;
  /// Values of basis
  unsigned int * basis_;
  /// Number of basis changes
  int numberChanged_;
  /// Which variable (top bit if upper bound changing)
  int * variables_;
  // New bound
  double * newBounds_;
  /// Number of bound changes
  int numberChangedBounds_;
private:
  
  /// Illegal Assignment operator 
  SbbPartialNodeInfo & operator=(const SbbPartialNodeInfo& rhs);
};
// Trivial class for Branch and Bound

class SbbNode  {
 
public:
    
  // Default Constructor 
  SbbNode ();

  // Constructor from current state (and list of integers)
  SbbNode (SbbModel * model,
	   SbbNode * lastNode);
  // constructs basis information etc
  void
  createInfo(SbbModel * model,
	     SbbNode * lastNode,
	     CoinWarmStartBasis & lastws,
	     const double * lastLower, const double * lastUpper,
	     int numberOldActiveCuts,int numberNewCuts);
  
  // Copy constructor 
  SbbNode ( const SbbNode &);
   
  // Assignment operator 
  SbbNode & operator=( const SbbNode& rhs);

  // Destructor 
  ~SbbNode ();
  
  // Chooses branching variable (if none set to -1)
  // Strategy - 1 branch on largest min(up,down)
  //          - 0 branch opposite largest max(up,down)
  // returns 0 normally, -1 if fixed, -2 if infeasible
  int chooseBranch (SbbModel * model,
		    SbbNode * lastNode);
  
  /// Decrement active cut counts
  void decrementCuts(int change=1);

  /// Decrement all active cut counts in chain starting at parent
  void decrementParentCuts(int change=1);

  /// Nulls out node info
  void nullNodeInfo();
  /// Initialize node - amount will depend on branching object
  void initializeInfo();

  /// Does next branch and updates state
  int branch();

  // Information to make basis and bounds
  inline SbbNodeInfo * nodeInfo() const
  {return nodeInfo_;};

  // Objective value
  inline double objectiveValue() const
  { return objectiveValue_;};
  inline void setObjectiveValue(double value)
  { objectiveValue_=value;};
  /// Normal number of branches
  inline int numberBranches() const
  { return branch_->numberBranches();};

  // Branching variable (0 is first integer)
  inline int variable() const
  {if (branch_) return branch_->variable();else return -1;};
  // was - Way to branch - -1 down (first), 1 up, -2 down (second), 2 up (second)
  /* Way to branch -1 down, +1 up.  
     Use nodeInfo--.numberBranchesLeft_ to see how active
  */
  inline int way() const
  {if (branch_) return branch_->way();else return -1;};
  // was - Way to branch - -1 down (first), 1 up, -2 down (second), 2 up (second)
  // Depth
  inline int depth() const
  {return depth_;};
  // Number unsatisfied
  inline int numberUnsatisfied() const
  {return numberUnsatisfied_;};

  // Guessed objective value (for solution)
  inline double guessedObjectiveValue() const
  {return guessedObjectiveValue_;};
  inline void setGuessedObjectiveValue(double value)
  {guessedObjectiveValue_=value;};

private:
  // Data
  // Information to make basis and bounds
  SbbNodeInfo * nodeInfo_;
  // Objective value
  double objectiveValue_;
  // Guessed satisfied Objective value
  double guessedObjectiveValue_;
  SbbBranchingObject * branch_;
  // Depth
  int depth_;
  // Number unsatisfied
  int numberUnsatisfied_;
};


#endif
