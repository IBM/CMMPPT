#ifndef LGFRDATAINTERFACEIFSPECIALPURPOSE_H
#define LGFRDATAINTERFACEIFSPECIALPURPOSE_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/difAbstr.h>

// This is a base class for DIF with special purpose derived from LgFrDataInterfaceAbstract.
// Most DIF can be handled by LgFrDataInterface (a general DIF providing query functions
// and handling C interface to other databases, such as MAPICS.)
// LgFrDataInterfaceSpecialPurpose: In general, this class provides all other
// types of data interfaces for witrun which do not conform to the LgFrDataInterface
// criteria as mentioned above. It does not provide any query function and
// it is used for special purposes only.
// For example, LgFrFileDataInterface falls in this category because it reads in
// wit data file directly without "knowing" what is read.

//Forward reference
class LgFrDataInterfaceEssentials;


class LgFrDataInterfaceSpecialPurpose: public LgFrDataInterfaceAbstract
{
public:

    virtual int GetDIType() const { return __LGFRDATAINTERFACESPECIALPURPOSEFROMFILE; }
    
  //-------------------------------------------------------------------
  //  
  //   Methods to obtain global attributes
  //
  //-------------------------------------------------------------------

  // Set problem title
  virtual
    void
      title(std::string t) = 0;

  // Return problem title
  virtual
    std::string title(const LgFrDataInterfaceEssentials * = 0) const = 0;

  // This method should be used when the dif will not be used for a while
  // and resources should be freed until the dif is needed again.  It
  // is expected that this method might be called at the
  // end of initialProblem::feedInto.  In this base class this method just
  // returns.
  virtual
    void
      deleteExtraResources() = 0;

  //--------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------

  // Make a clone copy of this object
  virtual
    LgFrDataInterfaceAbstract *
        clone() const = 0;

  // Constructor
  LgFrDataInterfaceSpecialPurpose();

  // Destructor
  virtual
    ~LgFrDataInterfaceSpecialPurpose();
    
};
#endif
