#ifndef LGFRATTRIBUTEINFORMATIONH
#define LGFRATTRIBUTEINFORMATIONH
// RW2STL -- inserted:
// End of RW2STL -- inserted includes.

#include <scenario/src/generVc.h>
#include <scenario/src/error.h>

class LgFrDataInterfaceImplementation;
class LgFrDataInterfaceEssentials;

typedef void* (*LgFrPtrToValueExtractionFunction)
              (const LgFrDataInterfaceImplementation* difImp,
               const LgFrItemPtrVector* itemOrdVec,
               LgFrError& status,

	       // syntax error - typedefs can (no longer) have default args
	       const LgFrDataInterfaceEssentials* difEssenPtr /////= 0
	       );

// need to add the rest of them here
enum LgFrAttributeValueType {unknown, vectorFloat, vectorInt, timeVecInt,
                             timeVecFloat, orderedVecPart, orderedVecDemand, vectorRWBoolean, vectorRWCString,
			     boundSet};

// need to add the rest of them here
enum LgFrItemType {demand, part, BOMEntry, substituteBOMEntry};

// need to add the rest of them here
enum LgFrItemSubType {material, capacity, demandType};

class LgFrAttributeInformation
{
public:

  LgFrItemSubType itemSubType() const;
  LgFrItemType itemType() const;
  LgFrAttributeValueType attributeValueType() const;
  LgFrPtrToValueExtractionFunction valueFunctionPtr() const;
  
  // Constructor
  LgFrAttributeInformation(
                  const LgFrItemType& itemType,
                  const LgFrItemSubType& itemSubType,
                  const LgFrAttributeValueType = unknown,
                  const LgFrPtrToValueExtractionFunction = 0 );

// Copy constructor
LgFrAttributeInformation(const LgFrAttributeInformation& );
  
  // Destructor
  ~LgFrAttributeInformation();

private:

  // disallow assignment and equality as they don't really make sense here
  int operator==(const LgFrAttributeInformation&);
  int operator!=(const LgFrAttributeInformation&);
  LgFrAttributeInformation& operator=(const LgFrAttributeInformation&);

private:  
  LgFrItemType itemType_;
  LgFrItemSubType itemSubType_;
  LgFrAttributeValueType attributeValueType_;
  LgFrPtrToValueExtractionFunction valueFunctionPtr_;  
};
#endif
