// file: sceAlMrp.C
// author:  dan/tom

#include <stdio.h>
//#include <string>
//#include <rw/ctoken.h>
#include <stdlib.h>
//#include <fstream.h>
#include <assert.h>
#include <wit/src/wit.h>
#include <witutil/src/machdep.h> 
#include <witutil/src/allocMrp.h>

#include <string>

void doAllocatedMRP(WitRun *, char, char, char *);
void addStructureBelowPart(WitRun *, WitRun *, std::string, std::string, 
   std::string, int);
void setupOperation(WitRun *, WitRun *, std::string, std::string);
void setTheBopData(WitRun *, WitRun *, std::string, std::string, std::string, 
   std::string, int, int);
void addStructureBelowOperation(WitRun *, WitRun*, std::string, std::string, 
   std::string, int);
void setupPart(WitRun *, WitRun *, std::string, std::string);
void setTheBomData(WitRun *, WitRun*, std::string, std::string, std::string, 
   std::string, int, int, int);
void setTheDemand(WitRun *, WitRun *, std::string, std::string, std::string);
void performMRP(WitRun *);


// default constructor
WUtlAllocatedMrp::WUtlAllocatedMrp()
  :   theWitRun_ (NULL),
      localWitRun_(NULL),
      separatorChar_(NULL),
      state_(0),
      version_(0),
      nPeriods_(0)
{
  assert("Can't do this:" ==  "INVOKE the default constructor");
}


// fairly general constructor:
WUtlAllocatedMrp::WUtlAllocatedMrp(WitRun * const theWitRun, const char separatorChar)
  :   theWitRun_ (theWitRun),
      localWitRun_(NULL),
      separatorChar_(separatorChar),
      state_(0),
      version_(0),
      nPeriods_(0)
{
}


// compute the allocated MRP, but don't print yet
int
WUtlAllocatedMrp::computeAndPopulate(const int version)
{
   int equitability;
   char **partList, **demandList;
   int partListLen, demandListLen;
   witAttr partCategory;
   float *supplyVol, *demandVol, *hardLower, *hardUpper, *softLower;
   int *priority, shipLateLimit, buildAheadLimit;
   int i,j; 
   witBoolean exists;

   version_ = version;
   if (version_ != 1 && version_ != 2){
      printf("Allocated MRP only supports two versions: 1 and 2.\n");
      printf("You entered version %d, which is not supported.\n", version_);
      return(8);
   }

   witNewRun (& localWitRun_);
   witSetMesgFileAccessMode (localWitRun_, WitTRUE, "w");
   witSetMesgFileName (localWitRun_, WitTRUE, "sceAMRP.msg");

   // shut off ALL info messages
   witSetMesgTimesPrint(localWitRun_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
   // Let ANY warning message come out once
   witSetMesgTimesPrint(localWitRun_, WitTRUE, WitWARNING_MESSAGES, 1);    
   // Suppress a few WARNING messages that we already know about
   witSetMesgTimesPrint(localWitRun_, WitTRUE, 338, 0);
   witSetMesgTimesPrint(localWitRun_, WitTRUE, 446, 0);
   witSetMesgTimesPrint(localWitRun_, WitTRUE, 538, 0);        
   witSetMesgTimesPrint(localWitRun_, WitTRUE, 505, 0);        
   
   witInitialize (localWitRun_);
   
   witSetIndependentOffsets(localWitRun_, WitTRUE);

   witGetNPeriods(theWitRun_, &nPeriods_);
   witSetNPeriods(localWitRun_, nPeriods_);
 
   witGetEquitability(theWitRun_, &equitability);
   witSetEquitability(localWitRun_, equitability);

   witSetTruncOffsets(localWitRun_, WitTRUE);

   // get all the parts from theWitRun 
   // and create equivalent parts in localWitRun;
   // copy over the supply volume for the parts in theWitRun
   witGetParts(theWitRun_, &partListLen, &partList);
   for (i=0; i<partListLen; i++){
      witGetPartCategory(theWitRun_, partList[i], &partCategory);
      // For Wit3.4 compatibility, must check part type
      if (partCategory == WitPRODUCT || partCategory == WitRAW) 
          witAddPart(localWitRun_, partList[i], WitMATERIAL);
      else
          witAddPart(localWitRun_, partList[i], partCategory);
      witGetPartSupplyVol(theWitRun_, partList[i], &supplyVol);
      witSetPartSupplyVol(localWitRun_, partList[i], supplyVol);
      portableFree(supplyVol);
   }

   // For each demand, create a demand specific copy of the demanded part
   for (i=0; i<partListLen; i++){
      witGetPartDemands (theWitRun_, partList[i], &demandListLen, &demandList);
      for (j=0; j<demandListLen; j++){
	std::string prefix(demandList[j]); 
	prefix += separatorChar_; 
	prefix += partList[i];
	prefix += separatorChar_; 
	std::string workingPart(prefix);
	workingPart += partList[i];
	
	std::string MRPDemandName("MRP"); 
	MRPDemandName += separatorChar_; 
	MRPDemandName += prefix;
	witGetPartExists(localWitRun_, prefix.c_str(), &exists);
	if (exists){
	  printf("Replicated part name:  %s\n", prefix.c_str());
	  return (8);
	}
	witGetPartExists(localWitRun_, MRPDemandName.c_str(), &exists);
	if (exists){
	  printf("Replicated demand name:  %s\n", MRPDemandName.c_str());
	  return (8);
	}
	
	
	witAddPart(localWitRun_, workingPart.c_str(), WitMATERIAL);            
	witAddPartWithOperation(localWitRun_, MRPDemandName.c_str());
	witAddDemand(localWitRun_, MRPDemandName.c_str(), MRPDemandName.c_str());
	witAddBomEntry(localWitRun_, MRPDemandName.c_str(), workingPart.c_str());
	witAddSubsBomEntry(localWitRun_, MRPDemandName.c_str(), 0, partList[i]);
	setTheDemand(theWitRun_, localWitRun_, partList[i], demandList[j], 
		     MRPDemandName); 
	// Now create the BOM below the newly created demand
	addStructureBelowPart(theWitRun_, localWitRun_, prefix, partList[i], 
			      workingPart, version_);
	portableFree(demandList[j]);
      }
      portableFree(demandList);
   }
   witHeurImplode(localWitRun_);
//   witWriteData(localWitRun_, "witWrite.dat");
//   witWriteExecSched(localWitRun_, "witExec.out", WitBSV);
   if (version_ == 1)
      performMRP(localWitRun_);

   for (i=0; i<partListLen; i++)
     portableFree(partList[i]);
   portableFree(partList);


  // set the state so that the print() method can be assured that its 
  // all populated and ready to go
  state_ = 1;

  // if successful, return 0
  // if warnings,   return 4
  // if error,      return 8
  return 0;
}


  char separatorChar_;
  int state_;
  int version_;
  int nPeriods_;  

int 
WUtlAllocatedMrp::state() 
{
  return state_;
}

int 
WUtlAllocatedMrp::version() 
{
  return version_;
}

int 
WUtlAllocatedMrp::nPeriods() 
{
  return nPeriods_;
}

char
WUtlAllocatedMrp::separatorChar() 
{
  return separatorChar_;
}

WitRun * 
WUtlAllocatedMrp::witRun()
{
  return theWitRun_;
}

WitRun * 
WUtlAllocatedMrp::localWitRun()
{
  return localWitRun_;
}

// print the allocated demand output file
int
WUtlAllocatedMrp::print(const char * fileName)
{
   assert(state_ == 1);

   // put the generic print  here
   char **operationList;
   char *producedPartName, *consumingOperationName;
   int listLen, i, j, k, l, n, t;
   int nBomEntries, nBopEntries, nConsumingBomEntries, nSubsBomEntries;
   int bomEntryIndex;
   float *execVol, *MRPCoverage, *subVol, *MRPAllocReq, *MRPDelta;
   FILE *fpout;

   if ((fpout = fopen(fileName, "w")) == NULL){
      printf("Can't open output file %s for the allocated MRP report.\n", 
         fileName);
      return(8);
   }

   std::string separatorString(1,separatorChar_);

   MRPCoverage = (float *) calloc(nPeriods_, sizeof(float));
   MRPAllocReq = (float *) calloc(nPeriods_, sizeof(float));
   MRPDelta = (float *) calloc(nPeriods_, sizeof(float));

   witGetOperations(localWitRun_, &listLen, &operationList);
   for(i=0; i<listLen; i++){
      witGetOperationNBomEntries(localWitRun_, operationList[i], &nBomEntries);
      if (nBomEntries == 0){
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
       

         size_t position1, position2, length;
         std::string witName(operationList[i]);

		 int firstSpaceOffset = witName.find_first_of(' ');
		 int secondSpaceOffset = witName.find_first_of(' ',firstSpaceOffset+1);
		 int thirdSpaceOffset = witName.find_first_of(' ',secondSpaceOffset+1);

		 std::string demandName = witName.substr(0, firstSpaceOffset );
		 std::string demandedPart = witName.substr(firstSpaceOffset+1, secondSpaceOffset - firstSpaceOffset - 1 );
		 std::string partName = witName.substr(secondSpaceOffset+1, thirdSpaceOffset - secondSpaceOffset - 1 );

         fprintf(fpout, "%s, %s, %s, AMRPReq", 
            demandName.c_str(), demandedPart.c_str(), partName.c_str());
         for(t=0; t<nPeriods_; t++)
            fprintf(fpout, ", %f", MRPAllocReq[t]);
         fprintf(fpout, "\n");
         fprintf(fpout, "%s, %s, %s, AMRPCov", 
            demandName.c_str(), demandedPart.c_str(), partName.c_str());
         for(t=0; t<nPeriods_; t++)
            fprintf(fpout, ", %f", MRPCoverage[t]); 
         fprintf(fpout, "\n");
         fprintf(fpout, "%s, %s, %s, AMRDelta", 
            demandName.c_str(), demandedPart.c_str(), partName.c_str());
         for(t=0; t<nPeriods_; t++)
            fprintf(fpout, ", %f", MRPDelta[t]);
         fprintf(fpout, "\n");
      }
   }
   fclose(fpout);
   portableFree(execVol);
   portableFree(MRPCoverage);
   portableFree(MRPDelta);
   portableFree(MRPAllocReq);
   for(i=0; i<listLen; i++) 
      portableFree(operationList[i]);
   portableFree(operationList);

   return 0;
}
 
  

// copy constructor 
WUtlAllocatedMrp::WUtlAllocatedMrp(const WUtlAllocatedMrp& source)
  :   
      theWitRun_ (source.theWitRun_),
      localWitRun_(source.localWitRun_),
      separatorChar_(source.separatorChar_),
      state_(source.state_)
{
  assert("Cant call this" == "the copy constructor for WUtlAllocatedMrp");
}

// assignment operator
WUtlAllocatedMrp&
WUtlAllocatedMrp::operator=(const WUtlAllocatedMrp& rhs)
{

  assert("Cant call this" == " the assignment operator for WUtlAllocatedMrp");
  if (this != &rhs) {		// Check for assignment to self
    theWitRun_ = rhs.theWitRun_;
    localWitRun_ = rhs.localWitRun_;
    separatorChar_ = rhs.separatorChar_;
    state_ = rhs.state_;
  }
  return *this;
}

// destructor
WUtlAllocatedMrp::~WUtlAllocatedMrp()
{
   witDeleteRun(localWitRun_);
}


// This function sets the demands in the local wit run.
void setTheDemand(WitRun *theWitRun, WitRun *localWitRun, std::string partName, 
        std::string demandName, std::string  MRPDemandName){

   float *demandVol, *hardLower, *hardUpper, *softLower;
   int *priority, shipLateLimit, buildAheadLimit;

   witGetDemandDemandVol(theWitRun, partName.c_str(), demandName.c_str(), &demandVol);
   witSetDemandDemandVol(localWitRun, MRPDemandName.c_str(), MRPDemandName.c_str(), demandVol); 
   witGetDemandBuildAheadLimit(theWitRun, partName.c_str(), demandName.c_str(), 
      &buildAheadLimit);
   witSetDemandBuildAheadLimit(localWitRun, MRPDemandName.c_str(), MRPDemandName.c_str(), 
      buildAheadLimit); 
   witGetDemandCumShipBounds(theWitRun, partName.c_str(), demandName.c_str(), 
      &hardLower, &softLower, &hardUpper);
   witSetDemandCumShipBounds(localWitRun, MRPDemandName.c_str(), MRPDemandName.c_str(), 
      hardLower, softLower, hardUpper);
   witGetDemandPriority(theWitRun, partName.c_str(), demandName.c_str(), 
      &priority);
   witSetDemandPriority(localWitRun, MRPDemandName.c_str(), MRPDemandName.c_str(), 
      priority);
   witGetDemandShipLateLimit(theWitRun, partName.c_str(), demandName.c_str(), 
      &shipLateLimit);
   witSetDemandShipLateLimit(localWitRun, MRPDemandName.c_str(), MRPDemandName.c_str(), 
   shipLateLimit);

   portableFree(demandVol);
   portableFree(hardLower);
   portableFree(hardUpper);
   portableFree(softLower);
   portableFree(priority);
}

// This function builds the structure below parts in the local wit run.
void addStructureBelowPart(WitRun *theWitRun, WitRun *localWitRun, std::string prefix,
               std::string partName, std::string workingPart, int version){

   int nProducingBopEntries, nBomEntries;
   float  *yieldRate;
   int k, t, nPeriods;
   int bopEntryIndex, localBopEntryIndex;
   char *producingOperationName;
   witBoolean exists;

   witGetNPeriods(theWitRun, &nPeriods);
   witGetPartNProducingBopEntries(theWitRun, partName.c_str(), &nProducingBopEntries);
   if (nProducingBopEntries == 0){
   // This is a lowest level part so create an operation under it
      witGetOperationExists(localWitRun, workingPart.c_str(), &exists);
      if (!exists){
         witAddOperation(localWitRun, workingPart.c_str());
         witAddBopEntry(localWitRun, workingPart.c_str(), workingPart.c_str());
         witGetOperationYieldRate(localWitRun, workingPart.c_str(), &yieldRate);
         for(t=0; t<nPeriods; t++)
            yieldRate[t] = 1;
         witSetOperationYieldRate(localWitRun, workingPart.c_str(), yieldRate);
         portableFree(yieldRate);
      }
   }
   else 
   for (k=0; k<nProducingBopEntries; k++){
      witGetPartProducingBopEntry(theWitRun, partName.c_str(), k, 
         &producingOperationName, &bopEntryIndex);
      std::string localOperationName(prefix);
      localOperationName += producingOperationName;
      witGetOperationExists(localWitRun, localOperationName.c_str(), &exists);
      if (!exists)
         setupOperation(theWitRun, localWitRun, producingOperationName, 
            localOperationName);
      // If the producing operation doesn't have any bom records, 
      // set its yield to 1
      witGetOperationNBomEntries(theWitRun, producingOperationName, 
         &nBomEntries);
      if (nBomEntries == 0){
         witGetOperationYieldRate(localWitRun, localOperationName.c_str(), &yieldRate);
         for(t=0; t<nPeriods; t++)
            yieldRate[t] = 1;
         witSetOperationYieldRate(localWitRun, localOperationName.c_str(), yieldRate);
         portableFree(yieldRate);
      }
      // Add the BOP entry and set the BOP entry
      // Can't match bop indices so first get the current index for 
      // the operation
      witGetOperationNBopEntries(localWitRun, localOperationName.c_str(), 
         &localBopEntryIndex);
      witAddBopEntry(localWitRun, localOperationName.c_str(), workingPart.c_str());
      setTheBopData(theWitRun, localWitRun, producingOperationName,
         localOperationName, partName, workingPart, bopEntryIndex, 
         localBopEntryIndex);  
      if (!exists)
         addStructureBelowOperation(theWitRun, localWitRun, prefix, 
            producingOperationName, localOperationName, version);              
      portableFree(producingOperationName);
   }
}

void setupPart(WitRun *theWitRun, WitRun *localWitRun, std::string consumedPartName, 
        std::string localPartName){
    
   witAttr partCategory;
   float *hardLower, *softLower, *hardUpper;

   witGetPartCategory(theWitRun, consumedPartName.c_str(), &partCategory);
   if (partCategory == WitPRODUCT || partCategory == WitRAW) 
      witAddPart(localWitRun, localPartName.c_str(), WitMATERIAL);
   else
      witAddPart(localWitRun, localPartName.c_str(), partCategory);
   if (partCategory != WitCAPACITY){
      witGetPartStockBounds(theWitRun, consumedPartName.c_str(), &hardLower, &softLower, &hardUpper);
      witSetPartStockBounds(localWitRun, localPartName.c_str(), hardLower, softLower, hardUpper);
   portableFree(hardLower);
   portableFree(softLower);
   portableFree(hardUpper);
   }
}

// This function sets the BOP data in the local wit run.
void setTheBopData(WitRun *theWitRun, WitRun *localWitRun, 
        std::string producingOperationName, std::string localOperationName, 
        std::string partName, std::string workingPart, int bopEntryIndex, int localBopEntryIndex){

   int earliestPeriod, latestPeriod;
   witBoolean expAllowed;
   float expAversion, *offset, prodRate;

   witGetBopEntryEarliestPeriod(theWitRun, producingOperationName.c_str(), bopEntryIndex, 
      &earliestPeriod);
   witSetBopEntryEarliestPeriod(localWitRun, localOperationName.c_str(), localBopEntryIndex, 
      earliestPeriod);
   witGetBopEntryLatestPeriod(theWitRun, producingOperationName.c_str(), bopEntryIndex, 
      &latestPeriod);
   witSetBopEntryLatestPeriod(localWitRun, localOperationName.c_str(), localBopEntryIndex, 
      latestPeriod);
   witGetBopEntryExpAllowed(theWitRun, producingOperationName.c_str(), bopEntryIndex, 
      &expAllowed);
   witSetBopEntryExpAllowed(localWitRun, localOperationName.c_str(), localBopEntryIndex, 
      expAllowed);
   witGetBopEntryExpAversion(theWitRun, producingOperationName.c_str(), bopEntryIndex, 
      &expAversion);
   witSetBopEntryExpAversion(localWitRun, localOperationName.c_str(), localBopEntryIndex, 
      expAversion);
   witGetBopEntryOffset(theWitRun, producingOperationName.c_str(), bopEntryIndex, 
      &offset);
   witSetBopEntryOffset(localWitRun, localOperationName.c_str(), localBopEntryIndex, 
      offset);
   witGetBopEntryProdRate(theWitRun, producingOperationName.c_str(), bopEntryIndex, 
      &prodRate);
   witSetBopEntryProdRate(localWitRun, localOperationName.c_str(), localBopEntryIndex, 
      prodRate);
   portableFree(offset);
}   

// This function sets up the operations.
void setupOperation(WitRun *theWitRun, WitRun *localWitRun, 
        std::string producingOperationName, std::string localOperationName){

   float *hardLower, *softLower, *hardUpper;
   float *incLotSize, *minLotSize, *yieldRate;

   witAddOperation(localWitRun, localOperationName.c_str());
   witGetOperationExecBounds(theWitRun, producingOperationName.c_str(),
      &hardLower, &softLower, &hardUpper);
   witSetOperationExecBounds(localWitRun, localOperationName.c_str(),
      hardLower, softLower, hardUpper);
   witGetOperationIncLotSize(theWitRun, producingOperationName.c_str(),
      &incLotSize);
   witSetOperationIncLotSize(localWitRun, localOperationName.c_str(),
      incLotSize);
   witGetOperationMinLotSize(theWitRun, producingOperationName.c_str(),
      &minLotSize);
   witSetOperationMinLotSize(localWitRun, localOperationName.c_str(),
      minLotSize);
   witGetOperationYieldRate(theWitRun, producingOperationName.c_str(),
      &yieldRate);
   witSetOperationYieldRate(localWitRun, localOperationName.c_str(),
      yieldRate);
   portableFree(hardLower);
   portableFree(softLower);
   portableFree(hardUpper);
   portableFree(incLotSize);
   portableFree(minLotSize);
   portableFree(yieldRate);
}

void addStructureBelowOperation(WitRun *theWitRun, WitRun *localWitRun, 
   std::string prefix, std::string consumingOperationName, 
   std::string localOperationName, int version){
   int nBomEntries;
   int t, k, nPeriods;
   int bomEntryIndex, localBomEntryIndex;
   char *consumedPartName;
   witBoolean exists;
   
   witGetNPeriods(theWitRun, &nPeriods);    
   witGetOperationNBomEntries(theWitRun, consumingOperationName.c_str(), &nBomEntries);
   for (k=0; k<nBomEntries; k++){
      witGetBomEntryConsumedPart(theWitRun, consumingOperationName.c_str(), 
         k, &consumedPartName);
      std::string localPartName(prefix);
      localPartName += consumedPartName;
      witGetPartExists(localWitRun, localPartName.c_str(), &exists);
      if (!exists)
         setupPart(theWitRun, localWitRun, consumedPartName, localPartName);

      // Add the BOM entry and set the BOM entry
      witGetOperationNBomEntries(localWitRun, localOperationName.c_str(), 
         &localBomEntryIndex);
      witAddBomEntry(localWitRun, localOperationName.c_str(), localPartName.c_str());
      if (version == 1)
         witAddSubsBomEntry(localWitRun, localOperationName.c_str(), k,
            consumedPartName);
      if (version == 2){
         for(t=0; t<nPeriods; t++)
            witAddSubsBomEntry(localWitRun, localOperationName.c_str(), k,
               consumedPartName);
         }
      setTheBomData(theWitRun, localWitRun, consumingOperationName,
         localOperationName, consumedPartName, localPartName, k, 
         localBomEntryIndex, version);  
      if (!exists)
         addStructureBelowPart(theWitRun, localWitRun, prefix, 
            consumedPartName, localPartName, version);                
      portableFree(consumedPartName);
   }
}

// This function sets up the BOM data in the local wit run.
void setTheBomData(WitRun *theWitRun, WitRun *localWitRun, 
        std::string consumingOperationName, std::string localOperationName, 
        std::string consumedPartName, std::string localPartName, 
        int bomEntryIndex, int localBomEntryIndex, int version){

   int earliestPeriod, latestPeriod, t, k, nPeriods;   
   float falloutRate, usageRate, *offset, *subOffset;
   witBoolean mandEC;

   witGetNPeriods(localWitRun, &nPeriods);
   witGetBomEntryEarliestPeriod(theWitRun, consumingOperationName.c_str(), bomEntryIndex,
      &earliestPeriod);
   witSetBomEntryEarliestPeriod(localWitRun, localOperationName.c_str(), localBomEntryIndex,
      earliestPeriod);
   witGetBomEntryLatestPeriod(theWitRun, consumingOperationName.c_str(), bomEntryIndex,
      &latestPeriod);
   witSetBomEntryLatestPeriod(localWitRun, localOperationName.c_str(), localBomEntryIndex,
      latestPeriod);
   witGetBomEntryFalloutRate(theWitRun, consumingOperationName.c_str(), bomEntryIndex,
      &falloutRate);
   witSetBomEntryFalloutRate(localWitRun, localOperationName.c_str(), localBomEntryIndex,
      falloutRate);
   witGetBomEntryMandEC(theWitRun, consumingOperationName.c_str(), bomEntryIndex,
      &mandEC);
   witSetBomEntryMandEC(localWitRun, localOperationName.c_str(), localBomEntryIndex,
      mandEC);
   witGetBomEntryUsageRate(theWitRun, consumingOperationName.c_str(), bomEntryIndex,
      &usageRate);
   witSetBomEntryUsageRate(localWitRun, localOperationName.c_str(), localBomEntryIndex,
      usageRate);
   witGetBomEntryOffset(theWitRun, consumingOperationName.c_str(), bomEntryIndex,
      &offset);
   witGetBomEntryOffset(theWitRun, consumingOperationName.c_str(), bomEntryIndex,
      &subOffset);
   witSetBomEntryOffset(localWitRun, localOperationName.c_str(), localBomEntryIndex,
      offset);

   if (version == 1){
      witSetSubsBomEntryEarliestPeriod(localWitRun, localOperationName.c_str(), localBomEntryIndex,
         0, earliestPeriod);
      witSetSubsBomEntryLatestPeriod(localWitRun, localOperationName.c_str(), localBomEntryIndex,
         0, latestPeriod);
      witSetSubsBomEntryFalloutRate(localWitRun, localOperationName.c_str(), localBomEntryIndex,
         0, falloutRate);
      witSetSubsBomEntryUsageRate(localWitRun, localOperationName.c_str(), localBomEntryIndex,
         0, usageRate);
      witSetSubsBomEntryOffset(localWitRun, localOperationName.c_str(), localBomEntryIndex,
         0, offset);
   }
   if (version == 2){
      for(t=0; t<nPeriods; t++){
         witSetSubsBomEntryEarliestPeriod(localWitRun, localOperationName.c_str(), 
            localBomEntryIndex, t, earliestPeriod);
         witSetSubsBomEntryLatestPeriod(localWitRun, localOperationName.c_str(), 
            localBomEntryIndex, t, latestPeriod);
         witSetSubsBomEntryFalloutRate(localWitRun, localOperationName.c_str(), 
            localBomEntryIndex, t, falloutRate);
         witSetSubsBomEntryUsageRate(localWitRun, localOperationName.c_str(), 
            localBomEntryIndex, t, usageRate);
         for(k=0; k<nPeriods; k++)
            subOffset[k] = offset[k] - t;
         witSetSubsBomEntryOffset(localWitRun, localOperationName.c_str(), 
            localBomEntryIndex, t, subOffset);
      }
   }
   portableFree(subOffset);
   portableFree(offset);
}

// This function performs an MRP on the local wit run.
void performMRP(WitRun *localWitRun){
   
   int i, t, listLen, nPeriods;
   char **partList;
   float *supplyVol;
 
   witGetNPeriods(localWitRun, &nPeriods);
   supplyVol = (float *) calloc(nPeriods, sizeof(float));
   for(t=0; t<nPeriods; t++)
      supplyVol[t] = 0;
 
   witGetParts(localWitRun, &listLen, &partList);
   for(i=0; i<listLen; i++) {
      witSetPartSupplyVol(localWitRun, partList[i], supplyVol);
      portableFree(partList[i]);
   }
   portableFree(partList);
   portableFree(supplyVol);
   witMrp(localWitRun);
}
