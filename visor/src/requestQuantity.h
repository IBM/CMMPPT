#ifndef VISOR_requestQuantity_h
#define VISOR_requestQuantity_h

#include "scoFile.h"


class VISORrequestQuantity : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(location);
  getIntFieldH(date);
  getIntFieldH(requestedQuantity);
  
  // preferred constructor
  VISORrequestQuantity(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    hasHeaderRecord(3);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & location,
    const std::string & date ) 
  {
    std::string retVal = location+"$+&"+date;
    return retVal;
  };
  
  // destructor
  ~VISORrequestQuantity(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    VISORrequestQuantity file("../data/DataFilesV1/requestQuantity.csv");
    
    // Test reading first record

    assert( file.location(0)=="HospitalA");
    assert( file.date(0) == "0" );
    assert( file.requestedQuantity(0) == "25" );
    assert( file.dateAsInt(0) == 0 );
    assert( file.requestedQuantityAsInt(0) == 25 );
    // assert( file.altConvTimeAsFloat(0) == 2.0f );

    assert( file.requestedQuantityAsInt(key("HospitalB","0"))==10);
   
  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      location(recNum),
      date(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  VISORrequestQuantity();
  
  // copy constructor  -- don't use!
  VISORrequestQuantity( const VISORrequestQuantity& source );
  
  // assignment operator -- don't use!
  VISORrequestQuantity&
  operator=(const VISORrequestQuantity& rhs);

  
};



class VISORrequestQuantityIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(location);
  getIntFieldFileWithIteratorH(date);
  getIntFieldFileWithIteratorH(requestedQuantity);
  
  
  // preferred constructor
  VISORrequestQuantityIterator(const VISORrequestQuantity & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~VISORrequestQuantityIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    VISORrequestQuantity file("../data/DataFilesV1/requestQuantity.csv");
    VISORrequestQuantityIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 0 ) {
        
        assert( fileIter.location()=="HospitalA");
        assert( fileIter.date() == "0" );
        assert( fileIter.requestedQuantity() == "25" );
        assert( fileIter.dateAsInt() == 0 );
        assert( fileIter.requestedQuantityAsInt() == 25 );
        //assert( fileIter.altConvTimeAsFloat() == 2.0f );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  VISORrequestQuantityIterator();
  
  // copy constructor  -- don't use!
  VISORrequestQuantityIterator( const VISORrequestQuantityIterator& source );
  
  // assignment operator -- don't use!
  VISORrequestQuantityIterator&
  operator=(const VISORrequestQuantityIterator& rhs);

  
};
#endif
