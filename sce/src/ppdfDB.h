#ifndef sce_partPdfDB_h
#define sce_partPdfDB_h

#include <sce/src/dbObj.h>
#include <sce/src/dbLink.h>

class SceDBObj;

class ScePartPdfDB : public SceDBObj
{
public:
 
  ScePartPdfDB();
  void update(const char* field, const SceDBLink& partPdf);
  void groupUpdate(
                   const char *   field,
                   int            nPeriods,                   
                   const char*    part,
                   const char *   geo,
                   float *   timeVec);
 
  
};

#endif
