#ifndef scenarioSrcOrdVecs_h
#define scenarioSrcOrdVecs_h

// RW2STL -- inserted:
#include <string>
#include <vector>
// End of RW2STL -- inserted includes.
// Generic Vectors and Generic Ordered Vectors based on generic classes.
// Except for the generic vectors used for:
//     time vectors (see timeVec.h), 
//     item - timeVector pairs (see itemTimV.h) 
// all generic vectors that require the use of the implement macro should be 
// defined here.  These are defined here to prevent the errors that occur 
// when multiple .C files use the "implement" macro to instantiate the same 
// class.

#include <scenario/src/part.h>
#include <scenario/src/demdPerd.h>
#include <scenario/src/dateTime.h>


#define DEFAULT_VECTOR_CAPACITY 64

// is this used?
typedef LgFrDemand* LgFrDemandPtr;
typedef std::vector<LgFrDemandPtr> LgFrDemandPtrVector;
typedef std::vector<LgFrDemand> LgFrDemandVector;

// is this used?
typedef LgFrPart* LgFrPartPtr;
typedef std::vector<LgFrPartPtr> LgFrPartPtrVector;
typedef std::vector<LgFrPart> LgFrPartVector;

typedef std::vector<LgFrDemandPeriodPair> LgFrDemandPeriodPairVector;

typedef std::vector<std::string> LgFrStringVector;

typedef std::vector<size_t> LgFrSize_tVector;

typedef std::vector<int> LgFrIntVector;

typedef std::vector<bool> LgFrBoolVector;

typedef std::vector<float> LgFrFloatVector;

//typedef std::vector<SCETime> LgFrTimeVector;
typedef std::vector<LgFrDatetime> LgFrDatetimeVector;

typedef LgFrItem* LgFrItemPtr;
typedef std::vector<LgFrItemPtr> LgFrItemPtrVector;

class LgFrTimeVecFloat;
typedef std::vector<LgFrTimeVecFloat> LgFrTimeVecFloatVector;

#endif
