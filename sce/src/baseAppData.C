// file: baseappData.C
// author: tom and robin

// appData objects for:
//    parts
//    subBomEntries
//    operations

#include <sce/src/baseAppData.h>
#include <assert.h>


// default constructor 
LgFrSceAppData::LgFrSceAppData()
{
  // do nothing
}

// assignment operator
LgFrSceAppData&
LgFrSceAppData::operator=(const LgFrSceAppData& rhs)

{
  if (this != &rhs) {		// Check for assignment to self
    ;
  }
  return *this;
}

// copy constructor 
LgFrSceAppData::LgFrSceAppData(const LgFrSceAppData& source)
{
  // nothing to do
}

// destructor
LgFrSceAppData::~LgFrSceAppData()
{
  // nothing to do, 
}




#ifdef NDEBUG
#undef NDEBUG
#endif





