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
#include "fileConvOct03.h"
#include "globalAttrData.h"
#include "alternatePartData.h"
#include "featureData.h"
#include "featureConversionData.h"
#include "featureConversionOverrideData.h"
#include "featureXrefData.h"
#include "loosePartData.h"
#include "mtmDemanBomData.h"
#include "alternatePartData.h"
#include "partInterplantData.h"
#include "partInterplantOverrideData.h"
#include "mtmInterplantData.h"
#include "mtmInterplantOverrideData.h"
#include "partDemandData.h"
//#include "eso2Problem.h"


typedef std::vector<std::string> StdStringVec;



int
main (int argc, char * argv[])
{
  
  std::cout <<"fileConvOct03 Build date: " <<__DATE__ <<std::endl;
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
    
    std::string heading;
    
    std::string altDataFileName = outputDirectory + "/altData.csv";
    FILE * altDataFilePtr = fopen(altDataFileName.c_str(),"w");
    heading="\"partOrFeature\",\"altPart\",\"altConvCost\",\"altConvTime\""; 
    fprintf(altDataFilePtr,"%s\n",heading.c_str());

    std::string altOverrideDataFileName = outputDirectory + "/altOverrideData.csv";
    FILE * altOverrideDataFilePtr = fopen(altOverrideDataFileName.c_str(),"w");
    heading="\"partOrFeature\",\"altPart\",\"loc\",\"altConvCost\",\"altConvTime\""; 
    fprintf(altOverrideDataFilePtr,"%s\n",heading.c_str());
    
    std::string altExcludeLocDataFileName = outputDirectory + "/altExcludeLocData.csv";
    FILE * altExcludeLocDataFilePtr = fopen(altExcludeLocDataFileName.c_str(),"w");
    heading="\"partOrFeature\",\"altPart\",\"excludeLoc\""; 
    fprintf(altExcludeLocDataFilePtr,"%s\n",heading.c_str());

    std::string featureXrefFileName = outputDirectory + "/featureXrefData.csv";
    FILE * featureXrefFilePtr = fopen(featureXrefFileName.c_str(),"w");
    heading="\"part\",\"feature\",\"plantLoc\",\"period\",\"prodRate\""; 
    fprintf(featureXrefFilePtr,"%s\n",heading.c_str());
    
    std::string featureConversionFileName = outputDirectory + "/featureConversionData.csv";
    FILE * featureConversionFilePtr = fopen(featureConversionFileName.c_str(),"w");
    heading="\"plantLoc\",\"cost\",\"usageTime\""; 
    fprintf(featureConversionFilePtr,"%s\n",heading.c_str());
    
    std::string featureConversionOverrideFileName = outputDirectory + "/featureConversionOverrideData.csv";
    FILE * featureConversionOverrideFilePtr = fopen(featureConversionOverrideFileName.c_str(),"w");
    heading="\"part\",\"feature\",\"plantLoc\",\"period\",\"cost\",\"usageTime\""; 
    fprintf(featureConversionOverrideFilePtr,"%s\n",heading.c_str());
    
    // Read altenate part data and convert to xref files
    {
      std::cout <<"Reading input files: alternatePartData and converting to featureXref" <<std::endl;
      
      std::string alternatePartFileName = inputDirectory + "/alternatePartData.csv";
      ESO2alternatePartData alternatePartFile(alternatePartFileName);   
      ESO2alternatePartDataIterator alternatePartFileIter(alternatePartFile);
      
      //ESO2featureConversionData featureConversionFile(featureConversionFileName);
      //ESO2featureConversionOverrideData featureConversionOverrideFile(featureConversionOverrideFileName);
      
      std::string featureFileName = inputDirectory + "/featureData.csv";
      ESO2featureData featureFile(featureFileName);

      std::map<std::string,float> convTimeMap;
      std::map<std::string,float> convCostMap;
      
      // loop once for each record in alternatePart file
      for ( ; alternatePartFileIter()!=NULL; ) {
        std::string part = alternatePartFileIter.upLevelPart();
        std::string feature = alternatePartFileIter.downLevelPart();
        std::string plantLoc = alternatePartFileIter.plantLoc();
        int period = alternatePartFileIter.periodAsInt();
        float prodRate = alternatePartFileIter.prodRateAsFloat();
        float cost = alternatePartFileIter.costAsFloat();
        float usageTime = alternatePartFileIter.usageTimeAsFloat();
        
        // make sure period is in range
        if (period > nPeriods ) continue;
        
        // make sure feature exists
        bool featExists = false;
        int t;
        for (t=0; t<nPeriods; t++ ) {
          std::string key = featureFile.key(feature,plantLoc,t);
          featExists = featureFile.keyExists(key);
          if ( featExists ) break;
        }
        if ( !featExists ) continue;

        // Write feature xrefRecord       
        fprintf(featureXrefFilePtr,"\"%s\",\"%s\",\"%s\",%d,%f\n",part.c_str(),feature.c_str(),plantLoc.c_str(),period,prodRate);

        // Check to see if part-feature pair have already been written to featureConversion file.
        std::string featureConvKey = plantLoc;
        std::map<std::string,float>::const_iterator it;
        it = convCostMap.find(featureConvKey);
        if ( it == convCostMap.end() ) {
          // loc does not exist in featureConvFile, so add it
          fprintf(featureConversionFilePtr,"\"%s\",%f,%f\n",plantLoc.c_str(),cost,usageTime);
          convCostMap.insert(std::map<std::string,float>::value_type(featureConvKey,cost));
          convTimeMap.insert(std::map<std::string,float>::value_type(featureConvKey,usageTime));
        }
        else {
          // loc was already written to featureConvFile
          // get costs and see if Override needs to be written
          float featureConvCost = it->second;
          float featureConvUsageTime = convTimeMap.find(featureConvKey)->second;

          if ( featureConvCost != cost  ||  featureConvUsageTime != usageTime ) {
            // an override must be written        
            fprintf(featureConversionOverrideFilePtr,"\"%s\",\"%s\",\"%s\",%d,%f,%f\n",part.c_str(),feature.c_str(),plantLoc.c_str(),period,cost,usageTime);
          }          
        }
       }
    }
    
    //--------------------------------------
    fclose(altDataFilePtr);
    fclose(altOverrideDataFilePtr);
    fclose(altExcludeLocDataFilePtr);
    fclose(featureXrefFilePtr);
    fclose(featureConversionFilePtr);
    fclose(featureConversionOverrideFilePtr);
    //--------------------------------------
    
    
  }
  
  std::cout <<"fileConvOct03 application is finished." <<std::endl;
  return 0;
}
