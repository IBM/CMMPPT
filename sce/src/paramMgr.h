#ifndef LGFRSCEPARAMETERMANAGER_H
#define LGFRSCEPARAMETERMANAGER_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.


#include <sce/src/sceDefin.h>
#include <mcl/src/mcl.h>
#include <sce/src/sceIOLog.h>

// forward references
class LgFrSceSetOfParameters;

class LgFrSceParameterManager
{

public:

void 
initializeSetOfParameters(const std::string & parameterFileName,
                          LgFrSceSetOfParameters & setOfParameters);

// this is a new method which will be implemented in initializeSetOfParameters
//  -- it will be responsible for reading through the parameter file and
//     over-riding the default parameters if necessary with user defined vals
//  ** returns 1 if no problems; 0 otherwise
int
processParameterFile(const std::string & parameterFileName,
		     LgFrSceSetOfParameters & setOfParameters);


// These are neccessary for error messaging by parameter manager
void
setErrFacility(MclFacility * existingFacility);

MclFacility*
getErrFacility();


void
setIOLogMgr(LgFrSceIOLogMgr * existingMgr);

LgFrSceIOLogMgr*
getIOLogMgr();



  // Assignment operator.
  LgFrSceParameterManager&
    operator=(
              const LgFrSceParameterManager& rhs);

  // Destructor
  ~LgFrSceParameterManager();


  // Copy constructor
  LgFrSceParameterManager(
               const LgFrSceParameterManager& source);

  // Default Constructor
  LgFrSceParameterManager();
  

private:


void 
initializeParameterDefaults(const std::string & paramFile,
                            LgFrSceSetOfParameters & defaultSet)
const;
  
  
const
std::string
searchParamFile(
    bool & doesParameterExist,                    
    const std::string & parameterFile,
    const std::string & targetParameter,
    const int mandatory = PURELY_OPTIONAL)
const;

const
float
searchParamFileFloat(bool & doesParameterExist,
                     const std::string & parameterFile,
                     const std::string & targetParameter,
                     const int mandatory = PURELY_OPTIONAL)
const;


const
int 
searchParamFileInt(
    bool & exists,
    const std::string & parameterFile,
    const std::string & targetParameter,
    const int mandatory = PURELY_OPTIONAL,
    const int defaultVal = 0)
const;

const
bool
searchParamFileBoolean(
    bool & exists,
    const std::string & parameterFile,
    const std::string & targetParameter,
    const int mandatory = PURELY_OPTIONAL)
const;
  

bool
doesInputFileExist(
                   const std::string & targetFile);

//--Member Data -- (pointers to useful objects)

// This is necessary for error messaging by parameter manager!
MclFacility * sceErrFacility_;
// This is used pretty much just for the purpose of printing to the iolog files
LgFrSceIOLogMgr * sceIOLogMgr_;

//--More Member Data -- these are to keep track of data for SCE input file log
std::string parameterFileName_;
long goodCount_;
long badCount_;
long commentCount_;
long totalCount_;

// Self-test
static void test();

};

#endif
