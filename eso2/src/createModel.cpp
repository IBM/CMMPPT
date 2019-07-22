#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif


#include "wit.h"

#include <map>
#include <vector>
#include <algorithm>

#include <cassert>
#include <iostream>
#include <cfloat>
#include <cctype>
#include <cmath>


#include "eso2.h"
#include "globalAttrData.h"
#include "mtmData.h"
#include "applyScaleFactorLocData.h"
#include "featureData.h"
#include "featureXrefData.h"
#include "featureConversionData.h"
#include "featureConversionOverrideData.h"
#include "loosePartData.h"
#include "mtmDemanBomData.h"
#include "altData.h"
#include "altExcludeLocData.h"
#include "altOverrideData.h"
#include "partInterplantData.h"
#include "partInterplantOverrideData.h"
#include "mtmInterplantData.h"
#include "mtmInterplantOverrideData.h"
#include "partDemandData.h"
#include "demanData.h"
#include "demanOverrideData.h"
#include "eso2Problem.h"


typedef std::vector<std::string> StdStringVec;


// Add all interplant moves from the specified mtm & srcLoc
// This is not a recursive function because an mtm is only moved once.
void addAllMtmInterplants(
                    ESO2problem & eso2Prob,
                    std::map<std::string,StdStringVec> srcToDestLocs,
                    const ESO2mtmInterplantData & mtmInterplantFile,
                    const ESO2demanData & demanFile,
                    const ESO2demanOverrideData & demanOverrideFile,
                    const std::string & mtm,
                    const std::string & srcLoc)
{
  int nPeriods = eso2Prob.getNPeriods();
  const StdStringVec & destLocs = srcToDestLocs[srcLoc];

  // Loop once for each destination location
  size_t l=0;
  for ( l=0; l<destLocs.size(); l++ ) {
    std::string destLoc = destLocs[l];
    // does mtm at dest loc exist
    bool mtmExist = eso2Prob.mtmExists(mtm,destLoc);
    
    // If link between srcLoc and destLoc for part does not exist, then add it to model
    if ( !eso2Prob.mtmInterplantExists(mtm,srcLoc,destLoc ) ) {
      
      // Get default move cost and time between srcLoc and destLoc
      float defaultMoveCost = ESO2problem::bigM();
      float defaultMoveTime = 0.0f;
      if ( mtmInterplantFile.keyExists(srcLoc,destLoc) ) {
        defaultMoveCost = mtmInterplantFile.defaultMoveCostAsFloat(srcLoc,destLoc);
        defaultMoveTime = mtmInterplantFile.defaultMoveTimeAsFloat(srcLoc,destLoc);
      }
      else {
        // Default mtm interplant move time and cost does not exist
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: default mtm interplant move time and cost does not exist in mtmInterplantData file" <<std::endl
          <<"   mtmInterplantData filename: " <<mtmInterplantFile.filename().c_str()   <<std::endl
          <<"   srcLoc: " <<srcLoc.c_str()                                   <<std::endl
          <<"   destLoc: " <<destLoc.c_str()                                 <<std::endl
          <<"   defaultMoveCost will be set to: " <<defaultMoveCost          <<std::endl
          <<"   defaultMoveTime will be set to: " <<defaultMoveTime          <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
      }
           
      eso2Prob.addMtmInterplant(mtm,srcLoc,destLoc,defaultMoveCost,defaultMoveTime);
      
      // Get demanufacturing time and cost
      float demanCostDefault = ESO2problem::bigM();
      float demanTimeDefault = (float)nPeriods+1;
      bool usingDefault=true;
      std::vector<float> demanCost = eso2Prob.floatToStlVec(demanCostDefault);
      std::vector<float> demanTime = eso2Prob.floatToStlVec(demanTimeDefault);
      if ( demanFile.keyExists(destLoc) ) {
         demanCost = eso2Prob.floatToStlVec(demanFile.demanCostAsFloat(destLoc));
         demanTime = eso2Prob.floatToStlVec(demanFile.demanTimeAsFloat(destLoc));
         usingDefault=false;
      }   
      // Loop once for each period looking for demanCost and demanTime 
      for ( int p=0; p<nPeriods; p++ ) {
        std::string keyO = demanOverrideFile.key(mtm,destLoc,p);
        if ( demanOverrideFile.keyExists(keyO) ) {
          demanCost[p] = demanOverrideFile.demanCostAsFloat(keyO);
          demanTime[p] = demanOverrideFile.demanTimeAsFloat(keyO);
          usingDefault=false;
        }
      }
      if (usingDefault) {
            // No demanufacturing cost or time specified
            std::cout 
              <<"-------------------------------------------------------"        <<std::endl
              <<"Warning: An mtm interplant does not have a demanufacturing cost or time specified."   <<std::endl
              <<"   mtmInterplant filename: " <<mtmInterplantFile.filename().c_str()                 <<std::endl
              <<"   mtm: " <<mtm.c_str()                                         <<std::endl
              <<"   srcLoc: " <<srcLoc.c_str()                                   <<std::endl
              <<"   destLoc: " <<destLoc.c_str()                                 <<std::endl
              <<"   deman filename: " <<demanFile.filename().c_str()             <<std::endl
              <<"   demanOverride filename: " <<demanOverrideFile.filename().c_str()     <<std::endl
              <<"   A demanCost of " <<demanCostDefault <<" will be used."              <<std::endl
              <<"   A demanTime of " <<demanTimeDefault <<" will be used."              <<std::endl
              <<"-------------------------------------------------------"        <<std::endl;
      }
      
      
      eso2Prob.setMtmDemanCost(mtm,srcLoc, destLoc,demanCost);
      eso2Prob.setMtmDemanTime(mtm,srcLoc, destLoc,demanTime);
      
    } // end adding interplant.

    //if ( !mtmExist ) {
    //  addAllMtmInterplants( eso2Prob, srcToDestLocs, mtmInterplantFile, mtm, destLoc);
    //}
  }  // end for l 
}

void createModel(
                 ESO2problem & eso2Prob, 
                 const std::string & inputDirectory,
                 bool useGlobalAttrFileSettings  )
{
  std::cout <<"Reading input files" <<std::endl;
  
  int nPeriods;      

  // Set up interplant moves info
  typedef std::vector<std::string> StdStringVec;
  //std::map<std::string,StdStringVec> partSrcToDests;
  std::map<std::string,StdStringVec> mtmSrcToDests;
  
  // Read Global Attributes
  std::string globalAttrFileName = inputDirectory + "/globalAttrData.csv";
  ESO2globalAttrData globalAttrFile(globalAttrFileName);
  {      
    std::string title=globalAttrFile.fieldAsString("title","attrValue");
    eso2Prob.setTitle(title);
    
    nPeriods=globalAttrFile.fieldAsInt("nPeriods","attrValue");
    eso2Prob.setNPeriods(nPeriods);

    // Handle period1SupplyPerishPeriod attribute
    {
      bool attrExists = globalAttrFile.keyExists("period1SupplyPerishPeriod");
      if ( !attrExists ) {
        // Priority attribute does not exist
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Error: period1SupplyPerishPeriod not specified."                <<std::endl
          <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
          <<"   valid range: 1<=period1SupplyPerishPeriod<=" <<nPeriods      <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        exit(1);
      }
      
      int period1SupplyPerishPeriod = globalAttrFile.fieldAsInt("period1SupplyPerishPeriod","attrValue");
      if ( period1SupplyPerishPeriod<1 || period1SupplyPerishPeriod>nPeriods ) {
        // period1SupplyPerishPeriod is out of range
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Error: period1SupplyPerishPeriod is out of range."              <<std::endl
          <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
          <<"   period1SupplyPerishPeriod: " <<period1SupplyPerishPeriod     <<std::endl
          <<"   valid range: 1<=period1SupplyPerishPeriod<=" <<nPeriods      <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        exit(1);
      }         

      // Check to see if perishPeriod is to be set using value obtained from globalAttr file.
      // In unitTest this is skipped some many perishPeriod can be run.
      if ( useGlobalAttrFileSettings ) {        
        eso2Prob.setPeriod1SupplyPerishPeriod(period1SupplyPerishPeriod-1);
      }
    } // end of Handle period1SupplyPerishPeriod attribute

    // Handle allowInventoryHoldingAtDemandLocation attribute
    {
      bool attrExists = globalAttrFile.keyExists("allowInventoryHoldingAtDemandLocation");
      bool allowInventoryHoldingAtDemandLocation = false;
      
      if ( !attrExists ) {
      // allowInventoryHoldingAtDemandLocation attribute does not exist
      std::cout 
      <<"-------------------------------------------------------"        <<std::endl
      <<"Error: allowInventoryHoldingAtDemandLocation not specified."    <<std::endl
      <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
      <<"   valid range: 0<=allowInventoryHoldingAtDemandLocation<=1"    <<std::endl
      <<"-------------------------------------------------------"        <<std::endl;
      exit(1);
      }

      if ( attrExists ) {
        int allowInventoryHoldingAtDemandLocationAsInt = globalAttrFile.fieldAsInt("allowInventoryHoldingAtDemandLocation","attrValue");
        if ( allowInventoryHoldingAtDemandLocationAsInt<0 || allowInventoryHoldingAtDemandLocationAsInt>1 ) {
          // allowInventoryHoldingAtDemandLocation is out of range
          std::cout 
            <<"-------------------------------------------------------"                                 <<std::endl
            <<"Error: allowInventoryHoldingAtDemandLocation is out of range."                           <<std::endl
            <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()                           <<std::endl
            <<"   allowInventoryHoldingAtDemandLocation: " <<allowInventoryHoldingAtDemandLocationAsInt <<std::endl
            <<"   valid range: 0<=allowInventoryHoldingAtDemandLocation<=1"                             <<std::endl
            <<"-------------------------------------------------------"                                 <<std::endl;
          exit(1);
        }  
        if (allowInventoryHoldingAtDemandLocationAsInt==1)
          allowInventoryHoldingAtDemandLocation = true;
      }

      // Check to see if allowInventoryHoldingAtDemandLocation is to be set using value obtained from globalAttr file.
      // In unitTest this is skipped some many perishPeriod can be run.
      if ( useGlobalAttrFileSettings ) {        
        eso2Prob.setAllowInventoryHoldingAtDemandLocation(allowInventoryHoldingAtDemandLocation);
      }
    }  // End of Handle allowInventoryHoldingAtDemandLocation attribute


    // Handle removeCycles attribute
    {
      bool attrExists = globalAttrFile.keyExists("removeCycles");
      bool removeCycles = false;
      
      if ( !attrExists ) {
      // removeCycles attribute does not exist
      std::cout 
      <<"-------------------------------------------------------"        <<std::endl
      <<"Error: removeCycles not specified."    <<std::endl
      <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
      <<"   valid range: 0<=removeCycles<=1"    <<std::endl
      <<"-------------------------------------------------------"        <<std::endl;
      exit(1);
      }

      if ( attrExists ) {
        int removeCyclesAsInt = globalAttrFile.fieldAsInt("removeCycles","attrValue");
        if ( removeCyclesAsInt<0 || removeCyclesAsInt>1 ) {
          // removeCycles is out of range
          std::cout 
            <<"-------------------------------------------------------"                                 <<std::endl
            <<"Error: removeCycles is out of range."                           <<std::endl
            <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()                           <<std::endl
            <<"   removeCycles: " <<removeCyclesAsInt <<std::endl
            <<"   valid range: 0<=removeCycles<=1"                             <<std::endl
            <<"-------------------------------------------------------"                                 <<std::endl;
          exit(1);
        }  
        if (removeCyclesAsInt==1)
          removeCycles = true;
      }

      // Check to see if removeCycles is to be set using value obtained from globalAttr file.
      // In unitTest this is skipped some many perishPeriod can be run.
      if ( useGlobalAttrFileSettings ) {        
        eso2Prob.setRemoveCycles(removeCycles);
      }
    }  // End of Handle removeCycles attribute










    // Handle wacScaleFactor & wacScaleFactor1 Attributes
    {
      std::vector<std::string> wacScaleFactorAttrNames;
      wacScaleFactorAttrNames.push_back("wacScaleFactor");
      wacScaleFactorAttrNames.push_back("wac2ScaleFactor");
      std::vector<float> wacScaleFactors;

        // loop once for each scale factor
        for (size_t a=0; a<wacScaleFactorAttrNames.size(); ++a ) {
          bool attrExists = globalAttrFile.keyExists(wacScaleFactorAttrNames[a]);
          if ( !attrExists ) {
            // Priority attribute does not exist
            std::cout 
              <<"-------------------------------------------------------"        <<std::endl
              <<"Error: "+wacScaleFactorAttrNames[a]+" not specified."           <<std::endl
              <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
              <<"-------------------------------------------------------"        <<std::endl;
            exit(1);
          }

          float wacScaleFactor = globalAttrFile.fieldAsFloat(wacScaleFactorAttrNames[a],"attrValue");
          wacScaleFactors.push_back(wacScaleFactor);
        }  // end loop for each scale factor 

        // Set the scale factors.
        eso2Prob.setWacScaleFactors(wacScaleFactors[0],wacScaleFactors[1]);
    } // end handling wacScaleFactor & wacScaleFactor1 Attributes

    // Does useApplyScaleFactorLocData in global attribute file indicate that useApplyScaleFactorLocData should be used?
    std::string useApplyScaleFactorLocData = globalAttrFile.fieldAsString("useApplyScaleFactorLocData","attrValue");
    std::transform(
        useApplyScaleFactorLocData.begin(), 
        useApplyScaleFactorLocData.end(), 
        useApplyScaleFactorLocData.begin(),
        (int(*)(int)) std::toupper);
    if ( useApplyScaleFactorLocData=="N" || useApplyScaleFactorLocData=="NO" ) {
        std::cout <<"Not using useApplyScaleFactorLocData" <<std::endl;
        eso2Prob.setUseApplyScaleFactorLoc(false);
    } else {

        std::cout <<"Using useApplyScaleFactorLocData" <<std::endl;
        eso2Prob.setUseApplyScaleFactorLoc(true);

        // Read applyScaleFactorLoc file
        {  
            std::string applyScaleFactorLocFileName = inputDirectory + "/applyScaleFactorLocData.csv";
            ESO2applyScaleFactorLocData applyScaleFactorLocFile(applyScaleFactorLocFileName);

            ESO2applyScaleFactorLocDataIterator applyScaleFactorLocFileIter(applyScaleFactorLocFile);

            // loop once for each record in partInterplant file
            for ( ; applyScaleFactorLocFileIter()!=NULL; ) {
                std::string plantLoc = applyScaleFactorLocFileIter.plantLoc();
                int demandFileNumber = applyScaleFactorLocFileIter.partDemandFileNumberAsInt();
                eso2Prob.setApplyLocWfmvScaleFactor(plantLoc,demandFileNumber);
            }

        } // end of read applyScaleFactorLoc file
    }  // else part of "if ( useApplyScaleFactorLocData=="N" || useApplyScaleFactorLocData=="NO" )"

    std::vector<std::string> priorities;
    priorities.push_back("mtmPriority");
    priorities.push_back("featurePriority");
    priorities.push_back("loosePartPriority");
    priorities.push_back("mtm2Priority");
    priorities.push_back("feature2Priority");
    priorities.push_back("loosePart2Priority");
    size_t p;
    for ( p=0; p<priorities.size(); ++p ) {
      bool attrExists = globalAttrFile.keyExists(priorities[p]);
      if ( !attrExists ) {
        // Priority attribute does not exist
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Error: " <<priorities[p].c_str() <<" not specified."            <<std::endl
          <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
          <<"   valid range: 1<=" <<priorities[p].c_str() <<"<=4"            <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        exit(1);
      }
      int priority = globalAttrFile.fieldAsInt(priorities[p],"attrValue");
      if ( priority<1 || priority>4 ) {
        // Priority is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Error: " <<priorities[p].c_str() <<" is out of allowed range"   <<std::endl
          <<"   gloabalAttr filename: " <<globalAttrFile.filename().c_str()  <<std::endl
          <<"   " <<priorities[p].c_str() <<": " <<globalAttrFile.fieldAsString(priorities[p],"attrValue").c_str() <<std::endl
          <<"   valid range: 1<=" <<priorities[p].c_str() <<"<=4"            <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        exit(1);
      }
      if ( p==0 ) eso2Prob.setMtmPriority(priority);
      else if ( p==1 )  eso2Prob.setFeaturePriority(priority);
      else if ( p==2 )  eso2Prob.setLoosePartPriority(priority);
      else if ( p==3 )  eso2Prob.setMtm2Priority(priority);
      else if ( p==4 )  eso2Prob.setFeature2Priority(priority);
      else if ( p==5 )  eso2Prob.setLoosePart2Priority(priority);
    }
  }

  // Read part interplant File and set up structures for using data
  std::cout <<"Reading input file: partInterplantData" <<std::endl;
  std::string partInterplantFileName = inputDirectory + "/partInterplantData.csv";
  ESO2partInterplantData partInterplantFile(partInterplantFileName);
  {
    ESO2partInterplantDataIterator partInterplantFileIter(partInterplantFile);
    
    // loop once for each record in partInterplant file
    for ( ; partInterplantFileIter()!=NULL; ) {
      std::string srcLoc = partInterplantFileIter.srcLoc();
      std::string destLoc = partInterplantFileIter.destLoc();
      float defaultMoveCost = partInterplantFileIter.defaultMoveCostAsFloat();
      float defaultMoveTime = partInterplantFileIter.defaultMoveTimeAsFloat();
      eso2Prob.addPartsInterplant(srcLoc,destLoc,defaultMoveCost,defaultMoveTime);      
    }
  }

  // Read mtm interplant File and set up structures for using data
  std::string mtmInterplantFileName = inputDirectory + "/mtmInterplantData.csv";
  ESO2mtmInterplantData mtmInterplantFile(mtmInterplantFileName);
  {
    ESO2mtmInterplantDataIterator mtmInterplantFileIter(mtmInterplantFile);
    
    // loop once for each record in partInterplant file
    for ( ; mtmInterplantFileIter()!=NULL; ) {
      std::string srcLoc = mtmInterplantFileIter.srcLoc();
      std::string destLoc = mtmInterplantFileIter.destLoc();
      mtmSrcToDests[srcLoc].push_back(destLoc);     
    }
  }
  
  
  // Read MTM File, and demanFiles
  {
    std::cout <<"Reading input file: mtmData" <<std::endl;
    
    std::string mtmFileName = inputDirectory + "/mtmData.csv";
    ESO2mtmData mtmFile(mtmFileName);
    ESO2mtmDataIterator mtmFileIter(mtmFile);
    
    std::string demanFileName = inputDirectory + "/demanData.csv";
    ESO2demanData demanFile(demanFileName);
    
    std::string demanOverrideFileName = inputDirectory + "/demanOverrideData.csv";
    ESO2demanOverrideData demanOverrideFile(demanOverrideFileName);
    
    // loop once for each record in mtm file
    for ( ; mtmFileIter()!=NULL; ) {
      std::string mtm = mtmFileIter.mtm();
      std::string plantLoc = mtmFileIter.plantLoc();
      int period = mtmFileIter.periodAsInt();
      float supply = mtmFileIter.supplyAsFloat();
      float wfmv = mtmFileIter.wfmvAsFloat();
      //float demanCost = mtmFileIter.demanCostAsFloat();
      //float demanTime = mtmFileIter.demanTimeAsFloat();
      
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   mtm filename: " <<mtmFile.filename().c_str()                 <<std::endl
          <<"   mtm file line number: " <<mtmFileIter.recNum()               <<std::endl
          <<"   mtm: " <<mtm.c_str()                                         <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }

      // Get demanufacturing time and cost
      float demanCost = ESO2problem::bigM();
      float demanTime = (float)nPeriods+1;
      
      // Add Mtm if it has not been created
      bool addedMtm = false;
      if ( !eso2Prob.mtmExists(mtm,plantLoc) ) {
        
        if ( demanFile.keyExists(plantLoc) ) {
          demanCost = demanFile.demanCostAsFloat(plantLoc);
          demanTime = demanFile.demanTimeAsFloat(plantLoc);
        }
        
        addedMtm=true;
        eso2Prob.addMtm(mtm,plantLoc,demanCost,demanTime);
      }

      {
        std::string keyO = demanOverrideFile.key(mtm,plantLoc,period);
        if ( demanOverrideFile.keyExists(keyO) ) {
          demanCost = demanOverrideFile.demanCostAsFloat(keyO);
          demanTime = demanOverrideFile.demanTimeAsFloat(keyO);
        }
        else {
          if ( demanFile.keyExists(plantLoc) ) {
            demanCost = demanFile.demanCostAsFloat(plantLoc);
            demanTime = demanFile.demanTimeAsFloat(plantLoc);
          }
          else {
            // No demanufacturing cost or time specified
            std::cout 
              <<"-------------------------------------------------------"        <<std::endl
              <<"Warning: An mtm does not have a demanufacturing cost or time specified."   <<std::endl
              <<"   mtm filename: " <<mtmFile.filename().c_str()                 <<std::endl
              <<"   mtm file line number: " <<mtmFileIter.recNum()               <<std::endl
              <<"   mtm: " <<mtm.c_str()                                         <<std::endl
              <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
              <<"   period: " <<period                                           <<std::endl
              <<"   deman filename: " <<demanFile.filename().c_str()             <<std::endl
              <<"   demanOverride filename: " <<demanOverrideFile.filename().c_str()     <<std::endl
              <<"   A demanCost of " <<demanCost <<" will be used."              <<std::endl
              <<"   A demanTime of " <<demanTime <<" will be used."              <<std::endl
              <<"-------------------------------------------------------"        <<std::endl;
          }
        }
      }
      
      eso2Prob.setMtmSupply(mtm,plantLoc,period-1,supply);
      eso2Prob.setMtmWfmv(mtm,plantLoc,period-1,wfmv);
      eso2Prob.setMtmDemanCost(mtm,plantLoc,period-1,demanCost);
      eso2Prob.setMtmDemanTime(mtm,plantLoc,period-1,demanTime);
        
      addAllMtmInterplants(
        eso2Prob, mtmSrcToDests, 
        mtmInterplantFile, demanFile, demanOverrideFile, 
        mtm,plantLoc);
      
    } // end loop reading mtm file record
  } // end of block reading mtm file

  // Read mtmInterplantOverride File
  {
    std::cout <<"Reading input file: mtmInterplantOverrideData" <<std::endl;
    std::string mtmInterplantOverrideFileName = inputDirectory + "/mtmInterplantOverrideData.csv";
    ESO2mtmInterplantOverrideData mtmInterplantOverrideFile(mtmInterplantOverrideFileName);
    ESO2mtmInterplantOverrideDataIterator mtmInterplantOverrideFileIter(mtmInterplantOverrideFile);
    
    // loop once for each record in mtmInterplant file
    for ( ; mtmInterplantOverrideFileIter()!=NULL; ) {
      std::string mtm = mtmInterplantOverrideFileIter.mtm();
      std::string srcLoc = mtmInterplantOverrideFileIter.srcLoc();
      std::string destLoc = mtmInterplantOverrideFileIter.destLoc();
      int period = mtmInterplantOverrideFileIter.periodAsInt();
      float moveCost = mtmInterplantOverrideFileIter.moveCostAsFloat();
      float moveTime = mtmInterplantOverrideFileIter.moveTimeAsFloat();
      
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   mtmInterplant filename: " <<mtmInterplantOverrideFile.filename().c_str()   <<std::endl
          <<"   mtmInterplant file line number: " <<mtmInterplantOverrideFileIter.recNum() <<std::endl
          <<"   mtmInterplant: " <<mtm.c_str()                               <<std::endl
          <<"   srcLoc: " <<srcLoc.c_str()                                   <<std::endl           
          <<"   destLoc: " <<destLoc.c_str()                                 <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
             
      // Test to make sure mtm exists
      if ( !eso2Prob.mtmExists(mtm,srcLoc) ) {
        // mtm does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"      <<std::endl
          <<"Warning: mtm at srcLoc does not exist"                        <<std::endl
          <<"   mtmInterplant filename: " <<mtmInterplantOverrideFile.filename().c_str()   <<std::endl
          <<"   mtmInterplant file line number: " <<mtmInterplantOverrideFileIter.recNum() <<std::endl
          <<"   mtmInterplant: " <<mtm.c_str()                             <<std::endl
          <<"   srcLoc: " <<srcLoc.c_str()                                 <<std::endl          
          <<"   destLoc: " <<destLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                         <<std::endl
          <<"   record will be ignored. "                                  <<std::endl
          <<"-------------------------------------------------------"      <<std::endl;
        continue;
      }  
      
      // Test to make sure not interplanting to self
      if ( srcLoc==destLoc ) {
        // can not interplant to self
        std::cout 
          <<"-------------------------------------------------------"      <<std::endl
          <<"Warning: srcLoc and destLoc are same location"                <<std::endl
          <<"   mtmInterplant filename: " <<mtmInterplantOverrideFile.filename().c_str()   <<std::endl
          <<"   mtmInterplant file line number: " <<mtmInterplantOverrideFileIter.recNum() <<std::endl
          <<"   mtmInterplant: " <<mtm.c_str()                             <<std::endl
          <<"   srcLoc: " <<srcLoc.c_str()                                 <<std::endl          
          <<"   destLoc: " <<destLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                         <<std::endl
          <<"   record will be ignored. "                                  <<std::endl
          <<"-------------------------------------------------------"      <<std::endl;
        continue;
      }

      // if mtmInterplant connection does not exist, then add it
      if ( !eso2Prob.mtmInterplantExists(mtm,srcLoc,destLoc) ) {
        // add mtmInterplant connection
        
        // Get default move cost and time between srcLoc and destLoc
        float defaultMoveCost = ESO2problem::bigM();
        float defaultMoveTime = 0.0f;
        if ( mtmInterplantFile.keyExists(srcLoc,destLoc) ) {
          defaultMoveCost = mtmInterplantFile.defaultMoveCostAsFloat(srcLoc,destLoc);
          defaultMoveTime = mtmInterplantFile.defaultMoveTimeAsFloat(srcLoc,destLoc);
        }
        else {
          // Default part interplant move time and cost does not exist
          std::cout 
            <<"-------------------------------------------------------"        <<std::endl
            <<"Warning: default mtm interplant move time and cost does not exist in mtmInterplantData file" <<std::endl
            <<"   mtmInterplantData filename: " <<mtmInterplantFile.filename().c_str()   <<std::endl
            <<"   srcLoc: " <<srcLoc.c_str()                                   <<std::endl
            <<"   destLoc: " <<destLoc.c_str()                                 <<std::endl
            <<"   defaultMoveCost will be set to: " <<defaultMoveCost          <<std::endl
            <<"   defaultMoveTime will be set to: " <<defaultMoveTime          <<std::endl
            <<"-------------------------------------------------------"        <<std::endl;
        }
        eso2Prob.addMtmInterplant(mtm,srcLoc,destLoc,defaultMoveCost,defaultMoveTime);
      }
      
      eso2Prob.setMtmInterplantMoveCost(mtm,srcLoc,destLoc,period-1,moveCost);
      eso2Prob.setMtmInterplantMoveTime(mtm,srcLoc,destLoc,period-1,moveTime);
      
    } // end loop reading mtmInterplant file record
  } // end of block reading mtmInterplant file





  
  // Read Feature File
  {
    std::cout <<"Reading input file: featureData" <<std::endl;
    std::string featureFileName = inputDirectory + "/featureData.csv";
    ESO2featureData featureFile(featureFileName);
    ESO2featureDataIterator featureFileIter(featureFile);
    
    // loop once for each record in feature file
    for ( ; featureFileIter()!=NULL; ) {
      std::string feature = featureFileIter.feature();
      std::string plantLoc = featureFileIter.plantLoc();
      int period = featureFileIter.periodAsInt();
      float supply = featureFileIter.supplyAsFloat();
      float wfmv = featureFileIter.wfmvAsFloat();
      
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   feature filename: " <<featureFile.filename().c_str()         <<std::endl
          <<"   feature file line number: " <<featureFileIter.recNum()       <<std::endl
          <<"   feature: " <<feature.c_str()                                 <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      if ( !eso2Prob.featureExists(feature,plantLoc) ) {
        eso2Prob.addFeature(feature,plantLoc);
      }
      
      eso2Prob.setFeatureSupply(feature,plantLoc,period-1,supply);
      eso2Prob.setFeatureWfmv(feature,plantLoc,period-1,wfmv);
      
    } // end loop reading feature file record
  } // end of block reading feature file
  

  

  
  // Read LoosePart File
  {
    std::cout <<"Reading input file: loosePartData" <<std::endl;
    std::string loosePartFileName = inputDirectory + "/loosePartData.csv";
    ESO2loosePartData loosePartFile(loosePartFileName);
    ESO2loosePartDataIterator loosePartFileIter(loosePartFile);
    
    // loop once for each record in loosePart file
    for ( ; loosePartFileIter()!=NULL; ) {
      std::string loosePart = loosePartFileIter.part();
      std::string plantLoc = loosePartFileIter.plantLoc();
      int period = loosePartFileIter.periodAsInt();
      float supply = loosePartFileIter.supplyAsFloat();
      float wfmv = loosePartFileIter.wfmvAsFloat();
      
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   loosePart filename: " <<loosePartFile.filename().c_str()         <<std::endl
          <<"   loosePart file line number: " <<loosePartFileIter.recNum()       <<std::endl
          <<"   part: " <<loosePart.c_str()                                 <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      if ( !eso2Prob.loosePartExists(loosePart,plantLoc) ) {
        eso2Prob.addLoosePart(loosePart,plantLoc);
#if 0
        // add move to all destLocs
        addAllPartInterplants(eso2Prob, partSrcToDests, partInterplantFile, loosePart,plantLoc);
#endif
      }
      
      eso2Prob.setLoosePartSupply(loosePart,plantLoc,period-1,supply);
      eso2Prob.setLoosePartWfmv(loosePart,plantLoc,period-1,wfmv);
      
    } // end loop reading loosePart file record
  } // end of block reading loosePart file
  
  // Read Mtm Demanufacturing Bom File
  {
    std::cout <<"Reading input file: mtmDemanBomData" <<std::endl;
    std::string mtmDemanBomFileName = inputDirectory + "/mtmDemanBomData.csv";
    ESO2mtmDemanBomData mtmDemanBomFile(mtmDemanBomFileName);
    ESO2mtmDemanBomDataIterator mtmDemanBomFileIter(mtmDemanBomFile);
    
    // loop once for each record in mtmDemanBom file
    for ( ; mtmDemanBomFileIter()!=NULL; ) {
      if ( (mtmDemanBomFileIter.recNum() % 500)==0 ) std::cout <<"  Reading record number:" <<mtmDemanBomFileIter.recNum() <<std::endl;
      std::string mtm = mtmDemanBomFileIter.mtm();
      std::string part = mtmDemanBomFileIter.part();
      std::string srcLoc = mtmDemanBomFileIter.srcLoc();
      int period = mtmDemanBomFileIter.periodAsInt();
      float attachRate = mtmDemanBomFileIter.attachRateAsFloat();
#if 0
      int etnFlag = mtmDemanBomFileIter.etnFlagAsInt();

      // check to see if this record is to be filtered
      if ( etnFlagFilter==0 && etnFlag!=0 ) continue;
      if ( etnFlagFilter==1 && etnFlag!=1 ) continue;
#endif
      
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   mtmDemanBom filename: " <<mtmDemanBomFile.filename().c_str() <<std::endl
          <<"   mtmDemanBom file line number: " <<mtmDemanBomFileIter.recNum() <<std::endl
          <<"   mtm: " <<mtm.c_str()                                         <<std::endl
          <<"   srcLoc: " <<srcLoc.c_str()                                   <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      // Test to make sure mtm exists
      if ( !eso2Prob.mtmExists(mtm,srcLoc) ) {
        // mtm does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"          <<std::endl
          <<"Warning: mtm at srcLoc does not exist"                            <<std::endl
          <<"   mtmDemanBom filename: " <<mtmDemanBomFile.filename().c_str()   <<std::endl
          <<"   mtmDemanBom file line number: " <<mtmDemanBomFileIter.recNum() <<std::endl
          <<"   mtm: " <<mtm.c_str()                                           <<std::endl
          <<"   srcLoc: " <<srcLoc.c_str()                                     <<std::endl
          <<"   record will be ignored. "                                      <<std::endl
          <<"-------------------------------------------------------"          <<std::endl;
        continue;
      }
      
      // If bom entry does not exist, then add it to model
      if ( !eso2Prob.demanBomExists(mtm,part,srcLoc ) ) {
        //bool partExists = eso2Prob.partExists(part,srcLoc);
        eso2Prob.addDemanBom(mtm,part,srcLoc);
        //std::cout <<"    " <<mtmDemanBomFileIter.recNum() <<" addDemanBom" <<std::endl;
 
      }
      
      eso2Prob.setDemanBomAttachRate(mtm,part,srcLoc,period-1,attachRate);
      //std::cout <<"    " <<mtmDemanBomFileIter.recNum() <<"   setDemanBomAttachRate" <<std::endl;
 
      
    } // end loop reading mtmDemanBom file record
  } // end of block reading mtmDemanBom file

  // Read feature xref and conversion Files
  {
    std::cout <<"Reading input files: featureXrefData, & featureConversionData" <<std::endl;
    std::string featureXrefFileName = inputDirectory + "/featureXrefData.csv";
    std::string featureConversionFileName = inputDirectory + "/featureConversionData.csv";
    ESO2featureXrefData featureXrefFile(featureXrefFileName);
    ESO2featureConversionData featureConversionFile(featureConversionFileName);
    ESO2featureXrefDataIterator featureXrefFileIter(featureXrefFile);
    
    // loop once for each record in featureXref file
    for ( ; featureXrefFileIter()!=NULL; ) {
      if ( (featureXrefFileIter.recNum() % 500)==0 ) std::cout <<"  Reading record number:" <<featureXrefFileIter.recNum() <<std::endl;
      std::string part = featureXrefFileIter.part();
      std::string feature = featureXrefFileIter.feature();
      std::string plantLoc = featureXrefFileIter.plantLoc();
      int period = featureXrefFileIter.periodAsInt();
      float prodRate = featureXrefFileIter.prodRateAsFloat();

 

      std::string featureConversionFileKey = featureConversionFile.key(plantLoc);
      if (!featureConversionFile.keyExists(featureConversionFileKey)) {
        // cost and usageTime not specifed
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: feature conversion cost and usageTime not specifed in featureConversion file."   <<std::endl
          <<"   featureConversion filename: " <<featureConversionFile.filename().c_str()               <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   feature: " <<feature.c_str()                                 <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   cost will be set to: " <<ESO2problem::bigM()                 <<std::endl
          <<"   usageTime will be set to: " <<ESO2problem::bigM()            <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
      }
      
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   featureXref filename: " <<featureXrefFile.filename().c_str() <<std::endl
          <<"   featureXref line number: " <<featureXrefFileIter.recNum()        <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   feature: " <<feature.c_str()                                 <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      // Test to make sure feature exists
      if ( !eso2Prob.featureExists(feature,plantLoc) ) {
        // feature does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: feature at plantLoc does not exist"              <<std::endl
          <<"   featureXref filename: " <<featureXrefFile.filename().c_str()           <<std::endl
          <<"   featureXref file line number: " <<featureXrefFileIter.recNum()         <<std::endl
          <<"   feature: " <<feature.c_str()                     <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
            
#if 0
      // Test to make sure part exists
      if ( !eso2Prob.partExists(part,plantLoc) ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part at plantLoc does not exist"              <<std::endl
          <<"   featureXref filename: " <<featureXrefFile.filename().c_str()           <<std::endl
          <<"   featureXref file line number: " <<featureXrefFileIter.recNum()         <<std::endl
          <<"   part: " <<part.c_str()                     <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
#endif

      // Test to make sure downLevel and upLevel part are not same part
      if ( part==feature ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part and feature are the same part."      <<std::endl
          <<"   featureXref filename: " <<featureXrefFile.filename().c_str()     <<std::endl
          <<"   featureXref file line number: " <<featureXrefFileIter.recNum()   <<std::endl
          <<"   feature: " <<feature.c_str()                     <<std::endl
          <<"   part: " <<part.c_str()                         <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      // If bom between part and feature part does not exist, then add it to model
      if ( !eso2Prob.featureXrefExists(part,feature,plantLoc ) ) {
        //bool partExists = eso2Prob.partExists(upLevelPart,plantLoc);
        eso2Prob.addFeatureXref(part,feature,plantLoc);
        //if ( !partExists )
        //  addAllPartInterplants(eso2Prob, partSrcToDests, partInterplantFile, upLevelPart,plantLoc);
        float cost = ESO2problem::bigM();
        float usageTime = ESO2problem::bigM();  
        if ( featureConversionFile.keyExists(featureConversionFileKey) ) {
          cost = featureConversionFile.costAsFloat(featureConversionFileKey);
          usageTime = featureConversionFile.usageTimeAsFloat(featureConversionFileKey);
        }
        eso2Prob.setFeatureXrefUsageTime(part,feature,plantLoc,usageTime);
        eso2Prob.setFeatureXrefCost(part,feature,plantLoc,cost);
      }
      
      eso2Prob.setFeatureXrefProdRate(part,feature,plantLoc,period-1,prodRate);
      
    } // end loop reading featureXref file record
  } // end of block reading feature files

  // Read feature conversion override File
  {
    std::cout <<"Reading input file featureConversionOverrideData" <<std::endl;
    std::string featureConversionOverrideFileName = inputDirectory + "/featureConversionOverrideData.csv";
    ESO2featureConversionOverrideData featureConversionOverrideFile(featureConversionOverrideFileName);
    ESO2featureConversionOverrideDataIterator featureConversionOverrideFileIter(featureConversionOverrideFile);
    
    // loop once for each record in featureXref file
    for ( ; featureConversionOverrideFileIter()!=NULL; ) {
      std::string part = featureConversionOverrideFileIter.part();
      std::string feature = featureConversionOverrideFileIter.feature();
      std::string plantLoc = featureConversionOverrideFileIter.plantLoc();
      int period = featureConversionOverrideFileIter.periodAsInt();
      float cost = featureConversionOverrideFileIter.costAsFloat();
      float usageTime = featureConversionOverrideFileIter.usageTimeAsFloat();

      
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   featureConversionOverride filename: " <<featureConversionOverrideFile.filename().c_str() <<std::endl
          <<"   featureConversionOverride line number: " <<featureConversionOverrideFileIter.recNum()        <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   feature: " <<feature.c_str()                                 <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      // Test to make sure feature exists
      if ( !eso2Prob.featureExists(feature,plantLoc) ) {
        // feature does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: feature at plantLoc does not exist"                    <<std::endl
          <<"   featureConversionOverride filename: " <<featureConversionOverrideFile.filename().c_str()   <<std::endl
          <<"   featureConversionOverride file line number: " <<featureConversionOverrideFileIter.recNum()     <<std::endl
          <<"   feature: " <<feature.c_str()                                 <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
            
      // Test to make sure part exists
      if ( !eso2Prob.partExists(part,plantLoc) ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part at plantLoc does not exist"                       <<std::endl
          <<"   featureConversionOverride filename: " <<featureConversionOverrideFile.filename().c_str() <<std::endl
          <<"   featureConversionOverride file line number: " <<featureConversionOverrideFileIter.recNum()   <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }

      // Test to make sure downLevel and upLevel part are not same part
      if ( part==feature ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part and feature are the same part."                   <<std::endl
          <<"   featureConversionOverride filename: " <<featureConversionOverrideFile.filename().c_str()     <<std::endl
          <<"   featureConversionOverride file line number: " <<featureConversionOverrideFileIter.recNum()       <<std::endl
          <<"   feature: " <<feature.c_str()                                 <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      // The period of usageTime and Cost are when the conversion is complete, so must be offset by usageTime
      int completionPeriod = (int)(period+usageTime+0.5);
      if (completionPeriod<nPeriods) {
        eso2Prob.setFeatureXrefUsageTime(part,feature,plantLoc,completionPeriod-1,usageTime);
        eso2Prob.setFeatureXrefCost(part,feature,plantLoc,completionPeriod-1,cost);
      }

      
    } // end loop reading featureConversionOverride file record
  } // end of block reading featureConversionOverride files

   
  // Read part interplant override File
  {
    std::cout <<"Reading input file: partInterplantOverrideData" <<std::endl;
    std::string partInterplantOverrideDataFileName = inputDirectory + "/partInterplantOverrideData.csv";
    ESO2partInterplantOverrideData partInterplantOverrideDataFile(partInterplantOverrideDataFileName);
    ESO2partInterplantOverrideDataIterator partInterplantOverrideFileIter(partInterplantOverrideDataFile);
    
    // loop once for each record in partInterplant file
    for ( ; partInterplantOverrideFileIter()!=NULL; ) {
      std::string part = partInterplantOverrideFileIter.part();
      std::string srcLoc = partInterplantOverrideFileIter.srcLoc();
      std::string destLoc = partInterplantOverrideFileIter.destLoc();
      int period = partInterplantOverrideFileIter.periodAsInt();
      float moveCost = partInterplantOverrideFileIter.moveCostAsFloat();
      float moveTime = partInterplantOverrideFileIter.moveTimeAsFloat();
      
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   partInterplantOverride filename: " <<partInterplantOverrideDataFile.filename().c_str()   <<std::endl
          <<"   partInterplantOverride file line number: " <<partInterplantOverrideFileIter.recNum() <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   srcLoc: " <<srcLoc.c_str()                                   <<std::endl
          <<"   destLoc: " <<destLoc.c_str()                                 <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      // Test to make sure part exists
      if ( !eso2Prob.partExists(part,srcLoc) ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part at srcLoc does not exist"                         <<std::endl
          <<"   partInterplantOverride filename: " <<partInterplantOverrideDataFile.filename().c_str()   <<std::endl
          <<"   partInterplantOverride file line number: " <<partInterplantOverrideFileIter.recNum() <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   srcLoc: " <<srcLoc.c_str()                                   <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      
      // If link between srcLoc and destLoc for part does not exist, then add it to model
      if ( !eso2Prob.partInterplantExists(part,srcLoc,destLoc ) ) {
        // Get default move cost and time between srcLoc and destLoc
        float defaultMoveCost = ESO2problem::bigM();
        float defaultMoveTime = 0.0f;
        if ( partInterplantFile.keyExists(srcLoc,destLoc) ) {
          defaultMoveCost = partInterplantFile.defaultMoveCostAsFloat(srcLoc,destLoc);
          defaultMoveTime = partInterplantFile.defaultMoveTimeAsFloat(srcLoc,destLoc);
        }
        else {
          // Default part interplant move time and cost does not exist
          std::cout 
            <<"-------------------------------------------------------"        <<std::endl
            <<"Warning: default part interplant move time and cost does not exist in partInterplantMoveDefaults file" <<std::endl
            <<"   partInterplant filename: " <<partInterplantFile.filename().c_str()   <<std::endl
            <<"   srcLoc: " <<srcLoc.c_str()                                   <<std::endl
            <<"   destLoc: " <<destLoc.c_str()                                 <<std::endl
            <<"   defaultMoveCost will be set to: " <<defaultMoveCost          <<std::endl
            <<"   defaultMoveTime will be set to: " <<defaultMoveTime          <<std::endl
            <<"-------------------------------------------------------"        <<std::endl;
        }
        eso2Prob.addPartInterplant(part,srcLoc,destLoc,defaultMoveCost,defaultMoveTime);
        
        
        //bool partExists = eso2Prob.partExists(part,destLoc);
        
        //std::map<std::string,StdStringVec>> singleSrcToDest;
        //singleSrcToDest[srcLoc].push_back(destLoc);
        //eso2Prob.addPartInterplant(part,srcLoc,destLoc,ESO2problem::bigM(),0.0);
        
        //if ( !partExists )
        //  addAllPartInterplants(eso2Prob, partSrcToDests, partInterplantFile, part,destLoc);
      }
     
      eso2Prob.setPartInterplantMoveCost(part,srcLoc,destLoc,period-1,moveCost);
      eso2Prob.setPartInterplantMoveTime(part,srcLoc,destLoc,period-1,moveTime);

    } // end loop reading partInterplant file record
  } // end of block reading partInterplant file

#if 0
  // Read alt and altExcludeLoc Files
  {
    std::cout <<"Reading input file: altData" <<std::endl;
    std::string altFileName = inputDirectory + "/altData.csv";
    ESO2altData altFile(altFileName);
    ESO2altDataIterator altFileIter(altFile);
    
    std::string altExcludeLocFileName = inputDirectory + "/altExcludeLocData.csv";
    ESO2altExcludeLocData altExludeLocFile(altExcludeLocFileName);
    
    std::string altOverrideFileName = inputDirectory + "/altOverrideData.csv";
    ESO2altOverrideData altOverrideFile(altOverrideFileName);

    std::string newAltDataFileName = inputDirectory + "/newAltData.csv";
    FILE * newAltDataFilePtr = fopen(newAltDataFileName.c_str(),"w");
    std::string heading="\"part\",\"altPart\",\"loc\",\"altConvCost\",\"altConvTime\""; 
    fprintf(newAltDataFilePtr,"%s\n",heading.c_str());
    
    
    // loop once for each record in alternatePart file
    for ( ; altFileIter()!=NULL; ) {
      std::string altPart = altFileIter.altPart();
      std::string part = altFileIter.part();
      //std::string plantLoc = alternatePartFileIter.plantLoc();
      //int period = alternatePartFileIter.periodAsInt();
      float prodRate = 1.0f;
      float defConvCost = altFileIter.altConvCostAsFloat();
      float defConvTime = altFileIter.altConvTimeAsFloat();
      
#if 0
      // Test to make sure period is in range
      if ( period<1 || period>nPeriods ) {
        // Period is out of range. Write message
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: period out of allowed range"                           <<std::endl
          <<"   alternatePart filename: " <<alternatePartFile.filename().c_str()           <<std::endl
          <<"   alternatePart file line number: " <<alternatePartFileIter.recNum()         <<std::endl
          <<"   upLevelPart: " <<upLevelPart.c_str()                         <<std::endl
          <<"   downLevelPart: " <<downLevelPart.c_str()                     <<std::endl
          <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
          <<"   period: " <<period                                           <<std::endl
          <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
#endif
      
      std::vector<std::string> locs = eso2Prob.getPartLocs(part);
      // Test to make sure part part exists
      if ( locs.size()==0 ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part does not exist"                                   <<std::endl
          <<"   alt filename: " <<altFile.filename().c_str()                 <<std::endl
          <<"   alt file line number: " <<altFileIter.recNum()               <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   altPart: " <<altPart.c_str()                                 <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }

      // Test to make sure downLevel and upLevel part are not same part
      if ( part==altPart ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part and altPart are the same part."                   <<std::endl
          <<"   alt filename: " <<altFile.filename().c_str()                 <<std::endl
          <<"   alt file line number: " <<altFileIter.recNum()               <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   altPart: " <<altPart.c_str()                                 <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      // loop once for each location where downLevel part exists
      int l;
      for ( l=0; l<locs.size(); ++l ) {

        // check to see if this location is to be excluded
        std::string altExcludeLocKey = altExludeLocFile.key(part,altPart,locs[l]);
        if ( altExludeLocFile.keyExists(altExcludeLocKey) ) 
          continue;
    
        // If bom between downLevel and upLevel part does not exist, then add it to model
        const std::string & loc = locs[l];
        if ( !eso2Prob.altExists(part,altPart,loc) ) {

          float convCost,convTime;
          
          // check to see if there is an override entry
          std::string overrideKey = altOverrideFile.key(part,altPart,loc);
          if ( altOverrideFile.keyExists(overrideKey) ) {            
            convCost = altOverrideFile.altConvCostAsFloat(overrideKey);
            convTime = altOverrideFile.altConvTimeAsFloat(overrideKey);
          }
          else {
            convCost = defConvCost;
            convTime = defConvTime;
          }
          eso2Prob.addAlt(part,altPart,locs[l],convCost,convTime);

          fprintf(newAltDataFilePtr,"\"%s\",\"%s\",\"%s\",%f,%f\n",part.c_str(),altPart.c_str(),locs[l].c_str(),convCost,convTime);

        }

      } // end loop for ( l=0; l<locs.size(); ++l ) 
    } // end loop reading alt file record
    fclose(newAltDataFilePtr);
  } // end of block reading alt file
#endif

  
#if 1
  // Read alt
  {
    std::cout <<"Reading input file: altData" <<std::endl;
    std::string altFileName = inputDirectory + "/altData.csv";
    ESO2altData altFile(altFileName);
    ESO2altDataIterator altFileIter(altFile);
    
    
    // loop once for each record in alternatePart file
    for ( ; altFileIter()!=NULL; ) {
      std::string altPart = altFileIter.altPart();
      std::string part = altFileIter.part();
      std::string loc = altFileIter.loc();
      float prodRate = 1.0f;
      float convCost = altFileIter.altConvCostAsFloat();
      float convTime = altFileIter.altConvTimeAsFloat();
      
      // Test to make sure part part exists
      if ( !eso2Prob.partExists(part,loc) ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part does not exist"                                   <<std::endl
          <<"   alt filename: " <<altFile.filename().c_str()                 <<std::endl
          <<"   alt file line number: " <<altFileIter.recNum()               <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   loc: "  <<loc.c_str()                                       <<std::endl
          <<"   altPart: " <<altPart.c_str()                                 <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      // Test to make sure downLevel and upLevel part are not same part
      if ( part==altPart ) {
        // part does not exist so record is invalid
        std::cout 
          <<"-------------------------------------------------------"        <<std::endl
          <<"Warning: part and altPart are the same part."                   <<std::endl
          <<"   alt filename: " <<altFile.filename().c_str()                 <<std::endl
          <<"   alt file line number: " <<altFileIter.recNum()               <<std::endl
          <<"   part: " <<part.c_str()                                       <<std::endl
          <<"   altPart: " <<altPart.c_str()                                 <<std::endl
          <<"   record will be ignored. "                                    <<std::endl
          <<"-------------------------------------------------------"        <<std::endl;
        continue;
      }
      
      
      if ( !eso2Prob.altExists(part,altPart,loc) ) {
        eso2Prob.addAlt(part,altPart,loc,convCost,convTime);
      }
      
    } // end loop reading alt file record
  } // end of block reading alt file
#endif


  
  // Read partDemand & partDemandLowPriority Files
  {
    std::vector<std::string> partDemandFileNames;
    partDemandFileNames.push_back("partDemandData");
    partDemandFileNames.push_back("partDemand2Data");
    
    std::vector<std::string> nEtnFlagsGlobalAttr;
    nEtnFlagsGlobalAttr.push_back("nEtnFlagFilters");
    nEtnFlagsGlobalAttr.push_back("nEtn2FlagFilters");
    
    std::vector<std::string> etnFlagNGlobalAttr;
    etnFlagNGlobalAttr.push_back("etnFlagFilter");
    etnFlagNGlobalAttr.push_back("etn2FlagFilter");
    
    int pdf;
    for ( pdf=0; pdf<2; ++pdf ) {
      std::cout <<"Reading input file: " <<partDemandFileNames[pdf] <<std::endl;
      std::string partDemandFileName = inputDirectory + "/" + partDemandFileNames[pdf] + ".csv";
      ESO2partDemandData partDemandFile(partDemandFileName);
      ESO2partDemandDataIterator partDemandFileIter(partDemandFile);
      
      // create set of etn flags that will be used for the current demand file
      std::set<std::string> etnFlags;
      {
         int nEtnFlagFilters = globalAttrFile.fieldAsInt(nEtnFlagsGlobalAttr[pdf],"attrValue");
         for ( int i=0; i<nEtnFlagFilters; ++i ) {
           char sAsChar[100];
           sprintf(sAsChar,"%d",i+1);
           std::string attrName = etnFlagNGlobalAttr[pdf]+std::string(sAsChar);
           std::string filter = globalAttrFile.fieldAsString(attrName,"attrValue");
           etnFlags.insert(filter);
         }
      } // finished creating set of etn flags
      
      // loop once for each record in partDemand file
      int nDemandsRead = 0;
      int nDemandsAdded = 0;
      for ( ; partDemandFileIter()!=NULL; ) {
        
        std::string part = partDemandFileIter.part();
        std::string custLoc = partDemandFileIter.custLoc();
        std::string plantLoc = partDemandFileIter.plantLoc();
        int period = partDemandFileIter.periodAsInt();
        float demandVol = partDemandFileIter.demandVolAsFloat();
        float wac = partDemandFileIter.wacAsFloat();
        std::string etnFlag = partDemandFileIter.etnFlag();
        nDemandsRead++;
        
        // check to see if this record is to be filtered
#if 0
        if ( etnFlagFilter==0 && etnFlag!=0 ) continue;
        if ( etnFlagFilter==1 && etnFlag!=1 ) continue;
#endif
        if ( etnFlags.find(etnFlag)==etnFlags.end() ) continue;
        
        // Test to make sure period is in range
        if ( period<1 || period>nPeriods ) {
          // Period is out of range. Write message
          std::cout 
            <<"-------------------------------------------------------"        <<std::endl
            <<"Warning: period out of allowed range"                           <<std::endl
            <<"   partDemand filename: " <<partDemandFile.filename().c_str()   <<std::endl
            <<"   partDemand file line number: " <<partDemandFileIter.recNum() <<std::endl
            <<"   part: " <<part.c_str()                                       <<std::endl
            <<"   custLoc: " <<custLoc.c_str()                                 <<std::endl
            <<"   plantLoc: " <<plantLoc.c_str()                               <<std::endl
            <<"   period: " <<period                                           <<std::endl
            <<"   valid range: 0<period<=" <<nPeriods                          <<std::endl
            <<"   record will be ignored. "                                    <<std::endl
            <<"-------------------------------------------------------"        <<std::endl;
          continue;
        }
        
        // Test to make sure mtm exists
        if ( !eso2Prob.partExists(part,plantLoc) ) {
          // part does not exist so record is invalid
          std::cout 
            <<"-------------------------------------------------------"        <<std::endl
            <<"Warning: part at plantLoc does not exist"                       <<std::endl
            <<"   partDemand filename: " <<partDemandFile.filename().c_str()   <<std::endl
            <<"   partDemand file line number: " <<partDemandFileIter.recNum() <<std::endl
            <<"   part: " <<part.c_str()                                       <<std::endl
            <<"   srcLoc: " <<plantLoc.c_str()                                 <<std::endl
            <<"   record will be ignored. "                                    <<std::endl
            <<"-------------------------------------------------------"        <<std::endl;
          continue;
        }
        
        // If demand does not exist, then add it to model
        if ( !eso2Prob.partDemandExists(part,custLoc,plantLoc ) ) {
          eso2Prob.addPartDemand(part,custLoc,plantLoc);
          eso2Prob.setPartDemandPriority(part,custLoc,plantLoc,pdf);
        }
        
        eso2Prob.setPartDemandDemandVol(part,custLoc,plantLoc,period-1,demandVol);
        eso2Prob.setPartDemandWac(part,custLoc,plantLoc,period-1,wac);
        nDemandsAdded++;
        
      } // end loop reading partDemand file record
      std::cout <<"   "<<nDemandsRead  <<" demand records read." <<std::endl;
      std::cout <<"   "<<nDemandsAdded <<" demand records added to eso2 model." <<std::endl;
    } // end of loop which iterates over pardDemand files. for ( pdf=0; pdf<2; ++pdf )
  } // end of block reading partDemand file

} // end function createModel

