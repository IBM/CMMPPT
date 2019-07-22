
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
// file: dDriver.C
// author:  tom


#include <stdlib.h>
//#include <fstream.h>
#include <assert.h>
#include <mcl/src/mcl.h>
#include <scenario/src/machdep.h>
#include <scenario/src/timeVecs.h>
#include <sce/src/sceDefin.h>
#include <sce/src/mPlantH.h>
#include <sce/src/dDriver.h>
#include <sce/src/appData.h>
#include <sce/src/sceParam.h>
#include <wit/src/wit.h>


// default constructor
LgFrSceDemandDriver::LgFrSceDemandDriver()
  :   theWitRun_ (0),
      mpHelper_(0),
      nParts_(0),
      incidenceMatrix_(0),
      state_(0)
{
    // all the work is done in initializer
  assert("Cant do this:" ==  "INVOKE the default constructor");
}


// fairly general constructor:
LgFrSceDemandDriver::LgFrSceDemandDriver(
                                         WitRun * const theWitRun, 
                                         LgFrMultiPlantHelper * mpHelper)
  :   theWitRun_ (theWitRun),
      mpHelper_(mpHelper),
      nParts_(0),
      incidenceMatrix_(0),
      state_(0)
{
  int nParts;
  char ** partList;

  witGetParts(theWitRun, &nParts, &partList);
  nParts_ = nParts;
  incidenceMatrix_ = (float **) malloc(nParts * sizeof(float *));
  
  // initialize the matrix to 0 pointers
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    incidenceMatrix_[i] = 0;
    witFree(partList[i]);
  }

  witFree(partList);
}


//
// NOTE:  This method has also been implemented in ipSce.C.  It is invoked from
// there because we need to adjust the sub Exp and Net allowed's BEFORE, we
// set up our part indices.  This is messy, but given the schedule, we don't
// have time to re-design the dDriver.
// Turn ON or OFF the subNetAllowed flag of every REAL and DUMMY sub arc or every 
// REAL and DUMMY operation
// NOTE:  
// FINISH_ME: If we ever introduce sub arcs that need to set NetAllowed=false, then this will
// screw it up and we need to use appData!!  
void
LgFrSceDemandDriver::setSubsNetAllowed(bool subNetAllowed, LgFrMultiPlantHelper & mpHelper)
{
  int nOperations;
  char ** opList;

  witGetOperations(theWitRun_, &nOperations, &opList);
  int o = 0; // Pulled out of the for below by RW2STL
  for (o=0; o<nOperations; o++) {
    // only deal with operations that have a corresponding REAL part
    if ( ! mpHelper.isPartNormal(theWitRun_, opList[o])) {
      witFree(opList[o]);
      continue;
    }
    int nBoms;
    witGetOperationNBomEntries(theWitRun_, opList[o], &nBoms);
    int b = 0; // Pulled out of the for below by RW2STL
    for (b=0; b<nBoms; b++) {
      int nSubBoms;
      witGetBomEntryNSubsBomEntries(theWitRun_, opList[o], b, &nSubBoms);
      int s = 0; // Pulled out of the for below by RW2STL
      for (s=0; s<nSubBoms; s++) {
	char * sub;
	witGetSubsBomEntryConsumedPart(theWitRun_, opList[o], b, s, &sub);
	if ( mpHelper.isPartNormal(theWitRun_, sub)) {
	  witSetSubsBomEntryNetAllowed(theWitRun_, opList[o], b, s, subNetAllowed);
	  witSetSubsBomEntryExpAllowed(theWitRun_, opList[o], b, s, subNetAllowed);
	}
	witFree(sub);
      }
    }
    witFree(opList[o]);
  }
  witFree(opList);
}

      




// compute the demand drivers table
void
LgFrSceDemandDriver::compute()
{


  // the incidenceMatrix:
  //              incidenceMatrix[i][k] = 2.0 if k is not used in i
  //              incidenceMatrix[i][k] = 1.0 if k is used in i as optional feature
  //              incidenceMatrix[i][k] = 0.0 if k is used in i as mandatory feature
  //              incidenceMatrix[i][k] < 0.0 if k is used in i as customerChoice feature
  int nParts;
  char ** partList;
  int nPeriods;

  witGetNPeriods(theWitRun_, &nPeriods);
  witGetParts(theWitRun_, &nParts, &partList);


  assert(nParts == nParts_);

   
  // initialize the matrix to 0 pointers
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    incidenceMatrix_[i] = 0;
  }



  // Make TWO passes.  ONce ot pick up the GPD demands and the second one to pick up the 
  // the independent demands
      
  // FIST PASS .... GPD only ....
  for ( i=0; i<nParts; i++) {
    std::string pp, geo;
    
    // only work on geo planner demand parts
    if (! mpHelper_->isPartSpecialGeoPlannerDemandPart(theWitRun_, partList[i], pp, geo))
      continue;

      


    std::string geoPlannerDemandPartName(partList[i]);

    std::cout << "dDriver: GPD: " << geoPlannerDemandPartName << std::endl;

    // compute a default incidence vec for this GPD.  It should be 0
    // to begin and we set it all to 2.0 
    assert(incidenceMatrix_[i] == 0);
    incidenceMatrix_[i] = (float *) malloc(nParts * sizeof(float));
    //    witMalloc(nPeriods_ * sizeof(float),(void**)&zeroVec);
    int k = 0; // Pulled out of the for below by RW2STL
    for (k=0; k<nParts; k++) {
      incidenceMatrix_[i][k] = 2.0;
    }

    // check for special case: GPD with no demands
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
    int jj = 0; // Pulled out of the for below by RW2STL
    for (jj=0; jj<nDemands; jj++) {
      witFree(dList[jj]);
    }
    witFree(dList);
      
    if (nDemands == 0) {
      continue;
    }
      


    // now loop over the children of the GPD if the child is a feature part, 
    // then we need to pick up the maxWithout vector.
    int nBom;
    witGetOperationNBomEntries(theWitRun_, geoPlannerDemandPartName.c_str(), &nBom);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<nBom; j++) {
      float minMwoScaled = 0.0;
      char * child;
      witGetBomEntryConsumedPart(theWitRun_, geoPlannerDemandPartName.c_str(), j, &child);
      std::string theChild(child);
      std::string ggg, bbc,pptlb;

      // anything other than normal or special Feature part gets skipped      
      if ((! mpHelper_->isPartNormal(theWitRun_, child)) 
	  && (! mpHelper_->isPartSpecialFeature(theWitRun_, theChild))
	  && (! mpHelper_->isPartSpecialBbCategory(theWitRun_, theChild,pptlb,ggg,bbc))) {
	witFree(child);
	continue;
      }

      if (mpHelper_->isPartSpecialFeature(theWitRun_, theChild)) {
	// if its customer choice, then minMwoScaled = 0.0
	// ow, figure it out.
	if (mpHelper_->isPartSpecialCustChoiceFeature(theWitRun_, theChild)) {
	  minMwoScaled = 0.0;
	}

	// otherwise, we need to go find the 0 SUB to find out what type of feature
	// it is:  
	else {
	  int nSubBoms;
	  witGetBomEntryNSubsBomEntries(theWitRun_, geoPlannerDemandPartName.c_str(), j, &nSubBoms);
	  assert(nSubBoms == 1);
	  char * nullSub;
	  witGetSubsBomEntryConsumedPart(theWitRun_, geoPlannerDemandPartName.c_str(), j, 0, &nullSub);
	  std::string pdf;
	  // if its a globalNullSub, then mwo=1
	  if (mpHelper_->isPartSpecialGlobalNullSubstitute(theWitRun_, nullSub, pdf))
	    minMwoScaled = 1.0;
	  // if its a globalMWONullSub, then mwo=0
	  else if (mpHelper_->isPartSpecialGlobalMaxWithoutNullSubstitute(theWitRun_, nullSub, pdf))
	    minMwoScaled = 0.0;
	  // otherwise, we need to go find the maxWithout vector and find the min
	  else {
	    LgFrScePartAppData * appDataPtr;
	    witGetPartAppData(theWitRun_, theChild.c_str(), (void **) &appDataPtr);
	    assert(appDataPtr != 0);
	   
	    LgFrTimeVecFloat maxWithout(appDataPtr->maxWithout());
	    minMwoScaled = maxWithout.min();
	  }
	  witFree(nullSub);
	}
      }



      // if you got here, then its bonafide child
      int childPartIndex = mpHelper_->witPartIndex(theWitRun_, child);
      assert(childPartIndex >= 0);

      // get the belowlist for this part and set the incidence
      // vector[i] to minMwo if i in belowList, +2 if not.

      // if the incidence matrix has not yet been populated, then go
      // compute it from the below list.
      if (incidenceMatrix_[childPartIndex] == 0)
	this->getDDincidenceVector(child,  minMwoScaled, childPartIndex);

      for ( k=0; k<nParts; k++) {
	if (incidenceMatrix_[i][k] > incidenceMatrix_[childPartIndex][k])
	  incidenceMatrix_[i][k] = incidenceMatrix_[childPartIndex][k];
      }

      witFree(child);
      // temp fix from Tom ... 3.101
      free(incidenceMatrix_[childPartIndex]);
      incidenceMatrix_[childPartIndex] = 0;
    }
    
  }




  // Second Pass Independent Demands ONLY ...
  for ( i=0; i<nParts; i++) {

    std::string indepDemPart(partList[i]);

    // only operate on NORMAL parts
    if (! mpHelper_->isPartNormal(theWitRun_, indepDemPart))
      continue;

    // only want part with one or more demands
    int nDemands;
    char ** dList;
    witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
    int jj = 0; // Pulled out of the for below by RW2STL
    for (jj=0; jj<nDemands; jj++) {
      witFree(dList[jj]);
    }
    witFree(dList);
      
    if (nDemands == 0) {
      continue;
    }



    // compute a default incidence vec for this indepDemPart.  
    // BUG FIX: 20010813,  The assert for incidenceMatrix_[i] == nULL was failing.
    //                     This is because the part i is not a GPD and it may (most likely)
    //                     will have been already computed.  DUH!!

    if (incidenceMatrix_[i] == 0) {
      // if the incidence matrix has not yet been populated, then go
      // compute it from the below list.
      this->getDDincidenceVector(partList[i],  0.0, i);
    }
    
  }



  for ( i=0; i<nParts; i++) {
    witFree(partList[i]);
  }
  witFree(partList);

  state_ = 1;
  
}


// print the demand drivers table
void
LgFrSceDemandDriver::getDDincidenceVector(char *  thePart, 
					  float minMwoScaled,
					  int childPartIndex)
{
  char ** belowList;
  int nBelowParts;


  witGetPartBelowList(theWitRun_, thePart, &nBelowParts, &belowList);

  
  float * incidenceVec = (float *) malloc(nParts_ * sizeof(float));
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts_; i++) {
    incidenceVec[i] = 2.0;
  }

  for (i=0; i<nBelowParts; i++) {
    int partIndex = mpHelper_->witPartIndex(theWitRun_, belowList[i]);
    assert(partIndex >= 0);
    incidenceVec[partIndex] = minMwoScaled;
    witFree(belowList[i]);
  }

  witFree(belowList);

  incidenceMatrix_[childPartIndex] = incidenceVec;  
				   
}


// print the demand drivers table
void
LgFrSceDemandDriver::print(std::string fileName, MclFacility * theErrFacil)
{
  assert(state_);

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*theErrFacil)("CannotOpenFileForWrite",MclArgList() 
		       << "Demand Drivers" 
		       << fileName);
  }

  long numRecords = 0;  // keep track of number of records



  int nParts;
  char ** partList;

  witGetParts(theWitRun_, &nParts, &partList);
  assert(nParts_ == nParts);

  std::string pp, geo;
    
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    // If it's a  geo planner demand parts
    if (mpHelper_->isPartSpecialGeoPlannerDemandPart(theWitRun_, partList[i], pp, geo)) {

  
      std::string geoPlannerDemandPartName(partList[i]);
      
      assert(incidenceMatrix_[i] != 0);
      
      int k = 0; // Pulled out of the for below by RW2STL
      for (k=0; k<nParts; k++) {
	if (incidenceMatrix_[i][k] > 1.0)
	  continue;
	if (! mpHelper_->isPartNormal(theWitRun_, partList[k]))
	  continue;
	
	// ok, print a record
	std::string partname = mpHelper_->partname(partList[k]);
	std::string pdf = mpHelper_->pdf(partList[k]);
	outFile << "\"" 
		<< pp << "\",\"" 
		<< geo << "\",\""
		<< partname << "\",\"" 
		<< pdf << "\","
		<< incidenceMatrix_[i][k] << "\n";
	numRecords++;
	
      }
    }
    else if (mpHelper_->isPartNormal(theWitRun_, partList[i])) {
      // see if it has any independent Demands
      // only want part with one or more demands
      int nDemands;
      char ** dList;
      witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
      int jj = 0; // Pulled out of the for below by RW2STL
      for (jj=0; jj<nDemands; jj++) {
	witFree(dList[jj]);
      }
      witFree(dList);
      
      if (nDemands == 0) {
	continue;
      }
      
      assert(incidenceMatrix_[i] != 0);
      
      int k = 0; // Pulled out of the for below by RW2STL
      for (k=0; k<nParts; k++) {
	if (incidenceMatrix_[i][k] > 1.0)
	  continue;
	if (! mpHelper_->isPartNormal(theWitRun_, partList[k]))
	  continue;
	
	// ok, print a record
	std::string pp = mpHelper_->partname(partList[i]);
	std::string geo = mpHelper_->pdf(partList[i]);
	std::string partname = mpHelper_->partname(partList[k]);
	std::string pdf = mpHelper_->pdf(partList[k]);
	outFile << "\"" 
		<< pp << "\",\"" 
		<< geo << "\",\""
		<< partname << "\",\"" 
		<< pdf << "\","
		<< incidenceMatrix_[i][k] << "\n";
	numRecords++;
	
      }

    }


  }

  // now clean up 
  for (i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree(partList);

  outFile.close();
}



// a more memory efficent method
void
LgFrSceDemandDriver::computeAndPrint(std::string fileName, MclFacility * theErrFacil)
{



  // the incidenceMatrix:
  //              incidenceMatrix[i][k] = 2.0 if k is not used in i
  //              incidenceMatrix[i][k] = 1.0 if k is used in i as optional feature
  //              incidenceMatrix[i][k] = 0.0 if k is used in i as mandatory feature
  //              incidenceMatrix[i][k] < 0.0 if k is used in i as customerChoice feature

  int nPeriods;
  witGetNPeriods(theWitRun_, &nPeriods);

  std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
  if (outFile.fail()) {
    (*theErrFacil)("CannotOpenFileForWrite",MclArgList() 
		       << "Demand Drivers" 
		       << fileName);
  }

  long numRecords = 0;  // keep track of number of records



  int nParts;
  char ** partList;

  witGetParts(theWitRun_, &nParts, &partList);
  assert(nParts_ == nParts);

  // initialize the matrix to 0 pointers
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    incidenceMatrix_[i] = 0;
  }


  std::string pp, geo;
    
  for (i=0; i<nParts; i++) {
    // If it's a  geo planner demand parts
    if (mpHelper_->isPartSpecialGeoPlannerDemandPart(theWitRun_, partList[i], pp, geo)) {


      // +++++++++++++++++++++++++++++++++++

      std::string geoPlannerDemandPartName(partList[i]);

      // compute a default incidence vec for this GPD.  It should be 0
      // to begin and we set it all to 2.0 
      assert(incidenceMatrix_[i] == 0);
      incidenceMatrix_[i] = (float *) malloc(nParts * sizeof(float));
      int k = 0; // Pulled out of the for below by RW2STL
      for (k=0; k<nParts; k++) {
	incidenceMatrix_[i][k] = 2.0;
      }

      // check for special case: GPD with no demands
      int nDemands;
      char ** dList;
      witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
      int jj = 0; // Pulled out of the for below by RW2STL
      for (jj=0; jj<nDemands; jj++) {
	witFree(dList[jj]);
      }
      witFree(dList);
      
      if (nDemands == 0) {
	free(incidenceMatrix_[i]);
	incidenceMatrix_[i] = 0;
	continue;
      }
      


      // now loop over the children of the GPD if the child is a feature part, 
      // then we need to pick up the maxWithout vector.
      int nBom;
      witGetOperationNBomEntries(theWitRun_, geoPlannerDemandPartName.c_str(), &nBom);
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<nBom; j++) {
	float minMwoScaled = 0.0;
	char * child;
	witGetBomEntryConsumedPart(theWitRun_, geoPlannerDemandPartName.c_str(), j, &child);
	std::string theChild(child);
	std::string ggg, bbc,pptlb;
	
	// anything other than normal or special Feature part gets skipped      
	if ((! mpHelper_->isPartNormal(theWitRun_, child)) 
	    && (! mpHelper_->isPartSpecialFeature(theWitRun_, theChild))
	    && (! mpHelper_->isPartSpecialBbCategory(theWitRun_, theChild,pptlb,ggg,bbc))) {
	  witFree(child);
	  continue;
	}

	
	if (mpHelper_->isPartSpecialFeature(theWitRun_, theChild)) {
	  // if its customer choice, then minMwoScaled = 0.0
	  // ow, figure it out.
	  if (mpHelper_->isPartSpecialCustChoiceFeature(theWitRun_, theChild)) {
	    minMwoScaled = 0.0;
	  }
	  
	  // otherwise, we need to go find the 0 SUB to find out what type of feature
	  // it is:  
	  else {
	    int nSubBoms;
	    witGetBomEntryNSubsBomEntries(theWitRun_, geoPlannerDemandPartName.c_str(), j, &nSubBoms);
	    assert(nSubBoms == 1);
	    char * nullSub;
	    witGetSubsBomEntryConsumedPart(theWitRun_, geoPlannerDemandPartName.c_str(), j, 0, &nullSub);
	    std::string pdf;
	    // if its a globalNullSub, then mwo=1
	    if (mpHelper_->isPartSpecialGlobalNullSubstitute(theWitRun_, nullSub, pdf))
	      minMwoScaled = 1.0;
	    // if its a globalMWONullSub, then mwo=0
	    else if (mpHelper_->isPartSpecialGlobalMaxWithoutNullSubstitute(theWitRun_, nullSub, pdf))
	      minMwoScaled = 0.0;
	    // otherwise, we need to go find the maxWithout vector and find the min
	    else {
	      LgFrScePartAppData * appDataPtr;
	      witGetPartAppData(theWitRun_, theChild.c_str(), (void **) &appDataPtr);
	      assert(appDataPtr != 0);
	      
	      LgFrTimeVecFloat maxWithout(appDataPtr->maxWithout());
	      minMwoScaled = maxWithout.min();
	    }
	    witFree(nullSub);
	  }
	}
	
	
	
	// if you got here, then its bonafide child
	int childPartIndex = mpHelper_->witPartIndex(theWitRun_, child);
	assert(childPartIndex >= 0);

	// get the belowlist for this part and set the incidence
	// vector[i] to minMwo if i in belowList, +2 if not.
	
	// if the incidence matrix has not yet been populated, then go
	// compute it from the below list.
	if (incidenceMatrix_[childPartIndex] == 0)
	  this->getDDincidenceVector(child,  minMwoScaled, childPartIndex);
	
	for ( k=0; k<nParts; k++) {
	  if (incidenceMatrix_[i][k] > incidenceMatrix_[childPartIndex][k])
	    incidenceMatrix_[i][k] = incidenceMatrix_[childPartIndex][k];
	}
	
	witFree(child);
	// temp fix from Tom ... 3.101
	free(incidenceMatrix_[childPartIndex]);
	incidenceMatrix_[childPartIndex] = 0;
      }
    


      // +++++++++++++++++++++++++++++++++++
      assert(incidenceMatrix_[i] != 0);
      
      for (k=0; k<nParts; k++) {
	if (incidenceMatrix_[i][k] > 1.0)
	  continue;
	if (! mpHelper_->isPartNormal(theWitRun_, partList[k]))
	  continue;
	
	// ok, print a record
	std::string partname = mpHelper_->partname(partList[k]);
	std::string pdf = mpHelper_->pdf(partList[k]);
	outFile << "\"" 
		<< pp << "\",\"" 
		<< geo << "\",\""
		<< partname << "\",\"" 
		<< pdf << "\","
		<< incidenceMatrix_[i][k] << "\n";
	numRecords++;
	
      }

      // clear the memory
      free(incidenceMatrix_[i]);
      incidenceMatrix_[i] = 0;
    }






    else if (mpHelper_->isPartNormal(theWitRun_, partList[i])) {
      // see if it has any independent Demands
      // only want part with one or more demands
      int nDemands;
      char ** dList;
      witGetPartDemands(theWitRun_, partList[i], &nDemands, &dList);
      int jj = 0; // Pulled out of the for below by RW2STL
      for (jj=0; jj<nDemands; jj++) {
	witFree(dList[jj]);
      }
      witFree(dList);
      
      if (nDemands == 0) {
	continue;
      }

      // +++++++++++++++++++++++++++
      // compute the incidence vec for this indepDemPart.  
      if (incidenceMatrix_[i] == 0) {
	// if the incidence matrix has not yet been populated, then go
	// compute it from the below list.
	this->getDDincidenceVector(partList[i],  0.0, i);
      }
      // +++++++++++++++++++++++++++

      assert(incidenceMatrix_[i] != 0);
      
      int k = 0; // Pulled out of the for below by RW2STL
      for (k=0; k<nParts; k++) {
	if (incidenceMatrix_[i][k] > 1.0)
	  continue;
	if (! mpHelper_->isPartNormal(theWitRun_, partList[k]))
	  continue;
	
	// ok, print a record
	std::string pp = mpHelper_->partname(partList[i]);
	std::string geo = mpHelper_->pdf(partList[i]);
	std::string partname = mpHelper_->partname(partList[k]);
	std::string pdf = mpHelper_->pdf(partList[k]);
	outFile << "\"" 
		<< pp << "\",\"" 
		<< geo << "\",\""
		<< partname << "\",\"" 
		<< pdf << "\","
		<< incidenceMatrix_[i][k] << "\n";
	numRecords++;
	
      }
      free(incidenceMatrix_[i]);
      incidenceMatrix_[i] = 0;
    }


  }

  // now clean up 
  for (i=0; i<nParts; i++) 
    witFree(partList[i]);
  witFree(partList);

  outFile.close();
}






 
  

// copy constructor 
LgFrSceDemandDriver::LgFrSceDemandDriver(const LgFrSceDemandDriver& source)
  :   
      theWitRun_ (source.theWitRun_),
      mpHelper_(source.mpHelper_),
      nParts_(source.nParts_),
      incidenceMatrix_(source.incidenceMatrix_),
      state_(source.state_)
{
  assert("Cant call this" == "the copy constructor for LgFrSceDemandDriver");
}

// assignment operator
LgFrSceDemandDriver&
LgFrSceDemandDriver::operator=(const LgFrSceDemandDriver& rhs)
{

  assert("Cant call this" == " the assignment operator for LgFrSceDemandDriver");
  if (this != &rhs) {		// Check for assignment to self
    theWitRun_ = rhs.theWitRun_;
    mpHelper_ = rhs.mpHelper_;
    nParts_ = rhs.nParts_;
    incidenceMatrix_ = rhs.incidenceMatrix_,
    state_ = rhs.state_;
  }
  return *this;
}

// destructor
LgFrSceDemandDriver::~LgFrSceDemandDriver()
{

  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts_; i++)
    free(incidenceMatrix_[i]);

  free(incidenceMatrix_);
}

