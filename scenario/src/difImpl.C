#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
#include <scenario/src/difImpl.h>


// Set problem title
void
LgFrDataInterfaceImplementation::title(
    std::string t)
{
    title_ = t;
}

// Return problem title
// LgFrDataInterfaceEssentials* is needed for derived class,such as
// LgFrDataInterfaceImplementationFromCmrpFile 

std::string 
LgFrDataInterfaceImplementation::title(
    const LgFrDataInterfaceEssentials * difEssenPt)
const
{
  return title_;
}


void
LgFrDataInterfaceImplementation::registerFunction(
    const std::string& attributeName,
    const LgFrItemSubType& itemSubType,
    const LgFrItemType& itemType,
    const LgFrAttributeValueType& attrValueType,
    LgFrPtrToValueExtractionFunction valFuncPtr)
{
  LgFrAttributeInformation attrInfo(itemType, itemSubType,
                                    attrValueType, valFuncPtr);
  attributeTable_.registerFunction(attributeName, itemSubType,
                                   itemType, attrValueType, valFuncPtr);
}

void
LgFrDataInterfaceImplementation::unregisterFunction(
    const std::string& attributeName,
    const LgFrItemSubType& itemSubType,
    const LgFrItemType& itemType)
{
  attributeTable_.unregisterFunction(attributeName, itemSubType, itemType);
}

LgFrAttributeValueType
LgFrDataInterfaceImplementation::attributeValueType(
    const std::string& attributeName,
    const LgFrItemSubType& itemSubType,
    const LgFrItemType& itemType)
const
{
  return attributeTable_.attributeValueType
           (attributeName, itemSubType, itemType);
}

void *
LgFrDataInterfaceImplementation::getAttributeValue(
    const std::string& attributeName,
    const LgFrItemSubType& itemSubType,
    const LgFrItemType& itemType,
    const LgFrItemPtrVector& pv,
    const LgFrDataInterfaceEssentials* difEssenPtr
    )
const
{
  LgFrPtrToValueExtractionFunction vfunc =
    attributeTable_.getAttributeFunction
      (attributeName, itemSubType, itemType);
  LgFrError status;
  return vfunc(this,&pv,status, difEssenPtr);
}

bool
LgFrDataInterfaceImplementation::isAttributeFunctionImplemented(
    const std::string& attributeName,
    const LgFrItemSubType& itemSubType,
    const LgFrItemType& itemType)
const
{
  bool isImp = true;
  LgFrPtrToValueExtractionFunction vf =
         attributeTable_.getAttributeFunction(attributeName,
                                              itemSubType,
                                              itemType);
  if ( vf == 0 ) isImp = false;
  return isImp;
}

// Assignment
LgFrDataInterfaceImplementation&
LgFrDataInterfaceImplementation::operator=(const LgFrDataInterfaceImplementation& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
     title_ = rhs.title_;
//      attributeTable_ = rhs.attributeTable_;
  }
  return *this;
}

// clone 
LgFrDataInterfaceImplementation *
LgFrDataInterfaceImplementation::clone()
const
{
  return new LgFrDataInterfaceImplementation(*this);
}

// Copy constructor in same scenario
LgFrDataInterfaceImplementation::LgFrDataInterfaceImplementation
  (const LgFrDataInterfaceImplementation& copy)
      : title_(copy.title_),
	attributeTable_(copy.attributeTable_)
{
// nothing to do here
}

// Default constructor
LgFrDataInterfaceImplementation::LgFrDataInterfaceImplementation()
    : title_("Unknown")
{
// nothing to do here
}

// Another constructor
LgFrDataInterfaceImplementation::LgFrDataInterfaceImplementation(std::string t)
    : title_(t)
{
// nothing to do here
}

// Destructor
LgFrDataInterfaceImplementation::~LgFrDataInterfaceImplementation()
{
// nothing to do here
}
