#include <wit/src/wit.h>
#include <witutil/src/machdep.h>

#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <witutil/src/fullExpl.h>

#include <iostream>
#include <fstream>

// #define FULL_DEBUG


// states:
//   0: uninitialized (miniWitRun not populated)
//   1: initiailized (miniWitRun populated, no explosion solution)
//   2: Explosion solution (nPeriodsSupplyAvailForPastDue_ = 0)
//   3: Explosion solution (nPeriodsSupplyAvailForPastDue_ > 0)


  // get the mrpProdVol
int
WUtlFullMrpExploder::mrpProdVol(const char * part,
                                float ** mrpProdVolPtr)
{
  assert(state_ >= 2);
  witGetOperationMrpExecVol(localWitRun_, part, mrpProdVolPtr);
  return 0;
}

  // get the reqVol
int
WUtlFullMrpExploder::reqVol(const char * part,
                            float ** reqVolPtr)
{
  assert(state_ >= 2);
  witGetPartReqVol(localWitRun_, part, reqVolPtr);
  return 0;
}

  // get the past due reqVol
int
WUtlFullMrpExploder::pastDueReqVol(const char * part,
                                   float * pastDueReqVolPtr )
{
  (*pastDueReqVolPtr) = 0;
  return 0;
}

  // get the  reqVol with past due reqVol added to period 0
  // we don't ave reqVol with past due, so we just call
  // the regular one
int
WUtlFullMrpExploder::reqVolWithPastDue(const char * part,
                                       float ** reqVolPtr )
{
  this->reqVol(part, reqVolPtr);
  return 0;
}


  // get the mrpConsVol
int
WUtlFullMrpExploder::mrpConsVol(const char * part,
                                float ** mrpConsVolPtr)
{
  assert(state_ >= 2);
  witGetPartMrpConsVol(localWitRun_, part, mrpConsVolPtr);
  return 0;
}

  // get the past due mrpConsVol
int
WUtlFullMrpExploder::pastDueMrpConsVol(const char * part,
                                       float * pastDueMrpConsVolPtr )
{
  *pastDueMrpConsVolPtr = 0;
  return 0;

}

  // get the  mrpConsVol with past due mrpConsVol added to period 0
int
WUtlFullMrpExploder::mrpConsVolWithPastDue(const char * part,
                                           float ** mrpConsVolPtr )
{
  this->mrpConsVol(part, mrpConsVolPtr);
  return 0;
}





  // get the mrpExcessVol
int
WUtlFullMrpExploder::mrpExcessVol(const char * part,
                                  float ** mrpExcessVolPtr)
{
  assert(state_ >= 2);
  witGetPartMrpExcessVol(localWitRun_, part, mrpExcessVolPtr);
  return 0;
}





  // preferred constructor
WUtlFullMrpExploder::WUtlFullMrpExploder(WitRun * originalWitRun)
  :
  originalWitRun_(originalWitRun),
  localWitRun_(NULL),
  state_(0),
  nPeriodsSupplyAvailForPastDue_(0)
{  
  witNewRun(&localWitRun_);

  /* turn off a bunch off annoying messages */
  witSetMesgTimesPrint(localWitRun_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 82, 0);
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 338, 0);
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 538, 0);
  witSetMesgTimesPrint(localWitRun_, WitTRUE, 505, 0);        
  
  this->populateLocalWitRun();

  // ok, we're initialized, but not post processed.
  assert(state_ == 1);
  
}


void
WUtlFullMrpExploder::smartCopyData(WitRun * const lhsWitRun,
                                   WitRun * const rhsWitRun)
{

  witInitialize(lhsWitRun);
  witSetWit34Compatible(lhsWitRun, WitTRUE);
  
  int nParts;
  char ** partList;
  witGetParts(rhsWitRun, &nParts, &partList);


  // ========= part Attributes  ==========
  float * supplyVol;
  //      --- products only ----
  float * cycleTime;
  float * incLotSize;
  float * minLotSize;
  int   * yield;

  // ========= BOM attributes ===========
  char * child;
  float usageRate;
  float usageTime;
  int fallout;
  witBoolean mandEC;
  int early;
  int late;

  // ==========  Demand attributes =============
  float * demandVol;

  // ==========  Global attributes =============
  witBoolean buildWhenEmpty;
  int objValue = 0;  // set it up for no optimization
  int nPeriods;


  // First Set the global values
  witGetNPeriods(rhsWitRun, &nPeriods);
  witSetNPeriods(lhsWitRun, nPeriods);

  witGetBuildWhenEmpty(rhsWitRun, &buildWhenEmpty);
  witSetBuildWhenEmpty(lhsWitRun, buildWhenEmpty);

  witSetObjChoice(lhsWitRun, objValue);
  

  // filter out parts and structure that is not necessary for
  // MRP.
  int i;
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];

    witAttr category;
    witGetPartCategory(rhsWitRun, thePart, &category);

    witGetPartSupplyVol(rhsWitRun, thePart, &supplyVol);

    


    
    if (category == WitPRODUCT) {
      witGetPartCycleTime(rhsWitRun, thePart, &cycleTime);
      witGetPartIncLotSize(rhsWitRun, thePart, &incLotSize);
      witGetPartMinLotSize(rhsWitRun, thePart, &minLotSize);
      witGetPartYield(rhsWitRun, thePart, &yield);

      witAddPart           (lhsWitRun, thePart, category);
      witSetPartCycleTime  (lhsWitRun, thePart, cycleTime);
      witSetPartIncLotSize (lhsWitRun, thePart, incLotSize);
      witSetPartMinLotSize (lhsWitRun, thePart, minLotSize);
      witSetPartYield      (lhsWitRun, thePart, yield);
      witSetPartSupplyVol  (lhsWitRun, thePart, supplyVol);

      portableFree(cycleTime);
      portableFree(incLotSize);
      portableFree(minLotSize);
      portableFree(yield);

      // Now copy all the  bom records
      int nBom;
      int bomEntIdx;
      witGetPartNBomEntries(rhsWitRun, thePart, &nBom);
      for (int j=0; j<nBom; j++) {
        witGetBomEntryConsumedPart(rhsWitRun, thePart, j, &child); 
        witGetBomEntryUsageRate(rhsWitRun, thePart, j, &usageRate); 
        witGetBomEntryUsageTime(rhsWitRun, thePart, j, &usageTime); 
        witGetBomEntryEarliestPeriod(rhsWitRun, thePart, j, &early); 
        witGetBomEntryLatestPeriod(rhsWitRun, thePart, j, &late);
        witGetBomEntryFallout(rhsWitRun, thePart, j, &fallout);
        witGetBomEntryMandEC(rhsWitRun, thePart, j, &mandEC);

        witGetOperationNBomEntries (
             lhsWitRun,
             thePart,
           & bomEntIdx);

        witAddBomEntry (lhsWitRun, thePart, child);

        witSetBomEntryEarliestPeriod (
           lhsWitRun,
           thePart,
           bomEntIdx,
           early);

        witSetBomEntryLatestPeriod (
           lhsWitRun,
           thePart,
           bomEntIdx,
           late);

        witSetBomEntryFallout (
           lhsWitRun,
           thePart,
           bomEntIdx,
           fallout);

        witSetBomEntryMandEC (
           lhsWitRun,
           thePart,
           bomEntIdx,
           mandEC);

        witSetBomEntryUsageRate (
           lhsWitRun,
           thePart,
           bomEntIdx,
           usageRate);

        witSetBomEntryUsageTime (
           lhsWitRun,
           thePart,
           bomEntIdx,
           usageTime);

        portableFree(child);
      }
      
    }

    // RAW and CAPACITY parts are easy
    else {
      witAddPart          (lhsWitRun, thePart, category);
      witSetPartSupplyVol (lhsWitRun, thePart, supplyVol);
    }
    portableFree(supplyVol);      


    // ==== Now add any Demands ====
    if (category != WitCAPACITY) {
      int nDemands;
      char ** dList;
      witGetPartDemands(rhsWitRun, thePart, &nDemands, &dList);
      for (int d=0; d<nDemands; d++) {
        witGetDemandDemandVol(rhsWitRun, thePart, dList[d], &demandVol);
        witAddDemand          (lhsWitRun, thePart, dList[d]);
        witSetDemandDemandVol (lhsWitRun, thePart, dList[d], demandVol);
        portableFree(demandVol);
        portableFree(dList[d]);
      }
      portableFree(dList);
    }
  }
      
    
  // free the parts 
  for (i=0; i<nParts; i++)  
    portableFree(partList[i]);
  portableFree(partList);
  
}




  // copy constructor
WUtlFullMrpExploder::WUtlFullMrpExploder( const WUtlFullMrpExploder& source )
{
  assert(0 == "Do not use this method");
}
  
  // assignment operator
WUtlFullMrpExploder&
WUtlFullMrpExploder::operator=(const WUtlFullMrpExploder& rhs) {
  assert(0 == "Do not use this method");
  return *this;
}

  
  // destructor
WUtlFullMrpExploder::~WUtlFullMrpExploder()
{
  
  witDeleteRun(localWitRun_);
}




void
WUtlFullMrpExploder::print(char * filename)
{
  assert(state_ >= 2);

  std::ofstream outFile(filename);
  int nPeriods;
  witGetNPeriods(originalWitRun_, &nPeriods);

  int nParts;
  char ** partList;
  witGetParts(originalWitRun_, &nParts, &partList);

  int i;
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];
    witAttr category;
    witGetPartCategory(originalWitRun_, thePart, &category);

    // pastDue demand
    float pastDueMrpConsVol;
    this->pastDueMrpConsVol(thePart, &pastDueMrpConsVol);
    outFile << "\"" << thePart << "\", pastDueMrpConsVol\", " << pastDueMrpConsVol;
    outFile << "\n";
    
    outFile << "\"" << thePart << "\", mrpConsVol\"";
    float * mrpConsVol;    
    this->mrpConsVol(thePart, &mrpConsVol);    
    int t;
    for (t=0; t<nPeriods; t++) 
      outFile << "," << mrpConsVol[t];
    outFile << "\n";
    portableFree(mrpConsVol);

    if (category == WitPRODUCT) {
      float * mrpProdVol;
      this->mrpProdVol(thePart, &mrpProdVol);
      outFile << "\"" << thePart << "\", mrpProdVol\" ";
      for (t=0; t<nPeriods; t++) 
        outFile << "," << mrpProdVol[t];
      outFile << "\n";
      portableFree(mrpProdVol);
    }

    float * mrpExcessVol;
    this->mrpExcessVol(thePart, &mrpExcessVol);
    outFile << "\"" << thePart << "\", mrpExcessVol\" ";
    for (t=0; t<nPeriods; t++) 
      outFile << "," << mrpExcessVol[t];
    outFile << "\n";
    portableFree(mrpExcessVol);

    float pastDueReqVol;
    this->pastDueReqVol(thePart, &pastDueReqVol);
    outFile << "\"" << thePart << "\", pastDueReqVol\", " << pastDueReqVol;
    outFile << "\n";


    
    float * reqVol;
    this->reqVol(thePart, &reqVol);        
    outFile << "\"" << thePart << "\", reqVol\" ";
    for (t=0; t<nPeriods; t++) 
      outFile << "," << reqVol[t];
    outFile << "\n";
    portableFree(reqVol);

  }
  outFile.close();

  for (i=0; i<nParts; i++)
    portableFree(partList[i]);
  portableFree(partList);
  
}

void
WUtlFullMrpExploder::populateLocalWitRun()
{
  witAttr category;

  assert(state_ == 0);
  witInitialize(localWitRun_);

  
  float aZero = 0.0;
  float * zeroSingVec = &aZero;

  smartCopyData(localWitRun_, originalWitRun_);
  //  witCopyData(localWitRun_, originalWitRun_);
  witSetWit34Compatible(localWitRun_, WitFALSE);
  
  int nPeriods;
  witGetNPeriods(localWitRun_, &nPeriods);

  int nParts;
  char ** partList;
  witGetParts(originalWitRun_, &nParts, &partList);
  

  // Now copy the  yields from the original witRun
  // Do this because they get mucked up when we set
  // wit34Compatible to False.  Note! we need to be careful
  // about parts that previously had wit34cycleTime = -1.0
  int i;
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];
    witGetPartCategory(originalWitRun_, thePart, &category);
    if (category == WitPRODUCT)  {
      int * yield;
      float * cycleTime;
      // If the cycleTime was -1 in a period, then we want the
      // yield to be 0 to enforce that the part cannot be built.
      witGetPartCycleTime(originalWitRun_, thePart, &cycleTime);
      witGetPartYield(originalWitRun_, thePart, &yield);
      for (int t=0; t<nPeriods; t++) {
        if (cycleTime[t] < -0.001) {
          yield[t] = 0;
        }
      }
      witSetOperationYield(localWitRun_, thePart, yield);
      portableFree(yield);
      portableFree(cycleTime);
    }
  }
    
  


  float * offset;
  int nBom;
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];
    int nBop;
    witGetPartNProducingBopEntries(localWitRun_, thePart, &nBop);
    if (nBop > 0)  {
       witGetOperationNBomEntries(localWitRun_, thePart, &nBom);
       for (int j=0; j<nBom; j++) {
          witGetBomEntryOffset(localWitRun_, thePart, j, &offset);
          int changedOffset = 0;
          for (int t=0; t<nPeriods; t++)  {
             if (offset[t] > (float) t)  {
                changedOffset = 1;
                offset[t] = t;
             }
          }
          if (changedOffset)  {
             witSetBomEntryOffset(localWitRun_, thePart, j, offset);
          }
          portableFree(offset);
       }
    }       
  }

  for (i=0; i<nParts; i++)
    portableFree(partList[i]);
  portableFree(partList);
  
  state_ = 1;
}

// set supplyVols and demandVols to the current
// setting in originalWitRun_.  this is needed for
// FSS users.
void
WUtlFullMrpExploder::synchronize()
{

  // must be in initialized state
  assert(state_ > 0);

  int nParts;
  char ** partList;
  witGetParts(originalWitRun_, &nParts, &partList);
  
  int i;
  for (i=0; i<nParts; i++)  {
    char * thePart = partList[i];
    float * supplyVol;
    witGetPartSupplyVol(originalWitRun_, thePart, &supplyVol);
    witSetPartSupplyVol(localWitRun_, thePart, supplyVol);
    portableFree(supplyVol);
    int nDemands;
    char ** demandList;
    witGetPartDemands(originalWitRun_, thePart, &nDemands, &demandList);
    for (int j=0; j<nDemands; j++)   {
      float * demandVol;
      witGetDemandDemandVol(originalWitRun_, thePart, demandList[j], &demandVol);
      witSetDemandDemandVol(localWitRun_, thePart, demandList[j], demandVol);
      portableFree(demandVol);
      portableFree(demandList[j]);
    }
    portableFree(demandList);
  }

  for (i=0; i<nParts; i++)
    portableFree(partList[i]);
  portableFree(partList);
  
}



int
WUtlFullMrpExploder::explode(int nPeriodsSupplyAvailForPastDue)
{

  // must be in initialized state
  assert(state_ > 0);
  int nPeriods;
  witGetNPeriods(originalWitRun_, &nPeriods);

  nPeriodsSupplyAvailForPastDue_ = (nPeriodsSupplyAvailForPastDue < nPeriods) ?
    nPeriodsSupplyAvailForPastDue : nPeriods -1;



  // run the mrp on the localWitRun 
  witMrp(localWitRun_);

#ifdef FULL_DEBUG
  witSetMesgTimesPrint(localWitRun_, WitTRUE, WitINFORMATIONAL_MESSAGES, UCHAR_MAX);
  witDisplayData(localWitRun_, "localWitRun.echo");
  witWriteData(localWitRun_, "localWitRun.witData");
  witSetMesgTimesPrint(localWitRun_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
#endif  
    
  if (nPeriodsSupplyAvailForPastDue_ == 0) 
    state_ = 2;
  else
    state_ = 3;
  
  return 0;
}

#if 0
// we don't need this, but we may need somehting like it to
// postprocess the mrpExcessVol if it exceeds supplyVol.  This
// could only happen when nPeriodsSupplyAvailForPastDeu > 0
void WUtlFullMrpExploder::netConsumedSupplyVector(float * supplyVol,
                                                  float   initialMiniWRSupply,
                                                  float   miniWRExcessVol)
{
  float consumedSupply = initialMiniWRSupply - miniWRExcessVol;
  // loop backwards
  int t = nPeriodsSupplyAvailForPastDue_ - 1;
  while (consumedSupply > 0.0001) {
    assert(t >= 0);    
    float amountToNetThisPeriod = (supplyVol[t] > consumedSupply) ? consumedSupply : supplyVol[t];
    supplyVol[t] -= amountToNetThisPeriod;
    consumedSupply -= amountToNetThisPeriod;
    t--;
  }
}
#endif


#ifdef NDEBUG
#undef NDEBUG
#endif

void
WUtlFullMrpExploder::test()
{

  float flt_eps = 0.001;
    {
      WitRun * theWitRun;
      char  ** partList;
      int      nParts;
      int i;
      
      witNewRun(&theWitRun);

      witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
      witSetMesgTimesPrint(theWitRun, WitTRUE, 82, 0);
      witSetMesgTimesPrint(theWitRun, WitTRUE, 338, 0);
      witSetMesgTimesPrint(theWitRun, WitTRUE, 538, 0);        

      witInitialize(theWitRun);
      witSetWit34Compatible(theWitRun, WitTRUE);

      fprintf(stderr, "Entering witReadData()\n");
#if defined(_MSC_VER)
      witReadData(theWitRun, "..\\..\\..\\..\\wit-data\\examples\\wutltest\\wit.data");
#else
      witReadData(theWitRun, "/u/wit/wit-data/examples/wutltest/wit.data");
#endif
      fprintf(stderr, "    Leaving witReadData()\n");

      // get the list of  parts 
      witGetParts(theWitRun, &nParts, &partList);

      WUtlFullMrpExploder testExploder(theWitRun);
      testExploder.explode();

      float pastDueMrpConsVol;
      float * mrpConsVolWithPastDue;
      float * mrpConsVol;
    

      // test some things ...
      testExploder.mrpConsVolWithPastDue("947_C1", &mrpConsVolWithPastDue);
      testExploder.pastDueMrpConsVol("947_C1", &pastDueMrpConsVol);
      testExploder.mrpConsVol("947_C1", &mrpConsVol);            
      assert(fabs(mrpConsVolWithPastDue[0] - 700.0)   < flt_eps);
      assert(fabs(mrpConsVol[0] - 700.0) < flt_eps);
      assert(fabs(pastDueMrpConsVol - 0.0)    < flt_eps);        
      portableFree(mrpConsVol);
      portableFree(mrpConsVolWithPastDue);      


      testExploder.mrpConsVolWithPastDue("947_A2", &mrpConsVolWithPastDue);
      testExploder.pastDueMrpConsVol("947_A2", &pastDueMrpConsVol);
      testExploder.mrpConsVol("947_A2", &mrpConsVol);            
      assert(fabs(mrpConsVolWithPastDue[0] - 200.0)   < flt_eps);
      assert(fabs(mrpConsVol[0] - 200.0) < flt_eps);
      assert(fabs(pastDueMrpConsVol - 0.0)    < flt_eps);        
      portableFree(mrpConsVol);
      portableFree(mrpConsVolWithPastDue);      
      
      testExploder.print("testFullExplode.out");
    
      // free the parts 
      for (i=0; i<nParts; i++)  
	portableFree(partList[i]);
      portableFree(partList);

      // free the witRun
      witDeleteRun(theWitRun);
    }


    // Here's test number two ...
    {
      WitRun * theWitRun;
      char  ** partList;
      int      nParts;
      int i;
      
      witNewRun(&theWitRun);

      witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
      witSetMesgTimesPrint(theWitRun, WitTRUE, 82, 0);
      witSetMesgTimesPrint(theWitRun, WitTRUE, 338, 0);
      witSetMesgTimesPrint(theWitRun, WitTRUE, 538, 0);        

      witInitialize(theWitRun);
      witSetWit34Compatible(theWitRun, WitTRUE);

      fprintf(stderr, "Entering witReadData()\n");
#if defined(_MSC_VER)
      witReadData(theWitRun, "..\\..\\..\\..\\wit-data\\examples\\wutltest\\wit2.data");
#else
      witReadData(theWitRun, "/u/wit/wit-data/examples/wutltest/wit2.data");
#endif
      fprintf(stderr, "    Leaving witReadData()\n");

      // get the list of  parts 
      witGetParts(theWitRun, &nParts, &partList);

      WUtlFullMrpExploder testExploder(theWitRun);

      float pastDueMrpConsVol;
      float * mrpConsVol;

      float pastDueReqVol;
      float * reqVol;

      float * mrpProdVol;
      float * mrpExcessVol;

      // do a default explode ...
      testExploder.explode();

      
      // then do another explode that should make the
      // 325 supply in pd 2 of Assem, UN-AVAILABLE to net
      // against in pastDue.
      testExploder.explode(1);
      
      testExploder.pastDueMrpConsVol("Comp", &pastDueMrpConsVol);
      assert(fabs(pastDueMrpConsVol - 0.0)    < flt_eps);

      

      testExploder.mrpExcessVol("Assem", &mrpExcessVol);
      assert(fabs(mrpExcessVol[2]) - 325.0   < flt_eps);
      portableFree(mrpExcessVol);
      testExploder.mrpProdVol("Assem", &mrpProdVol);
      assert(fabs(mrpProdVol[0]) - 400.0   < flt_eps);
      portableFree(mrpProdVol);
      testExploder.pastDueReqVol("Assem", &pastDueReqVol);
      assert(fabs(pastDueReqVol - 0.0)    < flt_eps);        
      

      testExploder.mrpProdVol("CompUnConstrained", &mrpProdVol);
      assert(fabs(mrpProdVol[0]) - 500.0   < flt_eps);
      portableFree(mrpProdVol);
      testExploder.pastDueReqVol("CompUnConstrained", &pastDueReqVol);
      assert(fabs(pastDueReqVol - 0.0)    < flt_eps);        
      
      
      // free the parts 
      for (i=0; i<nParts; i++)  
	portableFree(partList[i]);
      portableFree(partList);

      // free the witRun
      witDeleteRun(theWitRun);
    }


} 

