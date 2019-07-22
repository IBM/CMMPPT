#ifndef LGFRDATAINTERFACEABSTARCT_H
#define LGFRDATAINTERFACEABSTARCT_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/idnumber.h>

// This is the DIF abstract class.
// Two subclasses derived from this class:
// (1) LgFrDataInterface: This is a generl purpose DIF class which maps data from
//     one specific database to witrun and provides query functions.  The detail
//     implementation is done in an implementation class called
//     LgFrDataInterfaceImplementation, which is contained in LgFrDataInterface.
//     (related to Bridge pattern)
// (2) LgFrDataInterfaceSpecialPurpose: In general, this class provides all other
//     types of data interfaces for witrun which do not conform to the LgFrDataInterface
//     criteria as mentioned in above (1). It does not provide any query function
//     and it is used for special purposes only.
//     For example, LgFrFileDataInterface falls in this category because it reads in
//     wit data file directly without "knowing" what is read.

// Forward reference 
class LgFrDataInterfaceEssentials;


class LgFrDataInterfaceAbstract
{
public:

  virtual int GetDIType() const = 0;

  // Set problem title
  virtual
    void
      title(std::string t) = 0;

  // Return problem title
  virtual
    std::string title(const LgFrDataInterfaceEssentials * = 0 ) const = 0;



  // This method should be used when the dif will not be used for a while
  // and resources should be freed until the dif is needed again.  It
  // is expected that this method might be called at the
  // end of initialProblem::feedInto.  In this base class this method just
  // returns.
  virtual
    void
      deleteExtraResources()=0;


  //-------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------

  // Make a clone copy of this object
  virtual
    LgFrDataInterfaceAbstract *
        clone()
           const = 0;

  // Destructor
  virtual
    ~LgFrDataInterfaceAbstract();

};
#endif
