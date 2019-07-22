// file: sceCrtLs.h
// author: robin and tom

#ifndef  LGFRSCECRITLIST_H
#define  LGFRSCECRITLIST_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.



#include <scenario/src/generVc.h>


// forward reference
class WitRun;


class LgFrSceCritList {
  
public:
  
  // default constructor
  LgFrSceCritList();
  
  // general constructor
  LgFrSceCritList(int length, char ** witCritList, int * witCritPds);

  // copy constructor
  LgFrSceCritList(const LgFrSceCritList& source);
  
  // assignment operator
  LgFrSceCritList&
  operator=(const LgFrSceCritList& rhs);
  
  // destructor
  virtual
  ~LgFrSceCritList();

  // return the i-th part name in the critList
  std::string getPart( int i);

  // return the i-th critical period in the critList
  int getPeriod( int i);

  // return the length of the critList 
  int getLength() ;

  // merge
  void merge( WitRun * theWitRun);

  // merge the critical list and periods removing duplicates
  void merge( int       length, 
              char   ** witCritList, 
              int     * witCritPds );

  // merge the critical list
  void merge( LgFrSceCritList & inList );

  
private:
  LgFrStringVector list_;
  LgFrIntVector periods_;
  
};
#endif
