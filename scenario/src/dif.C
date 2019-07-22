// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <scenario/src/dif.h>
#include <scenario/src/difImpl.h>

// Set problem title
void
LgFrDataInterface::title(
    std::string t)
{
    difImpPtr_->title(t);
}

// Return problem title

std::string 
LgFrDataInterface::title(const LgFrDataInterfaceEssentials * difEssenPtr)
const
{
  return difImpPtr_->title(difEssenPtr);
}

// Extract part attributes
void*
LgFrDataInterface::partAttribute(
  LgFrPartVector & pv,
  const std::string & itemSubTypeStr,
  const std::string & attrName,
  const LgFrDataInterfaceEssentials* difEssenPtr
  ) const
{
  LgFrItemType itemType = part;
  LgFrItemSubType itemSubType = material;
  if ( itemSubTypeStr == "capacity" )
     itemSubType =  capacity;
  else if ( itemSubTypeStr != "material" ) {
    std::cerr << "\n Wrong item sub-type is used.  "
      << "It should be \"material\" or \"capacity\"." 
      << std::endl;
  }
  
  LgFrItemPtrVector itemPtrVect;
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < pv.size(); i++)
    {
      LgFrPart& part = pv[i];
      itemPtrVect.push_back(&part);
    }
  return this->getAttributeValue(attrName, itemSubType,
				 itemType, itemPtrVect, difEssenPtr);
}

// Extract demand attributes
void*
LgFrDataInterface::demandAttribute(
    LgFrDemandVector & dv,
    const std::string & attrName,
    const LgFrDataInterfaceEssentials* difEssenPtr
    ) const
{
  LgFrItemType itemType = demand;
  LgFrItemSubType itemSubType = demandType;
  LgFrItemPtrVector itemPtrVect;
  int i = 0; // Pulled out of the for below by RW2STL
  for ( i = 0; i < dv.size(); i++)
    {
      LgFrDemand& demand = dv[i];
      itemPtrVect.push_back(&demand);
    }
  return this->getAttributeValue(attrName, itemSubType,
				 itemType, itemPtrVect, difEssenPtr);
}

LgFrAttributeValueType
LgFrDataInterface::attributeValueType(const std::string& attributeName,
                                      const LgFrItemSubType& itemSubType,
                                      const LgFrItemType& itemType)
     const
{
  return difImpPtr_->attributeValueType(attributeName,
                                        itemSubType, itemType);
}

void*
LgFrDataInterface::getAttributeValue(
    const std::string& attrName,
    const LgFrItemSubType& itemSubType,
    const LgFrItemType& itemType,
    const LgFrItemPtrVector& pv,
    const LgFrDataInterfaceEssentials* difEssenPtr)
const
{
  return difImpPtr_->getAttributeValue
           (attrName, itemSubType, itemType, pv, difEssenPtr);
}

bool
LgFrDataInterface::isAttributeFunctionImplemented(
                           const std::string& attrName,
                           const LgFrItemSubType& itemSubType,
                           const LgFrItemType& itemType)
     const
{
  return difImpPtr_->
           isAttributeFunctionImplemented(attrName,
					  itemSubType, itemType);
}

// Assignment
LgFrDataInterface&
LgFrDataInterface::operator=(const LgFrDataInterface& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
    difImpPtr_ = rhs.difImpPtr_->clone();
  }
  return *this;
}

// Free up any resources
void
LgFrDataInterface::deleteExtraResources()
{
  // Nothing to do here in base class
}


// Overloaded operator ->
// This is used to invoke methods in difImpPtr_ stored in this class
// to avoid duplicating all signatures in difImpPtr_.
// This follows the handle/body (counted pointer idiom) in 
// "Advanced C++" by James. Coplien.
// LgFrDataInterface is used as a handle class and 
// LgFrDataInterfaceImplementation is used as a body class.
// 6/29/95 JMT have not yet found a good use of this operator.
LgFrDataInterfaceImplementation* 
LgFrDataInterface::operator->() 
const
{
    return difImpPtr_;
}

// Return the LgFrDataInterfaceImplementation (return const refernece for security)
LgFrDataInterfaceImplementation*
LgFrDataInterface::dataInterfaceImplementation() 
const
{
    return difImpPtr_;
}


// Make copy of DIF without making it belong to a scenario
LgFrDataInterfaceAbstract *
LgFrDataInterface::clone()
const
{
  return new LgFrDataInterface(*this);
}

// Copy constructor in same scenario
LgFrDataInterface::LgFrDataInterface(const LgFrDataInterface& copy)
    : LgFrDataInterfaceAbstract(copy)
{
   difImpPtr_ = copy.difImpPtr_->clone();
}

// Default constructor
LgFrDataInterface::LgFrDataInterface()
    : LgFrDataInterfaceAbstract(),
      difImpPtr_(0)
{
// nothing to do here
}

// Constructor
LgFrDataInterface::LgFrDataInterface(std::string title,
			     LgFrDataInterfaceImplementation* impPtr)
    : LgFrDataInterfaceAbstract(),
      difImpPtr_(impPtr->clone())
{
    difImpPtr_->title(title);
}

// Constructor
LgFrDataInterface::LgFrDataInterface(LgFrDataInterfaceImplementation* impPtr)
    : LgFrDataInterfaceAbstract(),
      difImpPtr_(impPtr->clone())
{
// Nothing to do here
}

// Destructor
LgFrDataInterface::~LgFrDataInterface()
{
  delete difImpPtr_;
}
