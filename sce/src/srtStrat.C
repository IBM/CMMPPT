#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// #include <witutil/src/machdep.h> 
#include <sce/src/srtStrat.h>
#include <sce/src/allocLet.h>
#include <sce/src/sceDefin.h>







  // Preferred Constructor
LgFrSceSortStrategy::LgFrSceSortStrategy(int sortId)
  :  sortId_(sortId)
{
  assert(sortId <= LgFrSceSortID_Maximum);
  assert(sortId >= 0);
}



  // get the sortID
int 
LgFrSceSortStrategy::sortId()
{ 
  return sortId_; 
}





// Compare Function f(Void): PDS 
int
compareVoid_PDS(const void * aa, const void * bb)
{     
  int rc;

  LgFrSceAllocLet ** aaa = (LgFrSceAllocLet **) aa;
  LgFrSceAllocLet ** bbb = (LgFrSceAllocLet **) bb;

  LgFrSceAllocLet * b =  *bbb;
  LgFrSceAllocLet * a =  *aaa;

  // allocated
  rc = b->allocated() - a->allocated();
  if (rc != 0) return rc;
  // priority(P)
  rc = a->priority() - b->priority();
  if (rc != 0) return rc;
  // demand period(D)
  rc = a->demandPd() - b->demandPd();
  if( rc != 0 ) return rc;
  // ship period (S)
  rc = a->shipPd() - b->shipPd();
  if( rc != 0 ) return rc;
  // lexicographic Part
  rc = a->partName().compare(b->partName());
  if( rc != 0 ) return rc;
  else {
    rc = a->demandName().compare(b->demandName());
    if( rc != 0 ) return rc;
  }

  return 0;
}







int 
compareVoid_PSD(const void *aa, const void *bb)
{
  int rc;

  LgFrSceAllocLet ** aaa = (LgFrSceAllocLet **) aa;
  LgFrSceAllocLet ** bbb = (LgFrSceAllocLet **) bb;

  LgFrSceAllocLet * b =  *bbb;
  LgFrSceAllocLet * a =  *aaa;

  // allocated
  rc = b->allocated() - a->allocated();
  if (rc != 0) return rc;
  // priority(P)
  rc = a->priority() - b->priority();
  if (rc != 0) return rc;
  // ship period (S)
  rc = a->shipPd() - b->shipPd();
  if( rc != 0 ) return rc;
  // demand period(D)
  rc = a->demandPd() - b->demandPd();
  if( rc != 0 ) return rc;
  // lexicographic Part
  rc = a->partName().compare(b->partName());
  if( rc != 0 ) return rc;
  else {
    rc = a->demandName().compare(b->demandName());
    if( rc != 0 ) return rc;
  }
  return 0;
}


int 
compareVoid_DPS(const void *aa, const void *bb)
{
  int rc;


  LgFrSceAllocLet ** aaa = (LgFrSceAllocLet **) aa;
  LgFrSceAllocLet ** bbb = (LgFrSceAllocLet **) bb;

  LgFrSceAllocLet * b =  *bbb;
  LgFrSceAllocLet * a =  *aaa;


  // allocated
  rc = b->allocated() - a->allocated();
  if (rc != 0) return rc;
  // demand period(D)
  rc = a->demandPd() - b->demandPd();
  if( rc != 0 ) return rc;
  // priority(P)
  rc = a->priority() - b->priority();
  if (rc != 0) return rc;
  // ship period (S)
  rc = a->shipPd() - b->shipPd();
  if( rc != 0 ) return rc;
  // lexicographic Part
  rc = a->partName().compare(b->partName());
  if( rc != 0 ) return rc;
  else {
    rc = a->demandName().compare(b->demandName());
    if( rc != 0 ) return rc;
  }
  return 0;
}

int 
compareVoid_DSP(const void *aa, const void *bb)
{
  int rc;

  LgFrSceAllocLet ** aaa = (LgFrSceAllocLet **) aa;
  LgFrSceAllocLet ** bbb = (LgFrSceAllocLet **) bb;

  LgFrSceAllocLet * b =  *bbb;
  LgFrSceAllocLet * a =  *aaa;

  // allocated
  rc = b->allocated() - a->allocated();
  if (rc != 0) return rc;
  // demand period(D)
  rc = a->demandPd() - b->demandPd();
  if( rc != 0 ) return rc;
  // ship period (S)
  rc = a->shipPd() - b->shipPd();
  if( rc != 0 ) return rc;
  // priority(P)
  rc = a->priority() - b->priority();
  if (rc != 0) return rc;
  // lexicographic Part
  rc = a->demandName().compare(b->demandName());
  if( rc != 0 ) return rc;
  else {
    rc = a->partName().compare(b->partName());
    if( rc != 0 ) return rc;
  }
  return 0;
}


int 
compareVoid_SDP(const void *aa, const void *bb)
{
  int rc;


  LgFrSceAllocLet ** aaa = (LgFrSceAllocLet **) aa;
  LgFrSceAllocLet ** bbb = (LgFrSceAllocLet **) bb;

  LgFrSceAllocLet * b =  *bbb;
  LgFrSceAllocLet * a =  *aaa;

  // allocated
  rc = b->allocated() - a->allocated();
  if (rc != 0) return rc;
  // ship period (S)
  rc = a->shipPd() - b->shipPd();
  if( rc != 0 ) return rc;
  // demand period(D)
  rc = a->demandPd() - b->demandPd();
  if( rc != 0 ) return rc;
  // priority(P)
  rc = a->priority() - b->priority();
  if (rc != 0) return rc;
  // lexicographic Part
  rc = a->demandName().compare(b->demandName());
  if( rc != 0 ) return rc;
  else {
    rc = a->partName().compare(b->partName());
    if( rc != 0 ) return rc;
  }
  return 0;
}

int 
compareVoid_SPD(const void *aa, const void *bb)
{
  int rc;



  LgFrSceAllocLet ** aaa = (LgFrSceAllocLet **) aa;
  LgFrSceAllocLet ** bbb = (LgFrSceAllocLet **) bb;

  LgFrSceAllocLet * b =  *bbb;
  LgFrSceAllocLet * a =  *aaa;



  // allocated
  rc = b->allocated() - a->allocated();
  if (rc != 0) return rc;
  // ship period (S)
  rc = a->shipPd() - b->shipPd();
  if( rc != 0 ) return rc;
  // priority(P)
  rc = a->priority() - b->priority();
  if (rc != 0) return rc;
  // demand period(D)
  rc = a->demandPd() - b->demandPd();
  if( rc != 0 ) return rc;
  // lexicographic Part
  rc = a->partName().compare(b->partName());
  if( rc != 0 ) return rc;
  else {
    rc = a->demandName().compare(b->demandName());
    if( rc != 0 ) return rc;
  }
  return 0;
}




LgFrSceCompareVoidFnPtr
LgFrSceSortStrategy::compareVoidFunc()
{
  switch (sortId_) 
    {
    case LgFrSceSortId_PDS:
      return compareVoid_PDS;
      break;
    case LgFrSceSortId_PSD:
      return compareVoid_PSD;
      break;
    case LgFrSceSortId_DPS:
      return compareVoid_DPS;
      break;
    case LgFrSceSortId_DSP:
      return compareVoid_DSP;
      break;
    case LgFrSceSortId_SDP:
      return compareVoid_SDP;
      break;
    case LgFrSceSortId_SPD:
      return compareVoid_SPD;
      break;
    default:
      break;
    }
  return compareVoid_PDS;
}


#if 0
LgFrSceCompareAllocLetFnPtr
LgFrSceSortStrategy::compareAllocFunc()
{
  return compareAllocLet_PDS;
}
#endif





  // default constructor
LgFrSceSortStrategy::LgFrSceSortStrategy()
:
  sortId_(0)
  {
    // nothing else to do
  }


  // destructor
LgFrSceSortStrategy::~LgFrSceSortStrategy()
{
	// do nothing
}


// assignment operator
LgFrSceSortStrategy&
LgFrSceSortStrategy::operator=(const LgFrSceSortStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    sortId_ = rhs.sortId_;
  }
  return *this;
}

// copy constructor 
LgFrSceSortStrategy::LgFrSceSortStrategy(const LgFrSceSortStrategy& source)
  : sortId_(source.sortId_)
{
  // nothing to do
}



