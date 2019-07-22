// Author: wally
// Based on code by Joe Huizenga

#ifndef YKTSCEN_H
#define YKTSCEN_H
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/scenario.h>

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// Important: This class was used to diagnose a problem in Rochesters           !!
// scenario but is defunct right now.                                           !!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

typedef int scheduler;

class LgFrYktScenario : public LgFrScenario
{

  public:
  virtual int GetScenarioType() const { return __LGFRSCENARIO; }

  LgFrYktScenario (const LgFrInitialProblemAndParameters& initProb, 
	       scheduler& sch);
  ~LgFrYktScenario();

  // Self-test
  static void test();

  private:
};

#endif
