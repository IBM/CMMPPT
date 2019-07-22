#ifndef sce_dblink_h
#define sce_dblink_h

// RW2STL - inserted:
#include <vector>
// End of RW2STL - inserted includes.

#include <sce/src/dbObj.h>
#include <sce/src/arg.h>

class SceDBObj;
class SceArgList;

typedef SceArgList *  SceArgListPtr;
// RW2STL - declare   (RWGVector,        SceArgListPtr)

// RW2STL - declare   (RWGOrderedVector, SceArgListPtr)


typedef std::vector<SceArgListPtr> SceArgListPtrVector; // RW2STL - typedef RWGOrderedVector(SceArgListPtr) SceArgListPtrVector;


class SceDBLink
{ 
public:
  SceDBLink(SceDBObj&);
  virtual ~SceDBLink();
  //execute DB update
  virtual void update(const char* field);
  //execute DB insert for a field
  virtual void insert(const char* field){};
  //execute DB insertion of new records
  virtual void insert();
  // insert data into data set
  virtual void insert(const SceArgList&);
  void export(const char* field){}
  void import(const char* fileName){}
  SceArgList& operator[](size_t i) const;
  size_t size() const;
  inline void nPeriods (int thePeriods) { nPeriods_ = thePeriods;}
  inline int nPeriods () const { return nPeriods_;}
  
protected:
  
  SceDBLink();
  int nPeriods_;
  SceArgListPtrVector& dataSet_;
  SceDBObj& tbl_;
  
private:
  SceDBLink(const SceDBLink&);
  SceDBLink& operator= (const SceDBLink&);
};

#endif
