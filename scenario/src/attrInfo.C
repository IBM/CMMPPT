// RW2STL -- inserted:
// End of RW2STL -- inserted includes.
#include "scenario/src/attrInfo.h"

LgFrPtrToValueExtractionFunction
LgFrAttributeInformation::valueFunctionPtr()
     const
{
  return valueFunctionPtr_;
}

LgFrAttributeValueType
LgFrAttributeInformation::attributeValueType()
     const
{
    return attributeValueType_;
}

LgFrItemType
LgFrAttributeInformation::itemType()
     const
{
    return itemType_;
}

LgFrItemSubType
LgFrAttributeInformation::itemSubType()
     const
{
    return itemSubType_;
}

// Copy constructor
LgFrAttributeInformation::LgFrAttributeInformation(const LgFrAttributeInformation& copy)
    : itemType_(copy.itemType_),
      itemSubType_(copy.itemSubType_),
      attributeValueType_(copy.attributeValueType_),
      valueFunctionPtr_(copy.valueFunctionPtr_)
{
// nothing to do here
}

// Constructor
LgFrAttributeInformation::LgFrAttributeInformation(
                           const LgFrItemType& itemType,
                           const LgFrItemSubType& itemSubType,
                           const LgFrAttributeValueType attributeValueType,
                           const LgFrPtrToValueExtractionFunction valFuncPtr)
  : itemType_(itemType),
    itemSubType_(itemSubType),
    attributeValueType_(attributeValueType),
    valueFunctionPtr_(valFuncPtr)
{
// nothing to do here
}

LgFrAttributeInformation::~LgFrAttributeInformation()
{
// nothing to do here
}







