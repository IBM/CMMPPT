//#include <mcl/src/mdsPragma.h>
// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
// file: sceFssMg.C
// author:  tom

#include <stdlib.h>
#include <assert.h>
#include <mcl/src/mcl.h>
#include <sce/src/sceDefin.h>
#include <sce/src/sceFssMg.h>
#include <scenario/src/calendar.h>
#include <wit/src/wit.h>
#include <sce/src/mPlantH.h>
#include <sce/src/exploders.h>
#include <sce/src/scenSce.h>
#include <sce/src/sceParam.h>


// + ABout FSS:
// +
// + 

// + There are three fssSchemes:
// + fssFlag=S ==> Singleton Fss: Loops thru every independent demand (geoPlannerDemands only,
// + Doesn't look at 2nd pass features yet)  If the demand is not
// + met completely within FSSHorizon, An FSS is done with That
// + one (ie, singleton) demand as the demand focus.  It then picks
// + up shortage of components that are lowest level only!!
// +
// + fssFlag=G ==> General FSS: 
// + User defines sets via the FSSFile.  Each set is seperate FSS.
//
//



// Potential FSS strategies:  NOte: these are our original notes
// from that famous meeting.  We have stuck with the green method
// ever since.  The sleazy red method was implemented but only with
// the witU5 fullexplode approach (ie, the iterative mrp's where
// you pick up assembly reqVols and place them as a demand).  That
// method has been removed.  IF you want it, get the
// SCE 1.30 19961114FIX out of the repository.  Beware though, its obsolete.
//
// The green method ahs gone through some evolution (for the better).
// Check the actual code for better documetnation than is right here.
// TE 11/21/1996
// ==========================================================
// ======== Sleazy Red =====
//
// Compute FSSCov:
// Set demandVol = shipVol FOR ALL demands,
// Set demandVol(FocusDemand) = 0
// Run witMrp
// FSSCov = mrpExcessVol FOR ALL parts
//
// Compute FSSReq:
// Set supplyVol = mrpExcessVol FOR ALL parts
// Set demandVol = 0 for all demands
// Set demandVol = original demandVol for focus demand only
// Run WUtlComputeDependentDemandAndPlanOrders
// FSSReq = totalDemand (ie, mrpConsVol from full explode mrp)
//  ---

// ==========================================================
// ======== Blue =====
//
// Compute FSSCov:
//
// -- STEP 1: Compute Supply available for Focus after netting out  ---
//            the supply consumed by all the Non-focus demand
// Init FSSCov = excessVol
// Set supplyVol = 0 for all parts
// Set demandVol = 0 FOR ALL demands,
// Set demandVol(FocusDemand) = shipVol(FocusDemand)
// Run WUtlComputeDependentDemandAndPlanOrders (ie, full explode MRP)
// FSSCov += totalDemand (ie, mrpConsVol)  for all raw parts
//
// Compute FSSReq:
// Set supplyVol = excessVol FOR ALL parts
// Set demandVol = 0 for all demands
// Set demandVol = original demandVol for focus demand only
// Run WUtlComputeDependentDemandAndPlanOrders
// FSSReq = totalDemand (ie, mrpConsVol from full explode mrp)

// ==========================================================
// ======== Green =====
//
// Compute FSSCov:
//
// -- STEP 1: Compute Supply available for Focus after netting out  ---
//            the supply consumed by all the Non-focus demand
// Init FSSCov = excessVol
// Set supplyVol = 0 for all parts
// Set demandVol = 0 FOR ALL demands,
// Set demandVol(FocusDemand) = shipVol(FocusDemand)
// run witMrp
// FSSCov += mrpConsVol  for all raw parts
//
// Compute FSSReq:
// Set supplyVol = excessVol + reqVol FOR ALL parts
// Set demandVol = 0 for all demands
// Set demandVol = original demandVol for focus demand only
// Run WUtlComputeDependentDemandAndPlanOrders
// FSSReq = totalDemand (ie, mrpConsVol from full explode mrp)

// ===========================================================


#ifndef ELIMINATE_OLD_MAIN




void
LgFrSceFssMgr::generalSetFss()
{
  std::string fssGeneralSetMethod = scenScePtr_->setOfParameters().getString("fssGeneralSetMethod");
  std::string fssFeatureGeneralSetMethod = scenScePtr_->setOfParameters().getString("fssFeatureGeneralSetMethod");
  int       fssGeneralSetHorizon = scenScePtr_->setOfParameters().getInt("fssGeneralSetHorizon");
  std::string fssGeneralSetFileName = scenScePtr_->setOfParameters().getString("fssGeneralSetFileName");
  std::string fssGeneralSetFile  = scenScePtr_->setOfParameters().getString("fssGeneralSetFile");

  std::string type("generalSet");
  this->runFss(type, 
	       fssGeneralSetMethod, 
	       fssFeatureGeneralSetMethod, 
	       fssGeneralSetHorizon,
	       fssGeneralSetFileName, 
	       fssGeneralSetFile);  
}

void
LgFrSceFssMgr::singletonFss()
{
  std::string fssSingletonMethod = scenScePtr_->setOfParameters().getString("fssSingletonMethod");
  std::string fssFeatureSingletonMethod = scenScePtr_->setOfParameters().getString("fssFeatureSingletonMethod");
  int       fssSingletonHorizon = scenScePtr_->setOfParameters().getInt("fssSingletonHorizon");
  std::string fssSingletonFileName = scenScePtr_->setOfParameters().getString("fssSingletonFileName");
  std::string fssSingletonFile;

  std::string type("singleton");
  this->runFss(type, 
	       fssSingletonMethod, 
	       fssFeatureSingletonMethod, 
	       fssSingletonHorizon,
	       fssSingletonFileName, 
	       fssSingletonFile);  
}

void
LgFrSceFssMgr::machineFss()
{
  std::string fssMachineMethod = scenScePtr_->setOfParameters().getString("fssMachineMethod");
  std::string fssFeatureMachineMethod = scenScePtr_->setOfParameters().getString("fssFeatureMachineMethod");
  int       fssMachineHorizon = scenScePtr_->setOfParameters().getInt("fssMachineHorizon");
  std::string fssMachineFileName = scenScePtr_->setOfParameters().getString("fssMachineFileName");
  std::string fssMachineFile  = scenScePtr_->setOfParameters().getString("fssMachineFile");

  std::string type("machine");
  this->runFss(type, 
	       fssMachineMethod, 
	       fssFeatureMachineMethod, 
	       fssMachineHorizon,
	       fssMachineFileName, 
	       fssMachineFile);  
}



// ==================
// ====  run FSS ====
// ==================
void
LgFrSceFssMgr::runFss(std::string & fssType,
                      std::string & fssMethod,
		      std::string & fssFeatureMethod,
		      int         fssHorizon,
		      std::string & fssOutputFileName,
		      std::string & fssInputSetFileName)
{

  WitRun * theWitRun = this->theWitRun_;
  int      nPeriods;
  int      i, j, t, ii;



  WitRun *  localWitRun;
  char   *  focusPartName;
  char   *  focusDemandName;
  int       nParts;  
  char  **  partList;
  float *   focusDemandVol;
  float **  FSSReq;  
  float **  FSSCov;

  // counter for number of printed output records
  long numRecords = 0;
  // also needs a seperate output record counter for log reporting
  long numRecModified = 0;

  // these are for reporting on the fss input files if they exist.
  long goodRec = 0;
  long badRec = 0;
  long commentRec = 0;
  long totalRec = 0;
  long numDistinctSetIDs = 0;  // input number to check against output
  long numDistinctPrintedSetIDs = 0;  // output number to check against input
  long numModifiedDistinctPrintedSetIDs = 0; // ditto -- for 'modified' files
  int existsInputFile = 0;  // flag for whether or not there is an input file

  // Get the setOfParameters
  LgFrSceSetOfParameters & setOfParameters =  scenScePtr_->setOfParameters();
  
  int nStandaloneFeatureArcs = setOfParameters.getInt("nStandaloneFeatureArcs");

  MclFacility * theErrFacil = scenScePtr_->getErrFacility();  
  LgFrSceFssableExploderDecorator * myExploder;
  float **  forwardRollableStockVols = 0;  

  // open the Input file 
  std::ifstream fssInFile;
  std::ofstream scrubOutFile;
  LgFrSceScrubber * myScrubber = scenScePtr_->scrubber();
  if (fssType != "singleton") {
    fssInFile.open(fssInputSetFileName.c_str());

    // also must add summary to input file log, but this is a bit hairy
    existsInputFile = 1;  // true

    if (! fssInFile)  {
      std::string fileErrorString("Cannot open FSS " + fssType + "Input File: ");
      (*theErrFacil)("WarningCannotOpenFile",MclArgList() 
		     << fileErrorString 
		     << fssInputSetFileName 
		     << "\nSCE will skip this FSS operation.\n" );
      return;
    }
    // Open a file for the scrubbed output 
    if (myScrubber -> scrubMode())     {
      std::string scrubFileName = myScrubber->scrubDir() + fssInputSetFileName;
      scrubOutFile.open(scrubFileName.c_str(), std::ofstream::out);
      if (!scrubOutFile.good())
	{
	  (*theErrFacil)("ScrubOutPutFileNameBad",MclArgList() 
			 << scrubFileName); 
	}
    }

  }


  std::ofstream fssOutFile;
  std::ofstream fssModifiedOutFile;
  std::ofstream * fssModifiedOutFilePtr;

    bool printModifiedFssMachReport = setOfParameters.getBool("printModifiedFssMachReport");

  // DONT do this if your are ONLY doing scrubbing
  if (! myScrubber->scrubOnly()) {


    // Now open the FSS OUTPUT file
    fssOutFile.open(fssOutputFileName.c_str(), std::ofstream::out);
    if (fssOutFile.fail()) {
      std::string fileErrorString("FSS " + fssType + "Report");
      (*theErrFacil)("CannotOpenFileForWrite",MclArgList() 
		     << fileErrorString 
		     << fssOutputFileName);
    }
    
    
    // Now open a special "modified" output file
    // NOTE this is an undocumented function
    if ( printModifiedFssMachReport) {
      std::string FSSMachReportFileName = setOfParameters.getString("FSSModifiedMachReportFileName");  
      fssModifiedOutFile.open(FSSMachReportFileName.c_str(), std::ofstream::out);
      fssModifiedOutFilePtr = &fssModifiedOutFile;
    }
    

    // get the list of  parts 
    witGetParts(theWitRun, &nParts, &partList);
    witGetNPeriods(theWitRun, &nPeriods);
  


    std::string siteCustomization = setOfParameters.getString("siteCustomization");
    
    if (fssHorizon < 0) fssHorizon = 0;
    if (fssHorizon >= nPeriods) fssHorizon = nPeriods - 1;
    
    // NOTE: we cast the thing right into its FSSAble incarnation, becuase
    //       the fss methods are only declared for the fssAble decorator.
    myExploder = (LgFrSceFssableExploderDecorator *) scenScePtr_->exploder();
    assert(myExploder->isA() == __LGFRSCEFSSABLEEXPLODERDECORATOR);
    
    // set these to zero up front.  We'll come back to it later
    // FINISH_ME: si this necesary??
    myExploder->setAllSupplyToZero();
    myExploder->setAllDemandToZero();
    
    // FINISH_ME: this eventually has to go (not a priority)!!  
    localWitRun = myExploder->localWitRun();
    


    // forward rollable stock vols:  This is only used with liberal
    // fss method.  
    if (fssMethod == "liberal") {  
      this->getForwardRollableStockSchedule(theWitRun,
					    nParts, 
					    partList, 
					    &forwardRollableStockVols);
    }
  }



  // ==================================================
  // Do an fssAnalysis for each User Defined Set with
  // exactly one entry!!
  // ==================================================

  std::string theFssSetName;
  std::string partReadFromlastTime;
  std::string fssSetNameReadFromLastTime;    
  LgFrStringVector focusPartNameList;
  int singletonPartIndex = -1;
  while (1) {
    if (fssType == "singleton")
      this->getNextSingletonFssSet(focusPartNameList,
                                   theWitRun,
                                   singletonPartIndex,
                                   theFssSetName,
                                   nParts,
                                   partList);
    
    else 
      this->getNextFssSet(focusPartNameList,
                          theWitRun,
                          theFssSetName,
                          fssSetNameReadFromLastTime,              
                          partReadFromlastTime,
                          fssInputSetFileName,
                          fssInFile,
			  scrubOutFile,
			  totalRec,
			  goodRec,
			  numDistinctSetIDs,
			  badRec,
			  commentRec);

    // when the list is empty, then the fss is basically over.
    if (focusPartNameList.empty())
      break;

    // DONT do this if your are ONLY doing scrubbing
    if (! myScrubber->scrubOnly()) {
      if (fssType == "machine")
	std::cout << "   fssMachineAnalysis: " << theFssSetName;    
      else if (fssType == "generalSet")
	std::cout << "   fssGeneralSetAnalysis: " << theFssSetName;
      else
	std::cout << "   fssSingletonAnalysis: " << theFssSetName;      
      
      
      // + fssHorizon: This is used in all scheme's (used to be ignored in generalSet)
      if (fssHorizon > -1) {
	
	if (this->isFssSetSatisfiedWithinHorizon(theWitRun,
						 focusPartNameList,
						 fssHorizon)) {
	  
	  // echo this to stdout as well as a message ...
	  std::cout << "  (Fully committed within FSS" << fssType << " horizon)" << std::endl;
	  
	  MclFacility * theErrFacil = scenScePtr_->getErrFacility();
	  (*theErrFacil)("FssHorizon",MclArgList() 
			 << theFssSetName 
			 << fssHorizon);
	  
	  //	badRec++;
	  continue;
	}
      }
      
      this->fssAnalysisBlue(myExploder,
			    fssMethod,
			    focusPartNameList,
			    nStandaloneFeatureArcs,
			    fssFeatureMethod,
			    nParts,
			    partList,
			    forwardRollableStockVols,                          
			    &FSSReq,
			    &FSSCov);
      
      numRecords += this->printFssGeneralFile(theWitRun,
					      nParts,
					      partList,
					      theFssSetName,
					      FSSReq,
					      FSSCov,
					      fssOutFile);
      
      // throw in a carriage return to indicate that the operation is done.
      std::cout << std::endl;

#ifdef SCE_ODBC
      if (fssType == "singleton")
	this->insertFssSingleSchemeDB(theWitRun,
				      nParts,
				      partList,
				      theFssSetName,
				      FSSReq,
				      FSSCov);
      
#endif
      
      // FINISH_ME: the printModifiedFssMachReport param and function is
      // enable but should not be used for singleton.  It should nopt be
      // used at all, for that matter until it is generalized to all three
      // scheme
      if ( printModifiedFssMachReport) {     
	numRecModified += this->printModifiedFssFile(theWitRun,
						     nParts,
						     partList,
						     theFssSetName,
						     FSSReq,
						     FSSCov,
						     *fssModifiedOutFilePtr);
	numModifiedDistinctPrintedSetIDs++;
      }
      
      // increment output record counter -- we will only count unique
      // "theFssSetName"'s
      numDistinctPrintedSetIDs++;
      
      
      // you must free the FSSCov and FSSReq vectors
      // NOTE: this must be free'd after EVERY invocation of
      // fssAnalysis()!!! 
      for (ii=0; ii<nParts; ii++)   {
	if (FSSCov[ii] != 0)                    
	  portableFree(FSSCov[ii]);
	if (FSSReq[ii] != 0)                    
	  portableFree(FSSReq[ii]);      
      }
      portableFree(FSSCov);
      portableFree(FSSReq);    
    }
  }
  
  //#ifdef SCE_ODBC                    
  // if (fssType == "singleton")
  //  std::cout << "Inserting FSSSingleCheme SUCCESSFULLY!" << std::endl;
  //#endif



  if (myScrubber->scrubMode())
    scrubOutFile.close();

  // DONT do this if your are ONLY doing scrubbing
  if (! myScrubber->scrubOnly()) {
    fssOutFile.close();
    if ( printModifiedFssMachReport) {     
      (*fssModifiedOutFilePtr).close();
    }
  

    // Saved attributes must be free'd
    if (fssMethod == "liberal") {
      for (i=0; i<nParts; i++)  {
	if (forwardRollableStockVols[i] != 0)
	  portableFree(forwardRollableStockVols[i]);
      }
      portableFree(forwardRollableStockVols);
    }
  }
  if (existsInputFile) {
    // print log for fssSetDef file into inputlogfile
    scenScePtr_->flatFileMgr().sceIOLogManager().printInputLog(fssInputSetFileName,totalRec,goodRec,badRec,commentRec,numDistinctSetIDs);
    // the 'checkRec' = numDistinctSetIDs - badRec
    
    // print the resultant output summary log into outputlogfile
    scenScePtr_->flatFileMgr().sceIOLogManager().printOutputLog(fssOutputFileName,numRecords,000,numDistinctPrintedSetIDs);
  }
  else { // this is the Singleton Run
    // print the resultant output summary log into outputlogfile
    scenScePtr_->flatFileMgr().sceIOLogManager().printOutputLog(fssOutputFileName,numRecords,000,numDistinctPrintedSetIDs+badRec);
    // I'm cheating here, but this is the easiest way to do this....
    // Problem: if this is a SINGLETON run and there are any 'badRec's then
    //          the input log will never be updated (and it probably shouldn't
    //          be because the badRec's here do not mean there is anythin 
    //          wrong with the input, but that it was skipped in the fssrun)
    //          ...However, since a badRec > 0 means records were skipped, the
    //          output will be wrong  ... and an error will be produced.
    //          so this is how i'm cheating: I just add the # of badRec to
    //          the number of sets printed.
    //
    //          Another option is to actually update the demand vol file log
    //          using the sceIOLogMgr().updateNode(..) method
    //          -- we might want to change it back though so it doesn't screw
    //          up other things later.
  }


  // DONT do this if your are ONLY doing scrubbing
  if (! myScrubber->scrubOnly()) {

    // also if there is going to be a modified report generated, must print
    // summary in outputlogfile
    if ( printModifiedFssMachReport) {     
      std::string FSSMachReportFileName =
	setOfParameters.getString("FSSModifiedMachReportFileName");  
      scenScePtr_->flatFileMgr().sceIOLogManager().printOutputLog(FSSMachReportFileName,
								  numRecModified,
								  000,
								  numModifiedDistinctPrintedSetIDs+badRec);
    }

    
    // free the parts list
    for (i=0; i<nParts; i++)  
      portableFree(partList[i]);
    portableFree(partList);
  }
}






void
LgFrSceFssMgr::fssAnalysisBlue(
    LgFrSceFssableExploderDecorator * myExploder,
    std::string & fssMethod,
    LgFrStringVector & focusPartNameList,
    int         nStandaloneFeatureArcs,
    std::string & fssFeatureMethod,
    int       nParts,
    char   ** partList,
    float  ** forwardRollableStockVols,    
    float *** FSSReqPtr,
    float *** FSSCovPtr)
{
  int i,j,k,t;
  witAttr  category;
  witBoolean exists;
  int nPeriods;

  // We will need to access the original and local witRun.
  WitRun * originalWitRun = myExploder->theWitRun();
  WitRun * localWitRun    = myExploder->localWitRun();  

  LgFrSceSetOfParameters & setOfParameters =  scenScePtr_->setOfParameters();  

  
  float * zeroVec;
  float *  mrpConsVol;  
  
  witGetNPeriods(originalWitRun, &nPeriods);
  
  // malloc a vector to be a vector of Zero's 
  zeroVec = (float *) malloc (nPeriods * sizeof(float));
  assert(zeroVec != 0);
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0.0;
  
  
  // malloc a vector to be a vector of FSSCov 
  float ** FSSCov = (float **) malloc (nParts * sizeof(float *));
  assert(FSSCov != 0);

  // malloc a vector to be a vector of FSSReq
  float ** FSSReq = (float **) malloc (nParts * sizeof(float *));
  assert(FSSReq != 0);

  for (i=0; i<nParts; i++) {
    FSSReq[i] = 0;
    FSSCov[i] = 0;
  }




  // ==========
  // fssMethod is either liberal or conservative.
  // -- liberal --  means Each fss run gets full access to the excessVol
  // including the sub-assembly excess.  
  // -- conservative -- means Each fss does NOT get access to the excessVol.
  // therefore FSSCov reports only on the supply consumed to meet the
  // demand as is.
  
  if (fssMethod == "liberal") {
    // a) Init FSSCov = excessVol
    // b) Set supplyVol = 0 for all parts  
    for (i=0; i<nParts; i++)  {
      float * excessVol;
      witGetPartExcessVol(originalWitRun, partList[i], &excessVol);
      FSSCov[i] = excessVol;
      // in single wr approach, we do NOT want to overwrite
      // the supplyVol for nullSub parts!!
      // FINISH_ME: whats the big deal here????
      std::string sub(partList[i]);
      if (! mpHelper_->isPartSpecialNullSubstitute(localWitRun, sub)) {
        witSetPartSupplyVol(localWitRun, partList[i], zeroVec);
      }
    }
  }
  

  // Set the demandVols in the localWitRun as follows:
  //   -  Set demandVol(FocusDemandSet) = shipVol(FocusDemandSet)
  //   -  Set demandVols(featuresInFocus) = amountOfFeaturesShipped
  //   -  Set supplVols(featuresInFocus) = shipVol(FocusMachine) * featRatio
  for (k=0; k<focusPartNameList.size(); k++) {
    // get a demand list for each part in the fssSet.  In the current SCE model,
    // there will only be ONE demand stream per part.  We do an assert here.
    // In the future, we may allow "sub-geo" demand.
    int fpNDemands;
    char ** fpDlist;
    witGetPartDemands(originalWitRun, focusPartNameList[k].c_str(), &fpNDemands, &fpDlist);
    assert(fpNDemands == 1);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<fpNDemands; j++) {
      std::string focusDemandName = fpDlist[j];
      float * focusShipVol;
      witGetDemandShipVol(originalWitRun, focusPartNameList[k].c_str(), focusDemandName.c_str(), &focusShipVol);
      witSetDemandDemandVol(localWitRun, focusPartNameList[k].c_str(), focusDemandName.c_str(), focusShipVol);

      // Set demandVol(GPD,F) += shipVol(GPD,F) 
      //                            + ratio(GPD,F) * (shipVol(GPD) - SPV(GPD,F))
      if (nStandaloneFeatureArcs > 0) {
        adjustFssCovDemandForFeatures(localWitRun, originalWitRun, 
                                      focusPartNameList[k].c_str(), focusDemandName.c_str(), focusShipVol);
        adjustFssCovSupplyForFeatures(localWitRun, originalWitRun, 
                                      focusPartNameList[k].c_str(), focusDemandName.c_str(), focusShipVol);
      }

      
      portableFree(focusShipVol);
      portableFree(fpDlist[j]);
    }
    portableFree(fpDlist);
  }

  
    
  // e) run fullExplode
  myExploder->explode();

  
#if 0
  // for DEBUG use only
  std::string partDBFile = "output.partDB.FSSCovBlue";
  scenScePtr_->printPartDataBaseDumpFile(localWitRun, *mpHelper_, nPeriods, nParts, 
                                        partList, partDBFile);

  std::string debugDBFile = "output.debugDB.FSSCovBlue";
  scenScePtr_->printWitDebugFileForDummyParts(localWitRun, *mpHelper_, nPeriods, nParts, 
                                         partList, debugDBFile);
#endif
  


  // f) FSSCov += mrpConsVol  for all raw parts
  //    note: for fssMethod=liberal, FSSCov is already initialized and
  //    we need to actually add the vectors.  Otherwise, its null and we
  //    simply replace.
  for (i=0; i<nParts; i++)  {
    float * mrpConsVol;
    myExploder->mrpConsVol(partList[i], &mrpConsVol);
    if (FSSCov[i] != 0) {
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<nPeriods; t++)
        FSSCov[i][t] += mrpConsVol[t];
      portableFree(mrpConsVol);
    }
    else
      FSSCov[i] = mrpConsVol;
  }



  // f') Roll FSSCov forward if there's any forwardRollableStockVols avail
  //     Note: we only do this with the liberal method.
  if (fssMethod == "liberal") {  
    for (i=0; i<nParts; i++)  {
      witGetPartExists(localWitRun, partList[i], &exists);
      if (exists) {
        if (forwardRollableStockVols[i] != 0) {
          // If there is FSSCov in period t+1 AND non-excess stock in period t,
          // then move some of the FSSCov in period t+1 to period t.  The amount
          // to move is the minimum of FssCov[t+1] and forwardRollableStockVols[t]
          for (t=nPeriods-2; t>=0; t--) {
            float singlePeriodRollFwdQty =
              (forwardRollableStockVols[i][t] > FSSCov[i][t+1])
              ? FSSCov[i][t+1] : forwardRollableStockVols[i][t];
            if (singlePeriodRollFwdQty > 0.0001) {
              FSSCov[i][t+1] -= singlePeriodRollFwdQty;
              FSSCov[i][t]   += singlePeriodRollFwdQty;
#if 0
              // THIS TRICK IS DISABLED
              // Here's a nice little trick ... if you can roll Coverage
              // into an earlier period, then you should also decrement
              // that same quantity from "stock", that way it's not available
              // to the next fss "user".
              forwardRollableStockVols[i][t] -= singlePeriodRollFwdQty;
#endif            
            }
          }
        }
      }
    }
  }








  // Now for fssReq, reset the supplyVols(featuresInFocus) as follows
  //    supplyVol(F') = 0 if  fssFeatureMethod=maximal
  // and if fssFeatureMethod = minimal, then 
  //     supplyVol(F') = focusDemandVol * ratio(GPD,F) if nullSub(Gpd,F) is Global and 
  //                   = supplyVol(nullSub(GPD,F)) * ratio  if nullSub(Gpd,F) is maxWithout
  if (nStandaloneFeatureArcs > 0) {  
    for (k=0; k<focusPartNameList.size(); k++) {
      // get a demand list for each part in the fssSet.  In the current SCE model,
      // there will only be ONE demand stream per part.  We do an assert here.
      // In the future, we may allow "sub-geo" demand.
      int fpNDemands;
      char ** fpDlist;
      witGetPartDemands(originalWitRun, focusPartNameList[k].c_str(), &fpNDemands, &fpDlist);
      assert(fpNDemands == 1);
      int j = 0; // Pulled out of the for below by RW2STL
      for (j=0; j<fpNDemands; j++) {
        std::string focusDemandName = fpDlist[j];
        if (fssFeatureMethod == "maximal") {          
          adjustFssReqSupplyForFeaturesToZero(localWitRun, originalWitRun, 
                                              focusPartNameList[k].c_str(),
                                              focusDemandName.c_str(), zeroVec);
        }
        else {
          float * focusDemandVol;
          witGetDemandDemandVol(originalWitRun, focusPartNameList[k].c_str(), focusDemandName.c_str(), &focusDemandVol);
          adjustFssReqSupplyForFeatures(localWitRun, originalWitRun, 
                                        focusPartNameList[k].c_str(),
                                        focusDemandName.c_str(), focusDemandVol);
          portableFree(focusDemandVol);
        }
        portableFree(fpDlist[j]);
      }
      portableFree(fpDlist);
    }
  }
  

  
  
  // i) Set demandVol(focusDemandSet) = originalDemandVol(focusDemandSet)
  //        demandVol(featuresInFocus) = 0
  for (k=0; k<focusPartNameList.size(); k++) {
    // get a demand list for each part in the fssSet.  In the current SCE model,
    // there will only be ONE demand stream per part.  We do an assert here.
    // In the future, we may allow "sub-geo" demand.
    int fpNDemands;
    char ** fpDlist;
    std::string focusPartName = focusPartNameList[k];    
    witGetPartDemands(originalWitRun, focusPartName.c_str(), &fpNDemands, &fpDlist);
    assert(fpNDemands == 1);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<fpNDemands; j++) {
      std::string focusDemandName = fpDlist[j];
      float * focusDemandVol;
      witGetDemandDemandVol(originalWitRun, focusPartName.c_str(), focusDemandName.c_str(), &focusDemandVol);      
      witSetDemandDemandVol(localWitRun, focusPartName.c_str(), focusDemandName.c_str(), focusDemandVol);
      portableFree(focusDemandVol);
      if (nStandaloneFeatureArcs > 0) {        
        adjustFssReqDemandForFeaturesToZero(localWitRun, originalWitRun, 
                                            focusPartNameList[k].c_str(),
                                            focusDemandName.c_str(), zeroVec);
      }
      portableFree(fpDlist[j]);
    }
    portableFree(fpDlist);
  }
  
  
  // j) Run full Explode mrp
  myExploder->explode();
  
  // k) FSSReq = mrpConsVol (from full explode mrp)
  for (i=0; i<nParts; i++)  {
    myExploder->mrpConsVol(partList[i], &(FSSReq[i]));
  }

  // i) Set demandVol(focusDemandSet) = 0
  for (k=0; k<focusPartNameList.size(); k++) {
    // get a demand list for each part in the fssSet.  In the current SCE model,
    // there will only be ONE demand stream per part.  We do an assert here.
    // In the future, we may allow "sub-geo" demand.
    int fpNDemands;
    char ** fpDlist;
    std::string focusPartName = focusPartNameList[k];    
    witGetPartDemands(originalWitRun, focusPartName.c_str(), &fpNDemands, &fpDlist);
    assert(fpNDemands == 1);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<fpNDemands; j++) {
      std::string focusDemandName = fpDlist[j];
      witSetDemandDemandVol(localWitRun, focusPartName.c_str(), focusDemandName.c_str(), zeroVec);
      portableFree(fpDlist[j]);
    }
    portableFree(fpDlist);
  }
  

#if 0
  // for DEBUG USE only
  std::string partDBFile2 = "output.partDB.FSSReqBlue";
  scenScePtr_->printPartDataBaseDumpFile(localWitRun, *mpHelper_, nPeriods, nParts, 
                                        partList, partDBFile2);

  std::string debugDBFile2 = "output.debugDB.FSSReqBlue";
  scenScePtr_->printDummyPartDataBaseDumpFile(localWitRun, *mpHelper_, nPeriods, nParts, 
                                         partList, debugDBFile2);
#endif


  portableFree(zeroVec);

  (*FSSCovPtr) = FSSCov;
  (*FSSReqPtr) = FSSReq;  
}


int
LgFrSceFssMgr::isFssSetSatisfiedWithinHorizon(WitRun *  theWitRun,
                                              LgFrStringVector & focusPartNameList,
                                              int fssHorizon)
{
  // Check the fssHorizon !!
  // if ALL the demands within the set are satisfied on time within the
  // fssHorizon, then skip this set with an informational message.
  int allDemandInSetSatisfied = 1;
  int k = 0; // Pulled out of the for below by RW2STL
  for (k=0; k<focusPartNameList.size(); k++) {
    // get a demand list for each part in the fssSet.  In the current SCE model,
    // there will only be ONE demand stream per part.  We do an assert here.
    // In the future, we may allow "sub-geo" demand.
    int fpNDemands;
    char ** fpDlist;
    witGetPartDemands(theWitRun, focusPartNameList[k].c_str(), &fpNDemands, &fpDlist);
    assert(fpNDemands == 1);
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<fpNDemands; j++) {
      std::string focusDemandName = fpDlist[j];
      float * focusShipVol;
      float * focusDemandVol;        
      witGetDemandShipVol(theWitRun, focusPartNameList[k].c_str(), focusDemandName.c_str(), &focusShipVol);
      witGetDemandDemandVol(theWitRun, focusPartNameList[k].c_str(), focusDemandName.c_str(), &focusDemandVol);
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=0; t<=fssHorizon; t++) {
        if (focusShipVol[t] < focusDemandVol[t]) {
          allDemandInSetSatisfied = 0;
          break; 
        }
      }
      portableFree(focusShipVol);
      portableFree(focusDemandVol);          
      portableFree(fpDlist[j]);
    }
    portableFree(fpDlist);
    // if you've found one demand in the set that is unsatisfied then there
    // is no reason to check for more ... you gotta run fss
    if (! allDemandInSetSatisfied)
      break;
  }
  return allDemandInSetSatisfied;
}






#if 0


// ==================================================================
// ==================================================================
// ==================================================================
// ==================================================================


    
  
// ==========================================================
// ======== Green =====
//
// Compute FSSCov:
//
// -- STEP 1: Compute Supply available for Focus after netting out  ---
//            the supply consumed by all the Non-focus demand
// a) Init FSSCov = excessVol
// b) Set supplyVol = 0 for all parts
// b') supplyVol(F) += shipVol(FocusDemand) * ratio(FocusDemand,F)
//     for all features F that hang off of FocusDemand (the GPD part)
// c) Set demandVol = 0 FOR ALL demands,
// d) Set demandVol(FocusDemand) = shipVol(FocusDemand)
// d') Set demandVol(GPD,F) += shipVol(GPD,F) 
//                            + ratio(GPD,F) * (shipVol(GPD) - SPV(GPD,F))
// e) run witMrp
// f) FSSCov += mrpConsVol  for all raw parts
// f') Roll FSSCov forward if there's any non-excess stockVol avail
//
// Compute FSSReq:
// g) Set supplyVol = excessVol + reqVol for ALL parts
// g') NOTE only do this step if fssFeatureMethod == "minimal"
// g') supplyVol(F') = focusDemandVol * ratio(GPD,F) if nullSub(Gpd,F) is Global
// g')               = supplyVol(nullSub(GPD,F)) * ratio  if nullSub(Gpd,F) is maxWithout
//
// h) Set demandVol = 0 for all demands
// i) Set demandVol = original demandVol for focus demand only
// j) Run full Explode MRP
// k) FSSReq = totalDemand (ie, mrpConsVol from full explode mrp)
// ==========================================================  


#endif





   // This is clearly a bit kludgy.  We need to read the next "set" of
    // records.  However, we don't know that we're at the end of the set
    // until we've started the next one.  SO, remeber, the last record
    // and save it, so that next time you can start the search with that
    // last record.
void
LgFrSceFssMgr::getNextFssSet(LgFrStringVector & focusPartNameList,
                             WitRun *  theWitRun,
                             std::string & theFssSetName,
                             std::string & fssSetNameReadFromLastTime,
			     std::string & partReadFromlastTime,
                             std::string & FSSFile,
                             std::ifstream & fssInFile,
                             std::ofstream & scrubOutFile,
			     long & totalRec,
			     long & goodRec,
			     long & numDistinctSetIDs,
			     long & badRec,
			     long & commentRec)
{
  std::string dataLine;
  int result;
  std::string plannerPart;
  std::string geo;
  std::string pp;
  std::string g;
  std::string setId;
  LgFrSceFlatFileMgr & flatFileManager =  scenScePtr_->flatFileMgr();
  
  LgFrSceScrubber * myScrubber = scenScePtr_->scrubber();
  
  // start by emtpying the part List.  You ALWAYS wnat to
  // start with a clean one.
  focusPartNameList.clear();

  // Before you read any records, check to see if you've
  // got one from last time.  IF you do, then pop that
  // sucker onto the list.
  if (! fssSetNameReadFromLastTime.empty()) {
    theFssSetName = fssSetNameReadFromLastTime;
    focusPartNameList.push_back(partReadFromlastTime);
  }

  // reset fssSetNameReadFromLastTime and partReadFromlastTime
  // to null strings.  Otherwise you could get into a reaal
  // long loop
  fssSetNameReadFromLastTime = "";
  partReadFromlastTime = "";
  
  long lineNo = 0;
  while(fssInFile.peek() != EOF) {
    std::getline( fssInFile, dataLine );
    lineNo++;
    // keep track of total number of lines read
    totalRec++;

    //  skip blanks or comments *
    dataLine = sce::strip( dataLine );
    if ((dataLine.empty()) || (dataLine[0] == '*')) {
      commentRec++;
      continue;
    }
    SCETokenizer next(dataLine);



    
    // get the next record
    setId = flatFileManager.nextQuotedString(next, result, FSSFile, dataLine, 
                                     lineNo, PURELY_OPTIONAL);
    setId.insert(0, "\"");
    setId.append("\"");
    
    plannerPart = flatFileManager.nextQuotedString(next, result, FSSFile, dataLine, 
                                           lineNo, MANDATORY);
    geo = flatFileManager.nextQuotedString(next, result, FSSFile, dataLine, lineNo, MANDATORY);

    // check to see that the part/geo entered is valid.
    // Note: we skip it and read another record even if the setId
    // has changed.  It really doesn't matter whether you reject it now
    // or reject it later.
    std::string fullWitPart = mpHelper_->geoPlannerDemandPartName(plannerPart, geo);
    if (! mpHelper_->isPartSpecialGeoPlannerDemandPart(theWitRun,
                                                       fullWitPart,
                                                       pp,g)) {
      MclFacility * theErrFacil = scenScePtr_->getErrFacility();
      (*theErrFacil)("InvalidPartFSS",MclArgList() 
		     << FSSFile 
		     << (int)lineNo 
		     << plannerPart 
		     << geo 
		     << dataLine);
      badRec++;
      continue;      
    }

    // check to see that the geoPlannerDemandPart has exactly one demand on it.
    // if not, then its something that had a geoSource record, but did not have
    // a demand record for it.
    int fpNDemands;
    char ** fpDlist;
    witGetPartDemands(theWitRun, fullWitPart.c_str(), &fpNDemands, &fpDlist);

    // free this memory right away, we only need fpNDemands
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<fpNDemands; j++) {
      portableFree(fpDlist[j]);
    }
    portableFree(fpDlist);

    if (fpNDemands == 0) {
      MclFacility * theErrFacil = scenScePtr_->getErrFacility();
      (*theErrFacil)("NoDemandFSS",MclArgList() 
		     << FSSFile 
		     << (int)lineNo 
		     << plannerPart 
		     << geo);
      badRec++;
      continue;      
    }
    

    // if theFssSetName was null, then this is the first time you've
    // been here.
    if (theFssSetName.empty()) {
      theFssSetName = setId;
      numDistinctSetIDs++;
    }

    // if the new setId is the same as the "current" one, then
    // pop that part onto the list
    if (setId == theFssSetName) {
      focusPartNameList.push_back(fullWitPart);
    }
    // otherwise, you've found a new set.  So, remember the setId
    // and part that you've just read and return the calling method.
    else {
      partReadFromlastTime = fullWitPart;
      fssSetNameReadFromLastTime = setId;
      numDistinctSetIDs++;  
      goodRec++;
      return;
    }
    goodRec++;

    // put it over to the scrubber
    if (myScrubber->scrubMode()) {
      scrubOutFile << dataLine << std::endl;
    }


  }
  // well, if you get here, you've hit an EOF

}




// + Singleton scheme is now implemented using the set approach.  We 
// + Get the next set based on the current partIndex.  Each time you request 
// + the next set, the partIndex is incremented and we simply search for the 
// + next GPD part and make it into a set of cardinality 1.  The setid is
// + ignored (set to "")
void
LgFrSceFssMgr::getNextSingletonFssSet(LgFrStringVector & focusPartNameList,
                                      WitRun *    theWitRun,
                                      int    &    singletonPartIndex,
                                      std::string & theFssSetName,
                                      int         nParts,
                                      char **     partList)
{
  std::string plannerPart;
  std::string geo;
  
  
  // start by emtpying the part List.  You ALWAYS want to
  // start with a clean one.
  focusPartNameList.clear();

  long lineNo = 0;
  while(++singletonPartIndex < nParts) {
    // find the next GPD part, add it to the set and return
    std::string fullWitPart(partList[singletonPartIndex]);
    if (mpHelper_->isPartSpecialGeoPlannerDemandPart(theWitRun,
                                                     fullWitPart,
                                                     plannerPart, geo)) {

      // check to see that the geoPlannerDemandPart has exactly one demand on it.
      // if not, then its something that had a geoSource record, but did not have
      // a demand record for it.  Skip it!
      int fpNDemands;
      char ** fpDlist;
      witGetPartDemands(theWitRun, fullWitPart.c_str(), &fpNDemands, &fpDlist);
      if (fpNDemands == 1) {
        focusPartNameList.push_back(fullWitPart);
        theFssSetName = "\"" + plannerPart + "\",\"" + geo + "\"";
        int j = 0; // Pulled out of the for below by RW2STL
        for (j=0; j<fpNDemands; j++) {
          portableFree(fpDlist[j]);
        }
        portableFree(fpDlist);
        return;
      }
      else {
        int j = 0; // Pulled out of the for below by RW2STL
        for (j=0; j<fpNDemands; j++) {
          portableFree(fpDlist[j]);
        }
        portableFree(fpDlist);
      }      
    }
  }
}



// default constructor:
LgFrSceFssMgr::LgFrSceFssMgr()
  :   theWitRun_ (0),
      theCal_(0),
      mpHelper_(0),
      scenScePtr_(0)
{
    // all the work is done in initializer
}


// fairly general constructor:
LgFrSceFssMgr::LgFrSceFssMgr(WitRun * const theWitRun, 
                             LgFrCalendar * theCal,
                             LgFrMultiPlantHelper * mpHelper,
                             LgFrScenarioForSce * scenScePtr)
  :   theWitRun_ (theWitRun),
      theCal_(theCal),
      mpHelper_(mpHelper),
      scenScePtr_(scenScePtr)
{
    // all the work is done in initializer
}

  
  

// copy constructor -- need to work out (?)
LgFrSceFssMgr::LgFrSceFssMgr(const LgFrSceFssMgr& source)
  :   theWitRun_ (source.theWitRun_),
      theCal_(source.theCal_),
      mpHelper_(source.mpHelper_),
      scenScePtr_(source.scenScePtr_)
{
  // nothing to do
}

// assignment operator
LgFrSceFssMgr&
LgFrSceFssMgr::operator=(const LgFrSceFssMgr& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    theWitRun_ = rhs.theWitRun_;
    theCal_ = rhs.theCal_;
    mpHelper_ = rhs.mpHelper_;
    scenScePtr_ = rhs.scenScePtr_;
  }
  return *this;
}

// destructor
LgFrSceFssMgr::~LgFrSceFssMgr()
{
  // nothing to do, 
}




// =================================================================== 
int
LgFrSceFssMgr::getForwardRollableStockSchedule(
    WitRun  * theWitRun,
    int       nParts, 
    char   ** partList, 
    float *** forwardRollableStockVolsPtr)
{

    int i, t;
    int supplyNonNull;
    int nPeriods;

    witGetNPeriods(theWitRun, &nPeriods);

    float ** forwardRollableStockVols = (float **) malloc (nParts * sizeof(float *));
    assert(forwardRollableStockVols != 0);

    for (i=0; i<nParts; i++)   {
      forwardRollableStockVols[i] = 0;
      bool vectorNonNull = false;
      witAttr category;
      float * stockVol;
      float * excessVol;
      witGetPartCategory(theWitRun, partList[i], &category);
      // if there's any stockVol that you can roll forward, do it
      if (category != WitCAPACITY) {
        witGetPartStockVol(theWitRun, partList[i], &stockVol);
        witGetPartExcessVol(theWitRun, partList[i], &excessVol);
        float cumExcess = 0.0;
        int t = 0; // Pulled out of the for below by RW2STL
        for (t=0; t<nPeriods; t++) {
          cumExcess += excessVol[t];
          stockVol[t] -= cumExcess;
          if (stockVol[t] > 0.00001)
            vectorNonNull = true;
        }
        if (vectorNonNull)
          forwardRollableStockVols[i] = stockVol;
        else
          portableFree(stockVol);
        portableFree(excessVol);
      }
    }

    (*forwardRollableStockVolsPtr) = forwardRollableStockVols;
    return 0;
}
 
// =================================================================== 
int
LgFrSceFssMgr::getSupplyVols(
    WitRun  * theWitRun,
    int       nParts, 
    char   ** partList, 
    float *** supplyVolsPtr)
{

    int i, t;
    int supplyNonNull;
    int numSupplyVols = 0;
    int nPeriods;

    float * supplyVol;
    float ** originalSupplyVols;

    witGetNPeriods(theWitRun, &nPeriods);

    originalSupplyVols = (float **) malloc (nParts * sizeof(float *));
    assert(originalSupplyVols != 0);

    for (i=0; i<nParts; i++)   {
	supplyNonNull = 0;
	witGetPartSupplyVol(theWitRun, partList[i], &supplyVol);
	for (t=0; t<nPeriods; t++) {
	    if (supplyVol[t] > 0.00001) {
		supplyNonNull = 1;
		numSupplyVols++;
		break;
	    }
	}
	if (supplyNonNull)   
	    originalSupplyVols[i] = supplyVol;
	else {
	    originalSupplyVols[i] = 0;
	    portableFree(supplyVol);
	}

    }
    
    

    (*supplyVolsPtr) = originalSupplyVols;
    return(numSupplyVols);
}




// =================================================================== 
void
LgFrSceFssMgr::getDemandLists(
    WitRun   * theWitRun,
    int        nParts, 
    char    ** partList, 
    int     ** nDemandsPtr,
    char  **** demandListPtr)
{

    char ** dList;
    char *** demandList;
    int i,j,k,t;
    int nPeriods;
    int * nDemands;

    witGetNPeriods(theWitRun, &nPeriods);


    // malloc a vector to be a vector of pointers to demandName lists
    demandList = (char ***) malloc (nParts * sizeof(char **));
    assert(demandList != 0);
    // malloc a vector to be a vector of ints for storing the
    // number of demands per part
    nDemands = (int *) malloc (nParts * sizeof(int));
    assert(nDemands != 0);

    for (i=0; i<nParts; i++)  {
	demandList[i] = 0;
	nDemands[i] = 0;
    }


    for (i=0; i<nParts; i++)   {
	witGetPartDemands(theWitRun, partList[i], &(nDemands[i]), &dList);	
	if (nDemands[i] > 0)   {
	    demandList[i] = dList;
	}
    }

    (*demandListPtr) = demandList;    
    (*nDemandsPtr) = nDemands;
}


// =================================================================== 
void
LgFrSceFssMgr::getDemandVols(
    WitRun   * theWitRun,
    int        nParts, 
    char    ** partList, 
    int      * nDemands,
    char   *** dList,
    float **** demandVolsPtr)
{

    float *** demandVols;
    float **  dvolList;
    float * dVol;
    int i,j,k,t;
    int nPeriods;

    witGetNPeriods(theWitRun, &nPeriods);


// malloc a vector to be a vector of demandVols 
    demandVols = (float ***) malloc (nParts * sizeof(float **));
    assert(demandVols != 0);
    for (i=0; i<nParts; i++)
	demandVols[i] = 0;


    for (i=0; i<nParts; i++)   {
	if (nDemands[i] > 0)   {
	    dvolList = (float **) malloc(nDemands[i] * sizeof(float *));
	    demandVols[i] = dvolList;
	    assert(dvolList != 0);
	    for (j=0; j<nDemands[i]; j++)   {
		witGetDemandDemandVol(theWitRun, partList[i], dList[i][j], &dVol);
		dvolList[j] = dVol;
	    }
	}
    }

    (*demandVolsPtr) = demandVols;    

}



// =================================================================== 
void
LgFrSceFssMgr::getShipVols(
    WitRun   * theWitRun, 
    int        nParts, 
    char    ** partList, 
    int      * nDemands,
    char   *** dList,
    float **** shipVolsPtr)
{

    float *** shipVols;
    float **  svolList;
    float * svol;
    int i,j,k,t;
    int nPeriods;
    witBoolean postProcessed;
    

    witGetNPeriods(theWitRun, &nPeriods);


// malloc a vector to be a vector of shipVols 
    shipVols = (float ***) malloc (nParts * sizeof(float **));
    assert(shipVols != 0);
    for (i=0; i<nParts; i++)
	shipVols[i] = 0;


    for (i=0; i<nParts; i++)   {
	if (nDemands[i] > 0)   {
	    svolList = (float **) malloc(nDemands[i] * sizeof(float *));
	    shipVols[i] = svolList;
	    assert(svolList != 0);
	    for (j=0; j<nDemands[i]; j++)   {
		witGetDemandShipVol(theWitRun, partList[i], dList[i][j], &svol);
		svolList[j] = svol;
	    }
	}
    }

    (*shipVolsPtr) = shipVols;    

}



/* =================================================================== */
long
LgFrSceFssMgr::printFssGeneralFile(WitRun * theWitRun,
                                   int      nParts,
                                   char  ** partList,
                                   std::string & setID,
                                   float ** FSSReq,
                                   float ** FSSCov,
                                   std::ofstream & fssOutFile)   // output stream (already opened)        
{ 
  long goodRec = 0;
  int i, t;
  float cumTot;
  witAttr category;
  int nBom;
  int nPeriods;
  int isPartShort;

  float * fssReq;
  float * fssCov;
  LgFrSceFlatFileMgr & flatFileManager =  scenScePtr_->flatFileMgr();

  witGetNPeriods(theWitRun, &nPeriods);

    
  float * FSSDelta = new float[nPeriods];
  float * zeroVec = new float[nPeriods];
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0;

  for (i=0; i<nParts; i++) {
    std::string thePart(partList[i]);
    
    // ignore any special SCE dummy parts (for now)
    if (! mpHelper_->isPartNormal(theWitRun, thePart))
      continue;

    if (FSSReq[i] != 0)
      fssReq = FSSReq[i];
    else
      fssReq = zeroVec;

    if (FSSCov[i] != 0)
      fssCov = FSSCov[i];
    else
      fssCov = zeroVec;
    

    // only report on PCF parts
    if (! mpHelper_->isPartPcf(theWitRun, thePart))
      continue;
    



    // determine if the part is short
    isPartShort = 0;
    cumTot = 0.0;
    for (t=0; t<nPeriods; t++)   {
      cumTot += (fssCov[t] - fssReq[t]);
      if (cumTot < -0.0001)   {
        isPartShort = 1;
        break;
      }
    }
    
    // ignore components that aren't short 	    
    if (! isPartShort)
      continue;
      
    //  write the total Demand due to Focus: FSSReq
    fssOutFile << setID << ",\""
               << mpHelper_->partname(thePart) << "\",\""
               << mpHelper_->pdf(thePart) << "\","
               << "\"FSSReq\"";
    flatFileManager.writeCsvFloatVector(fssOutFile, nPeriods, fssReq, 0, 15);
    goodRec++;

    //  write the total Coverage available for Focus: FSSCoverage
    fssOutFile << setID << ",\""
               << mpHelper_->partname(thePart) << "\",\""
               << mpHelper_->pdf(thePart) << "\","
               << "\"FSSCoverage\"";
    flatFileManager.writeCsvFloatVector(fssOutFile, nPeriods, fssCov, 0, 15);
    goodRec++;
        

    //  Write out the Projected On-Hand */
    for (t=0; t<nPeriods; t++)   {
      FSSDelta[t] = fssReq[t] - fssCov[t];
    }
    fssOutFile << setID << ",\""
               << mpHelper_->partname(thePart) << "\",\""
               << mpHelper_->pdf(thePart) << "\","
               << "\"FSSDelta\"";
    flatFileManager.writeCsvFloatVector(fssOutFile, nPeriods, FSSDelta, 0, 15);
    goodRec++;  
  }
  delete [] FSSDelta;
  delete [] zeroVec;

  return goodRec;
}


long
LgFrSceFssMgr::printModifiedFssFile(WitRun * theWitRun,
                                    int      nParts,
                                    char  ** partList,
                                    std::string & setID,
                                    float ** FSSReq,
                                    float ** FSSCov,
                                    std::ofstream & fssOutFile)   // output stream (already opened)        
{ 
  long goodRec = 0;

  int i, t;
  float cumTot;
  witAttr category;
  int nBom;
  int nPeriods;
  int isPartShort;

  float * fssReq;
  float * fssCov;
  LgFrSceFlatFileMgr & flatFileManager =  scenScePtr_->flatFileMgr();  

  witGetNPeriods(theWitRun, &nPeriods);

    
  float * FSSDelta = new float[nPeriods];
  float * zeroVec = new float[nPeriods];
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0;

  for (i=0; i<nParts; i++) {
    std::string thePart(partList[i]);
    
    // ignore any special SCE dummy parts (for now)
    if (! mpHelper_->isPartNormal(theWitRun, thePart))
      continue;

    if (FSSReq[i] == 0 && FSSCov[i] == 0)
      continue;

    if (FSSReq[i] != 0)
      fssReq = FSSReq[i];
    else
      fssReq = zeroVec;

    if (FSSCov[i] != 0)
      fssCov = FSSCov[i];
    else
      fssCov = zeroVec;
    


    // determine if the part is short
    isPartShort = 0;
    cumTot = 0.0;
    for (t=0; t<nPeriods; t++)   {
      cumTot += (fssCov[t] - fssReq[t]);
      if (cumTot < -0.0001)   {
        isPartShort = 1;
        break;
      }
    }
    
    // ignore components that aren't short 	    
    if (! isPartShort)
      continue;
      
    //  write the total Demand due to Focus: FSSReq
    fssOutFile << "\""
               << setID << "\",\""
               << mpHelper_->partname(thePart) << "\",\""
               << mpHelper_->pdf(thePart) << "\","
               << "\"FSSReq\"";
    flatFileManager.writeCsvFloatVector(fssOutFile, nPeriods, fssReq, 0, 15);
    goodRec++;

    //  write the total Coverage available for Focus: FSSCoverage
    fssOutFile << "\""
               << setID << "\",\""
               << mpHelper_->partname(thePart) << "\",\""
               << mpHelper_->pdf(thePart) << "\","
               << "\"FSSCoverage\"";
    flatFileManager.writeCsvFloatVector(fssOutFile, nPeriods, fssCov, 0, 15);
    goodRec++;
        

    //  Write out the Projected On-Hand */
    for (t=0; t<nPeriods; t++)   {
      FSSDelta[t] = fssReq[t] - fssCov[t];
    }
    fssOutFile << "\""
               << setID << "\",\""
               << mpHelper_->partname(thePart) << "\",\""
               << mpHelper_->pdf(thePart) << "\","
               << "\"FSSDelta\"";
    flatFileManager.writeCsvFloatVector(fssOutFile, nPeriods, FSSDelta, 0, 15);
      
    goodRec++;
  }
  delete [] FSSDelta;
  delete [] zeroVec;

  return goodRec;
}


void 
LgFrSceFssMgr::adjustFssCovSupplyForFeatures(WitRun * tempWitRun,
                                             WitRun * originalWitRun,
                                             const char  * focusPart,
                                             const char  * focusDemand,
                                             float * focusShipVol)
{
  LgFrMultiPlantHelper & mpHelper = scenScePtr_->multiPlantHelper();

  std::string plannerPart;
  std::string geo;
  // first test to see that the focus part is indeed a GPD part (otherwise
  // it could never have any features.
  if ( ! mpHelper.isPartSpecialGeoPlannerDemandPart(originalWitRun, focusPart,
                                                  plannerPart, geo)) {
    return;
  }
  
  std::string plannerTopLevelPart(focusPart);
  
  // Now loop through the bom arcs searching for a child with
  // a nullSubstitute 
  int nBom;
  witGetOperationNBomEntries(originalWitRun, plannerTopLevelPart.c_str(), &nBom);
  int k = 0; // Pulled out of the for below by RW2STL
  for (k=0; k<nBom; k++)  {
    char * fullWitFeaturePartName;
    witGetBomEntryConsumedPart(originalWitRun, plannerTopLevelPart.c_str(), k, &fullWitFeaturePartName);

    if (! mpHelper.isPartSpecialStandaloneFeature(originalWitRun, fullWitFeaturePartName)) {
      portableFree(fullWitFeaturePartName);
      continue;
    }
      //  add the supplyVol times the usageRate
      // (over the effectivity range only!)

    float ratio;
    witGetBomEntryUsageRate(originalWitRun, plannerTopLevelPart.c_str(), k, &ratio);
    int early, late;
    witGetBomEntryEarliestPeriod(originalWitRun, plannerTopLevelPart.c_str(), k, &early);
    witGetBomEntryLatestPeriod(originalWitRun, plannerTopLevelPart.c_str(), k, &late);
    float * supplyVol;
    witGetPartSupplyVol(tempWitRun, fullWitFeaturePartName, &supplyVol);
    int t = 0; // Pulled out of the for below by RW2STL
    for (t=early; t<=late; t++)
      supplyVol[t] += ratio * focusShipVol[t];
    witSetPartSupplyVol(tempWitRun, fullWitFeaturePartName, supplyVol);
    portableFree(supplyVol);
    
    portableFree(fullWitFeaturePartName);
  }
}



void 
LgFrSceFssMgr::adjustFssCovDemandForFeatures(WitRun * tempWitRun,
                                             WitRun * originalWitRun,
                                             const char  * focusPart,
                                             const char  * focusDemand,
                                             float * focusShipVol)
{
  LgFrMultiPlantHelper & mpHelper = scenScePtr_->multiPlantHelper();

  std::string plannerPart;
  std::string geo;
  // first test to see that the focus part is indeed a GPD part (otherwise
  // it could never have any features.
  if ( ! mpHelper.isPartSpecialGeoPlannerDemandPart(originalWitRun, focusPart,
                                                  plannerPart, geo)) {
    return;
  }
  
  std::string plannerTopLevelPart(focusPart);
  
  // Now loop through the bom arcs searching for a child with
  // a nullSubstitute 
  int nBom;
  witGetOperationNBomEntries(originalWitRun, plannerTopLevelPart.c_str(), &nBom);
  int k = 0; // Pulled out of the for below by RW2STL
  for (k=0; k<nBom; k++)  {
    char * fullWitFeaturePartName;
    witGetBomEntryConsumedPart(originalWitRun, plannerTopLevelPart.c_str(), k, &fullWitFeaturePartName);

    if (! mpHelper.isPartSpecialStandaloneFeature(originalWitRun, fullWitFeaturePartName)) {
      portableFree(fullWitFeaturePartName);
      continue;
    }

    // now see if the fullWitFeaturePartName is indeed a feature, by testing
    // to see if it has a nullSub part as a substitute.  
    int nSubBom;
    witGetBomEntryNSubsBomEntries(originalWitRun, plannerTopLevelPart.c_str(), k, &nSubBom);
    // if it has a sub, check to see that its a special null sub
    if (nSubBom > 0) {
      char * sub;
      witGetSubsBomEntryConsumedPart(originalWitRun, plannerTopLevelPart.c_str(), k, 0, &sub);
      // if it has a null sub, then add the supplyVol times the usageRate
      // (over the effectivity range only!)
      if (mpHelper.isPartSpecialNullSubstitute(originalWitRun, sub)) {
        // Yeah we found one!!
        // Now find the demand associated with this feature,gpd pair
        std::string featureDemandName =
          mpHelper.pureOptionBuildDemandName(plannerTopLevelPart);
        // make sure the demand exists
        std::string pp, g;
        assert(1 == mpHelper.isDemandSpecialPureOptionBuildDemand(tempWitRun, 
                                                                  fullWitFeaturePartName,
                                                                  featureDemandName,
                                                                  pp, g));
        float * featureShipVol;
        float * featureDemandVol;
        witGetDemandDemandVol(tempWitRun, fullWitFeaturePartName,
                              featureDemandName.c_str(), &featureDemandVol);
        witGetDemandShipVol(originalWitRun, fullWitFeaturePartName,
                            featureDemandName.c_str(), &featureShipVol);
        // now get the subProdVol
        float * subProdVol;
        witGetSubsBomEntryProdVol(originalWitRun, plannerTopLevelPart.c_str(), k, 0, &subProdVol);        
        
        float ratio;
        witGetBomEntryUsageRate(originalWitRun, plannerTopLevelPart.c_str(), k, &ratio);
        int early, late;
        witGetBomEntryEarliestPeriod(originalWitRun, plannerTopLevelPart.c_str(), k, &early);
        witGetBomEntryLatestPeriod(originalWitRun, plannerTopLevelPart.c_str(), k, &late);
        // And now, hold your breath, here comes the BIG formula!!
        int t = 0; // Pulled out of the for below by RW2STL
        for (t=early; t<=late; t++)
          featureDemandVol[t] += featureShipVol[t]
                   + (ratio * (focusShipVol[t] - subProdVol[t]));
        witSetDemandDemandVol(tempWitRun, fullWitFeaturePartName,
                              featureDemandName.c_str(), featureDemandVol);
        
        portableFree(featureDemandVol);
        portableFree(featureShipVol);
        portableFree(subProdVol);        
      }
      portableFree(sub);
    }
    portableFree(fullWitFeaturePartName);
  }
}


void 
LgFrSceFssMgr::adjustFssReqDemandForFeaturesToZero(WitRun * tempWitRun,
                                                   WitRun * originalWitRun,
                                                   const char  * focusPart,
                                                   const char  * focusDemand,
                                                   float * zeroVec)
{
  LgFrMultiPlantHelper & mpHelper = scenScePtr_->multiPlantHelper();

  std::string plannerPart;
  std::string geo;
  // first test to see that the focus part is indeed a GPD part (otherwise
  // it could never have any features.
  if ( ! mpHelper.isPartSpecialGeoPlannerDemandPart(originalWitRun, focusPart,
                                                  plannerPart, geo)) {
    return;
  }
  
  std::string plannerTopLevelPart(focusPart);
  
  // Now loop through the bom arcs searching for a child with
  // a nullSubstitute 
  int nBom;
  witGetOperationNBomEntries(originalWitRun, plannerTopLevelPart.c_str(), &nBom);
  int k = 0; // Pulled out of the for below by RW2STL
  for (k=0; k<nBom; k++)  {
    char * fullWitFeaturePartName;
    witGetBomEntryConsumedPart(originalWitRun, plannerTopLevelPart.c_str(), k, &fullWitFeaturePartName);

    if (! mpHelper.isPartSpecialStandaloneFeature(originalWitRun, fullWitFeaturePartName)) {
      portableFree(fullWitFeaturePartName);
      continue;
    }

    
    // now see if the fullWitFeaturePartName is indeed a feature, by testing
    // to see if it has a nullSub part as a substitute.  
    int nSubBom;
    witGetBomEntryNSubsBomEntries(originalWitRun, plannerTopLevelPart.c_str(), k, &nSubBom);
    // if it has a sub, check to see that its a special null sub
    if (nSubBom > 0) {
      // abc
      char * sub;
      witGetSubsBomEntryConsumedPart(originalWitRun, plannerTopLevelPart.c_str(), k, 0, &sub);
      // if it has a null sub, then add the supplyVol times the usageRate
      // (over the effectivity range only!)
      if (mpHelper.isPartSpecialNullSubstitute(originalWitRun, sub)) {
        // Yeah we found one!!
        // Now find the demand associated with this feature,gpd pair
        std::string featureDemandName =
          mpHelper.pureOptionBuildDemandName(plannerTopLevelPart);
        // make sure the demand exists
        std::string pp, g;
        assert(1 == mpHelper.isDemandSpecialPureOptionBuildDemand(tempWitRun, 
                                                                  fullWitFeaturePartName,
                                                                  featureDemandName,
                                                                  pp, g));
        witSetDemandDemandVol(tempWitRun, fullWitFeaturePartName,
                              featureDemandName.c_str(), zeroVec);
        
      }
      portableFree(sub);
    }
    portableFree(fullWitFeaturePartName);
  }
}


void 
LgFrSceFssMgr::adjustFssReqSupplyForFeatures(WitRun * tempWitRun,
                                             WitRun * originalWitRun,
                                             const char  * focusPart,
                                             const char  * focusDemand,
                                             float * focusDemandVol)
{
  // g') supplyVol(F') = focusDemandVol * ratio(GPD,F) if nullSub(Gpd,F) is Global
  // g')               = supplyVol(nullSub(GPD,F)) * ratio  if nullSub(Gpd,F) is maxWithout
  
  LgFrMultiPlantHelper & mpHelper = scenScePtr_->multiPlantHelper();

  std::string plannerPart;
  std::string geo;
  // first test to see that the focus part is indeed a GPD part (otherwise
  // it could never have any features.
  if ( ! mpHelper.isPartSpecialGeoPlannerDemandPart(originalWitRun, focusPart,
                                                  plannerPart, geo)) {
    return;
  }

  std::string d1, d2, d3;  
  
  std::string plannerTopLevelPart(focusPart);
  
  // Now loop through the bom arcs searching for a child that is a 
  // special feature part
  int nBom;
  witGetOperationNBomEntries(originalWitRun, plannerTopLevelPart.c_str(), &nBom);
  int k = 0; // Pulled out of the for below by RW2STL
  for (k=0; k<nBom; k++)  {
    char * fullWitFeaturePartName;
    witGetBomEntryConsumedPart(originalWitRun, plannerTopLevelPart.c_str(), k, &fullWitFeaturePartName);

    if (! mpHelper.isPartSpecialStandaloneFeature(originalWitRun, fullWitFeaturePartName)) {
      portableFree(fullWitFeaturePartName);
      continue;
    }


    // now see if the fullWitFeaturePartName is indeed a feature, by testing
    // to see if it has a nullSub part as a substitute.  
    int nSubBom;
    witGetBomEntryNSubsBomEntries(originalWitRun, plannerTopLevelPart.c_str(), k, &nSubBom);
    // if it has a sub, check to see that its a special null sub
    if (nSubBom > 0) {
      char * sub;
      witGetSubsBomEntryConsumedPart(originalWitRun, plannerTopLevelPart.c_str(), k, 0, &sub);
      // if it has a null sub, then add the supplyVol times the usageRate
      // (over the effectivity range only!)
      if (mpHelper.isPartSpecialNullSubstitute(originalWitRun, sub)) {
        // Yeah we found one!!
        float * currentFeatSupplyVol;
        witGetPartSupplyVol(tempWitRun, fullWitFeaturePartName, &currentFeatSupplyVol);
        float ratio;
        witGetBomEntryUsageRate(originalWitRun, plannerTopLevelPart.c_str(), k, &ratio);
        int early, late;
        witGetBomEntryEarliestPeriod(originalWitRun, plannerTopLevelPart.c_str(), k, &early);
        witGetBomEntryLatestPeriod(originalWitRun, plannerTopLevelPart.c_str(), k, &late);
        // g') supplyVol(F') = focusDemandVol * ratio(GPD,F) if nullSub(Gpd,F) is Global
        // g')               = supplyVol(nullSub(GPD,F)) * ratio  if nullSub(Gpd,F) is maxWithout
        if (mpHelper.isPartSpecialGlobalNullSubstitute(originalWitRun, sub, d1)) {
          int t = 0; // Pulled out of the for below by RW2STL
          for (t=early; t<=late; t++)
            currentFeatSupplyVol[t] += ratio * focusDemandVol[t];
        }
        else {
          float * maxWithoutSupplyVol;
          // Make sure you get the original supplyVol on the nullSub from the original
          // witrun
          witGetPartSupplyVol(originalWitRun, sub, &maxWithoutSupplyVol);
          int t = 0; // Pulled out of the for below by RW2STL
          for (t=early; t<=late; t++)
            currentFeatSupplyVol[t] += ratio * maxWithoutSupplyVol[t];
          portableFree(maxWithoutSupplyVol);
        }
        witSetPartSupplyVol(tempWitRun, fullWitFeaturePartName, currentFeatSupplyVol);
        portableFree(currentFeatSupplyVol);        
      }
      portableFree(sub);
    }
    portableFree(fullWitFeaturePartName);
  }
}

void 
LgFrSceFssMgr::adjustFssReqSupplyForFeaturesToZero(WitRun * tempWitRun,
                                                   WitRun * originalWitRun,
                                                   const char  * focusPart,
                                                   const char  * focusDemand,
                                                   float * zeroVec)
{
  LgFrMultiPlantHelper & mpHelper = scenScePtr_->multiPlantHelper();

  std::string plannerPart;
  std::string geo;
  // first test to see that the focus part is indeed a GPD part (otherwise
  // it could never have any features.
  if ( ! mpHelper.isPartSpecialGeoPlannerDemandPart(originalWitRun, focusPart,
                                                  plannerPart, geo)) {
    return;
  }

  std::string d1, d2, d3;  
  
  std::string plannerTopLevelPart(focusPart);
  
  // Now loop through the bom arcs searching for a child that is a 
  // special feature part
  int nBom;
  witGetOperationNBomEntries(originalWitRun, plannerTopLevelPart.c_str(), &nBom);
  int k = 0; // Pulled out of the for below by RW2STL
  for (k=0; k<nBom; k++)  {
    char * fullWitFeaturePartName;
    witGetBomEntryConsumedPart(originalWitRun, plannerTopLevelPart.c_str(), k, &fullWitFeaturePartName);

    if (! mpHelper.isPartSpecialStandaloneFeature(originalWitRun, fullWitFeaturePartName)) {
      portableFree(fullWitFeaturePartName);
      continue;
    }

    witSetPartSupplyVol(tempWitRun, fullWitFeaturePartName, zeroVec);
    portableFree(fullWitFeaturePartName);
  }
}


//------------ Added a new method for SCE ODBC access -------

#ifdef SCE_ODBC

void
LgFrSceFssMgr::insertFssSingleSchemeDB(WitRun * theWitRun,
                                       int      nParts,
                                       char  ** partList,
                                       std::string & setID,
                                       float ** FSSReq,
                                       float ** FSSCov)
{ 

  SceFSSSingleSchemeDB fsssDB;
  SceDBLink fsss(fsssDB);
  if (!fsssDB.fDelete())
    fsssDB.deleteTbl();
 
  int i, t;
  float cumTot;
  witAttr category;
  int nBom;
  int nPeriods;
  int isPartShort;

  float * fssReq;
  float * fssCov;
  LgFrSceFlatFileMgr & flatFileManager =  scenScePtr_->flatFileMgr();

  witGetNPeriods(theWitRun, &nPeriods);
  
  fsss.nPeriods(nPeriods);
    
  float * FSSDelta = new float[nPeriods];
  float * zeroVec = new float[nPeriods];
  for (t=0; t<nPeriods; t++)
    zeroVec[t] = 0;

  for (i=0; i<nParts; i++) {
    std::string thePart(partList[i]);
    
    // ignore any special SCE dummy parts (for now)
    if (! mpHelper_->isPartNormal(theWitRun, thePart))
      continue;

    if (FSSReq[i] != 0)
      fssReq = FSSReq[i];
    else
      fssReq = zeroVec;

    if (FSSCov[i] != 0)
      fssCov = FSSCov[i];
    else
      fssCov = zeroVec;
 
    // only report on PCF parts
    if (! mpHelper_->isPartPcf(theWitRun, thePart))
      continue;
 
    // determine if the part is short
    isPartShort = 0;
    cumTot = 0.0;
    for (t=0; t<nPeriods; t++)   {
      cumTot += (fssCov[t] - fssReq[t]);
      if (cumTot < -0.0001)   {
        isPartShort = 1;
        break;
      }
    }
    
    // ignore components that aren't short 	    
    if (! isPartShort)
      continue;
      
    //  Write out the Projected On-Hand */
    for (t=0; t<nPeriods; t++)   {
      FSSDelta[t] = fssReq[t] - fssCov[t];
    }
  
    // split "SetID" into "plannerPart" and "Geo"
    size_t idx = setID.index(",");
    if (idx !=RW_NPOS)
      {
        std::string plannerPart(setID(1, idx-2));
        std::string geo(setID(idx+2, setID.length()-(idx+3)));
        fsss.insert(SceArgList()
                    << plannerPart.c_str()
                    << geo.c_str()
                    << mpHelper_->partname(thePart)
                    << mpHelper_->pdf(thePart)
                    << SceMakeVec(fssReq, nPeriods)
                    << SceMakeVec(fssCov, nPeriods)
                    << SceMakeVec(FSSDelta, nPeriods));              
      }
   
  }

  fsss.insert();
  
  delete [] FSSDelta;
  delete [] zeroVec;

}

#endif

#ifdef NDEBUG
#undef NDEBUG
#endif



// self-test : can I self test ??
void
LgFrSceFssMgr::test()
{
  
 
 
 }

#endif
