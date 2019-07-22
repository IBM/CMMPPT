#ifndef sce_capacityAttrDB_h
#define sce_capacityAttrDB_h

//#include <sce/src/dbObj.h>
#include <sce/src/dbLink.h>

class SceDBObj;

class SceCapacityAttrDB : public SceDBObj
{
public:
 
  SceCapacityAttrDB();
  void update(const char* field, const SceDBLink& demand);
  void groupUpdate(
                   const char *   field,
                   int            nPeriods,                   
                   const char*    part,
                   const char *   geo,
                   float *   timeVec);
 
  
};

#endif
