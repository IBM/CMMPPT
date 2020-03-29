
#include "wit.h"

#include <map>
#include <vector>

#include <assert.h>
#include <iostream>
#include <float.h>
#include <math.h>



//#include "CoinFloatEqual.h"
#include "visor.h"
#include "printer.h"
#include "requestQuantity.h"
#include "onHandMaterial.h"
#include "visorProblem.h"


typedef std::vector<std::string> StdStringVec;


int
main (int argc, char * argv[])
{

  std::cout <<"visor Build date: " <<__DATE__ <<std::endl;
  {
    
    // Put Parms into a vector
    std::vector<std::string> args;
    for (int i=0; i<argc; i++) 
      args.push_back(argv[i]);
    
    {
      // Check parm passed to program
      bool parmError = false;
      
      if ( args.size()> 1 ) {
        if (args[1]=="?" ) {
          parmError = true; // set flag to provide usage info
        }
      }
      
      if ( ! parmError ) {
        if ( args.size()!=2 ) {
          // Wrong number of parms. Write message
          std::cerr 
            <<"-------------------------------------------------------"  <<std::endl
            <<"Wrong number of parmeters specified"                      <<std::endl
            <<"  Number parameters specified: " <<(int)args.size()-1     <<std::endl
            <<"  Expected number of parameters: 1"                       <<std::endl
            <<"-------------------------------------------------------"  <<std::endl;
          parmError = true;
        }
      }
      
      
      
      if ( parmError ) {
        // Write correct usage
        std::cerr 
          <<"-------------------------------------------------------"  <<std::endl
          <<argv[0] <<" dataDirectory "                                <<std::endl
          <<"  where:"                                                 <<std::endl
          <<"    dataDirectory:"                                       <<std::endl
          <<"      Directory where input files are located and"        <<std::endl
          <<"      output files are written."                          <<std::endl
          <<"  example usage:"                                         <<std::endl
          <<"    " <<argv[0] <<" ../example1"                          <<std::endl
          <<"-------------------------------------------------------"  <<std::endl;
        return 1;
      }
    }
    
    std::string outputDirectory=args[1];
    std::string inputDirectory=args[1];
    VISORproblem visorProb;
    int nPeriods;
    
    int nWfmvScaleFactors;
    bool writeZeros=true;
    
    
    visorProb.setSolverLogFileName(outputDirectory+"/clpMesgFile.txt");
    bool useGlobalAttrFileSettings = true;
    //createModel(eso2Prob, inputDirectory, useGlobalAttrFileSettings);
    nPeriods = visorProb.getNPeriods();

    
    //---------------------------------------------------------
#if 0    
    std::string mtmAcquirePlanFileName = outputDirectory+"/mtmAcquirePlanO.csv";
    FILE * mtmAcquirePlanFilePtr = fopen(mtmAcquirePlanFileName.c_str(),"w");
    
    std::string featureAcquirePlanFileName = outputDirectory+"/featureAcquirePlanO.csv";
    FILE * featureAcquirePlanFilePtr = fopen(featureAcquirePlanFileName.c_str(),"w");
    
    std::string loosePartAcquirePlanFileName = outputDirectory+"/loosePartAcquirePlanO.csv";
    FILE * loosePartAcquirePlanFilePtr = fopen(loosePartAcquirePlanFileName.c_str(),"w");
    
    std::string partShipFileName = outputDirectory+"/partShipO.csv";
    FILE * partShipFilePtr = fopen(partShipFileName.c_str(),"w");

    std::string partShipReportFileName = outputDirectory+"/partShipReportO.csv";
    FILE * partShipReportFilePtr = fopen(partShipReportFileName.c_str(),"w");
    
    std::string featureXrefFileName = outputDirectory+"/featureXrefO.csv";
    FILE * featureXrefFilePtr = fopen(featureXrefFileName.c_str(),"w");
    
    std::string altFileName = outputDirectory+"/altO.csv";
    FILE * altFilePtr = fopen(altFileName.c_str(),"w");
    
    std::string partInterplantFileName = outputDirectory+"/partInterplantO.csv";
    FILE * partInterplantFilePtr = fopen(partInterplantFileName.c_str(),"w");

    std::string peggedDemandPlanFileName = outputDirectory+"/peggedDemandPlanO.csv";
    FILE * peggedDemandPlanFilePtr = fopen(peggedDemandPlanFileName.c_str(),"w");
    
    std::string kpiFileName = outputDirectory+"/keyPerformanceIndicatorsO.csv";
    FILE * kpiFilePtr = fopen(kpiFileName.c_str(),"w");
    
    //---------------------------------------------------------
    
    // Get some metrics before prunning
    double mtmTotalWfmvAtScale1;
    double mtmSumAcquireCostTemp;
    double mtmSumMoveCostTemp;
    double mtmSumDemanCostTemp;
    int mtmNumUnique;
    int mtmNumUniqueAcquiredTemp;
    double mtmSupplyQuantity;
    double mtmAcquiredQuantityTemp;
    
    eso2Prob.mtmMetrics(
      mtmTotalWfmvAtScale1,
      mtmSumAcquireCostTemp,
      mtmSumMoveCostTemp,
      mtmSumDemanCostTemp,
      mtmNumUnique,
      mtmNumUniqueAcquiredTemp,
      mtmSupplyQuantity,
      mtmAcquiredQuantityTemp);
    
    double featureTotalWfmvAtScale1;
    double featureSumAcquireCostTemp;
    double featureSumXrefCostTemp;
    int featureNumUnique;
    int featureNumUniqueAcquiredTemp;
    double featureSupplyQuantity;
    double featureAcquiredQuantityTemp;
    
    eso2Prob.featureMetrics(
      featureTotalWfmvAtScale1,
      featureSumAcquireCostTemp,
      featureSumXrefCostTemp,
      featureNumUnique,
      featureNumUniqueAcquiredTemp,
      featureSupplyQuantity,
      featureAcquiredQuantityTemp);

    double loosePartTotalWfmvAtScale1;
    double loosePartSumAcquireCostTemp;
    double loosePartSumAltCost;
    int loosePartNumUnique;
    int loosePartNumUniqueAcquiredTemp;
    double loosePartSupplyQuantity;
    double loosePartAcquiredQuantityTemp;
    
    eso2Prob.loosePartMetrics(
      loosePartTotalWfmvAtScale1,
      loosePartSumAcquireCostTemp,
      loosePartSumAltCost,
      loosePartNumUnique,
      loosePartNumUniqueAcquiredTemp,
      loosePartSupplyQuantity,
      loosePartAcquiredQuantityTemp);
    
    double partDemandSumDemandWac;
    double partDemandSumShippedWacTemp;
    int partDemandNumUnique;
    int partDemandNumUniqueShippedTemp;
    double partDemandSumDemandVol;
    double partDemandSumShipVolTemp;
    std::map<std::string,double> sumShippedWacComponents;
    
    eso2Prob.partDemandMetrics(
      partDemandSumDemandWac,
      partDemandSumShippedWacTemp,
      sumShippedWacComponents,
      partDemandNumUnique,
      partDemandNumUniqueShippedTemp,
      partDemandSumDemandVol,
      partDemandSumShipVolTemp);
           

    //eso2Prob.writeWitData(outputDirectory + "/eso2.wit");
    eso2Prob.pruneWitModel();
    //eso2Prob.writeWitData(outputDirectory + "/example3.wit");


    std::string globalAttrFileName = inputDirectory + "/globalAttrData.csv";
    ESO2globalAttrData globalAttrFile(globalAttrFileName);

    // Loop once for each scale factor
    int s;
    for ( s=1; s<=nWfmvScaleFactors;  ++s ) {
      
      // get scale factor for this solve
      char sAsChar[100];
      sprintf(sAsChar,"%d",s);
      std::string attrName = std::string("wfmvScaleFactor")+std::string(sAsChar);
      float wfmvScaleFactor = globalAttrFile.fieldAsFloat(attrName,"attrValue");
      std::string attr2Name = std::string("wfmv2ScaleFactor")+std::string(sAsChar);
      float wfmv2ScaleFactor = globalAttrFile.fieldAsFloat(attr2Name,"attrValue");
      eso2Prob.setWfmvScaleFactor(wfmvScaleFactor,wfmv2ScaleFactor);

      // Solve
      std::cout 
        <<"Solving with wfmvScaleFactor="<<wfmvScaleFactor
        <<" and wfmv2ScaleFactor="<<wfmv2ScaleFactor
        <<". solve " <<s <<" of " <<nWfmvScaleFactors <<"."
        <<std::endl;
      eso2Prob.solveWithPegging();
      
      
      
      // Write Results
      std::cout <<"Writing results." <<std::endl;
      bool writeHeader = false;
      if ( s==1 ) writeHeader = true;
      writeResults(
        eso2Prob,
        writeHeader,
        mtmAcquirePlanFilePtr,
        featureAcquirePlanFilePtr,
        loosePartAcquirePlanFilePtr,
        partShipFilePtr,
        featureXrefFilePtr,
        altFilePtr,
        partInterplantFilePtr,
        peggedDemandPlanFilePtr,
        kpiFilePtr,
        
        mtmTotalWfmvAtScale1,
        mtmNumUnique,
        mtmSupplyQuantity,
        
        featureTotalWfmvAtScale1,
        featureNumUnique,
        featureSupplyQuantity,
        
        loosePartTotalWfmvAtScale1,
        loosePartNumUnique,
        loosePartSupplyQuantity,
        
        partDemandSumDemandWac,
        partDemandNumUnique,
        partDemandSumDemandVol,
        
        writeZeros );
      
      writeShipReportForKevin(eso2Prob,writeHeader,partShipReportFilePtr,writeZeros );

      
    } // end of s loop

    //eso2Prob.reportPartDemand("0000000P1690","93B","93B",24-1);
    //eso2Prob.reportPartDemand("p1","cust","sim",3-1);
    //eso2Prob.reportPartDemand("pn1","980","980",1-1);
    //eso2Prob.reportPartDemand("0000053P6023","980","980",12-1);
    //eso2Prob.reportPartDemand("u3","kgn","wat",4-1);
    //eso2Prob.reportPartDemand("u3","kgn","wat",1-1);
    //eso2Prob.reportPartDemand("u3","kgn","wat",2-1);


    
    //--------------------------------------
    fclose(mtmAcquirePlanFilePtr);
    fclose(featureAcquirePlanFilePtr);
    fclose(partShipFilePtr);
    fclose(partShipReportFilePtr);
    fclose(featureXrefFilePtr);
    fclose(partInterplantFilePtr);
    fclose(peggedDemandPlanFilePtr);
    fclose(kpiFilePtr);
    //--------------------------------------
#endif

      
  }

  std::cout <<"visor application is finished." <<std::endl;
  return 0;
}
