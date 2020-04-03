#ifndef VISOR_GLOBALATTRDATA_h
#define VISOR_GLOBALATTRDATA_h

#include "scoFile.h"


class VISORglobalAttrData : public SCOcsvFormatFile {
public:

    // fields  
    getFieldH(attrName);
    getFieldH(attrValue);

    // preferred constructor
    VISORglobalAttrData(std::string filename)
            : SCOcsvFormatFile(filename)
    {
        fieldSeperator(',');
        strip(true);
        hasHeaderRecord(2);
        commentRecords(true);
    };

    // destructor
    ~VISORglobalAttrData()
    {
        // Nothing to do here
    }

    // Self-test
    static void test()
    {
        std::string fileName = "../data/DataFilesV1/globalAttrData.csv";
        assert(  SCOfile::fileExist(fileName) );
        assert( !SCOfile::fileExist(fileName+"x123x") );

        VISORglobalAttrData file(fileName);

        //assert( file.attrValue("title") == "Sample Title" );
        //assert( file.fieldAsString("title","attrValue") == "Sample Title" );
        assert( file.fieldAsInt("eqAllocPerBnd","attrValue") == 6 );
        //assert( file.fieldAsInt("nWfmvScaleFactors","attrValue") == 4 );
        //assert( file.fieldAsFloat("wfmvScaleFactor1","attrValue") == 1. );
        //assert( file.fieldAsFloat("wfmvScaleFactor2","attrValue") == 0.8f );
        //assert( file.fieldAsFloat("wfmvScaleFactor3","attrValue") == 0.6f );
        //assert( file.fieldAsFloat("wfmvScaleFactor4","attrValue") == 0.4f );

    }

protected:

    // compute key for storing record in dictionary
    virtual
    std::string
    key( unsigned recNum ) const
    {
        std::string retVal = fieldAsString(recNum,"attrName");
        return retVal;
    }

private:


    // default constructor -- don't use!
    VISORglobalAttrData();

    // copy constructor  -- don't use!
    VISORglobalAttrData( const VISORglobalAttrData& source );

    // assignment operator -- don't use!
    VISORglobalAttrData&
    operator=(const VISORglobalAttrData& rhs);


};


#if 0
class VISORglobalAttrDataIterator : public SCOfileIterator {
public:
  
  // fields
  //std::string locName() const;

  // preferred constructor
  VISORglobalAttrDataIterator(const VISORglobalAttrData &);
  
  // destructor
  ~VISORglobalAttrDataIterator();
  
  // Self-test
  static void test();

private:
  
  
  // default constructor -- don't use!
  VISORglobalAttrDataIterator();
  
  // copy constructor  -- don't use!
  VISORglobalAttrDataIterator( const VISORglobalAttrDataIterator& source );
  
  // assignment operator -- don't use!
  VISORglobalAttrDataIterator&
  operator=(const VISORglobalAttrDataIterator& rhs);

  
};
#endif
#endif