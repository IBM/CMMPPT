#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif


//#include "wit.h"

#include <map>
#include <vector>

#include <assert.h>
#include <iostream>
#include <float.h>
#include <math.h>



#include "CoinFloatEqual.h"
#include "fileConvDec03.h"
#include "globalAttrData.h"
//#include "mtmDemanBomData.h"
//#include "partDemandData.h"
#include "mtmDemanBomData1.h"
#include "partDemandData1.h"
//#include "eso2Problem.h"


typedef std::vector<std::string> StdStringVec;



int
main (int argc, char * argv[])
{
  
  std::cout <<"fileConvDec03 Build date: " <<__DATE__ <<std::endl;
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
            <<"  Number parameters specified: " <<args.size()-1          <<std::endl
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
    int nPeriods;
    
    {
      // Get number of solves to be done (ie number of scale factors)
      std::string globalAttrFileName = inputDirectory + "/globalAttrData.csv";
      ESO2globalAttrData globalAttrFile(globalAttrFileName);
      
      nPeriods=globalAttrFile.fieldAsInt("nPeriods","attrValue");
    }
    
    std::string mtmDemanBomDataFileName    = outputDirectory + "/mtmDemanBomData.csv";
    std::string newMtmDemanBomDataFileName = outputDirectory + "/newMtmDemanBomData.csv";

    std::string partDemandDataFileName    = outputDirectory + "/partDemandData.csv";
    std::string newPartDemandDataFileName = outputDirectory + "/newPartDemandData.csv";
    
    // Read mtmDemanBomData and remove etnFlag
    {
      std::cout <<"Converting mtmDemanBomData" <<std::endl;
      
      ESO2mtmDemanBomData1 mtmDemanBomFile(mtmDemanBomDataFileName);   
      ESO2mtmDemanBomData1Iterator mtmDemanBomFileIter(mtmDemanBomFile);
      
      FILE * newMtmDemanBomDataFilePtr = fopen(newMtmDemanBomDataFileName.c_str(),"w");
      std::string heading="\"mtm\",\"part\",\"srcLoc\",\"period\",\"attachRate\""; 
      fprintf(newMtmDemanBomDataFilePtr,"%s\n",heading.c_str());   
      
      // loop once for each record in mtmDemanBomData file
      for ( ; mtmDemanBomFileIter()!=NULL; ) {
        std::string mtm = mtmDemanBomFileIter.mtm();
        std::string part = mtmDemanBomFileIter.part();
        std::string srcLoc = mtmDemanBomFileIter.srcLoc();
        int period = mtmDemanBomFileIter.periodAsInt();
        float attachRate = mtmDemanBomFileIter.attachRateAsFloat();
        
        // Write feature xrefRecord       
        fprintf(newMtmDemanBomDataFilePtr,
          "\"%s\",\"%s\",\"%s\",%d,%f\n",
          mtm.c_str(),
          part.c_str(),
          srcLoc.c_str(),
          period,
          attachRate);
      }  
      
      fclose(newMtmDemanBomDataFilePtr);
    }
    
  
    // Read partDemandData and add etnFlag
    {
      std::cout <<"Converting partDemandData" <<std::endl;
      
      ESO2partDemandData1 partDemandFile(partDemandDataFileName);   
      ESO2partDemandData1Iterator partDemandFileIter(partDemandFile);
      
      FILE * newPartDemandDataFilePtr = fopen(newPartDemandDataFileName.c_str(),"w");
      std::string heading="\"part\",\"custLoc\",\"plantLoc\",\"period\",\"demandVol\",\"wac\",\"etnFlag\""; 
      fprintf(newPartDemandDataFilePtr,"%s\n",heading.c_str());   
      
      // loop once for each record in partDemandData file
      for ( ; partDemandFileIter()!=NULL; ) {

        std::string part = partDemandFileIter.part();
        std::string custLoc = partDemandFileIter.custLoc();
        std::string plantLoc = partDemandFileIter.plantLoc();
        int period = partDemandFileIter.periodAsInt();
        float demandVol = partDemandFileIter.demandVolAsFloat();
        float wac = partDemandFileIter.wacAsFloat();
        int etnFlag = 1;
        
        // Write feature xrefRecord       
        fprintf(newPartDemandDataFilePtr,
          "\"%s\",\"%s\",\"%s\",%d,%f,%f,%d\n",
          part.c_str(),
          custLoc.c_str(),
          plantLoc.c_str(),
          period,
          demandVol,
          wac,
          etnFlag);
      }  
      
      fclose(newPartDemandDataFilePtr);
    }
    
  
#if 1
  int rc;
  rc=remove(mtmDemanBomDataFileName.c_str());
  rc=rename(newMtmDemanBomDataFileName.c_str(),mtmDemanBomDataFileName.c_str());
  rc=remove(partDemandDataFileName.c_str());
  rc=rename(newPartDemandDataFileName.c_str(),partDemandDataFileName.c_str());
#endif
  }

  std::cout <<"fileConvDec03 application is finished." <<std::endl;
  return 0;
}
