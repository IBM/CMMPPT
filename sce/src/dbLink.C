
// RW2STL - inserted:
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
#include <sce/src/dbLink.h>
#include <iostream.h>

// RW2STL - implement (RWGVector,        SceArgListPtr)

// RW2STL - implement (RWGOrderedVector, SceArgListPtr)


SceDBLink::SceDBLink(SceDBObj& db)
  : dataSet_ (* new SceArgListPtrVector),
    tbl_(db)
{
  // cout<< "dbLink.size() = " << dataSet_.size() << endl;
  // for (size_t i = 0; i < dataSet_.size(); i++)
  //  {
  //   dataSet_[i] = 0;
  // }
}

SceDBLink::~SceDBLink()
{
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<dataSet_.size(); i++ )
    {
      if (dataSet_[i]) 
        delete dataSet_[i];
    }
  delete &dataSet_;
  //dataSet_ = 0;
  //delete & tbl_;
}

void SceDBLink::update(const char* field)
{
  tbl_.update(field, *this);
}

//void SceDBLink::insert(const char* field)

void SceDBLink::insert()
{
  tbl_.push_back(*this);
}

SceArgList & SceDBLink::operator[](size_t i) const
{
  return *(dataSet_[i-1]);
}

void SceDBLink::insert(const SceArgList& al)
{
  SceArgList *pAl = new SceArgList(al);
  // SceArgList *pAl = new SceArgList;
  //for ( size_t i=1; i<al.length() + 1; i++ )
  // {
  // pAl  -> insert(al[i]);
  //}
  assert(pAl);
  dataSet_.push_back((SceArgListPtr)pAl);
}

size_t SceDBLink::size() const
{
  return dataSet_.size();
}
