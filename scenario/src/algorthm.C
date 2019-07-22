// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/algorthm.h>

// Default constructor
LgFrAlgorithm::LgFrAlgorithm()
:
   LgFrSubscription()
{
  // All the work is done in the initialization part
}


//Copy constructor
LgFrAlgorithm::LgFrAlgorithm 
   (const LgFrAlgorithm& source)
:
   LgFrSubscription(source)
{
  // Nothing to do but reutrn.
}

// Destructor
LgFrAlgorithm::~LgFrAlgorithm()
{
  // Nothing to do but reutrn.
}

// Assignment
LgFrAlgorithm&
LgFrAlgorithm::operator=
(const LgFrAlgorithm& rhs)
{
  if (this != &rhs) {
     (this->LgFrSubscription::operator= ) (rhs);
  }
  return (*this);
}
