#ifndef LGFRDATAINTERFACEIMPLEMENTATIONH
#define LGFRDATAINTERFACEIMPLEMENTATIONH
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/attTable.h>
#include <scenario/src/scenario.h>

// This is a DIF implementation class used to do the
// actual data interface implementation.
// The current implementation is to register all global attribute functions
// in a table, LgFrAttributeTable_.  These global functions can be C++ or C functions
// assumming the user will write these interfaces to their database, such as MAPICS.
// All registering/unregistering of functions are controlled by the user.
// This class is supposed to be hidden.  
// All golobal functions will return a 'newed' object and the client of those
// functions owns the object and needs to free it when done to avoid memory leaks.

class LgFrError;
class LgFrDataInterface;

class LgFrDataInterfaceImplementation
{
public:

  virtual int GetDIIType() const { return __LGFRDATAINTERFACEIMPLEMENTATION; }

  // Register global attribute functoion 
  void registerFunction(const std::string& attributeName,
                        const LgFrItemSubType& itemSubType,
                        const LgFrItemType& itemType,
                        const LgFrAttributeValueType& attrValueType,
                        LgFrPtrToValueExtractionFunction valFuncPtr);

  // un-register global attribute function
  void unregisterFunction(const std::string& attributeName,
                          const LgFrItemSubType& itemSubType,
                          const LgFrItemType& itemType);

  // Return attribute value type
  LgFrAttributeValueType
    attributeValueType(const std::string& attributeName,
                       const LgFrItemSubType& itemSubType,
                       const LgFrItemType& itemType) const;
  // Check to see if a global function is 
  bool
    isAttributeFunctionImplemented(const std::string& attributeName,
                                   const LgFrItemSubType& itemSubType,
                                   const LgFrItemType& itemType) const;

  // Get attribute values    
  void* getAttributeValue(const std::string& attributeName,
                          const LgFrItemSubType& itemSubType,
                          const LgFrItemType& itemType,
                          const LgFrItemPtrVector& pv,
			  const LgFrDataInterfaceEssentials* difEssenPtr = 0
                          ) const;

  //-------------------------------------------------------------------
  //  
  //   Methods to obtain global attributes
  //
  //-------------------------------------------------------------------

  // Set problem title
  virtual
    void
      title(std::string t);

  // Return problem title
  virtual
    std::string 
      title(const LgFrDataInterfaceEssentials * = 0 ) const;

  //--------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------

  // Make a clone copy of this object
  virtual
    LgFrDataInterfaceImplementation *
      clone()
        const;

  // Copy constructor in same scenario
  LgFrDataInterfaceImplementation
    (const LgFrDataInterfaceImplementation& source);
  
  // Default constructor
  LgFrDataInterfaceImplementation();

  // Another constructor
  LgFrDataInterfaceImplementation(std::string t);

  // Destructor
  virtual
    ~LgFrDataInterfaceImplementation();  

protected:

  // Assignment operator.
  LgFrDataInterfaceImplementation&
    operator=(const LgFrDataInterfaceImplementation& rhs);

private:
  std::string title_;
  LgFrAttributeTable attributeTable_;
  
};
#endif
