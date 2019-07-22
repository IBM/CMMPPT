
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
// file: sceAlMrp.C
// author:  dan/tom

#include <mcl/src/mcl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <assert.h>
#include <scenario/src/machdep.h>
#include <scenario/src/timeVecs.h>
#include <sce/src/sceDefin.h>
#include <sce/src/mPlantH.h>
#include <sce/src/sceParam.h>
#include <sce/src/fFileMgr.h>
#include <sce/src/sceAlMrp.h>
#include <wit/src/wit.h>

// default constructor
LgFrSceAllocatedMrp::LgFrSceAllocatedMrp()
  :   WUtlAllocatedMrp(0, 0),
      mpHelper_(0),
      setOfParameters_(0)
{
  assert("Cant do this:" ==  "INVOKE the default constructor");
}


// fairly general constructor:
LgFrSceAllocatedMrp::LgFrSceAllocatedMrp(WitRun * const theWitRun, 
					 LgFrMultiPlantHelper * mpHelper,
					 const char aMrpSeparatorChar,
					 LgFrSceSetOfParameters * setOfParameters)
  :   WUtlAllocatedMrp(theWitRun, aMrpSeparatorChar),
      mpHelper_(mpHelper),
      setOfParameters_(setOfParameters)
{
  // NOTE: the construction must not be done till the original witRun is
  // all loaded up and ready to go

  // nothing to do, really. the base class initialization will do everything
  std::string pdfSeparator = setOfParameters_->getString("pdfSeparator");
  assert(pdfSeparator.c_str()[0] != aMrpSeparatorChar);
}




int
LgFrSceAllocatedMrp::scePrint(std::string& fileName, std::string& fileNameInd)
{
   assert(state() == 1);

      std::ofstream outFileIndDmd;
  
   if (!(fileNameInd.empty())) {
      outFileIndDmd.open(fileNameInd.c_str(), std::ofstream::out );//, 0666);
      if (outFileIndDmd.fail()) {
      std::cerr << "SCE0999E: Could not open output file: " << fileNameInd << std::endl
	   << "while printing allocated MRP results\n"
	   << "terminating the program\n";
      return 8;
      }
   }

   std::ofstream outFile(fileName.c_str());//, ios::out, 0666);
   if (outFile.fail()) {
   std::cerr << "SCE0999E: Could not open output file: " << fileName << std::endl
	 << "while printing allocated MRP results\n"
	 << "terminating the program\n";
   return 8;
   }

   std::string pdfSeparatorString = setOfParameters_->getString("pdfSeparator");
   std::string separatorString(1,separatorChar());

   long numRecords = 0;  // keep track of number of records

   int nPeriods_ = nPeriods();
   int version_ = version();
   WitRun *localWitRun_ = localWitRun();

   char **operationList;
   char *producedPartName, *consumingOperationName;
   int listLen, i, k, l, n, t;
   int nBomEntries, nBopEntries, nConsumingBomEntries, nSubsBomEntries;
   int bomEntryIndex;
   float *execVol, *MRPCoverage, *subVol, *MRPAllocReq, *MRPDelta;

   MRPCoverage = (float *) calloc(nPeriods_, sizeof(float));
   // TRE: only allocate MRPAllocReq for version==2
   if (version_ == 2) {
     MRPAllocReq = (float *) calloc(nPeriods_, sizeof(float));
   }
   MRPDelta = (float *) calloc(nPeriods_, sizeof(float));

   std::string geoDemandIndicator(pdfSeparatorString + "specialGeoPlannerDemand");

   witGetOperations(localWitRun_, &listLen, &operationList);
   for(i=0; i<listLen; i++){
      witGetOperationNBomEntries(localWitRun_, operationList[i], &nBomEntries);
      if (nBomEntries == 0){


	   std::string witName(operationList[i]);
	   SCETokenizer next(witName);
	   std::string geo = next(separatorString.c_str());
	   std::string temp1 = next(separatorString.c_str());
	   std::string witLowLevelPartName = next(separatorString.c_str());
	   
	   SCETokenizer next1(temp1);
	   std::string geo2 = next1(pdfSeparatorString);
	   std::string plannerPartName = next1(pdfSeparatorString);
	   
	   SCETokenizer next2(witLowLevelPartName);
	   std::string lowLevelpdf = next2(pdfSeparatorString);
	   std::string lowLevelPartName = next2(pdfSeparatorString);
 
        // if witLowLevelPartName is not a PCF part in the 
        // SCE witRun then continue        
          if (!(mpHelper_->isPartPcf(witRun(), witLowLevelPartName.c_str())))
             continue;

	 if (version_ == 1){
	    witGetOperationExecVol(localWitRun_, operationList[i], &execVol);
	    witGetOperationMrpExecVol(localWitRun_, operationList[i], 
	       &MRPAllocReq);
	    for(t=0; t<nPeriods_; t++){
	       MRPCoverage[t] = MRPAllocReq[t] - execVol[t];
	       MRPDelta[t] = execVol[t];
	    }
	 }
	 
	 if (version_ == 2){
	    for(t=0; t<nPeriods_; t++){
	       MRPCoverage[t] = 0;
	       MRPAllocReq[t] = 0;
	    }
	    witGetOperationNBopEntries(localWitRun_, operationList[i], 
	       &nBopEntries);         
	    assert(nBopEntries == 1);
	    witGetBopEntryProducedPart(localWitRun_, operationList[i], 0, 
	       &producedPartName);
	    witGetPartNConsumingBomEntries(localWitRun_, producedPartName, 
	       &nConsumingBomEntries);
	    for(k=0; k<nConsumingBomEntries; k++){
	       witGetPartConsumingBomEntry(localWitRun_, producedPartName, k,
		  &consumingOperationName, &bomEntryIndex);
	       witGetBomEntryNSubsBomEntries(localWitRun_, 
		  consumingOperationName, bomEntryIndex, &nSubsBomEntries);
	       assert(nSubsBomEntries == nPeriods_);
	       for(l=0; l<nSubsBomEntries; l++){
		  witGetSubsBomEntrySubVol(localWitRun_, consumingOperationName,
		     bomEntryIndex, l, &subVol); 
		  for(n=0; n<nPeriods_; n++){
		     MRPAllocReq[n] += subVol[n];
		     if(n >= l)
			MRPCoverage[n] += subVol[n-l];
		  }
		  portableFree(subVol);
	       }
	       portableFree(consumingOperationName);
	    }
	    portableFree(producedPartName);
	    witGetOperationExecVol(localWitRun_, operationList[i], &execVol);
	    for(t=0; t<nPeriods_; t++)
	       MRPAllocReq[t] += execVol[t];
	    for(t=0; t<nPeriods_; t++)
	       MRPDelta[t] = MRPAllocReq[t] - MRPCoverage[t];
	}


	 // TRE:  Only print if there's a positive mrpDelta
	 LgFrTimeVecFloat mrpDeltaTV((size_t) nPeriods_, MRPDelta);
	 if (mrpDeltaTV.max() > 0.001) {

        // See if part is a specialGeoPlanner part
         if (sce::find(witName, geoDemandIndicator) != std::string::npos) {
	   outFile << "\"" 
		   << plannerPartName << "\",\"" 
		   << geo << "\",\""
		   << lowLevelPartName << "\",\"" 
		   << lowLevelpdf << "\",\""
		   << "AMRPReq\"";
	   for(t=0; t<nPeriods_; t++)
	     outFile << "," <<  MRPAllocReq[t];
	   outFile << "\n";
	   numRecords ++;

	   outFile << "\"" 
		   << plannerPartName << "\",\"" 
		   << geo << "\",\""
		   << lowLevelPartName << "\",\"" 
		   << lowLevelpdf << "\",\""
		   << "AMRPCov\"";
	   for(t=0; t<nPeriods_; t++)
	     outFile << "," <<  MRPCoverage[t];
	   outFile << "\n";
	   numRecords ++;
	   
	   outFile << "\"" 
		   << plannerPartName << "\",\"" 
		   << geo << "\",\""
		   << lowLevelPartName << "\",\"" 
		   << lowLevelpdf << "\",\""
		   << "AMRPDelta\"";
	   for(t=0; t<nPeriods_; t++)
	     outFile << "," <<  MRPDelta[t];
	   outFile << "\n";
	   numRecords ++;
	 }
         else {
               outFileIndDmd << "\"" 
		   << plannerPartName << "\",\"" 
		   << geo2 << "\",\""
		   << geo  << "\",\""
		   << lowLevelPartName << "\",\"" 
		   << lowLevelpdf << "\",\""
		   << "AMRPReq\"";
	   for(t=0; t<nPeriods_; t++)
	     outFileIndDmd << "," <<  MRPAllocReq[t];
	   outFileIndDmd << "\n";
	   numRecords ++;

	   outFileIndDmd << "\"" 
		   << plannerPartName << "\",\"" 
		   << geo2 << "\",\""
		   << geo << "\",\""
		   << lowLevelPartName << "\",\"" 
		   << lowLevelpdf << "\",\""
		   << "AMRPCov\"";
	   for(t=0; t<nPeriods_; t++)
	     outFileIndDmd << "," <<  MRPCoverage[t];
	   outFileIndDmd << "\n";
	   numRecords ++;
	   
	   outFileIndDmd << "\"" 
		   << plannerPartName << "\",\"" 
		   << geo2 << "\",\""
		   << geo << "\",\""
		   << lowLevelPartName << "\",\"" 
		   << lowLevelpdf << "\",\""
		   << "AMRPDelta\"";
	   for(t=0; t<nPeriods_; t++)
	     outFileIndDmd << "," <<  MRPDelta[t];
	   outFileIndDmd << "\n";
	   numRecords ++;
         }
         }
	 
	 // TRE: moved execVolfree into appropriate loop
	 portableFree(execVol);
	 // TRE: 
	 if (version_ == 1)
	   portableFree(MRPAllocReq);
      }
   }   

   outFile.close();
   if (!(fileNameInd.empty()))
      outFileIndDmd.close();

   portableFree(MRPCoverage);
   portableFree(MRPDelta);
   // TRE: 
   if (version_ == 2)
     portableFree(MRPAllocReq);
   for(i=0; i<listLen; i++) 
      portableFree(operationList[i]);
   portableFree(operationList);

  // if successful, return 0
  // if warnings,   return 4
  // if error,      return 8
  return 0;
}


// This method is NOT IMPLEMENTED.  Do not use it.  It is a private method!!
// use print() instead.
// print the allocated MRP
// this would be the preferred implementation of print().  It assumes
// that the base class has get methods for the solution.  However, 
// since no get method exists, the method is not completely implemented.
int
LgFrSceAllocatedMrp::printWithoutAccessToAMrpGuts(std::string fileName, 
						  MclFacility * theErrFacil,
						  LgFrSceFlatFileMgr * fFileMgr)
{
  assert(this->state() == 1);

  std::ofstream outFile(fileName.c_str());//, ios::out , 0666);
  if (outFile.fail()) {
    (*theErrFacil)("CannotOpenFileForWrite",MclArgList() 
		       << "allocated MRP" 
		       << fileName);
  }

  long numRecords = 0;  // keep track of number of records


  int nPeriods;
  int nParts;
  char ** partList;

  witGetNPeriods(this->witRun(), &nPeriods);
  witGetParts(this->localWitRun(), &nParts, &partList);

  std::string plannerPartName, geo;

  // loop through all the parts and pick up the GPD parts only
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nParts; i++) {
    // only work on geo planner demand parts
    if (! mpHelper_->isPartSpecialGeoPlannerDemandPart(this->witRun(), partList[i], plannerPartName, geo))
      continue;
  
    // somehow, you need to loop over the PCF parts
    // For each PCF part ...
    while (1) {
      // need to define the interface for get methods ... FINISH_ME:
      char * thePcfFullWitPartName = 0;
      float * theReqCovOrDeltaDataLine = 0;
      
      // ok, print a record
      std::string partname = mpHelper_->partname(thePcfFullWitPartName);
      std::string pdf = mpHelper_->pdf(thePcfFullWitPartName);
      outFile << "\"" 
	      << plannerPartName << "\",\"" 
	      << geo << "\",\""
	      << partname << "\",\"" 
	      << pdf << "\",\""
	      << "AMRPReq, or AMRPCov, or AMRPDelta\",";
      fFileMgr->writeCsvFloatVector(outFile, nPeriods, theReqCovOrDeltaDataLine, 0, 15);
      numRecords++;
    }
  }
  outFile.close();

  // if successful, return 0
  // if warnings,   return 4
  // if error,      return 8
  return 0;

}
 
  

// copy constructor 
LgFrSceAllocatedMrp::LgFrSceAllocatedMrp(const LgFrSceAllocatedMrp& source)
  :   WUtlAllocatedMrp(0, 0),
      mpHelper_(source.mpHelper_),
      setOfParameters_(source.setOfParameters_)
{
  assert("Cant call this" == "the copy constructor for LgFrSceAllocatedMrp");
}

// assignment operator
LgFrSceAllocatedMrp&
LgFrSceAllocatedMrp::operator=(const LgFrSceAllocatedMrp& rhs)
{

  assert("Cant call this" == " the assignment operator for LgFrSceAllocatedMrp");
  if (this != &rhs) {           // Check for assignment to self
    (this->LgFrSceAllocatedMrp::operator= ) (rhs);    
    mpHelper_ = rhs.mpHelper_;
    setOfParameters_ = rhs.setOfParameters_;
  }
  return *this;
}

// destructor
LgFrSceAllocatedMrp::~LgFrSceAllocatedMrp()
{
  // do nothing
}

