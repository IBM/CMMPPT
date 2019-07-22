// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbHeuristicLocal_H
#define SbbHeuristicLocal_H

#include "SbbHeuristic.hpp"
/** LocalSearch class
 */

class SbbLocalSearch : public SbbHeuristic {
public:

  // Default Constructor 
  SbbLocalSearch ();

  /* Constructor with model - assumed before cuts
     Initial version does not do Lps
  */
  SbbLocalSearch (SbbModel & model);
  
  // Copy constructor 
  SbbLocalSearch ( const SbbLocalSearch &);
   
  // Destructor 
  ~SbbLocalSearch ();
  
  /// Clone
  virtual SbbHeuristic * clone() const;

  /// update model (This is needed if cliques update matrix etc)
  virtual void setModel(SbbModel * model);
  
  /** returns 0 if no solution, 1 if valid solution.
      Sets solution values if good, sets objective value (only if good)
      This is called after cuts have been added - so can not add cuts
      First tries setting a variable to better value.  If feasible then
      tries setting others.  If not feasible then tries swaps

      ********

      This first version does not do LP's and does swaps of two integer 
      variables.  Later versions could do Lps.
  */
  virtual int solution(double & objectiveValue,
		       double * newSolution);

protected:
  // Data

  // Original matrix by column
  CoinPackedMatrix matrix_;

  // Number of solutions so we only do after new solution
  int numberSolutions_;
  // Type of search 
  int swap_;

private:
  /// Illegal Assignment operator 
  SbbLocalSearch & operator=(const SbbLocalSearch& rhs);
};


#endif
