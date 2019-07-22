#ifndef sce_ipOrdersDB_h
#define sce_ipOrdersDB_h

#include <sce/src/dbObj.h>
#include <sce/src/dbLink.h>

class SceDBObj;

class SceIPOrdersDB : public SceDBObj
{
public:
 
  SceIPOrdersDB();
 
  void insert(const SceDBLink& fsss);
  void insert(const char* field, const SceDBLink& dataSet){}
  void groupInsert(
                    int            nPeriods,
                    const char *   part1, 
                    const char *   geo,
                    const char *   part2,
                    const char *   pdf1,
                    float *        ipDemandVol);
 
  void deleteTbl();
  static int  fDelete() { return fDelete_;}
  
private:

  static int fDelete_;
  
};

#endif
