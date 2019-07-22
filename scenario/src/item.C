#include <iostream.h>
#include <assert.h>
#include <rw/cstring.h>
#include <scenario/src/item.h>

// This is a pure virtual function.
// Windows platform requires a definition when creating 
// a dll.  Pure virtual functions can have definitions
// that are used by their derived methods, so there is
// no harm in defining this method to satisify the 
// windows linker.

LgFrItem&
LgFrItem::operator=(const LgFrItem& rhs)
{
  cerr <<"Item base class assignment operator invoked" <<endl;
  assert(0==1);
  return *this;
}
