#ifndef sce_supplyResponseDB_h
#define sce_supplyResponseDB_h

#include <sce/src/dbObj.h>
#include <sce/src/dbLink.h>

class SceDBObj;

class SceSupplyResponseDB : public SceDBObj
{
public:
 
  SceSupplyResponseDB();
 
  void insert(const SceDBLink& supply);
  void insert(const char* field, const SceDBLink& dataSet){}
  void groupInsert(
                    int            nPeriods,
                    const char *   part1, 
                    const char *   geo,
                    const char *   part2,
                    const char *   pdf1,
                    float *        supplyVol);
 
  void deleteTbl();
  static int  fDelete() { return fDelete_;}
  
private:

  static int fDelete_;
  
};

#endif
