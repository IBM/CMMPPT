// file: sortStrt.h
// author:  tom

#ifndef  LGFRSCESORTSTRAGETY_H
#define  LGFRSCESORTSTRAGETY_H

#include <stdlib.h>

// ==================================================================
// ====== Client Instructions: ======================================
// ====== New a LgFrSceSortStrategy by constructing with one of these 
// ====== unique sort ID's.
#define LgFrSceSortId_PDS 0
#define LgFrSceSortId_PSD 1
#define LgFrSceSortId_DPS 2
#define LgFrSceSortId_DSP 3
#define LgFrSceSortId_SDP 4
#define LgFrSceSortId_SPD 5

// You MUST increment this  Maximum if you add another sort 
//      ====================
#define LgFrSceSortID_Maximum 5
//      ====================







class LgFrSceAllocLet;
  

// ========================================================
// ===============  LgFrSceSortStrategy ======================
// ========================================================
typedef int(*LgFrSceCompareVoidFnPtr)(const void*,const void*);

class LgFrSceSortStrategy {
public:



  /*
  int compareVoid_PDS(const void * aa, const void * bb);
  int compareVoid_PSD(const void *aa, const void *bb);
  int compareVoid_DPS(const void *aa, const void *bb);
  int compareVoid_DSP(const void *aa, const void *bb);
  int compareVoid_SDP(const void *aa, const void *bb);
  int compareVoid_SPD(const void *aa, const void *bb);
  */



  // use this call to get the right compare function for void *
  virtual LgFrSceCompareVoidFnPtr
  compareVoidFunc();


  // preferred constructor
  LgFrSceSortStrategy(int sortId);
  


  // destructor
  virtual
  ~LgFrSceSortStrategy();


  // copy constructor
  LgFrSceSortStrategy(const LgFrSceSortStrategy& source);
  
  // assignment operator
  LgFrSceSortStrategy&
  operator=(const LgFrSceSortStrategy& rhs);
  
  // default constructor
  LgFrSceSortStrategy();


  // get the sortID
  int 
  sortId();



private:
  int sortId_;


};


#endif
















