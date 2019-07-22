#ifndef LGFRDATAINTERFACE_H
#define LGFRDATAINTERFACE_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

#include <scenario/src/attrInfo.h>
#include <scenario/src/generVc.h>
#include <scenario/src/scenario.h>
#include <scenario/src/difAbstr.h>


// LgFrDataInterface owns LgFrDataInterfaceImplementation
// and will free it when LgFrDataInterface is destroyed.

// At this time, LgFrDataInterface will only return void* and
// the client of this class should own the attribute data
// and free them when done.  Once we have implemented the smart
// pointer class, we can relieve this extra burden from the client.

class LgFrDataInterfaceImplementation;

class LgFrDataInterface : public LgFrDataInterfaceAbstract
{
public:

  virtual int GetDIType() const { return __LGFRDATAINTERFACE; }

  //-------------------------------------------------------------------
  //  
  //   Methods to obtain attribute information.
  //
  //-------------------------------------------------------------------

  virtual
  void* partAttribute(LgFrPartVector &,
                      const std::string &,
                      const std::string &,
                      const LgFrDataInterfaceEssentials* difEssenPtr = 0
		      ) const;
  
  virtual
  void* demandAttribute(LgFrDemandVector &,
                        const std::string &,
			const LgFrDataInterfaceEssentials* difEssenPtr = 0
			) const;
   
  virtual
  void* getAttributeValue(const std::string& attributeName,
                          const LgFrItemSubType& itemSubType,
                          const LgFrItemType& itemType,
                          const LgFrItemPtrVector& pv,
			  const LgFrDataInterfaceEssentials* difEssenPtr = 0
			  ) const;
  
  virtual
    LgFrAttributeValueType
      attributeValueType(const std::string& attributeName,
                         const LgFrItemSubType& itemSubType,
                         const LgFrItemType& itemType) const;

  virtual
    bool
      isAttributeFunctionImplemented(const std::string& attributeName,
                                     const LgFrItemSubType& itemSubType,
                                     const LgFrItemType& itemType) const;

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
      title(const LgFrDataInterfaceEssentials * = 0) const;

  //--------------------------------------------------------------------
  //
  //   Methods to manage resources
  //
  //-------------------------------------------------------------------

  // Free up extra resources
  virtual
    void
      deleteExtraResources();


  //--------------------------------------------------------------------
  //  
  //   Standard class methods
  //
  //-------------------------------------------------------------------

  // Overloaded operator ->
  // This is used to invoke methods in difImpPtr_ stored in this class
  // to avoid duplicating all signatures in difImpPtr_.
  LgFrDataInterfaceImplementation* operator->() const;

  // Return the LgFrDataInterfaceImplementation pointer
  LgFrDataInterfaceImplementation*
    dataInterfaceImplementation() const;
    
  // Make a clone copy of this object
  virtual
    LgFrDataInterfaceAbstract *
        clone()
           const;

  // Copy constructor
  LgFrDataInterface(const LgFrDataInterface&);

  // Constructor
  LgFrDataInterface(std::string title,
		    LgFrDataInterfaceImplementation* difImpPtr);

  // Constructor
  LgFrDataInterface(LgFrDataInterfaceImplementation* difImpPtr);

  // Destructor
  virtual
    ~LgFrDataInterface();

private:
    // Default constructor
  LgFrDataInterface();

  // Assigment
  LgFrDataInterface& operator=(const LgFrDataInterface&);

private:
  LgFrDataInterfaceImplementation* difImpPtr_;
    
};
#endif
