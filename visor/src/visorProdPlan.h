#ifndef VISOR_visorProdPlan_h
#define VISOR_visorProdPlan_h

#include "scoFile.h"
#include "VisorFloatEqual.h"


class VISORvisorProdPlan : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(producingLocation);
  getIntFieldH(date);
  getFloatFieldH(productionQuantity);

  // preferred constructor
  VISORvisorProdPlan(std::string filename)
      : SCOcsvFormatFile(filename)
  {
    fieldSeperator(',');
    strip(true);
    hasHeaderRecord(3);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key(
      const std::string & productionLocation,
      const std::string & date )
  {
    std::string retVal = productionLocation+"$+&"+date;
    return retVal;
  };

  // destructor
  ~VISORvisorProdPlan(){/*nothing to do here*/};

  // Self-test
  static void test()
  {
    VisorRelFltEq eq(1.e-05);
    VISORvisorProdPlan file("../data/DataFilesV1/visorProdPlan.csv");

    // Test reading first record

    assert( file.producingLocation(0)=="Kitchawan Rd");
    assert( file.date(0) == "0" );
    assert( file.productionQuantity(0)=="30" );
    assert( file.dateAsInt(0) == 0 );
    assert( eq(file.productionQuantityAsFloat(0),30.0) );
    // assert( file.altConvTimeAsFloat(0) == 2.0f );

    assert( eq(file.productionQuantityAsFloat(key("Briarcliff, NY","6")),16) );

  }

protected:

  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
        producingLocation(recNum),
        date(recNum) );
    return retVal;
  };

private:

  // default constructor -- don't use!
  VISORvisorProdPlan();

  // copy constructor  -- don't use!
  VISORvisorProdPlan( const VISORvisorProdPlan& source );

  // assignment operator -- don't use!
  VISORvisorProdPlan&
  operator=(const VISORvisorProdPlan& rhs);


};



class VISORvisorProdPlanIterator : public SCOfileIterator {
public:

  // Fields
  getFieldFileWithIteratorH(producingLocation);
  getIntFieldFileWithIteratorH(date);
  getFloatFieldFileWithIteratorH(productionQuantity);


  // preferred constructor
  VISORvisorProdPlanIterator(const VISORvisorProdPlan & file)
      :SCOfileIterator(file)
  { /* Nothing to do here */};

  // destructor
  ~VISORvisorProdPlanIterator(){/* nothing to do here */};

  // Self-test
  static void test()
  {
    VISORvisorProdPlan file("../data/DataFilesV1/visorProdPlan.csv");
    VISORvisorProdPlanIterator fileIter(file);
    VisorRelFltEq eq(1.e-05);

    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {
      if ( recNo == 0 ) {

        assert( fileIter.producingLocation()=="Kitchawan Rd");
        assert( fileIter.date() == "0" );
        assert( fileIter.productionQuantity()=="30" );
        assert( fileIter.dateAsInt() == 0 );
        assert( eq(fileIter.productionQuantityAsFloat(),30.0) );

      }
      recNo++;
    }

  }

private:


  // default constructor -- don't use!
  VISORvisorProdPlanIterator();

  // copy constructor  -- don't use!
  VISORvisorProdPlanIterator( const VISORvisorProdPlanIterator& source );

  // assignment operator -- don't use!
  VISORvisorProdPlanIterator&
  operator=(const VISORvisorProdPlanIterator& rhs);


};
#endif
