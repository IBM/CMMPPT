#ifndef LGFR_SCENARIO_SENSITIVE_FILTER_STRATEGY
#define LGFR_SCENARIO_SENSITIVE_FILTER_STRATEGY
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/filtStrt.h>

class LgFrScenario;

// Filter strategy for filter strategies to inherit from if they have to know
// what scenario they are in

class LgFrScenarioSensitiveFilterStrategy : public LgFrFilterStrategy {
public:

  virtual int GetFSType() const { return __LGFRSCENARIOSENSITIVEFILTERSTRATEGY; }

  // Set the scenario to be used when filtering
  void
  scenario(
						const LgFrScenario & scenario);

  // Get the pointer to scenario to be used when filtering
  const LgFrScenario *
  scenarioPtr()
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
  LgFrScenarioSensitiveFilterStrategy &
    operator=(
	      const LgFrScenarioSensitiveFilterStrategy& rhs);

  // Copy constructor
  LgFrScenarioSensitiveFilterStrategy (
     const LgFrScenarioSensitiveFilterStrategy &);

  // Copy constructor
  LgFrScenarioSensitiveFilterStrategy (
     const LgFrScenarioSensitiveFilterStrategy &, 
     LgFrScenario &);

  // Preferred Constructor
  LgFrScenarioSensitiveFilterStrategy (
	  const LgFrScenario & scenario);

  // Default Constructor
  LgFrScenarioSensitiveFilterStrategy ();

  // Destructor
  ~LgFrScenarioSensitiveFilterStrategy ();

  // test this class
  static void test();
  static void contextTest( 
     LgFrScenario &,
     LgFrScenario &);


private:

  const LgFrScenario * scenarioPtr_;
  const int id_;

};

#endif
