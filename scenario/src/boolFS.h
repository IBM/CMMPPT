#ifndef LGFR_DIADIC_FILTER_STRATEGY_H
#define LGFR_DIADIC_FILTER_STRATEGY_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/filtStrt.h>


//--------------------------------------------------------------------------
// LgFrDiadicFilterStrategy:
//
// Abstract base class for filter strategy which performs an operation
// on two other filter strategies.  An example of a concrete class 
// derived from this class is LgFrAndFilterStrategy and 
// LgFrOrFilterStrategy.
//
//--------------------------------------------------------------------------

class LgFrDiadicFilterStrategy : public LgFrFilterStrategy {
public:

  // Return true if itemTimeVec is to be excluded from schedule
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const = 0;

  // Set left hand side filter strategy
  virtual
     void
        leftHandSideFilterStrategy(
           const LgFrFilterStrategy & );

  // Get left hand side filter strategy.
  // If this object ceases to exist or changes its filterStrategy
  // then the returned reference will be come dangling.
  virtual
     const LgFrFilterStrategy &
        leftHandSideFilterStrategy()
           const;

  // Set right hand side filter strategy
  virtual
     void
        rightHandSideFilterStrategy(
           const LgFrFilterStrategy & );

  // Get right hand side filter strategy.
  // If this object ceases to exist or changes its filterStrategy
  // then the returned reference will be come dangling.
  virtual
     const LgFrFilterStrategy &
        rightHandSideFilterStrategy()
           const;

  // Make clone copy of this object
  virtual
     LgFrFilterStrategy *
        clone() 
           const = 0;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrFilterStrategy *
        clone(LgFrScenario &) 
           const = 0;

  // Assignment operator
  LgFrDiadicFilterStrategy &
    operator=(
	      const LgFrDiadicFilterStrategy& rhs);

  // Copy constructor
  LgFrDiadicFilterStrategy (
     const LgFrDiadicFilterStrategy &);

  // Copy constructor
  LgFrDiadicFilterStrategy (
     const LgFrDiadicFilterStrategy &, 
     LgFrScenario &);

  // Default Constructor
  LgFrDiadicFilterStrategy ();

  // Another Constructor
  LgFrDiadicFilterStrategy (
     LgFrFilterStrategy & leftHandSideFilterStrategy,
     LgFrFilterStrategy & rightHandSideFilterStrategy
  );


  // Destructor
  ~LgFrDiadicFilterStrategy ();

private:

  LgFrFilterStrategy * leftHandSideFilterStrategyPtr_;   
  LgFrFilterStrategy * rightHandSideFilterStrategyPtr_;   

};



//--------------------------------------------------------------------------
// LgFrAndFilterStrategy:
//
// Filter strategy which is the logical AND of two filter strategies.
//
//--------------------------------------------------------------------------

class LgFrAndFilterStrategy : public LgFrDiadicFilterStrategy {
public:

    virtual int GetFSType() const { return __LGFRANDFILTERSTRATEGY; }

  // The exclude method is implemented in the base class 
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // Make clone copy of this object
  virtual
     LgFrFilterStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrFilterStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrAndFilterStrategy &
    operator=(
	      const LgFrAndFilterStrategy& rhs);

  // Copy constructor
  LgFrAndFilterStrategy (
     const LgFrAndFilterStrategy &);

  // Copy constructor
  LgFrAndFilterStrategy (
     const LgFrAndFilterStrategy &, 
     LgFrScenario &);

  // Constructor
  LgFrAndFilterStrategy ();

  // Another Constructor
  LgFrAndFilterStrategy (
     LgFrFilterStrategy & leftHandSideFilterStrategy,
     LgFrFilterStrategy & rightHandSideFilterStrategy
  );

  // Destructor
  ~LgFrAndFilterStrategy ();

  // test this class
  static void test();

private:

  const int id_;
};




//--------------------------------------------------------------------------
// LgFrOrFilterStrategy:
//
// Filter strategy which is the logical OR of two filter strategies.
//
//--------------------------------------------------------------------------

class LgFrOrFilterStrategy : public LgFrDiadicFilterStrategy {
public:

    virtual int GetFSType() const { return __LGFRORFILTERSTRATEGY; }

  // The exclude method is implemented in the base class 
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // Make clone copy of this object
  virtual
     LgFrFilterStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrFilterStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrOrFilterStrategy &
    operator=(
	      const LgFrOrFilterStrategy& rhs);

  // Copy constructor
  LgFrOrFilterStrategy (
     const LgFrOrFilterStrategy &);

  // Copy constructor
  LgFrOrFilterStrategy (
     const LgFrOrFilterStrategy &, 
     LgFrScenario &);

  // Constructor
  LgFrOrFilterStrategy ();

  // Another Constructor
  LgFrOrFilterStrategy (
     LgFrFilterStrategy & leftHandSideFilterStrategy,
     LgFrFilterStrategy & rightHandSideFilterStrategy
  );

  // Destructor
  ~LgFrOrFilterStrategy ();

  // test this class
  static void test();

private:

  const int id_;
};




//--------------------------------------------------------------------------
// LgFrNotFilterStrategy:
//
// Filter strategy which is the logical NOT of a filter strategy.
//
//--------------------------------------------------------------------------

class LgFrNotFilterStrategy : public LgFrFilterStrategy {
public:

    virtual int GetFSType() const { return __LGFRNOTFILTERSTRATEGY; }
  // The exclude method is implemented in the base class 
  virtual
    bool
      exclude(
        const LgFrItemTimeVecPairFloat &)
          const;

  // Set filter strategy which will have the not operation applied to it.
  virtual
     void
        filterStrategy(
           const LgFrFilterStrategy & );

  // Get filter strategy.
  virtual
     const LgFrFilterStrategy &
        filterStrategy()
           const;

  // Make clone copy of this object
  virtual
     LgFrFilterStrategy *
        clone() 
           const;

  // Make clone copy of this object with new Scenario
  virtual
     LgFrFilterStrategy *
        clone(LgFrScenario &) 
           const;

  // Assignment operator
  LgFrNotFilterStrategy &
    operator=(
	      const LgFrNotFilterStrategy& rhs);

  // Copy constructor
  LgFrNotFilterStrategy (
     const LgFrNotFilterStrategy &);

  // Copy constructor
  LgFrNotFilterStrategy (
     const LgFrNotFilterStrategy &, 
     LgFrScenario &);

  // Constructor
  LgFrNotFilterStrategy ();

  // Another Constructor
  LgFrNotFilterStrategy (
     const LgFrFilterStrategy & filterStrategy
  );

  // Destructor
  ~LgFrNotFilterStrategy ();

  // test this class
  static void test();

private:

  const int id_;

  LgFrFilterStrategy * filterStrategyPtr_;   

};

#endif
