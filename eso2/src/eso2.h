#ifndef ESO2_h
#define ESO2_h

#include <string>
#include "eso2Problem.h"
 
void createModel(
                 ESO2problem & eso2Prob, 
                 const std::string & inputDirectory,
                 bool useGlobalAttrFileSettings );

#endif
