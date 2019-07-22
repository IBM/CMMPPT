// Copyright (C) 2003, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef SbbCutGenerator_H
#define SbbCutGenerator_H

#include "OsiSolverInterface.hpp"
#include "OsiCuts.hpp"

class SbbModel;
class OsiRowCut;
class OsiRowCutDebugger;
class CglCutGenerator;

//#############################################################################

/** This acts as an intermediary between Sbb and Cgl.
    The idea is that it should get more intelligence

*/

class SbbCutGenerator  {
  
  
public:
    
  /**@name Generate Cuts */
  //@{
  /** Generate cuts for the model data contained in si.
  The generated cuts are inserted into and returned in the
  collection of cuts cs.

  Generator makes up its own mind about calling Cgl generator
  (to some extent) unless fullScan.

  Returns true if a resolve is needed
  */
  bool generateCuts( OsiCuts & cs , bool fullScan); 
  //@}

    
  /**@name Constructors and destructors */
  //@{
  /// Default constructor 
  SbbCutGenerator (); 

  /// Normal constructor
  SbbCutGenerator(SbbModel * model,CglCutGenerator * generator,
		  int howOften=1, const char * name=NULL,
		  bool normal=true, bool atSolution=false, 
		  bool infeasible=false);
 
  /// Copy constructor 
  SbbCutGenerator ( const SbbCutGenerator &);

  /// Assignment operator 
  SbbCutGenerator & operator=(const SbbCutGenerator& rhs);

  /// Destructor 
  ~SbbCutGenerator ();

  /** This is used to refresh any inforamtion.
      It also refreshes the solver in the cut generator
      in case generator wants to do some work 
  */
  void refreshModel(SbbModel * model);
  //@}
  /**@name Gets and sets */
  //@{
  /// return name of generator
  inline const char * cutGeneratorName() const
  { return generatorName_;};
  /** How often to call generator, every so many nodes,
      -100 means off, -1 means up to code */
  inline int howOften() const
  { return whenCutGenerator_;};
  void setHowOften(int value) ;
  /// Whether to call generator in normal place
  inline bool normal() const
  { return normal_;};
  inline void setNormal(bool value) 
  { normal_=value;};
  /// Whether to call generator at a solution
  inline bool atSolution() const
  { return atSolution_;};
  inline void setAtSolution(bool value) 
  { atSolution_=value;};
  /// Whether to call generator when branching said infeasible
  inline bool whenInfeasible() const
  { return whenInfeasible_;};
  inline void setWhenInfeasible(bool value) 
  { whenInfeasible_=value;};
  /// Generator
  inline CglCutGenerator * generator() const
  { return generator_;};
  //@}
  
// private:
  /// Pointer to model
  SbbModel * model_;
  // Cut generator
  CglCutGenerator * generator_;
  /// When to use cut generator
  int whenCutGenerator_;
  /// Name of generator
  char * generatorName_;
  /// Whether to call generator in normal place
  bool normal_;
  /// Whether to call generator at a solution
  bool atSolution_;
  /// Whether to call generator when branching said infeasible
  bool whenInfeasible_;
  
};

#endif
