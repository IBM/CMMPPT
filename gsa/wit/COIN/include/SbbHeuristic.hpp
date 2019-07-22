// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbHeuristic_H
#define SbbHeuristic_H

#include <string>
#include <vector>
#include "CoinPackedMatrix.hpp"
#include "OsiCuts.hpp"

class OsiSolverInterface;

class SbbModel;

//#############################################################################
/** Heuristic base class */

class SbbHeuristic {
public:
  // Default Constructor 
  SbbHeuristic ();

  // Constructor with model - assumed before cuts
  SbbHeuristic (SbbModel & model);

  virtual ~SbbHeuristic();

  /// update model (This is needed if cliques update matrix etc)
  virtual void setModel(SbbModel * model);
  
  /// Clone
  virtual SbbHeuristic * clone() const=0;

  /** returns 0 if no solution, 1 if valid solution
      with better objective value than one passed in
      Sets solution values if good, sets objective value 
      This is called after cuts have been added - so can not add cuts
  */
  virtual int solution(double & objectiveValue,
		       double * newSolution)=0;

  /** returns 0 if no solution, 1 if valid solution, -1 if just
      returning an estimate of best possible solution
      with better objective value than one passed in
      Sets solution values if good, sets objective value (only if nonzero code)
      This is called at same time as cut generators - so can add cuts
      Default is do nothing
  */
  virtual int solution(double & objectiveValue,
		       double * newSolution,
		       OsiCuts & cs) {return 0;};

protected:

  /// Model
  SbbModel * model_;
private:
  
  /// Illegal Assignment operator 
  SbbHeuristic & operator=(const SbbHeuristic& rhs);
  
};

/** Rounding class
 */

class SbbRounding : public SbbHeuristic {
public:

  // Default Constructor 
  SbbRounding ();

  // Constructor with model - assumed before cuts
  SbbRounding (SbbModel & model);
  
  // Copy constructor 
  SbbRounding ( const SbbRounding &);
   
  // Destructor 
  ~SbbRounding ();
  
  /// Clone
  virtual SbbHeuristic * clone() const;

  /// update model (This is needed if cliques update matrix etc)
  virtual void setModel(SbbModel * model);
  
  /** returns 0 if no solution, 1 if valid solution
      with better objective value than one passed in
      Sets solution values if good, sets objective value (only if good)
      This is called after cuts have been added - so can not add cuts
  */
  virtual int solution(double & objectiveValue,
		       double * newSolution);


  /// Set seed
  void setSeed(int value)
  { seed_ = value;};

protected:
  // Data

  // Original matrix by column
  CoinPackedMatrix matrix_;

  // Original matrix by 
  CoinPackedMatrix matrixByRow_;

  // Seed for random stuff
  int seed_;

private:
  /// Illegal Assignment operator 
  SbbRounding & operator=(const SbbRounding& rhs);
};


#endif
