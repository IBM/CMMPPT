#ifndef VISOR_onHandMaterial_h
#define VISOR_onHandMaterial_h

#include "scoFile.h"


class VISORonHandMaterial : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(location);
  getFieldH(filamentSize);
  getFieldH(plasticType);
  getFloatFieldH(quantity);
  getIntFieldH(share);
  
  // preferred constructor
  VISORonHandMaterial(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    hasHeaderRecord(5);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & location,
    const std::string & filamentSize,
    const std::string & plasticType ) 
  {
    std::string retVal = location+"$+&"+filamentSize+"$+&"+plasticType;
    return retVal;
  };
  
  // destructor
  ~VISORonHandMaterial(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    VISORonHandMaterial file("../data/DataFilesV1/onHandMaterial.csv");
    
    // Test reading first record

    assert( file.location(3)=="Amawalk");
    assert( file.filamentSize(3) == "1.75mm" );
    assert( file.plasticType(3) == "PLA" );    
    assert( file.quantity(3) == "20" );
    assert( file.share(3) == "100" );
    
    assert( file.quantityAsFloat(3) == 20.0 );
    
    assert( file.shareAsInt(3) == 100 );
/*
    assert( file.quantityAsFloat(key("Pleasant Valley","1.75mm","ONYX"))==2.5);
*/ 
  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      location(recNum),
      filamentSize(recNum),
      plasticType(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  VISORonHandMaterial();
  
  // copy constructor  -- don't use!
  VISORonHandMaterial( const VISORonHandMaterial& source );
  
  // assignment operator -- don't use!
  VISORonHandMaterial&
  operator=(const VISORonHandMaterial& rhs);

  
};



class VISORonHandMaterialIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(location);
  getFieldFileWithIteratorH(filamentSize);
  getFieldFileWithIteratorH(plasticType);
  getFloatFieldFileWithIteratorH(quantity);
  getIntFieldFileWithIteratorH(share);
  
  
  // preferred constructor
  VISORonHandMaterialIterator(const VISORonHandMaterial & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~VISORonHandMaterialIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    VISORonHandMaterial file("../data/DataFilesV1/onHandMaterial.csv");
    VISORonHandMaterialIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 3 ) {
               
        assert( fileIter.location()=="Amawalk");
        assert( fileIter.filamentSize() == "1.75mm" );
        assert( fileIter.plasticType() == "PLA" );    
        assert( fileIter.quantity() == "20" );
        assert( fileIter.share() == "100" );
    
        assert( fileIter.quantityAsFloat() == 20.0 );
    
        assert( fileIter.shareAsInt() == 100 );
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  VISORonHandMaterialIterator();
  
  // copy constructor  -- don't use!
  VISORonHandMaterialIterator( const VISORonHandMaterialIterator& source );
  
  // assignment operator -- don't use!
  VISORonHandMaterialIterator&
  operator=(const VISORonHandMaterialIterator& rhs);

  
};
#endif
