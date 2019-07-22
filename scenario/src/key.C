// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
#include <scenario/src/key.h>
#include <assert.h>

// extract string
std::string
LgFrKey::key() const
{
  return key_;
}

// overloaded isEqual()
bool
LgFrKey::isEqual(const LgFrKey* a) const
{
  // should do a check to make sure that a is a LgFrKey
  return key_ == ((LgFrKey *)a)->key();
}

// compareTo method, uses std::string to do the comparison.
// should return 0  if this->key_ "is equal to" source->key_.
//               >0 if this->key_ "is larger" than source->key_.
//               <0 if this->key_ "is smaller" than source->key_.

int 
LgFrKey::compareTo(const LgFrKey* source) const
{
  const LgFrKey* skey = static_cast<const LgFrKey*>( source );
  return key_ == skey->key_;
}

// Equality and inequality
int
LgFrKey::operator==(const LgFrKey& otherKey) const
{
  return key_ == otherKey.key();
}

LgFrKey::operator!=(const LgFrKey& otherKey) const
{
  return key_ != otherKey.key();
}

// Assignment
LgFrKey&
LgFrKey::operator=(const LgFrKey& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
      key_ = rhs.key();
  }
  return *this;
}

// Copy constructor
LgFrKey::LgFrKey(const LgFrKey& copy)
{
  key_ = copy.key();
}

// Constructor
LgFrKey::LgFrKey(const char* string) : key_(string)
{
// nothing to do here
}

// Destructor
LgFrKey::~LgFrKey()
{
// nothing to do here
}

