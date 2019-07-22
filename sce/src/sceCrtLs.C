#include <mcl/src/mdsPragma.h>
// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.
// file: sceCrtLs.C
// author:  tom and robin

#include <stdlib.h>
#include <assert.h>
#include <sce/src/sceCrtLs.h>
#include <scenario/src/machdep.h>
#include <sce/src/sceDefin.h>
#include <wit/src/wit.h>


#ifndef ELIMINATE_OLD_MAIN

// return the i-th part in the critList
std::string
LgFrSceCritList::getPart( int i )
{
  return list_[i];
}

// return the i-th period in the critList
int
LgFrSceCritList::getPeriod( int i )
{
  return periods_[i];
}

// return the length of the critList
int
LgFrSceCritList::getLength( )
{
  assert (list_.size() == periods_.size());
  return list_.size();
}

// merge the critList
void
LgFrSceCritList::merge(  WitRun * theWitRun )

{
  int length;
  char ** witCritList;
  int * witCritPds;
  witGetCriticalList( theWitRun, &length, &witCritList, &witCritPds);
  LgFrSceCritList local(length, witCritList, witCritPds);
  this->merge(local);
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<length; i++) {
    witFree(witCritList[i]);
  }
  witFree(witCritList);
  witFree(witCritPds);
  
}

// merge the critList
void
LgFrSceCritList::merge(  int       length, 
                         char   ** witCritList, 
                         int     * witCritPds )

{
  LgFrSceCritList local(length, witCritList, witCritPds);
  this->merge(local);
}

// merge the critList
void
LgFrSceCritList::merge( LgFrSceCritList & inList) 

{
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<inList.getLength(); i++)  {
    int found = 0;
    int period = inList.getPeriod(i);
    std::string part = inList.getPart(i);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j< this->getLength(); j++) {
      if (this->getPeriod(j) == period) {
        if (this->getPart(j) == part) {
          found = 1;
          break ;
        }
      }
    }
    if (found)
      continue;

    this->list_.push_back(part);
    this->periods_.push_back(period);
  }
  assert (list_.size() == periods_.size());

}

// default constructor:
LgFrSceCritList::LgFrSceCritList()
  :   list_(),
      periods_()
{
    // all the work is done in initializer
}


// fairly general constructor:
LgFrSceCritList::LgFrSceCritList( int length, char ** witCritList, int * witCritPds)
  :   list_(),
      periods_()
{
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<length; i++) {
    list_.push_back(witCritList[i]);
    periods_.push_back(witCritPds[i]);
  }
  assert (list_.size() == periods_.size());
}

  
  

// copy constructor -- need to work out (?)
LgFrSceCritList::LgFrSceCritList(const LgFrSceCritList& source)
  :  list_(source.list_),
     periods_(source.periods_)
{
  // nothing to do
}

// assignment operator
LgFrSceCritList&
LgFrSceCritList::operator=(const LgFrSceCritList& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    list_ = rhs.list_;
    periods_ = rhs.periods_;
  }
  return *this;
}

// destructor
LgFrSceCritList::~LgFrSceCritList()
{
  // nothing to do, 
}

#endif
