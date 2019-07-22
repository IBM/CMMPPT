
// RW2STL - inserted:
#include <string>
#include <iostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <assert.h>
#include <stdlib.h>
#include <sce/src/exploders.h>
#include <math.h>
#include <mcl/src/mcl.h>
#include <iostream>
//#include <fstream.h>
#include <sce/src/appData.h>
#include <sce/src/mPlantH.h>
#include <sce/src/sceParam.h>
#include <sce/src/bopAData.h>
#include <sce/src/dmApData.h>
#include <sce/src/scePegging.h>
// #include <sce/src/scePIP.h>
#include <sce/src/sceCrtLs.h>
//  #include <sce/src/itHelpr.h>
#include <sce/src/scenSce.h>


// #define FULL_DEBUG
// #define DUMP_WIT_MRP_DATA_FILE
// #define DEBUG_INTERPLANT_PEG



// + ================================================================
// + Exploder Factory -- creates the right decorated exploder
// + based on SetOfParameters.  Global Function
// + ================================================================
// + Rules of Creation: The decorator pattern is a bit tricky to implement.
// + For reference, see the Design Patterns book.  In this implementation,
// + the decorators need to be created in a specific order outline by the
// + graph below.
// +
// + The client gets exactly one Exploder object and uses that one exclusively.
// + Under the covers, he's really getting an exploder decorater object that
// + "has-a" exploder object.  The exploder object that he "has" may possibly
// + be a exploder decorator object itself, which, in turn, "has-a" exploder
// + object of his own.
// +
// + The responsibility of this global function is to create the "chain"
// + of exploder objects and return the "head" of the chain to the client.
// + The creation is based solely on the Sce Set of parameters.  The setOfParameters
// + is analyzed to see what set of decorators is needed.  Then the chain of
// + exploder objects is created in the appropriate order.
// +
// + Advantages:
// +     - This system is very easy to use for the client code.
// +     - Given the complexity of the various explosion schemes, this pattern
// +       is very flexible and there is very little duplication of code.
// +     - The design is extendable to future needs (I hope)
// +
// + Disadvantages:
// +     - The implementation of the exploder objects and how they override,
// +       extend, and restrict each others function is quite tricky.
// +     - Not compile-time type safe.  Errors in the exploder creation
// +       process will not be found until run-time.
// +


  // FINISH_ME: if doImplode=false, then we should use the original witRun

// + ================================================================
// + LgFrSceBaseExploder: Implementation
// + ================================================================
int
LgFrSceBaseExploder::explode()
{
  // must be in initialized state
  assert(state() > 0);


    // Dump the heur version of the wit.data file
#ifdef DUMP_WIT_MRP_DATA_FILE
    std::cout << "Writing the (mrp) wit data file ..." << std::endl;
    std::string mrpWitDataFileName = "mrpWitDataFile";
    setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_ON);
    witWriteData(localWitRun_, mrpWitDataFileName.c_str());
    if (setOfParameters_->getBool("printWitExplodeMsgFile"))
      setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_ON);
    else
      setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_OFF);
#endif

  // run the mrp on the localWitRun
  witMrp(this->localWitRun());


  state(2);

  return 0;
}

std::string
LgFrSceBaseExploder::title()
{
  std::string myTitle("base witMrp");
  return myTitle;
}


  // determine if this part is something that you deal with
witBoolean
LgFrSceBaseExploder::exists(const std::string& part)
{
  witBoolean exists;
  witGetPartExists(localWitRun_, part.c_str(), &exists);
  return exists;
}



int
LgFrSceBaseExploder::mrpExcessVol(const std::string& part, float ** mrpExcessVolPtr)
{
  assert(state() >= 2);
  if (exists(part))
    witGetPartMrpExcessVol(localWitRun_, part.c_str(), mrpExcessVolPtr);
  else
    mallocZeroVec(mrpExcessVolPtr);
  return 0;
}


int
LgFrSceBaseExploder::mrpResidualVol(const std::string& part, float ** mrpResidualVolPtr)
{
  assert(state() >= 2);
  if (exists(part))
    witGetPartMrpResidualVol(localWitRun_, part.c_str(), mrpResidualVolPtr);
  else
    mallocZeroVec(mrpResidualVolPtr);
  return 0;
}


int
LgFrSceBaseExploder::mrpExecVol(const std::string& operation, float ** mrpExecVolPtr)
{
  assert(state() >= 2);
  witBoolean exists;
  witGetOperationExists(localWitRun_, operation.c_str(), &exists);
  if (exists)
    witGetOperationMrpExecVol(localWitRun_, operation.c_str(), mrpExecVolPtr);
  else
    mallocZeroVec(mrpExecVolPtr);
  return 0;
}


#if 0
  // Get the four wit mrp part attributes:
  // Default implementation
int
LgFrSceBaseExploder::mrpProdVol(  const std::string& part, float ** mrpProdVolPtr)
{
  assert(state() >= 2);
  if (exists(part))
    witGetPartMrpProdVol(localWitRun_, part, mrpProdVolPtr);
  else
    mallocZeroVec(mrpProdVolPtr);
  return 0;

}
#endif


int
LgFrSceBaseExploder::reqVol(const std::string& part, float ** reqVolPtr)
{
  assert(state() >= 2);
  if (exists(part))
    witGetPartReqVol(localWitRun_, part.c_str(), reqVolPtr);
  else
    mallocZeroVec(reqVolPtr);
  return 0;
}

int
LgFrSceBaseExploder::mrpConsVol(  const std::string& part, float ** mrpConsVolPtr)
{
  assert(state() >= 2);
  if (exists(part))
    witGetPartMrpConsVol(localWitRun_, part.c_str(), mrpConsVolPtr);
  else
    mallocZeroVec(mrpConsVolPtr);
  return 0;
}


  // subUsage Vol
  // must be overridden
int
LgFrSceBaseExploder::subUsageVol(  const std::string& operation,
                                   const int bomIndex,
                                   const int subBomIndex,
                                   float ** subUsageVolPtr)
{

  mallocZeroVec(subUsageVolPtr);
  return 0;
}


void
LgFrSceBaseExploder::mallocZeroVec(float ** floatVecPtr)
{

  //      witMalloc(strlen(partName)+1,(void**)&pn);


  float * zeroVec;
  witMalloc(nPeriods_ * sizeof(float),(void**)&zeroVec);
  assert(zeroVec != 0);
  int t = 0;
  for (t=0; t<nPeriods_; t++)
    zeroVec[t] = 0.0;
  (*floatVecPtr) = zeroVec;


#if 0
  float * zeroVec = (float *) malloc (nPeriods_ * sizeof(float));
  assert(zeroVec != 0);
  int t = 0;
  for (t=0; t<nPeriods_; t++)
    zeroVec[t] = 0.0;
  (*floatVecPtr) = zeroVec;
#endif
}

  // Print out all the attributes (in "del" format)
void
LgFrSceBaseExploder::print(std::string& filename)
{
}





  // preferred constructor
LgFrSceBaseExploder::LgFrSceBaseExploder(WitRun * originalWitRun,
                                         LgFrMultiPlantHelper * mpHelper,
                                         LgFrSceSetOfParameters * setOfParameters)
  : originalWitRun_ (originalWitRun),
    localWitRun_ (originalWitRun),
    mpHelper_ (mpHelper),
    setOfParameters_ (setOfParameters),
    nPeriods_ (0),
    state_ (0),
    id_ (__LGFRSCEBASEEXPLODER)
{
  // get nPeriods from originalWitRun
  witGetNPeriods(originalWitRun_, &nPeriods_);
  state_ = 1;
}

// constructor to be used by Decorator Base Class
LgFrSceBaseExploder::LgFrSceBaseExploder(WitRun * originalWitRun,
                                         WitRun * localWitRun,
                                         LgFrMultiPlantHelper * mpHelper,
                                         LgFrSceSetOfParameters * setOfParameters)
  : originalWitRun_ (originalWitRun),
    localWitRun_ (localWitRun),
    mpHelper_ (mpHelper),
    setOfParameters_ (setOfParameters),
    nPeriods_ (0),
    state_ (0),
    id_ (__LGFRSCEBASEEXPLODER)
{
  // get nPeriods from originalWitRun
  witGetNPeriods(originalWitRun_, &nPeriods_);
}


  // Return Exploder Identifier
isAReturnType
LgFrSceBaseExploder::isA() const
{
  return id_;
}

WitRun *
LgFrSceBaseExploder::theWitRun()
{
  return originalWitRun_;
}

WitRun *
LgFrSceBaseExploder::localWitRun()
{
  return localWitRun_;
}

LgFrMultiPlantHelper *
LgFrSceBaseExploder::mpHelper()
{
  return mpHelper_;
}

LgFrSceSetOfParameters *
LgFrSceBaseExploder::setOfParameters()
{
  return setOfParameters_;
}

int
LgFrSceBaseExploder::nPeriods()
{
  return nPeriods_;
}

int
LgFrSceBaseExploder::state()
{
  return state_;
}

void
LgFrSceBaseExploder::state(int state)
{
  state_ = state;
}


  // default constructor
LgFrSceBaseExploder::LgFrSceBaseExploder()
  : originalWitRun_ (0),
    localWitRun_ (0),
    mpHelper_ (0),
    setOfParameters_ (0),
    nPeriods_ (0),
    state_ (0),
    id_ (__LGFRSCEBASEEXPLODER)
{
  // nothing to be done
}


  // destructor
LgFrSceBaseExploder::~LgFrSceBaseExploder()
{
  // nothing to be done
}




// ======================================================================
// Implementation ---> class LgFrSceExploder : public LgFrSceBaseExploder
// ======================================================================
  // preferred constructor
LgFrSceExploder::LgFrSceExploder(WitRun * originalWitRun,
                                 LgFrMultiPlantHelper * mpHelper,
                                 LgFrSceSetOfParameters * setOfParameters)
  : LgFrSceBaseExploder(originalWitRun,
                        mpHelper,
                        setOfParameters),
    id_ (__LGFRSCEEXPLODER)
{
  // make a tempwitrun for yourself
  // populate it
  // set your pointers;
  witNewRun(&localWitRun_);


  // Set the wit message file
  std::string outputFilePrefix  = setOfParameters->getString("outputFilePrefix");
  std::string witExplodeMsgFileExtension = setOfParameters->getString("witExplodeMsgFileExtension");

  std::string witExplodeMsgFileName(outputFilePrefix + witExplodeMsgFileExtension);
  // check it for write access
  witSetMesgFileAccessMode( localWitRun_, WitTRUE, "w" );
  witSetMesgFileName( localWitRun_, WitTRUE, witExplodeMsgFileName.c_str());

  if (setOfParameters->getBool("printWitExplodeMsgFile"))
    setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_ON);
  else
    setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_OFF);

  witCopyData(localWitRun_, originalWitRun);
  // assert(state_ == 0);

  if (! setOfParameters->getBool("usePrioritizedExplodeUtility")) {
    witSetMultiRoute(localWitRun_, WitTRUE);

    // strictly 4.0 mode
    witSetWit34Compatible(localWitRun_, WitFALSE);
  }


  // 4.20 IPIP fix  (temp)
  witSetNewPipPggOrder(localWitRun_, WitTRUE);



  this->copyAppData();

  //
  if (! setOfParameters->getBool("usePrioritizedExplodeUtility")) {
    this->modifyCustChoiceSupplyVols();
  }

  // ok, we're initialized, but not post processed.
  state(1);

}




// + LgFrSceExploder::modifyCustChoiceSupplyVols:
// +      CustomerChoice modelling trick requires an adjustment of the supplyVols
// +      be adjusted for the explosion calculation to work.   This modification
// +      should NOT be done when usePrioritizedExplodeUtility=TRUE
void
LgFrSceExploder::modifyCustChoiceSupplyVols()
{
  int nParts;
  char ** partList;
  witGetParts(originalWitRun_, &nParts, &partList);

  int nPeriods;
  witGetNPeriods(originalWitRun_, &nPeriods);

  // ========= part Attributes  ==========
  float * supplyVol;


  int i = 0;
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];

    // CUSTOMER_CHOICE_FEATURES
    // See Customer Feature Model documentation. It's kind of wierd.
    if (mpHelper()->isPartSpecialCustChoiceFeature(originalWitRun_, thePart)) {
      witGetPartSupplyVol(originalWitRun_, thePart, &supplyVol);
      LgFrTimeVecFloat supplyAdjust(mpHelper()->custChoiceDummySupplyVolForMrpAdjustment(originalWitRun_,
                                                                                        thePart));
      // We must add the supply adjust ot any existing suply for the specialCustChpoice part
      // An existing supply may be there due to Machine Feature Supply
      LgFrTimeVecFloat existingSupplyTVF(nPeriods, supplyVol);
      LgFrTimeVecFloat newSupplyTVF = supplyAdjust.op(existingSupplyTVF, LgFrAdd);
      witSetPartSupplyVol(localWitRun_, thePart, newSupplyTVF.data());
      witFree(supplyVol);
    }
    // END_CUSTOMER_CHOICE_FEATURES

  }


  // free the parts
  for (i=0; i<nParts; i++)
    witFree(partList[i]);
  witFree(partList);

}


// ---------------------------------------------------------------
// One stop WIT messaging control
void
LgFrSceBaseExploder::setWitInformationalMessagingExpl(WitRun * const theWitRun,
                                                  int onOrOff)
{
  // if they want it off
  if (onOrOff == 0) {
    // shut off ALL info messages
    witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
    // Let ANY warning message come out once
    witSetMesgTimesPrint(theWitRun, WitTRUE, WitWARNING_MESSAGES, 1);
    // Suppress a few WARNING messages that we already know about
    witSetMesgTimesPrint(theWitRun, WitTRUE, 338, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 446, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 538, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 505, 0);
  }
  else {
    // turn all INFO on
    witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, UCHAR_MAX);
    // turn all WARNINGS on
    witSetMesgTimesPrint(theWitRun, WitTRUE, WitWARNING_MESSAGES, UCHAR_MAX);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 338, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 538, 0);
    witSetMesgTimesPrint(theWitRun, WitTRUE, 505, 0);
  }
}



std::string
LgFrSceExploder::title()
{
  std::string myTitle("witMrp");
  return myTitle;
}



// destructor
LgFrSceExploder::~LgFrSceExploder()
{

  // Use WIT Object Iteration to delete all the AppData's

  witAttr objItrState;

  witGetObjItrState(localWitRun_, &objItrState);
  if (objItrState != WitINACTIVE) {
    witResetObjItr(localWitRun_);
  }

  LgFrSceAppData * appDataPtr;

  witBoolean areWeDoneYet = WitFALSE;

  while ( ! areWeDoneYet) {
    witAdvanceObjItr(localWitRun_);

    witGetObjItrState(localWitRun_, &objItrState);

    if (objItrState ==  WitAT_PART) {
      char * partName;
      witGetObjItrPart(localWitRun_, &partName);
      witGetPartAppData(localWitRun_, partName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(partName);
    }

    else if (objItrState == WitAT_DEMAND) {
      char * partName;
      char * demandName;
      witGetObjItrDemand(localWitRun_, &partName, &demandName);
      witGetDemandAppData(localWitRun_, partName, demandName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(partName);
      witFree(demandName);
    }

    else if (objItrState == WitAT_OPERATION) {
      char * opName;
      witGetObjItrOperation(localWitRun_, &opName);
      witGetOperationAppData(localWitRun_, opName, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitAT_BOM_ENTRY) {
      char * opName;
      int b;
      witGetObjItrBomEntry(localWitRun_, &opName, &b);
      witGetBomEntryAppData(localWitRun_, opName, b, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }


    else if (objItrState == WitAT_SUB_ENTRY) {
      char * opName;
      int b, s;
      witGetObjItrSubsBomEntry(localWitRun_, &opName, &b, &s);
      witGetSubsBomEntryAppData(localWitRun_, opName, b, s, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitAT_BOP_ENTRY) {
      char * opName;
      int bp;
      witGetObjItrBopEntry(localWitRun_, &opName, &bp);
      witGetBopEntryAppData(localWitRun_, opName, bp, (void **) &appDataPtr);
      delete appDataPtr;
      witFree(opName);
    }

    else if (objItrState == WitINACTIVE) {
      areWeDoneYet = WitTRUE;
    }

    else
      assert(1 == 0);

  }

  witDeleteRun(localWitRun_);
}


// Return Exploder Identifier
isAReturnType
LgFrSceExploder::isA()
const
{
  return id_;
}

// + LgFrSceExploder::smartAppData:
// +      Use object iteration to clone all the appData objects
void
LgFrSceExploder::copyAppData()
{

  // Use WIT Object Iteration to clone all the AppData's

  witAttr objItrState;

  witGetObjItrState(originalWitRun_, &objItrState);
  if (objItrState != WitINACTIVE) {
    std::cout << "Cloning AppData:   ogjItrState is not WitINACTIVE.   Resetting ObjItr" << std::endl;
    witResetObjItr(originalWitRun_);
  }

  LgFrSceAppData * appDataPtr;
  LgFrSceAppData * cloneAppDataPtr;

  witBoolean areWeDoneYet = WitFALSE;

  while ( ! areWeDoneYet) {
    witAdvanceObjItr(originalWitRun_);

    witGetObjItrState(originalWitRun_, &objItrState);

    if (objItrState ==  WitAT_PART) {
      char * partName;
      witGetObjItrPart(originalWitRun_, &partName);
      witGetPartAppData(originalWitRun_, partName, (void **) &appDataPtr);
      if (appDataPtr != 0) {
        cloneAppDataPtr = appDataPtr->clone();
        witSetPartAppData(localWitRun_, partName, (void *) cloneAppDataPtr);
      }
      witFree(partName);
    }

    else if (objItrState == WitAT_DEMAND) {
      char * partName;
      char * demandName;
      witGetObjItrDemand(originalWitRun_, &partName, &demandName);
      witGetDemandAppData(originalWitRun_, partName, demandName, (void **) &appDataPtr);
      if (appDataPtr != 0) {
        cloneAppDataPtr = appDataPtr->clone();
        witSetDemandAppData(localWitRun_, partName, demandName, (void *) cloneAppDataPtr);
      }
      witFree(partName);
      witFree(demandName);
    }

    else if (objItrState == WitAT_OPERATION) {
      char * opName;
      witGetObjItrOperation(originalWitRun_, &opName);
      witGetOperationAppData(originalWitRun_, opName, (void **) &appDataPtr);
      if (appDataPtr != 0) {
        cloneAppDataPtr = appDataPtr->clone();
        witSetOperationAppData(localWitRun_, opName, (void *) cloneAppDataPtr);
      }
      witFree(opName);
    }

    else if (objItrState == WitAT_BOM_ENTRY) {
      char * opName;
      int b;
      witGetObjItrBomEntry(originalWitRun_, &opName, &b);
      witGetBomEntryAppData(originalWitRun_, opName, b, (void **) &appDataPtr);
      if (appDataPtr != 0) {
        cloneAppDataPtr = appDataPtr->clone();
        witSetBomEntryAppData(localWitRun_, opName, b, (void *) cloneAppDataPtr);
      }
      witFree(opName);
    }


    else if (objItrState == WitAT_SUB_ENTRY) {
      char * opName;
      int b, s;
      witGetObjItrSubsBomEntry(originalWitRun_, &opName, &b, &s);
      witGetSubsBomEntryAppData(originalWitRun_, opName, b, s, (void **) &appDataPtr);
      if (appDataPtr != 0) {
        cloneAppDataPtr = appDataPtr->clone();
        witSetSubsBomEntryAppData(localWitRun_, opName, b, s, (void *) cloneAppDataPtr);
      }
      witFree(opName);
    }

    else if (objItrState == WitAT_BOP_ENTRY) {
      char * opName;
      int bp;
      witGetObjItrBopEntry(originalWitRun_, &opName, &bp);
      witGetBopEntryAppData(originalWitRun_, opName, bp, (void **) &appDataPtr);
      if (appDataPtr != 0) {
        cloneAppDataPtr = appDataPtr->clone();
        witSetBopEntryAppData(localWitRun_, opName, bp, (void *) cloneAppDataPtr);
      }
      witFree(opName);
    }

    else if (objItrState == WitINACTIVE)
      areWeDoneYet = WitTRUE;

    else
      assert (1==0);
  }
}



// + ===========================================================
// + Base Decorator LgFrSceBaseExploderDecorator
// + ===========================================================
// +
// + Overrides:
// +    - isA()


  // The "Action" method ... Just do it
int
LgFrSceBaseExploderDecorator::explode()
{
  return myExploder_->explode();
}



  // determine if this part is something that you deal with
witBoolean
LgFrSceBaseExploderDecorator::exists(const std::string& part)
{
  return myExploder_->exists(part);
}

#if 0
  // Get the four wit mrp part attributes:
  // Default implementation
int
LgFrSceBaseExploderDecorator::mrpProdVol(  const std::string& part, float ** mrpProdVol)
{
  return myExploder_->mrpProdVol(part, mrpProdVol);
}
#endif

int
LgFrSceBaseExploderDecorator::mrpExcessVol(const std::string& part, float ** mrpExcessVol)
{
  return myExploder_->mrpExcessVol(part, mrpExcessVol);
}


int
LgFrSceBaseExploderDecorator::mrpResidualVol(const std::string& part, float ** mrpResidualVol)
{
  return myExploder_->mrpResidualVol(part, mrpResidualVol);
}


int
LgFrSceBaseExploderDecorator::reqVol(      const std::string& part, float ** reqVolPtr)
{
  return myExploder_->reqVol(part, reqVolPtr);
}

int
LgFrSceBaseExploderDecorator::mrpConsVol(  const std::string& part, float ** mrpConsVolPtr)
{
  return myExploder_->mrpConsVol(part, mrpConsVolPtr);
}

int
LgFrSceBaseExploderDecorator::mrpExecVol(const std::string& operation, float ** mrpExecVolPtr)
{
  return myExploder_->mrpExecVol(operation, mrpExecVolPtr);
}


  // subUsage Vol
int
LgFrSceBaseExploderDecorator::subUsageVol(  const std::string& operation,
                                            const int bomIndex,
                                            const int subBomIndex,
                                            float ** subUsageVolPtr)
{
  // just pass this baby along ...
  // if smartExpldoer gets it, fine.  Otherwise the concrete will choke on it.
  return myExploder_->subUsageVol(operation, bomIndex, subBomIndex, subUsageVolPtr);

}




  // preferred constructor
LgFrSceBaseExploderDecorator::LgFrSceBaseExploderDecorator(LgFrSceBaseExploder * myExploder)
  : LgFrSceBaseExploder(myExploder->theWitRun(),
                        myExploder->localWitRun(),
                        myExploder->mpHelper(),
                        myExploder->setOfParameters()),
    myExploder_ (myExploder),
    id_ (__LGFRSCEBASEEXPLODERDECORATOR)
{
  // nothing to do.  Concrete decorators do all the work
}


  // Return Exploder Identifier
isAReturnType
LgFrSceBaseExploderDecorator::isA() const
{
  return id_;
}



#ifdef BASE_DECORATOR_USES_CONCRETE_MEMBER_DATA
WitRun *
LgFrSceBaseExploderDecorator::theWitRun()
{
  return myExploder_->theWitRun();
}

WitRun *
LgFrSceBaseExploderDecorator::localWitRun()
{
  return myExploder_->localWitRun();
}

LgFrMultiPlantHelper *
LgFrSceBaseExploderDecorator::mpHelper()
{
  return myExploder_->mpHelper();
}

LgFrSceSetOfParameters *
LgFrSceBaseExploderDecorator::setOfParameters()
{
  return myExploder_->setOfParameters();
}


int
LgFrSceBaseExploderDecorator::nPeriods()
{
  return myExploder_->nPeriods();
}

#else

WitRun *
LgFrSceBaseExploderDecorator::theWitRun()
{
  return originalWitRun_;
}

WitRun *
LgFrSceBaseExploderDecorator::localWitRun()
{
  return localWitRun_;
}

LgFrMultiPlantHelper *
LgFrSceBaseExploderDecorator::mpHelper()
{
  return mpHelper_;
}

LgFrSceSetOfParameters *
LgFrSceBaseExploderDecorator::setOfParameters()
{
  return setOfParameters_;
}

int
LgFrSceBaseExploderDecorator::nPeriods()
{
  return nPeriods_;
}

#endif

void
LgFrSceBaseExploderDecorator::state(int state)
{
  myExploder_->state(state);
}

int
LgFrSceBaseExploderDecorator::state()
{
  return myExploder_->state();
}


void
LgFrSceBaseExploderDecorator::print(std::string& filename)
{
  myExploder_->print(filename);
}


  // destructor
LgFrSceBaseExploderDecorator::~LgFrSceBaseExploderDecorator()
{
  delete myExploder_;
}


// + ========================================================================
// + class LgFrSceFullExploderDecorator
// + ========================================================================
// +
// + Decorates:
// +    - the witRun via offset modifications
// +
// + Overrides:
// +    - isA()


int
LgFrSceFullExploderDecorator::explode()
{
  return myExploder_->explode();
}

std::string
LgFrSceFullExploderDecorator::title()
{
  std::string myTitle(myExploder_->title());
  myTitle.insert(0, "Full ");
  return myTitle;
}


  // preferred constructor
LgFrSceFullExploderDecorator::LgFrSceFullExploderDecorator(LgFrSceBaseExploder * myExploder)
  : LgFrSceBaseExploderDecorator(myExploder),
    id_ (__LGFRSCEFULLEXPLODERDECORATOR)
{
  this->decorateLocalWitRun();
}


  // destructor
LgFrSceFullExploderDecorator::~LgFrSceFullExploderDecorator()
{
  // do nothing.  The decorator base class deletes myExploder_
}

  // Return Exploder Identifier
isAReturnType
LgFrSceFullExploderDecorator::isA()
const
{
  return id_;
}



// + Full Explode decorates witRun by modifying BOM offsets
void
LgFrSceFullExploderDecorator::decorateLocalWitRun()
{
  // make sure you are dealing with the right witRun

  std::string theC;
  std::string theP;

  int nOps;
  char ** opList;
  witGetOperations(theWitRun(), &nOps, &opList);

  // Now loop over all operations ...
  // and modify the yieldRate back to its setting in the original
  // witRun.  We do this to "undo" what setting wit34Compabile to false did.
  // Namely, wit34Compat=true causes the yieldrates within the cycleTime be
  // set to 0.0.
  // do not do this swap with the CapacityGeneration Operations
  int o = 0;
  for (o=0; o<nOps; o++) {
    char * theOperation = opList[o];
    float * yieldRate;

    if (mpHelper_->isOperationForCapacityGeneration(localWitRun_, opList[o], theC, theP))
      continue;
    witGetOperationYieldRate(theWitRun(), theOperation, &yieldRate);
    witSetOperationYieldRate(localWitRun(), theOperation, yieldRate);
    witFree(yieldRate);
  }

  witSetTruncOffsets(localWitRun(), WitTRUE);

  // free the Operations
  for (o=0; o<nOps; o++)
    witFree(opList[o]);
  witFree(opList);

  // ++++++++++++++++++++++++++++++++
  // Ugly fix.  If we are doing smartExplode, then this last
  // yield fix will obliterate the smartExplode modelling trick
  // that enables PCF parts to be built.  Sooooooo, we must now
  // re-do the modelling trick (this is a duplicate of the snippet
  // of code from restructureForImplosionToSolveExplosion())
  // ++++++++++++++++++++++++++++++++++

  if ( setOfParameters_->getBool("useSmartExplodeUtility")) {
    int nParts;
    char ** partList;
    witGetParts(localWitRun(), &nParts, &partList);


    // Look for parts that are PCF
    int i = 0;
    for (i=0; i<nParts; i++) {
      char * thePart = partList[i];

      LgFrScePartAppData * partAppData;
      witGetPartAppData(theWitRun(), thePart, (void **) &partAppData);
      if (partAppData == 0)
        continue;

      if (! partAppData->isPartPcf())
        continue;

      // Yeah!! we found a bonafide PCF part.

      // find its default operation.
      std::string defaultOperation(thePart);
      witBoolean exists;
      witGetOperationExists(localWitRun(), defaultOperation.c_str(), &exists);
      if (! exists) {
        std::cerr << "TEMP_ERROR2: Default Operation for PCF not found in exploder\n"
             << "PCF: " << defaultOperation << "\n\n";
      }

      float * yieldRate;
      witGetOperationYieldRate(localWitRun(), defaultOperation.c_str(), &yieldRate);
      int t = 0;
      for (t=0; t<partAppData->unConstrainedStartPeriod(); t++) {
        yieldRate[t] = 1.0;
      }
      witSetOperationYieldRate(localWitRun(), defaultOperation.c_str(), yieldRate);
      witFree(yieldRate);
    }


    // free the parts
    for (i=0; i<nParts; i++)
      witFree(partList[i]);
    witFree(partList);
  }

}



// FINISH_ME ... Why can't we just use the version of this in Scenario?   
//               It's stupid to duplicate code
// ---------------------------------------------------------------
void
LgFrSceSmartExploderDecorator::printImpactPeriodFile(
    WitRun * const theWitRun,
    const int nPeriods,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper)
{

  int nOps;
  char ** opList;

  LgFrSceFlatFileMgr & flatFileManager =
    this->sceScenSP_->flatFileMgr();

  MclFacility * theErrFacil = this->sceScenSP_->getErrFacility();


  std::ofstream outFile(fileName.c_str());
  if (outFile.fail()) {
    (*theErrFacil)("CannotOpenFileForWrite",MclArgList()
                   << "ImpactPeriod"
                   << fileName);
  }


  witGetOperations(theWitRun, &nOps, &opList);
  int nBom;
  int op = 0;
  for (op=0; op<nOps; op++ ) {
    std::string parentPdf, parentOpName, childPartName, childPdf;
    int isItNormalOperation = 0;
    if (mpHelper.isOperationNormal(theWitRun, opList[op], parentPdf, parentOpName))
      isItNormalOperation = 1;
    else if (! mpHelper.isOperationSpecialInterplant(
                       theWitRun, opList[op],
                       parentOpName, parentPdf, childPartName, childPdf)) {
      witFree(opList[op]);
      continue;
    }

    witGetOperationNBomEntries(theWitRun, opList[op], &nBom);
    int b=0;
    for (b=0; b<nBom; b++) {
      if (isItNormalOperation) {
        char * child;
        witGetBomEntryConsumedPart(theWitRun, opList[op], b, &child);
        childPartName = mpHelper.partname(child);
        childPdf = parentPdf;
        witFree(child);
      }

      outFile << "\""
              << parentOpName << "\",\""
              << parentPdf << "\",\""
              << childPartName << "\",\""
              << childPdf << "\",";

      if (isItNormalOperation)
        outFile << "\"F\"";
      else
        outFile << "\"A\"";

      int * impactPeriod;
      witGetBomEntryImpactPeriod(theWitRun, opList[op], b, &impactPeriod);
      int t = 0;
      for (t=0; t<nPeriods; t++) {
        outFile << "," << impactPeriod[t];
      }
      outFile << "\n";
      witFree(impactPeriod);
    }

    witFree(opList[op]);
  }

  witFree(opList);

  outFile.close();
}









// + ==================================
// + class LgFrSceSmartExploderDecorator
// + ==================================
// +
// + Decorates:
// +    - the witRun via
// +        -
// +        -
// +        -
// +        -
// +
// + Overrides:
// +    - isA()
// +    - explode()
// +    -


int
LgFrSceSmartExploderDecorator::explode()
{
  // must be in initialized state
  assert(state() > 0);



  // Dump the heur version of the wit.data file
#ifdef DUMP_WIT_MRP_DATA_FILE
  std::cout << "Writing the (mrp) wit data file ..." << std::endl;
  std::string mrpWitDataFileName = "mrpWitDataFile";
  setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_ON);
  witWriteData(localWitRun_, mrpWitDataFileName.c_str());
  if (setOfParameters_->getBool("printWitExplodeMsgFile"))
    setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_ON);
  else
    setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_OFF);

  std::cout << "Writing the (mrp) echo  file ..." << std::endl;
  std::string mrpWitEchoFileName = "mrpWitEchoFile";
  setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_ON);
  witDisplayData(localWitRun_, mrpWitEchoFileName.c_str());
  if (setOfParameters_->getBool("printWitExplodeMsgFile"))
    setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_ON);
  else
    setWitInformationalMessagingExpl(localWitRun_, SCE_WIT_MESSAGING_OFF);

#endif


  bool doPegging = setOfParameters_->getBool("doPegging");
  if (doPegging) {
    witClearPipSeq(localWitRun_);
    witSetPipSeqFromHeur(localWitRun_, WitTRUE);
  }

  bool  usePrioritizedExplodeUtility = setOfParameters_->getBool("usePrioritizedExplodeUtility");
  std::string  usageName("explosion");

  bool  printMrpImpactPeriodDiagnosticFile = setOfParameters_->getBool("printMrpImpactPeriodDiagnosticFile");
  if (printMrpImpactPeriodDiagnosticFile) {
    std::string impactPdFile = setOfParameters_->getString("mrpImpactPeriodDiagFileName");
    std::cout << "Writing the Impact Period Diagnostic File (Explosion view) ..." << std::endl;
    printImpactPeriodFile(localWitRun_, nPeriods_,
			  impactPdFile, *mpHelper_);
  }





  // determine which explode engine to use
  if (usePrioritizedExplodeUtility) {
    LgFrSceCritList & theCritList = sceScenSP_->sceCritList();
    LgFrSceAllocationEngineMgr allocEngineMgr( sceScenSP_,
                                               localWitRun_,
                                               mpHelper_,
                                               usageName);
    allocEngineMgr.implode(theCritList);
  }
  else {
    witHeurImplode(localWitRun());

    //post-process
    witPostprocess (localWitRun_ );

    if (doPegging) {
      witBuildPip(localWitRun_);
      this->loadPIPDataIntoAppData(localWitRun_);
    }

  }


  state(2);

  return 0;
}





// ===============================
// ====== PIP Utilitiies ====
// ===============================
void
LgFrSceSmartExploderDecorator::loadPIPDataIntoAppData(WitRun * const theWitRun)
{
  int nPeriods;
  char  *partName, *demandName;
  float * demandVol;


  bool PegSubUsageToPrimary = setOfParameters_->getBool("PegSubUsageToPrimary");
  bool supplierPerspectiveInterplantPeg = setOfParameters_->getBool("supplierPerspectiveInterplantPeg");

  std::cout << "... collecting PIP information for MRP ..." << std::endl;

  witGetNPeriods(theWitRun, &nPeriods);
  int nParts;
  char  **partList;
  witGetParts(theWitRun, &nParts, &partList);

  std::string pp, geo;
  std::string fullWitGPDPartName, fullWitGPDDemandName;

  // Loop over parts to get demands
  int i = 0;
  for(i=0; i<nParts; i++){
    partName = partList[i];
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun, partList[i], &nDemands, &dList);
    int j = 0;
    for(j=0; j<nDemands; j++){
      demandName = dList[j];

      // Now for some fancy trickery !!!
      // if the part is a special feature part, then we must map the consVol over to
      //     the GPD appData
      int isDemandSpecial = 0;
      isDemandSpecial = mpHelper_->isDemandSpecialPureOptionBuildDemand(theWitRun, partName, demandName, pp, geo);
      if (! isDemandSpecial) {
        isDemandSpecial = mpHelper_->isDemandSpecialCustChoiceFeature(theWitRun, partName, demandName, pp, geo);
      }


      // get the demandAppData, or create a new one if it doesn't exist.
      LgFrSceDemandAppData * demandAppDataPtr = 0 ;

      // if the demand is a specialFeaturePart then get the gpd's appdata
      if (isDemandSpecial) {
        // get a witPartName for the GPD
        fullWitGPDPartName = mpHelper_->geoPlannerDemandPartName(pp, geo);

        witGetDemandAppData( theWitRun, fullWitGPDPartName.c_str(), geo.c_str(),
                           (void **) &demandAppDataPtr );
        if ( demandAppDataPtr == 0 ) {
          demandAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
          assert ( demandAppDataPtr != 0 );
          witSetDemandAppData( theWitRun, fullWitGPDPartName.c_str(), geo.c_str(),
                               (void *) demandAppDataPtr );
        }
      }
      else {
        witGetDemandAppData( theWitRun, partName, demandName,
                             (void **) &demandAppDataPtr );
        if ( demandAppDataPtr == 0 ) {
          demandAppDataPtr = new LgFrSceDemandAppData ((size_t) nPeriods, 1.0);
          assert ( demandAppDataPtr != 0 );
          witSetDemandAppData( theWitRun, partName, demandName,
                               (void *) demandAppDataPtr );
        }
      }

#ifdef DEBUG_DUMP_FOR_PEGGING

      if (isDemandSpecial) {
        std::cout << "specialDemand, being REPORTED..." << std::endl;
        std::cout << "PIP supply is being reported for: "
             << fullWitGPDPartName << " at geo " << geo << std::endl;
      }

      std::cout << "(before)PIPConsVolSched (" << partList[i] << "," << dList[j] << std::endl;

      demandAppDataPtr->PIPConsVolSched().print();
      std::cout << std::endl;

#endif

      // Get demands PIP consVol and directly store in appData
      ScePIP::getDemandConsVolPIP(theWitRun,
                                  partList[i],
                                  dList[j],
                                  0,
                                  nPeriods-1,
                                  demandAppDataPtr->PIPConsVolSched(),
                                  WitTRUE,
                                  PegSubUsageToPrimary);


      // Get demands PIP supplyVol and directly store in appData
      ScePIP::getSupplyVolPIP(theWitRun,
                              partList[i],
                              dList[j],
                              0,
                              nPeriods-1,
                              demandAppDataPtr->PIPSupplyVolSched());


      ScePIP::getDemandExecVolPIP(theWitRun,
				  partList[i],
				  dList[j],
				  0,
				  nPeriods-1,
				  demandAppDataPtr->PIPExecVolSched());



      ScePIP::getDemandInterplantVolPIP(theWitRun,
					mpHelper_,
					partList[i],
					dList[j],
					0,
					nPeriods-1,
					demandAppDataPtr->PIPInterplantVolSched(),
					supplierPerspectiveInterplantPeg);
      

#ifdef DEBUG_INTERPLANT_PEG
      std::cout << "laoding interplantPIP from exploders.C" << std::endl;
      demandAppDataPtr->PIPInterplantVolSched().print();
      std::cout << " ==========================" << std::endl << std::endl;
#endif



#ifdef DEBUG_DUMP_FOR_PEGGING
      std::cout << "(after) JP's Get" << std::endl;
      demandAppDataPtr->PIPConsVolSched().print();
      std::cout << " ==========================" << std::endl << std::endl;
#endif

      witFree(dList[j]);
    }
    witFree(dList);
  }
}


  // preferred constructor
LgFrSceSmartExploderDecorator::LgFrSceSmartExploderDecorator(LgFrSceBaseExploder * myExploder,
                                                             LgFrScenarioForSceSmartPointer sceScenSP,
                                                             LgFrMultiPlantHelper * mpHelper)
  : LgFrSceBaseExploderDecorator(myExploder),
  id_ (__LGFRSCESMARTEXPLODERDECORATOR),
  sceScenSP_ (sceScenSP),
  mpHelper_ (mpHelper)
{
  if (setOfParameters_->getBool("usePrioritizedExplodeUtility"))
    this->decorateLocalWitRunForPrioritizedExplode();
  else
    this->decorateLocalWitRun();
}



std::string
LgFrSceSmartExploderDecorator::title()
{
  std::string myTitle(myExploder_->title());
  myTitle.insert(0,"Smart ");
  return myTitle;
}



  // destructor
LgFrSceSmartExploderDecorator::~LgFrSceSmartExploderDecorator()
{
  // nothing to do here
}

  // Return Exploder Identifier
isAReturnType
LgFrSceSmartExploderDecorator::isA()
const
{
  return id_;
}


// + Full Explode decorates witRun by modifying BOM offsets
void
LgFrSceSmartExploderDecorator::decorateLocalWitRun()
{
  // set the equitability factor
  int localEquitability = setOfParameters_->getInt("smartExplodeEquit");
  witSetEquitability(localWitRun(), localEquitability);

  // set forcedEquitability parameter in wit.
  bool smartExplodeForceEquit = setOfParameters_->getBool("smartExplodeForceEquit");
  witSetForcedMultiEq(localWitRun_, smartExplodeForceEquit);

  // 2.30 fix
  witSetSkipFailures(localWitRun_, WitFALSE);

  // 2.30 addition: set expCutoff
  float expCutoffTolerance = setOfParameters_->getFloat("expCutoffTolerance");
  witSetExpCutoff(localWitRun_, expCutoffTolerance);


  // bool doPegging = setOfParameters_->getBool("doPegging");


  // FINISH_ME: this is a bit kludgy.   If we are doing Penalized Execution (by virtue of the
  // the groupSharing approach, then we want to shut off penalized execution in the smartExplode
  // Note that we let it stay on in prioritized Explode method.  Eventaully will need more explicit
  // paramters to control everything in explode as well as implode
  bool groupSharingViaExecPenalties =  setOfParameters_->getBool("groupSharingViaExecPenalties");
  if (groupSharingViaExecPenalties)
    witSetPenExec(localWitRun_, WitFALSE);

  // ----------------------------------------------
  // STEP 2: disable any sub bom entries with objectInEffectForExplosion=0
  // ----------------------------------------------
  this->disableSubBomAccordingToAppData();

  // ---------------------------------
  // STEP 3: Restructure for Implosion
  // ---------------------------------
  // In this step we restructure the model so that an implosion
  // can accurately represent an explosion
  this->restructureForImplosionToSolveExplosion();

  // ---------------------------------
  // STEP 3.5: Use pipPriority's if engine==optimization and pipPriorityFile non-null
  // ---------------------------------
  bool doPegging = setOfParameters_->getBool("doPegging");

  if (doPegging) {
    std::string engine = setOfParameters_->getString("engine");
    std::string pipPriorityFile = setOfParameters_->getString("pipPriorityFile");

    // if using optimization and a pipPriorityFile, then we need to load the
    // pipPriorities from the WitRun into the localWitrun
    if (engine == "optimization" && (! pipPriorityFile.empty())) {
      std::cout << "Starting Post Implosion Pegging ... " << std::endl;
      int nPeriods;
      int nParts, nDemands;
      char ** partList, ** demList;
      int i, j;

      witGetNPeriods(localWitRun_, &nPeriods);
      witGetParts( localWitRun_, &nParts, &partList );

      // Loop thru every part, looking for special cases to fix
      for (i=0; i<nParts; i++)  {
        witGetPartDemands(localWitRun_, partList[i], &nDemands, &demList);
        for (j=0; j<nDemands; j++) {
          // get it From AppData !!
          int * priority;
          witGetDemandPriority(localWitRun_, partList[i], demList[j], &priority);
          LgFrTimeVecFloat pipPriority(nPeriods, 0.0);
          int t;
          for (t=0; t<nPeriods; t++)
            pipPriority[t] = (float) priority[t];
          witFree(priority);

          LgFrSceDemandAppData * demandAppDataPtr;
          witGetDemandAppData(originalWitRun_,
                              partList[i],
                              demList[j],
                              (void **) &demandAppDataPtr);

          // If the demandAppDataPtr has been set, see if the pipPriority has been set.
          if (demandAppDataPtr != 0)  {
            pipPriority = demandAppDataPtr->pipPriority();
          }
          witSetDemandPriority(localWitRun_, partList[i], demList[j], pipPriority.intData());
          witFree(demList[j]);
        }
        witFree(partList[i]);
        witFree(demList);
      }
      witFree(partList);
    }
  }




  // ----------------------------------
  // STEP 4: Do Robin's split BOP thing
  // ----------------------------------
  // The Final step is to do the exact same processing of BOP entries
  // that is done in initialProblem.  This is where we look at
  // bop and subbom preferences.
  // FINISH_ME: For now we will strictly use heuristic implosion.  For
  //      LP, this step is unnecessary.  Of course, other steps are.
  //      WAhhh! I wanna go home.
  // FINISH_ME: implemnt this.  we do not need this for the MD
  // alpha run.
  // doRobinsBopThing()

  state(1);


}


void
LgFrSceSmartExploderDecorator::disableSubBomAccordingToAppData()
{
  witAttr objItrState;
  witResetObjItr(originalWitRun_);

  LgFrSceAppData * appDataPtr;

  witBoolean areWeDoneYet = WitFALSE;

  while ( ! areWeDoneYet) {
    witAdvanceObjItr(localWitRun_);
    witGetObjItrState(localWitRun_, &objItrState);

    if (objItrState == WitAT_SUB_ENTRY) {
      char * opName;
      int b, s;
      witGetObjItrSubsBomEntry(localWitRun_, &opName, &b, &s);


      witGetSubsBomEntryAppData(localWitRun_, opName, b, s, (void **) &appDataPtr);

      // + Sub Boms that have either: no appData, or, isObjectInEffectForOptExplosion=false
      // + are shut off via effectivity dates.
      if (appDataPtr != 0) {

        if (! ((LgFrSceSubBomAppData *) appDataPtr)->isObjectInEffectForOptExplosion()) {
          witSetSubsBomEntryEarliestPeriod(localWitRun_, opName, b, s, nPeriods());
          witSetSubsBomEntryNetAllowed(localWitRun_, opName, b, s, WitFALSE);
        }
      }
      else {
          witSetSubsBomEntryEarliestPeriod(localWitRun_, opName, b, s, nPeriods());
          witSetSubsBomEntryNetAllowed(localWitRun_, opName, b, s, WitFALSE);
      }
      witFree(opName);
    }

    if (objItrState == WitINACTIVE) {
      areWeDoneYet = WitTRUE;
    }

  }
}





// + Smartexplode: restructureForImplosionToSolveExplosion
// + (1) set the yieldRate on PCF default operations to 1.0
// + (1.5) set expAversion
// + (2) turn on the bop entry for pcfPart's default operation by
// +     setting expAllowed=true and effectivity to entire horizon
// + (3) Make capacity non-gating by addding a dummy operation to
// +     create it.
void
LgFrSceSmartExploderDecorator::restructureForImplosionToSolveExplosion()
{


  int nParts;
  char ** partList;
  witGetParts(localWitRun(), &nParts, &partList);

  int o = 0;
  int nOps;
  char ** opList;
  witGetOperations(localWitRun(), &nOps, &opList);

  // Look for parts that are PCF
  int i = 0;
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];


    if (! mpHelper_->isPartNormal(theWitRun(), thePart))
      continue;



    LgFrScePartAppData * partAppData;
    witGetPartAppData(theWitRun(), thePart, (void **) &partAppData);
    if (partAppData == 0)
      continue;

    if (! partAppData->isPartPcf()) {



#if 1
      // this snippet of code is looking for any situations where
      // a part might possibly be unexplodeable.   In particular, when
      //  a Normal Part has more than one producing BOP.  In this case,
      // the default BOP has been shut off and the other BOPs (interplant,
      // aggregation, userDefinedOp, ...)
      // when the
      witBoolean defOpExists;

      int  npBops;  // number of bops producing the i-th part
      witGetPartNProducingBopEntries ( localWitRun(), partList[i], & npBops );

      if ( npBops <= 1 )  // we only care about the case with multiple bops
        continue;

      std::string defaultOperationName (partList[i]);
      witGetOperationExists(localWitRun(), defaultOperationName.c_str(), &defOpExists);

      //  Capacity parts (and perhaps)
      // some dummy parts (in the future) may have more than one bop and no
      // default op.
      if (! defOpExists) {
        continue;
      }

      int nBom;
      witGetOperationNBomEntries(localWitRun(), defaultOperationName.c_str(), &nBom);
      // if the default operation is childless, shut off the default bop's effectivity
      if (nBom == 0) {

        // find bopEntryIndex for default operation
        bool didWefindBopEntryForDefaultOperation = false;

        int p = 0;
        for (p=0; p<npBops; p++) { // p = prodIndex
          char * opName;
          int bopEntryIndex;
          witGetPartProducingBopEntry(localWitRun(), partList[i], p, &opName, &bopEntryIndex);

          if (defaultOperationName == opName) {
            // Note: at this time there should only be one
            // bop entry for the default part
            assert(didWefindBopEntryForDefaultOperation == false);
            didWefindBopEntryForDefaultOperation = true;
            witSetBopEntryExpAllowed(localWitRun(), defaultOperationName.c_str(), bopEntryIndex, WitTRUE );
            witSetBopEntryEarliestPeriod(localWitRun(), defaultOperationName.c_str(), bopEntryIndex, 0 );
            witSetBopEntryLatestPeriod(localWitRun(), defaultOperationName.c_str(), bopEntryIndex, nPeriods()-1 );
            witSetBopEntryExpAversion(localWitRun(), defaultOperationName.c_str(), bopEntryIndex, 88888.8 );
          }
          witFree(opName);
        }
        // make sure you found a bop entry for default to shut off
        assert(didWefindBopEntryForDefaultOperation == true );
      }
    }



#endif

    else {
      // Else!! we found a bonafide PCF part.

      // find its default operation.
      std::string defaultOperation(thePart);
      witBoolean exists;
      witGetOperationExists(localWitRun(), defaultOperation.c_str(), &exists);
      assert(exists);

      float * yieldRate;
      witGetOperationYieldRate(localWitRun(), defaultOperation.c_str(), &yieldRate);
      int t = 0;
      for (t=0; t<partAppData->unConstrainedStartPeriod(); t++) {
        yieldRate[t] = 1.0;
      }
      witSetOperationYieldRate(localWitRun(), defaultOperation.c_str(), yieldRate);
      witFree(yieldRate);


      // find the bop entry that leads to the default operation
      // set effectivities on and byProduct off
      int nbBops;
      char * producedPart;
      witGetOperationNBopEntries(localWitRun(), defaultOperation.c_str(), &nbBops);
      int defaultOperationFound = 0;
      int b = 0;
      for (b=0; b<nbBops; b++) {
        witGetBopEntryProducedPart(localWitRun(), defaultOperation.c_str(), b, &producedPart);
        if (producedPart == defaultOperation) {
          defaultOperationFound = 1;
          witSetBopEntryExpAllowed(localWitRun(), defaultOperation.c_str(), b, WitTRUE);
          witSetBopEntryEarliestPeriod(localWitRun(), defaultOperation.c_str(), b, 0);
          witSetBopEntryLatestPeriod(localWitRun(), defaultOperation.c_str(), b, nPeriods()-1);
        }
        witFree(producedPart);
      }
      if (! defaultOperationFound) {
        std::cerr << "TEMP_ERROR: Default Operation for PCF not found in exploder\n"
                  << "PCF: " << defaultOperation << "\n\n";
      }
    }

  }

  // + Smart Explode modelling trick:  Add operations to produce capacity
  // + For each capacity part, add a dummy operation that produces the capacity.
  // + Then, to get reqVol, we simply pick up execVol of the dummy operation.
  // + Note: this modelling trick applies to Normal capacity parts as well as
  // + modelling trick generated capacity parts.
  // +

#if 0
  LgFrTimeVecFloat unitYieldRate(nPeriods(), 1.0);
  for (i=0; i<nParts; i++) {
    std::string theCapacity(partList[i]);

    witAttr category;
    witGetPartCategory(localWitRun_, theCapacity.c_str(), &category);
    if (category == WitCAPACITY) {
      witBoolean exists;
      std::string capGeneratorOperationName =
        mpHelper_->operationForCapacityGeneration(theCapacity);
      witGetOperationExists(localWitRun_, capGeneratorOperationName.c_str(), &exists);
      if (exists) {
        witSetOperationYieldRate(localWitRun_, capGeneratorOperationName.c_str(),unitYieldRate.data());
      }
      else {
        std::cout << "Found a capacity that doesn't have a capGeneratorOperation:"
                  << theCapacity.c_str << std::endl;
      }
    }
  }
#else
  LgFrTimeVecFloat unitYieldRate(nPeriods(), 1.0);
  for (o=0; o<nOps; o++) {
    std::string theCapacity;
    std::string thePdf;
    if (mpHelper_->isOperationForCapacityGeneration(localWitRun_, opList[o], theCapacity, thePdf)) {
      witSetOperationYieldRate(localWitRun_, opList[o],unitYieldRate.data());
    }
  }
 #endif

  // free the parts
  for (i=0; i<nParts; i++)
    witFree(partList[i]);
  witFree(partList);

  // free the Operations

  for (o=0; o<nOps; o++)
    witFree(opList[o]);
  witFree(opList);

}


// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXX  decorateLocalWitRunForPrioritizedExplode XXXXXXXXXXXXxXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
// XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
void
LgFrSceSmartExploderDecorator::decorateLocalWitRunForPrioritizedExplode()
{
  // set the equitability factor
  //  int localEquitability = setOfParameters_->getInt("smartExplodeEquit");
  //  witSetEquitability(localWitRun(), localEquitability);

  // set forcedEquitability parameter in wit.
  //  bool smartExplodeForceEquit = setOfParameters_->getBool("smartExplodeForceEquit");
  //  witSetForcedMultiEq(localWitRun_, smartExplodeForceEquit);

  // 2.30 addition: set expCutoff
  //   witSetSkipFailures(localWitRun_, WitFALSE);

  // 2.30 addition: set expCutoff
  //  float expCutoffTolerance = setOfParameters_->getFloat("expCutoffTolerance");
  //  witSetExpCutoff(localWitRun_, expCutoffTolerance);


  //  4.31: stock reallocation
  bool useStockReallocationInPrioExplode = setOfParameters_->getBool("useStockReallocationInPrioExplode");
  witSetStockReallocation(localWitRun_, useStockReallocationInPrioExplode);


  // bool doPegging = setOfParameters_->getBool("doPegging");

  //  witSetPenExec(localWitRun_, WitFALSE);

  // Need a special one for this case.   Only set for normal parts.  Leave the modellling trick subs alone (ie, special Features)
  //   this->disableSubBomAccordingToAppData();

  std::cout << "Entering ... decorateLocalWitRunForPrioritizedExplode" << std::endl;


  witAttr objItrState;

  //  witResetObjItr(originalWitRun_);
  witResetObjItr(localWitRun_);

  LgFrSceAppData * appDataPtr;

  witBoolean areWeDoneYet = WitFALSE;
  while ( ! areWeDoneYet) {
    witAdvanceObjItr(localWitRun_);
    witGetObjItrState(localWitRun_, &objItrState);

    if (objItrState == WitAT_SUB_ENTRY) {
      char * opName;
      int b, s;
      witGetObjItrSubsBomEntry(localWitRun_, &opName, &b, &s);

      std::string pdf, op;
      if (mpHelper_->isOperationNormal(localWitRun_, opName, pdf, op)) {

        witGetSubsBomEntryAppData(localWitRun_, opName, b, s, (void **) &appDataPtr);

        // + Sub Boms that have either: no appData, or, isObjectInEffectForOptExplosion=false
        // + are shut off via effectivity dates.
        if (appDataPtr != 0) {
          if (! ((LgFrSceSubBomAppData *) appDataPtr)->isObjectInEffectForOptExplosion()) {
            witSetSubsBomEntryEarliestPeriod(localWitRun_, opName, b, s, nPeriods());
            witSetSubsBomEntryNetAllowed(localWitRun_, opName, b, s, WitFALSE);
          }
        }
        else {
          witSetSubsBomEntryEarliestPeriod(localWitRun_, opName, b, s, nPeriods());
          witSetSubsBomEntryNetAllowed(localWitRun_, opName, b, s, WitFALSE);
        }
      }
      witFree(opName);
    }

    if (objItrState == WitINACTIVE) {
      areWeDoneYet = WitTRUE;
    }

  }



  // ---------------------------------
  // STEP 3: Restructure for Implosion
  // ---------------------------------
  // In this step we restructure the model so that an implosion
  // can accurately represent an explosion
  //  this->restructureForImplosionToSolveExplosion();
  // +
  // + 1) Enable Capacity Generation Operations  via yieldRate=1.0
  // + 2) Make PCF parts are made explodeable via:
  // +    a) yieldRate = 1 for it's producing operation
  // +    b) producing BOP entry gets effectivity open and expAllowed=T
  // + 3) Make any unexplodeable NON-PCF parts explodeable
  // + 4) witSetWit34Compatible=True causes yieldRates to get reset based on cycleTimes
  // +       we need to correct for that by copying back the orginal yieldRates from original
  // +       originalWitRun.   This can go away if we ever get away from wit34Comapatible mode
  LgFrTimeVecFloat unitYieldRate(nPeriods(), 1.0);
  int o = 0;
  std::string capacityName;
  std::string pdf;

  int nOps;
  char ** opList;
  witGetOperations(localWitRun(), &nOps, &opList);


  // Loop over operations once and fix everything in one pass
  for (o=0; o<nOps; o++) {
    std::string  theOperation = opList[o];

    // + Enable Capacity Generation Operations  via yieldRate=1.0
    if (mpHelper_->isOperationForCapacityGeneration(localWitRun_, theOperation.c_str(), capacityName, pdf)) {
      witSetOperationYieldRate(localWitRun_, theOperation.c_str(), unitYieldRate.data());
    }

    // + Make PCF parts explodeable via:
    else if  (mpHelper_->isPartPcf(localWitRun_, theOperation)) {
      witBoolean exists;
      witGetPartExists(localWitRun(), theOperation.c_str(), &exists);
      assert(exists);

      LgFrScePartAppData * partAppData;
      witGetPartAppData(localWitRun(), theOperation.c_str(), (void **) &partAppData);
      assert(partAppData != 0);

      float * yieldRate;
      witGetOperationYieldRate(localWitRun(), theOperation.c_str(), &yieldRate);
      int t = 0;
      for (t=0; t<partAppData->unConstrainedStartPeriod(); t++) {
        yieldRate[t] = 1.0;
      }
      witSetOperationYieldRate(localWitRun(), theOperation.c_str(), yieldRate);
      witFree(yieldRate);


      // find the bop entry that leads to the default operation
      // set effectivities open and expAllowed=true
      int nbBops;
      char * producedPart;
      witGetOperationNBopEntries(localWitRun(), theOperation.c_str(), &nbBops);
      int defaultOperationFound = 0;
      int b = 0;
      for (b=0; b<nbBops; b++) {
        witGetBopEntryProducedPart(localWitRun(), theOperation.c_str(), b, &producedPart);
        if (producedPart == theOperation) {
          defaultOperationFound = 1;
          witSetBopEntryExpAllowed(localWitRun(), theOperation.c_str(), b, WitTRUE);
          witSetBopEntryEarliestPeriod(localWitRun(), theOperation.c_str(), b, 0);
          witSetBopEntryLatestPeriod(localWitRun(), theOperation.c_str(), b, nPeriods()-1);
        }
        witFree(producedPart);
      }
      assert(defaultOperationFound);
    }

    witFree(opList[o]);
  }
  witFree(opList);


  // + 3) Loop Through Parts.
  //       a) manually truncate wit34 cycletimes.
  //       b) Make any unexplodeable, multi-bop'd,  NON-PCF parts explodeable
  //          FINISH_ME: this looks a bit wierd.   Review.
  // 4.20 rev1
  int nParts;
  char ** partList;
  witGetParts(localWitRun(), &nParts, &partList);

  // Loop through all the Normal parts ...
  int i = 0;
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];

    if (! mpHelper_->isPartNormal(theWitRun(), thePart)) {
      witFree(partList[i]);
      continue;
    }

    witAttr category;
    witGetPartCategory(theWitRun(), partList[i], &category);
    if (category != WitCAPACITY) {
      //  make sure buildASAP is off
      // FINISH_ME_440: this may be a parameter setting in future
      witSetPartBuildAsap(localWitRun(), thePart, WitFALSE);
    }

#ifdef VARIABLE_PERIODS

    /*  no need to manually trnuc the cycletimes.   Wit does it with truncOffsets on offsets
	HOWEVER ... we do need to think about truncing end of horizon leadtimes due to negative offsets so EVERYTHING can explode
    if (category == WitPRODUCT) {
      // do we need to truncate anything if we convert completely to offsets??
      // tuncate the cycleTimes, so these things can explode in early periods.   Since cycletime is a
      // legacy of wit34, we must manually truncate them.   wit's truncateOffsets will not do it.
      // finish this
      float * localCycleTime;
      witGetPartCycleTime(localWitRun(), thePart, &localCycleTime);
      int ttt=0;
      int cyMax = ceil(localCycleTime[0]);
      if (cyMax >= nPeriods() -1)
         cyMax = nPeriods() -1;
      for (ttt=0; ttt<=cyMax; ttt++) {
        localCycleTime[ttt]=ttt;
      }
      witSetPartCycleTime(localWitRun(), thePart, localCycleTime);
      witFree(localCycleTime);
    }
    */
#else
    if (category == WitPRODUCT) {
      // tuncate the cycleTimes, so these things can explode in early periods.   Since cycletime is a
      // legacy of wit34, we must manually truncate them.   wit's truncateOffsets will not do it.
      float * localCycleTime;
      witGetPartCycleTime(localWitRun(), thePart, &localCycleTime);
      int ttt=0;
      int cyMax = ceil(localCycleTime[0]);
      if (cyMax >= nPeriods() -1)
         cyMax = nPeriods() -1;
      for (ttt=0; ttt<=cyMax; ttt++) {
        localCycleTime[ttt]=ttt;
      }
      witSetPartCycleTime(localWitRun(), thePart, localCycleTime);
      witFree(localCycleTime);
    }
#endif

    LgFrScePartAppData * partAppData;
    witGetPartAppData(theWitRun(), thePart, (void **) &partAppData);
    if (partAppData == 0) {
      witFree(partList[i]);
      continue;
    }

    // Look for any unexplodeable non-pcf parts and ... fix it.
    // In particular, when
    //  a Normal Part has more than one producing BOP.  In this case,
    // the default BOP has been shut off and the other BOPs (interplant,
    // aggregation, userDefinedOp, ...)
    // when the
    if (! partAppData->isPartPcf()) {
      witBoolean defOpExists;

      int  npBops;  // number of bops producing the i-th part
      witGetPartNProducingBopEntries ( localWitRun(), partList[i], & npBops );

      if ( npBops <= 1 ) {  // we only care about the case with multiple bops
	witFree(partList[i]);
        continue;
      }

      std::string defaultOperationName (partList[i]);
      witGetOperationExists(localWitRun(), defaultOperationName.c_str(), &defOpExists);

      //  Capacity parts (and perhaps)
      // some dummy parts (in the future) may have more than one bop and no
      // default op.
      if (! defOpExists) {
	witFree(partList[i]);
        continue;
      }

      int nBom;
      witGetOperationNBomEntries(localWitRun(), defaultOperationName.c_str(), &nBom);
      // if the default operation is childless, shut off the default bop's effectivity
      if (nBom == 0) {

        // find bopEntryIndex for default operation
        bool didWefindBopEntryForDefaultOperation = false;

        int p = 0;
        for (p=0; p<npBops; p++) { // p = prodIndex
          char * opName;
          int bopEntryIndex;
          witGetPartProducingBopEntry(localWitRun(), partList[i], p, &opName, &bopEntryIndex);

          if (defaultOperationName == opName) {
            // Note: at this time there should only be one
            // bop entry for the default part
            assert(didWefindBopEntryForDefaultOperation == false);
            didWefindBopEntryForDefaultOperation = true;
            witSetBopEntryExpAllowed(localWitRun(), defaultOperationName.c_str(), bopEntryIndex, WitTRUE );
            witSetBopEntryEarliestPeriod(localWitRun(), defaultOperationName.c_str(), bopEntryIndex, 0 );
            witSetBopEntryLatestPeriod(localWitRun(), defaultOperationName.c_str(), bopEntryIndex, nPeriods()-1 );
            witSetBopEntryExpAversion(localWitRun(), defaultOperationName.c_str(), bopEntryIndex, 88888.8 );
          }
          // for 4.20: we now set execPenalty to very high number
          witSetOperationExecPenalty(localWitRun(), defaultOperationName.c_str(), 88888.8);
          witFree(opName);
        }
        // make sure you found a bop entry for default to shut off
        assert(didWefindBopEntryForDefaultOperation == true );
      }
    }
    witFree(partList[i]);
  }
  witFree(partList);

  witSetTruncOffsets(localWitRun(), WitTRUE);
  state(1);

}


// --------------
// get the reqVol
// --------------
// reqVol = 0, if part is NOT PCF
//        = prodVol if part is a PCF
int
LgFrSceSmartExploderDecorator::reqVol(const std::string& part,
                                      float ** reqVolPtr)
{


  witAttr category;
  witGetPartCategory(localWitRun_, part.c_str(), &category);
  if (category == WitCAPACITY) {
    std::string thePart(part);
    std::string dummyOperationName =
      mpHelper_->operationForCapacityGeneration(thePart);
    witGetOperationExecVol(localWitRun_, dummyOperationName.c_str(), reqVolPtr);
    return 0;
  }


  int isPartPcf = 0;

  if (exists(part)) {
    LgFrScePartAppData * partAppDataPtr;
    witGetPartAppData(localWitRun(), part.c_str(), (void **) &partAppDataPtr);
    if (partAppDataPtr != 0) {
      isPartPcf =  partAppDataPtr->isPartPcf();
    }
    if (isPartPcf) {
      witGetPartProdVol(localWitRun_, part.c_str(), reqVolPtr);
      int nPeriods;
      witGetNPeriods(localWitRun_, &nPeriods);
      // note: reqVol can only be within the consttraint horizon
      int t = 0;
      for (t=partAppDataPtr->unConstrainedStartPeriod(); t<nPeriods; t++)
        (*reqVolPtr)[t] = 0.0;
    }
    else
      mallocZeroVec(reqVolPtr);
  }
  else
    mallocZeroVec(reqVolPtr);
  return 0;
}



  // get the mrpConsVol
int
LgFrSceSmartExploderDecorator::mrpConsVol(const std::string& part,
                                          float ** mrpConsVolPtr)
{
  assert(state() >= 2);
  if (exists(part))
    witGetPartConsVol(localWitRun(), part.c_str(), mrpConsVolPtr);

  else
    mallocZeroVec(mrpConsVolPtr);

  return 0;
}




  // get the mrpExcessVol
int
LgFrSceSmartExploderDecorator::mrpExcessVol(const std::string& part,
                                            float ** mrpExcessVolPtr)
{
  assert(state() >= 2);
  if (exists(part))
    witGetPartExcessVol(localWitRun(), part.c_str(), mrpExcessVolPtr);
  else
    mallocZeroVec(mrpExcessVolPtr);
  return 0;
}

  // get the mrpResidualVol
int
LgFrSceSmartExploderDecorator::mrpResidualVol(const std::string& part,
                                            float ** mrpResidualVolPtr)
{
  assert(state() >= 2);
  if (exists(part))
    witGetPartResidualVol(localWitRun(), part.c_str(), mrpResidualVolPtr);
  else
    mallocZeroVec(mrpResidualVolPtr);
  return 0;
}


int
LgFrSceSmartExploderDecorator::mrpExecVol(const std::string& operation, float ** mrpExecVolPtr)
{
  assert(state() >= 2);
  witBoolean opExists, partExists;;
  witGetOperationExists(localWitRun_, operation.c_str(), &opExists);
  if (opExists) {
    witGetOperationExecVol(localWitRun_, operation.c_str(), mrpExecVolPtr);
    witGetPartExists(localWitRun_, operation.c_str(), &partExists);
    if (partExists) {
      LgFrScePartAppData * partAppDataPtr;
      witGetPartAppData(localWitRun(), operation.c_str(), (void **) &partAppDataPtr);
      if (partAppDataPtr && partAppDataPtr->isPartPcf()) {
        int t = 0;
        for (t=0; t<partAppDataPtr->unConstrainedStartPeriod(); t++)
          (*mrpExecVolPtr)[t] = 0.0;
      }
    }
  }
  else
    mallocZeroVec(mrpExecVolPtr);
  return 0;
}


int
LgFrSceSmartExploderDecorator::subUsageVol(  const std::string& operation,
                                             const int bomIndex,
                                             const int subBomIndex,
                                             float ** subUsageVolPtr)
{
  assert(state() >= 2);
  witBoolean exists;
  witGetOperationExists(localWitRun_, operation.c_str(), &exists);
  if (exists) {
    // do some sanity checking to ensure that the bom index and subbom index
    // are within reason.  One might even test to see that the child and sub
    // parts are the same.  This is, in general, a FINISH_ME to put in better
    // checks and/or do more testing.
    int nBom;
    witGetOperationNBomEntries(localWitRun_, operation.c_str(), &nBom);
    assert(nBom >= bomIndex);
    int nSubBoms;
    witGetBomEntryNSubsBomEntries(localWitRun_, operation.c_str(), bomIndex, &nSubBoms);
    assert(nSubBoms >= subBomIndex);
    witGetSubsBomEntrySubVol(localWitRun_, operation.c_str(), bomIndex, subBomIndex, subUsageVolPtr);
  }
  else
    mallocZeroVec(subUsageVolPtr);
  return 0;

}



void
LgFrSceSmartExploderDecorator::print(std::string& filename)
{

  std::ofstream outFile(filename.c_str());//, ios::out);

  int nParts;
  char ** partList;
  witGetParts(theWitRun(), &nParts, &partList);

  int i = 0;
  for (i=0; i<nParts; i++) {
    char * thePart = partList[i];
    witAttr category;
    witGetPartCategory(theWitRun(), thePart, &category);

    float * mrpConsVol;
    this->mrpConsVol(thePart, &mrpConsVol);
    outFile << "\"" << thePart << "\", mrpConsVol\" ";
    int t = 0;
    for (t=0; t<nPeriods(); t++)
      outFile << "," << mrpConsVol[t];
    outFile << "\n";
    witFree(mrpConsVol);

#if 0
    float * mrpProdVol;
    this->mrpProdVol(thePart, &mrpProdVol);
    outFile << "\"" << thePart << "\", mrpProdVol\" ";
    for (t=0; t<nPeriods(); t++)
      outFile << "," << mrpProdVol[t];
    outFile << "\n";
    witFree(mrpProdVol);
#endif


    float * mrpExcessVol;
    this->mrpExcessVol(thePart, &mrpExcessVol);
    outFile << "\"" << thePart << "\", mrpExcessVol\" ";
    for (t=0; t<nPeriods(); t++)
      outFile << "," << mrpExcessVol[t];
    outFile << "\n";
    witFree(mrpExcessVol);


    float * mrpResidualVol;
    this->mrpResidualVol(thePart, &mrpResidualVol);
    outFile << "\"" << thePart << "\", mrpResidualVol\" ";
    for (t=0; t<nPeriods(); t++)
      outFile << "," << mrpResidualVol[t];
    outFile << "\n";
    witFree(mrpResidualVol);


    float * reqVol;
    this->reqVol(thePart, &reqVol);
    outFile << "\"" << thePart << "\", reqVol\" ";
    for (t=0; t<nPeriods(); t++)
      outFile << "," << reqVol[t];
    outFile << "\n";
    witFree(reqVol);

  }
  outFile.close();

  for (i=0; i<nParts; i++)
    witFree(partList[i]);
  witFree(partList);

}



// + ================================================================
// + LgFrSceFssableExploderDecorator: Implementation
// + ================================================================


void
LgFrSceFssableExploderDecorator::setAllSupplyToZero()
{
  int nPeriods;
  witGetNPeriods(localWitRun_, &nPeriods);

  int nParts;
  char ** partList;
  witGetParts(localWitRun_, &nParts, &partList);
  float * zeroVec = new float[nPeriods];
  int t = 0;
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0;

  int i = 0;
  for (i=0; i<nParts; i++) {
    witSetPartSupplyVol(localWitRun_, partList[i], zeroVec);
    witFree(partList[i]);
  }

  witFree(partList);
  delete [] zeroVec;
}

std::string
LgFrSceFssableExploderDecorator::title()
{
  std::string myTitle(myExploder_->title());
  myTitle.insert(0,"Fssable ");
  return myTitle;
}


void
LgFrSceFssableExploderDecorator::setAllDemandToZero()
{
  int nPeriods;
  witGetNPeriods(localWitRun_, &nPeriods);

  int nParts;
  char ** partList;
  witGetParts(localWitRun_, &nParts, &partList);
  float * zeroVec = new float[nPeriods];
  int t = 0;
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0;

  int i = 0;
  for (i=0; i<nParts; i++) {
    int nDemands;
    char ** dlist;
    witGetPartDemands(localWitRun_, partList[i], &nDemands, &dlist);
    int j = 0;
    for (j=0; j<nDemands; j++) {
      witSetDemandDemandVol(localWitRun_, partList[i], dlist[j], zeroVec);
      witFree(dlist[j]);
    }
    witFree(dlist);
    witFree(partList[i]);
  }
  witFree(partList);
  delete [] zeroVec;
}

  // preferred constructor
LgFrSceFssableExploderDecorator::LgFrSceFssableExploderDecorator(LgFrSceBaseExploder * myExploder)
  : LgFrSceBaseExploderDecorator(myExploder),
    id_ (__LGFRSCEFSSABLEEXPLODERDECORATOR)
{
  // nothing to do
}



  // Return Exploder Identifier
isAReturnType
LgFrSceFssableExploderDecorator::isA()
const
{
  return id_;
}





  // destructor
LgFrSceFssableExploderDecorator::~LgFrSceFssableExploderDecorator()
{
  // nothing to do
}








// set supplyVols and demandVols to the current
// setting in originalWitRun_.  this is needed for
// FSS users.
void
LgFrSceFssableExploderDecorator::synchronize()
{

  // must be in initialized state
  assert(state_ > 0);

  int nParts;
  char ** partList;
  witGetParts(localWitRun_, &nParts, &partList);

  int i = 0;
  for (i=0; i<nParts; i++)  {
    char * thePart = partList[i];
    float * supplyVol;
    witGetPartSupplyVol(originalWitRun_, thePart, &supplyVol);
    witSetPartSupplyVol(localWitRun_, thePart, supplyVol);
    witFree(supplyVol);
    int nDemands;
    char ** demandList;
    witGetPartDemands(originalWitRun_, thePart, &nDemands, &demandList);
    int j = 0;
    for (j=0; j<nDemands; j++)   {
      float * demandVol;
      witGetDemandDemandVol(originalWitRun_, thePart, demandList[j], &demandVol);
      witSetDemandDemandVol(localWitRun_, thePart, demandList[j], demandVol);
      witFree(demandVol);
      witFree(demandList[j]);
    }
    witFree(demandList);
  }

  for (i=0; i<nParts; i++)
    witFree(partList[i]);
  witFree(partList);

}



int
LgFrSceFssableExploderDecorator::explode()
{
  //  this->synchronize();
  // FINISH_ME: check to see when we need to synchronize!!
  return myExploder_->explode();
}



#ifdef NDEBUG
#undef NDEBUG
#endif

