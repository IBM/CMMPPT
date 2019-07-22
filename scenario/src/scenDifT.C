
#include <stdlib.h>

#include <scenario/src/calendar.h>
#include <scenario/src/scenDifT.h>

// forward declarations
class LgFrDataInterfaceAbstract;

isAReturnType
LgFrScenarioForDifTesting::isA() const
{
    return id_;
}

LgFrDataInterfaceAbstract*
LgFrScenarioForDifTesting::dif()
{
   return LgFrScenario::dataInterface();
}

// Constructor
LgFrScenarioForDifTesting::LgFrScenarioForDifTesting(
   const LgFrDataInterfaceAbstract&    dif,
   const LgFrCalendar&                 calendar)
:LgFrScenario(dif, calendar),
 id_(__LGFRSCENARIOFORDIFTESTING)
{
  // builder does the rest
}

// Destructor 
LgFrScenarioForDifTesting::~LgFrScenarioForDifTesting()
{
  // Nothing to do here
}

// Assignment operator.
LgFrScenarioForDifTesting&
LgFrScenarioForDifTesting::operator=(
   const LgFrScenarioForDifTesting& rhs)
{
    cerr << "don't use LgFrScenarioForDifTesting::operator=(const LgFrScenarioForDifTesting&)" << endl;
    assert(0);
/*
   if (this != &rhs) {           // Check for assignment to self
     (this->LgFrScenario::operator= ) (rhs);
   }
*/
   return *this;
}


// Customized copy constructor
LgFrScenarioForDifTesting::LgFrScenarioForDifTesting(
   const LgFrScenarioForDifTesting& source )
: LgFrScenario( source )
{
  // Nothing to do here
}

// Default constructor
LgFrScenarioForDifTesting::LgFrScenarioForDifTesting()
: LgFrScenario()
{
  // Nothing to do here
}
