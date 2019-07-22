#ifndef sce_demandDB_h
#define sce_demandDB_h

#include <sce/src/dbObj.h>
#include <sce/src/dbLink.h>

class SceDBObj;

class SceDemandDB : public SceDBObj
{
public:
 
  SceDemandDB();
  // void setAttribute(SceAttrId, SceAttrValue&)
  //void groupUpdate(){}
  void update(const char* field, const SceDBLink& demand);
  void groupUpdate(
                   const char *   field,
                   int            nPeriods,                   
                   const char*    part,
                   const char *   geo,
                   float *        timeVec);
 
  
};

#endif
