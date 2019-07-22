#ifndef LGFRKEYH
#define LGFRKEYH
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/idnumber.h>

class LgFrKey 
{
public:
  
  // extract string
  std::string key() const;

  // Overloaded isEqual() (needed)
  bool isEqual(const LgFrKey* a) const;

  // Overloaded compareTo()
  int compareTo(const LgFrKey* source) const;
  
  // Equality and inequality
  int operator==(const LgFrKey&) const;
  int operator!=(const LgFrKey&) const;  

  // Assignment
  LgFrKey& operator=(const LgFrKey&);

  // Copy constructor
  LgFrKey(const LgFrKey&);

  // Constructor
  LgFrKey(const char*);

  // Destructor
  ~LgFrKey();

private:
  std::string key_;
};
#endif
