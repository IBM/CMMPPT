#ifndef VISOR_printer_h
#define VISOR_printer_h

#include <algorithm>
#include "scoFile.h"


class VISORprinter : public SCOcsvFormatFile {
public:

  // Fields
  getFieldH(name);
  getFieldH(location);
  getIntFieldH(prodRate);
  getFieldH(F175);
  getFieldH(F285);
  getFieldH(PETG);
  getFieldH(PLA);
  getFieldH(ABS);
  getFieldH(ONYX);
  
  bool isYes(std::string s)
  { 
     transform(s.begin(), s.end(), s.begin(), ::tolower); 
     return s=="yes";
  }  
  
  bool F175asBool(int rec)  { return isYes(F175(rec)); }
  bool F285asBool(int rec)  { return isYes(F285(rec));}
  bool PETGasBool(int rec)  { return isYes(PETG(rec));}
  bool PLAasBool(int rec)   { return isYes(PLA(rec));}
  bool ABSasBool(int rec)   { return isYes(ABS(rec));}
  bool ONYXasBool(int rec)  { return isYes(ONYX(rec));}
  
  // preferred constructor
  VISORprinter(std::string filename)
    : SCOcsvFormatFile(filename)
  {  
    fieldSeperator(',');
    strip(true);
    hasHeaderRecord(9);
    commentRecords(true);
  };

  // compute key for storing record in dictionary
  static std::string key( 
    const std::string & name,
    const std::string & location ) 
  {
    std::string retVal = name+"$+&"+location;
    return retVal;
  };
  
  // destructor
  ~VISORprinter(){/*nothing to do here*/};
  
  // Self-test
  static void test()
  {
    VISORprinter file("../data/DataFilesV1/printer.csv");
    
    // Test reading first record

    assert( file.name(1)=="FormlabsForm2");
    assert( file.location(1) == "Kitchawan Rd" );
    assert( file.prodRate(1) == "25" );
    assert( file.prodRateAsInt(1) == 25 );
    assert( file.F175(1) == "yes" );
    assert( file.F285(1) == "yes" );
    assert( file.PETG(1) == "yes" );
    assert( file.PLA(1) == "yes" );
    assert( file.ABS(1) == "no" );
    assert( file.ONYX(1) == "yes" );
    // assert( file.altConvTimeAsFloat(0) == 2.0f );

    assert( file.prodRateAsInt(key("FormlabsForm2","Kitchawan Rd"))==25);
    assert( file.prodRateAsInt(key("FormlabsForm2","Briarcliff, NY"))==16);
    
    assert( file.F175asBool(1) );
    assert( file.F285asBool(1) );
    assert( file.PETGasBool(1) );
    assert( file.PLAasBool(1) );
    assert( !file.ABSasBool(1) );
    assert( file.ONYXasBool(1) );
   
  }
  
protected:
  
  // compute key for storing record in dictionary
  virtual std::string key( unsigned int recNum ) const
  {
    std::string retVal = key(
      name(recNum),
      location(recNum) );
    return retVal;
  };

private:
  
  // default constructor -- don't use!
  VISORprinter();
  
  // copy constructor  -- don't use!
  VISORprinter( const VISORprinter& source );
  
  // assignment operator -- don't use!
  VISORprinter&
  operator=(const VISORprinter& rhs);

  
};



class VISORprinterIterator : public SCOfileIterator {
public:
    
  // Fields
  getFieldFileWithIteratorH(name);
  getFieldFileWithIteratorH(location);
  getIntFieldFileWithIteratorH(prodRate);
  getFieldFileWithIteratorH(F175);
  getFieldFileWithIteratorH(F285);
  getFieldFileWithIteratorH(PETG);
  getFieldFileWithIteratorH(PLA);
  getFieldFileWithIteratorH(ABS);
  getFieldFileWithIteratorH(ONYX);
  
  bool F175asBool()  { return F175()=="yes";}
  bool F285asBool()  { return F285()=="yes";}
  bool PETGasBool()  { return PETG()=="yes";}
  bool PLAasBool()   { return PLA() =="yes";}
  bool ABSasBool()   { return ABS() =="yes";}
  bool ONYXasBool()  { return ONYX()=="yes";}
    
  // preferred constructor
  VISORprinterIterator(const VISORprinter & file)
    :SCOfileIterator(file)
  { /* Nothing to do here */};
  
  // destructor
  ~VISORprinterIterator(){/* nothing to do here */};
  
  // Self-test
  static void test()
  {
    VISORprinter file("../data/DataFilesV1/printer.csv");
    VISORprinterIterator fileIter(file);
    
    // loop once for each record
    int recNo=0;
    for ( ; fileIter()!=NULL; ) {    
      if ( recNo == 2 ) {
        
        assert( fileIter.name()=="MakerBotReplicator+");
        assert( fileIter.location() == "Amawalk" );
        assert( fileIter.prodRate() == "10" );
        assert( fileIter.F175() == "yes" );
        assert( fileIter.F285() == "no" );
        assert( fileIter.PETG() == "yes" );
        assert( fileIter.PLA() == "yes" );
        assert( fileIter.ABS() == "yes" );
        assert( fileIter.ONYX() == "no" );    
        
        assert( fileIter.prodRateAsInt() == 10 );  
         
        assert( fileIter.F175asBool() );
        assert(!fileIter.F285asBool() );
        assert( fileIter.PETGasBool() );
        assert( fileIter.PLAasBool()  );
        assert( fileIter.ABSasBool()  );
        assert(!fileIter.ONYXasBool() );  
          
      }
      recNo++;
    }
    
  }

private:
  
  
  // default constructor -- don't use!
  VISORprinterIterator();
  
  // copy constructor  -- don't use!
  VISORprinterIterator( const VISORprinterIterator& source );
  
  // assignment operator -- don't use!
  VISORprinterIterator&
  operator=(const VISORprinterIterator& rhs);

  
};
#endif
