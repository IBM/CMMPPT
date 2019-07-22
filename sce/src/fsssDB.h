#ifndef sce_fsssingleSchemeDB_h
#define sce_fsssingleSchemeDB_h

#include <sce/src/dbObj.h>
#include <sce/src/dbLink.h>

class SceDBObj;

class SceFSSSingleSchemeDB : public SceDBObj
{
public:
 
  SceFSSSingleSchemeDB();
  // void setAttribute(SceAttrId, SceAttrValue&)
  //void groupUpdate(){}
  
  void insert(const SceDBLink& fsss);
  void insert(const char* field, const SceDBLink& dataSet){}
  void groupInsert(
                    int            nPeriods,
                    const char *   part1, 
                    const char *   geo,
                    const char *   part2,
                    const char *   pdf1,
                    float *        FSSReq,
                    float *        FSSCov,
                    float *        FSSDelta);
 
  void deleteTbl();
  static int  fDelete() { return fDelete_;}
  
private:

  static int fDelete_;
  
};

#endif
