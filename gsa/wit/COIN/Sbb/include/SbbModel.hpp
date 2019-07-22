// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbModel_H
#define SbbModel_H

#include <string>
#include <vector>

#include "CoinFinite.hpp"
#include "CoinMessageHandler.hpp"
#include "OsiSolverInterface.hpp"
#include "OsiCuts.hpp"
#include "CoinWarmStartBasis.hpp"
#include "SbbCompareBase.hpp"
#include "SbbMessage.hpp"

//class OsiSolverInterface;

class SbbCutGenerator;
class OsiRowCut;
class OsiRowCutDebugger;
class CglCutGenerator;
class SbbHeuristic;
class SbbObject;

//#############################################################################

/** Simple Branch and bound class

*/

class SbbModel  {
  
public:

enum SbbIntParam {
  /** The maximum number of nodes before terminating */
  SbbMaxNumNode=0,
  /** The maximum number of solutions before terminating */
  SbbMaxNumSol,
  /** Just a marker, so that a static sized array can store parameters. */
  SbbLastIntParam
};

enum SbbDblParam {
  /** The maximum amount an integer variable can be away from integer and
      still be considered feasible. */
  SbbIntegerTolerance=0,
  /** The amount to guess the solution will get worse for each
      integer infeasibility. */
  SbbInfeasibilityWeight,
  /** The amount by which to change cutoff at a solution */
  SbbCutoffIncrement,
  /** Stop when gap between best solution and best possible is less than
      this */
  SbbAllowableGap,
  /// maximum number of seconds (double - so should be enough!)
  SbbMaximumSeconds,
  /** Just a marker, so that a static sized array can store parameters. */
  SbbLastDblParam
};

  //---------------------------------------------------------------------------

public:
  ///@name Solve methods 
  //@{
    /// Solve initial LP relaxation 
    void initialSolve(); 

    /// Invoke enumeration algorithm
     void branchAndBound();

  /** Set up cliques.  Only do ones whose length is in range.
      If makeEquality true then a new model may be returned if
      modifications had to be made, otherwise "this" is returned.
      If infeasible numberObjects_ set to -1.
      Must use deleteObjects before a second findCliques.
      If priorities exist then set clique priority to default.
      See SbbBranchingBase.hpp for a discussion of "objects"
   */
  SbbModel * findCliques(bool makeEquality,
			 int atLeastThisMany, int lessThanThis, int defaultValue=1000);
  /** 
      Do Integer Presolve. Returns new model.
      I have to work out cleanest way of getting solution to
      original problem at end.  So this is very preliminary.
   */
  SbbModel * integerPresolve();

  /// Put back information into original model - after integerpresolve 
  void originalModel(SbbModel * presolvedModel);
  /// Delete all object information
  void deleteObjects();

  /** Add in any object information (objects are cloned - owner can delete
      originals */
  void addObjects(int numberObjects, SbbObject ** objects);

  /** For all vubs see if we can tighten bounds by solving Lp's
      type - 0 just vubs
             -1 all (could be very slow)
	     n just n "best" vubs where variable away from bound
	     Returns false if not feasible
	     If CglProbing there will use that as well.

	     If "n" then the n most expensive vub's are chosen
	     where it is assumed that x is at its maximum so delta
	     would have to go to 1 (if x not at bound)
      allowMultipleBinary - if true then vub is row with one
             continuous and any number of binary
      useCutoff - use this integer cutoff (as a constraint)
  */
  bool tightenVubs(int type,bool allowMultipleBinary=false,
		   double useCutoff=1.0e50);
  /// This version is just handed a list of variables
  bool tightenVubs(int numberVubs, const int * which,
		   double useCutoff=1.0e50);

    //@}

  //---------------------------------------------------------------------------
  /**@name Parameter set/get methods

     The set methods return true if the parameter was set to the given value,
     false if the value of the parameter is out of range.

     The get methods return the value of the parameter.

  */
  //@{
  /// Set an integer parameter
  inline bool setIntParam(SbbIntParam key, int value) {
    intParam_[key] = value;
    return true;
  }
  /// Set an double parameter
  inline bool setDblParam(SbbDblParam key, double value) {
    dblParam_[key] = value;
    return true;
  }
  /// Get an integer parameter
  inline int getIntParam(SbbIntParam key) const {
    return intParam_[key];
  }
  /// Get an double parameter
  inline double getDblParam(SbbDblParam key) const {
    return dblParam_[key];
  }
  /// Set Cutoff
  inline void setCutoff(double value)
  {solver_->setDblParam(OsiDualObjectiveLimit,value);};

  /// Get Cutoff
  inline double getCutoff() const
  {double value;solver_->getDblParam(OsiDualObjectiveLimit,value);return value;};

  /// Max nodes
  inline bool setMaximumNodes( int value) {
    return setIntParam(SbbMaxNumNode,value);
  }
  inline int getMaximumNodes() const {
    return getIntParam(SbbMaxNumNode);
  }
  /// Max solutions
  inline bool setMaximumSolutions( int value) {
    return setIntParam(SbbMaxNumSol,value);
  }
  inline int getMaximumSolutions() const {
    return getIntParam(SbbMaxNumSol);
  }
  /// Integer tolerance
  inline bool setIntegerTolerance( double value) {
    return setDblParam(SbbIntegerTolerance,value);
  }
  inline double getIntegerTolerance() const {
    return getDblParam(SbbIntegerTolerance);
  }
  /** The amount to guess the solution will get worse for each
      integer infeasibility. */
  inline bool setInfeasibilityWeight( double value) {
    return setDblParam(SbbInfeasibilityWeight,value);
  }
  inline double getInfeasibilityWeight() const {
    return getDblParam(SbbInfeasibilityWeight);
  }
  /** Stop when gap between best solution and best possible is less than
      this - this is absolute - user can convert from relative */
  inline bool setAllowableGap( double value) {
    return setDblParam(SbbAllowableGap,value);
  }
  inline double getAllowableGap() const {
    return getDblParam(SbbAllowableGap);
  }
  //@}

  //---------------------------------------------------------------------------
  ///@name Methods returning info on how the solution process terminated
  //@{
    /// Are there a numerical difficulties?
    bool isAbandoned() const;
    /// Is optimality proven?
    bool isProvenOptimal() const;
    /// Is  infeasiblity proven (or none better than cutoff)?
    bool isProvenInfeasible() const;
    /// Node limit reached?
    bool isNodeLimitReached() const;
    /// Solution limit reached?
    bool isSolutionLimitReached() const;
  //@}

  //---------------------------------------------------------------------------
  /**@name Problem information methods 
     
     These methods call the solver's query routines to return
     information about the problem referred to by the current object.
     Querying a problem that has no data associated with it result in
     zeros for the number of rows and columns, and NULL pointers from
     the methods that return vectors.
     
     Const pointers returned from any data-query method are valid as
     long as the data is unchanged and the solver is not called.
  */
  //@{
  /**@name Methods related to querying the input data */
    //@{
  /// Get number of columns
  int getNumCols() const
  { return solver_->getNumCols();};
  
  /// Get number of rows
  int getNumRows() const
  { return solver_->getNumRows();};
  
  /// Get number of nonzero elements
  int getNumElements() const
  { return solver_->getNumElements();};
  
  /// Get pointer to array[getNumCols()] of column lower bounds
  const double * getColLower() const
  { return solver_->getColLower();};
  
  /// Get pointer to array[getNumCols()] of column upper bounds
  const double * getColUpper() const
  { return solver_->getColUpper();};
  
  /** Get pointer to array[getNumRows()] of row constraint senses.
      <ul>
      <li>'L': <= constraint
      <li>'E': =  constraint
      <li>'G': >= constraint
      <li>'R': ranged constraint
      <li>'N': free constraint
      </ul>
  */
  const char * getRowSense() const
  { return solver_->getRowSense();};
  
  /** Get pointer to array[getNumRows()] of rows right-hand sides
      <ul>
      <li> if rowsense()[i] == 'L' then rhs()[i] == rowupper()[i]
      <li> if rowsense()[i] == 'G' then rhs()[i] == rowlower()[i]
      <li> if rowsense()[i] == 'R' then rhs()[i] == rowupper()[i]
      <li> if rowsense()[i] == 'N' then rhs()[i] == 0.0
      </ul>
  */
  const double * getRightHandSide() const
  { return solver_->getRightHandSide();};
  
  /** Get pointer to array[getNumRows()] of row ranges.
      <ul>
      <li> if rowsense()[i] == 'R' then
      rowrange()[i] == rowupper()[i] - rowlower()[i]
      <li> if rowsense()[i] != 'R' then
      rowrange()[i] is 0.0
      </ul>
  */
  const double * getRowRange() const
  { return solver_->getRowRange();};
  
  /// Get pointer to array[getNumRows()] of row lower bounds
  const double * getRowLower() const
  { return solver_->getRowLower();};
  
  /// Get pointer to array[getNumRows()] of row upper bounds
  const double * getRowUpper() const
  { return solver_->getRowUpper();};
  
  /// Get pointer to array[getNumCols()] of objective function coefficients
  const double * getObjCoefficients() const
  { return solver_->getObjCoefficients();};
  
  /// Get objective function sense (1 for min (default), -1 for max)
  double getObjSense() const
  { return solver_->getObjSense();};
  
  /// Return true if variable is continuous
  bool isContinuous(int colIndex) const
  { return solver_->isContinuous(colIndex);};
  
  /// Return true if variable is binary
  bool isBinary(int colIndex) const
  { return solver_->isBinary(colIndex);};
  
  /** Return true if column is integer.
      Note: This function returns true if the the column
      is binary or a general integer.
  */
  bool isInteger(int colIndex) const
  { return solver_->isInteger(colIndex);};
  
  /// Return true if variable is general integer
  bool isIntegerNonBinary(int colIndex) const
  { return solver_->isIntegerNonBinary(colIndex);};
  
  /// Return true if variable is binary and not fixed at either bound
  bool isFreeBinary(int colIndex) const
  { return solver_->isFreeBinary(colIndex) ;};
  
  /// Get pointer to row-wise copy of matrix
  const CoinPackedMatrix * getMatrixByRow() const
  { return solver_->getMatrixByRow();};
  
  /// Get pointer to column-wise copy of matrix
  const CoinPackedMatrix * getMatrixByCol() const
  { return solver_->getMatrixByCol();};
  
  /// Get solver's value for infinity
  double getInfinity() const
  { return solver_->getInfinity();};
  //@}
  
  
  /**@name Methods related to querying the solution */
  //@{
  /// Get pointer to array[getNumCols()] of primal solution vector
  const double * getColSolution() const
  { return solver_->getColSolution();};
  
  /// Get pointer to array[getNumRows()] of dual prices
  const double * getRowPrice() const
  { return solver_->getRowPrice();};
  
  /// Get a pointer to array[getNumCols()] of reduced costs
  const double * getReducedCost() const
  { return solver_->getReducedCost();};
  
  /// Get pointer to array[getNumRows()] of row activity levels.
  const double * getRowActivity() const
  { return solver_->getRowActivity();};
  
  /// Get current objective function value
  double getCurrentObjValue() const
  { return solver_->getObjValue();};
  
  /// Get how many iterations it took to solve the problem.
  int getIterationCount() const
  { return solver_->getIterationCount();};
  
  /// Get best objective function value
  double getObjValue() const
  { return bestObjective_;};
  
  /// Get array holding best solution (NULL if none)
  const double * bestSolution() const
  { return bestSolution_;};
  
  /// Get how many Nodes it took to solve the problem.
  int getNodeCount() const
  { return numberNodes_;};
  
  /// Get number of solutions
  int getSolutionCount() const
  { return numberSolutions_;};
  
  /// Set number of solutions (so heuristics will be different)
  void setSolutionCount(int value) 
  { numberSolutions_=value;};
  
  /// Get force priority level 
  int getForcePriority() const
  { return forcePriority_;};
  
  /// Set force priority level
  void setForcePriority(int value) 
  { forcePriority_=value;};
  
  /// Get number of heuristic solutions
  int getNumberHeuristicSolutions() const
  { return numberHeuristicSolutions_;};
  /// Set objective function sense (1 for min (default), -1 for max,)
  void setObjSense(double s)
  { solver_->setObjSense(s);};
  /// Set or get minimum drop to continue cuts
  inline double getMinimumDrop() const
  { return minimumDrop_;};
  inline void setMinimumDrop(double value)
  {minimumDrop_=value;};
  /** Set or get maximum number of cut passes at root node (default 20)
      Minimum drop can also be used for fine tuning */
  inline int getMaximumCutPassesAtRoot() const
  { return maximumCutPassesAtRoot_;};
  inline void setMaximumCutPassesAtRoot(int value)
  {maximumCutPassesAtRoot_=value;};
  /** Set or get maximum number of cut passes at other nodes (default 10)
      Minimum drop can also be used for fine tuning */
  inline int getMaximumCutPasses() const
  { return maximumCutPasses_;};
  inline void setMaximumCutPasses(int value)
  {maximumCutPasses_=value;};
  /// Number of integers in problem
  inline int numberIntegers() const
  { return numberIntegers_;};
  // Integer variables
  inline const int * integerVariable() const 
  { return integerVariable_;};
  /// Strong branching
  inline int numberStrong() const
  { return numberStrong_;};
  void setNumberStrong(int number);
  /// Final status - 0 finished, 1 stopped, 2 difficulties
  inline int status() const
  { return status_;};
  /** Print frequency (has very slight overhead if small)
      If <=0 100 for large, 1000 for small problem */
  inline int printFrequency() const
  { return printFrequency_;};
  void setPrintFrequency(int number)
  { printFrequency_=number;};
  /// Value of objective at continuous
  inline double getContinuousObjective() const
  { return continuousObjective_;};
  inline void setContinuousObjective(double value)
  { continuousObjective_=value;};
  /// Number of infeasibilities at continuous
  inline int getContinuousInfeasibilities() const
  { return continuousInfeasibilities_;};
  inline void setContinuousInfeasibilities(int value)
  { continuousInfeasibilities_=value;};
  // Comparison functions (which may be overridden by inheritance)
  inline SbbCompareBase * nodeComparison() const
  { return nodeCompare_;};
  inline void setNodeComparison(SbbCompareBase * compare)
  { nodeCompare_ = compare;};
  inline void setNodeComparison(SbbCompareBase & compare)
  { nodeCompare_ = &compare;};
  inline SbbBranchDecision * branchingMethod() const
  { return branchingMethod_;};
  inline void setBranchingMethod(SbbBranchDecision * method)
  { branchingMethod_ = method;};
  inline void setBranchingMethod(SbbBranchDecision & method)
  { branchingMethod_ = &method;};
  /// Number of cut generators
  inline int numberCutGenerators() const
  { return numberCutGenerators_;};
  /// Cut generators
  inline SbbCutGenerator ** cutGenerators() const
  { return generator_;};
  inline SbbCutGenerator * cutGenerator(int i) const
  { return generator_[i];};
  /** Add one generator - up to user to delete generators.
      howoften affects how generator is used. 0 or 1 means always,
      >1 means every that number of nodes.  Negative values have same
      meaning as positive but they may be switched off (-> -100) by code if
      not many cuts generated at continuous.  -99 is just done at root.
      Name is just for printout
  */
  void addCutGenerator(CglCutGenerator * generator,
		  int howOften=1, const char * name=NULL,
		  bool normal=true, bool atSolution=false, 
		  bool infeasible=false);

  /// Add one heuristic
  void addHeuristic(SbbHeuristic * generator);
  /// Number of objects
  inline int numberObjects() const
  { return numberObjects_;};
  /// Array of objects
  inline SbbObject ** objects() const
  { return object_;};
  /// One objects
  const inline SbbObject * object(int which) const
  { return object_[which];};
  /** Pass in priorities (added for Kurt Spielberg).  If ifClique then priorities on cliques
      otherwise priorities on integers.  
      Other type (if exists set to default)
      1 is highest priority. (well actually -INT_MAX is but that's ugly)
      If a priority < forcePriority() then branches are created to force
      the variable to the value given by best solution.  This enables a
      sort of hot start.  The node choice should be greatest depth
      and forcePriority should normally be switched off after a solution.
  */
  void passInPriorities(const int * priorities, bool ifClique, int defaultValue=1000);
  /// Priorities
  inline const int * priority() const
  { return priority_;};
  /// Returns priority level for integer variable (or 1000 if no priority)
  inline int priority(int sequence) const
  { 
    if (priority_)
      return priority_[sequence];
    else
      return 1000;
  };
  //@}
    
  
  //---------------------------------------------------------------------------

  /**@name Message handling */
  //@{
  /// Pass in Message handler (not deleted at end)
  void passInMessageHandler(CoinMessageHandler * handler);
  /// Set language
  void newLanguage(CoinMessages::Language language);
  void setLanguage(CoinMessages::Language language)
  {newLanguage(language);};
  /// Return handler
  CoinMessageHandler * messageHandler() const
  {return handler_;};
  /// Return messages
  CoinMessages messages() 
  {return messages_;};
  /// Return pointer to messages
  CoinMessages * messagesPointer() 
  {return &messages_;};
  //@}
  //---------------------------------------------------------------------------


  ///@name Constructors and destructors etc
  //@{
    /// Default Constructor
    SbbModel(); 
    
    /// Constructor from solver
    SbbModel(const OsiSolverInterface &);
  
    /// Assign solver
    void assignSolver(OsiSolverInterface *);
  
    /// Copy constructor 
    SbbModel(const SbbModel &);
  
    /// Assignment operator 
    SbbModel & operator=(const SbbModel& rhs);
  
    /// Destructor 
     ~SbbModel ();

    /// Returns solver - has current state
    OsiSolverInterface * solver() const
    { return solver_;};

    /// Returns solver with continuous state
    OsiSolverInterface * continuousSolver() const
    { return continuousSolver_;};
  //@}

public:

  ///@name Other public stuff
  //@{
  /// Array for cuts - null means not active
  SbbCountRowCut ** addedCuts() const
  { return addedCuts_;};
  /// Rows at continuous
  int numberRowsAtContinuous() const
  { return numberRowsAtContinuous_;};
  /// Current number of cuts
   int currentNumberCuts() const
  { return currentNumberCuts_;};
  /** Call this from anywhere when solution found.
      Solution is number columns in size */
  void setBestSolution(SBB_Message how,
		       double objectiveValue, const double * solution,bool fixVariables=false);
  /** Test if solution is feasible.
      Sets number of infeasibilities for normal and odd
  */
  bool feasibleSolution(int & numberIntegerInfeasibilities,
			int & numberObjectInfeasibilities) const;
  /// Current solution (i.e. returned from solver)
  inline double * currentSolution() const
  { return currentSolution_;};

  //@}

private:

  ///@name Other stuff
  //@{
  /// Adds in cuts (calls addCuts1)
  int addCuts(SbbNode * node, 
	      CoinWarmStartBasis & lastws);
  /// make sure model is correct
  void synchronizeModel();
  /// Fill in integers and create objects (if start again clears objects)
  void findIntegers(bool startAgain);
  /** 
      Do Integer Presolve - destroying current model
      returns true if still looks feasible
   */
  bool integerPresolveThisModel(OsiSolverInterface * originalSolver);
public:
  /// Creates addedCuts_ and correct basis
  void addCuts1(SbbNode * node, 
	      CoinWarmStartBasis & lastws);
private:
  /** Solve with cuts
      Return true if feasible */
  bool solveWithCuts(OsiCuts & cuts, int numberTries,SbbNode * node,
		     int & numberOldActiveCuts, int & numberNewCuts,
		     int & maximumWhich, int * & whichGenerator);
  /// Take off cuts
  void takeOffCuts(OsiCuts & cuts,int * whichGenerator,
		     int & numberOldActiveCuts, int & numberNewCuts);
  /// Solve a node - return true if feasible
  bool resolve();
  
  
  //@}
  //---------------------------------------------------------------------------

  //---------------------------------------------------------------------------

private:
  ///@name Private member data 
  //@{
  /// Model and solver
  OsiSolverInterface * solver_;
  /// Copy at continuous - mainly for getting clean solutions
  OsiSolverInterface * continuousSolver_;
   /// Message handler
  CoinMessageHandler * handler_;
  /// Flag to say if default handler (so delete)
  bool defaultHandler_;
  /// Messages
  CoinMessages messages_;
  /// Array of integer parameters
  int intParam_[SbbLastIntParam];
  /// Array of double parameters
  double dblParam_[SbbLastDblParam];
  CoinWarmStartBasis basis_;
  /// Best objective
  double bestObjective_;
  /// Array holding best solution
  double * bestSolution_;
  /// Array holding current solution
  double * currentSolution_;
  /// Global cuts
  OsiCuts globalCuts_;
  /// Minimum drop for continuoung to add cuts
  double minimumDrop_;
  /// Number of solutions
  int numberSolutions_;
  /// Force priority level
  int forcePriority_;
  /// Number of heuristic solutions
  int numberHeuristicSolutions_;
  /// Cumulative number of nodes
  int numberNodes_;
  /// Cumulative number of iterations
  int numberIterations_;
  /// Status of problem - 0 finished, 1 stopped, 2 difficulties
  int status_;
  /// Number of integers in problem
  int numberIntegers_;
  /// Number of rows at continuous
  int numberRowsAtContinuous_;
  /// Maximum number of cuts
  int maximumNumberCuts_;
  /// Current number of cuts
  int currentNumberCuts_;
  /// Maximum depth
  int maximumDepth_;
  /// Array for walkback
  SbbNodeInfo ** walkback_;
  /// Array for cuts (size maximumNumberCuts_)
  SbbCountRowCut ** addedCuts_;
  // Integer variables
  int * integerVariable_;
  /// 0 bit gomory, 1 probing, 2 knapsack, 3 oddhole
  int strategy_;
  /// User node comparison function
  SbbCompareBase * nodeCompare_;
  /// Variable selection function
  SbbBranchDecision * branchingMethod_;
  /// Number to look at for strong branching (0==off)
  int numberStrong_;
  /// Print frequency
  int printFrequency_;
  /// Number of cut generators
  int numberCutGenerators_;
  // Cut generators
  SbbCutGenerator ** generator_;
  /// Number of heuristics
  int numberHeuristics_;
  // Heuristic solvers
  SbbHeuristic ** heuristic_;
  /// Number of cliques
  int numberObjects_;
  /// Integer and Clique and ... information
  SbbObject ** object_;
  /// Original columns as created by integerPresolve
  int * originalColumns_;
  /// Priorities
  int * priority_;
  /// How often to scan global cuts
  int howOftenGlobalScan_;
  /// Value of objective at continuous (? array with depths)
  double continuousObjective_;
  /// Number of infeasibilities at continuous
  int continuousInfeasibilities_;
  /// Maximum number of cut passes at root
  int maximumCutPassesAtRoot_;
  /// Maximum number of cut passes
  int maximumCutPasses_;
 //@}
};

#endif
