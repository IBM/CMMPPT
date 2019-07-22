#ifndef LGFRATTRIBUTETABLE_H
#define LGFRATTRIBUTETABLE_H

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <map>
#include <string>
#include <vector>
// End of RW2STL -- inserted includes.

// This file conatins LgFrAttributeTable and LgFrGSlistAttributeInformation.

#include <scenario/src/attrInfo.h>
#include <scenario/src/generVc.h>
#include <scenario/src/key.h>

// This attribute table uses attribute name as the key and a singled
// linked-list of LgFrAttributeInformation(LgFrGSlistAttributeInformation)
// as the value.
// The search sequence is,
// (1) Search the right key first,
// (2) Then loop through the linked-list of LgFrAttributeInformation and
//     find the one that matches the itemSubType and itemType.

//declare (RWGSlist, LgFrAttributeInformation)
//typedef RWGSlist(LgFrAttributeInformation) LgFrGSlistAttributeInformation;

typedef std::vector<LgFrAttributeInformation*> LgFrGSlistAttributeInformation;
typedef std::pair<std::string,LgFrGSlistAttributeInformation*> AttrInfoDictPair;
typedef std::map<std::string,LgFrGSlistAttributeInformation*> AttributeInfoDictionary;

class LgFrAttributeTable 
{
public:
  
  LgFrAttributeValueType
    attributeValueType(const std::string& attributeName,
                       const LgFrItemSubType& itemSubType,
                       const LgFrItemType& itemType) const;

  void registerFunction(const std::string& attributeName,
                        const LgFrItemSubType& itemSubType,
                        const LgFrItemType& itemType,
                        const LgFrAttributeValueType& attrValueType,
                        const LgFrPtrToValueExtractionFunction& valFuncPtr);

  void unregisterFunction(const std::string& attributeName,
                          const LgFrItemSubType& itemSubType,
                          const LgFrItemType& itemType);


  LgFrPtrToValueExtractionFunction
    getAttributeFunction(const std::string& attributeName,
                         const LgFrItemSubType& itemSubType,
                         const LgFrItemType& itemType) const;

  // Copy constructor (deep copy)
  LgFrAttributeTable (const LgFrAttributeTable& copy);

  // Constructor
  LgFrAttributeTable ();

  // Destructor
  ~LgFrAttributeTable();
  
private:

  // disallow assignment and equality as they don't really make sense here
  int operator==(const LgFrAttributeTable&);
  LgFrAttributeTable& operator=(const LgFrAttributeTable&);

  void
      unregisterFunction(const std::string& key,
                     const LgFrAttributeInformation& attrInfo);

  // Insert new LgFrAttributeInformation into the dictionary_.
  void
      insert(const std::string& keyPtr,
         const LgFrAttributeInformation& attrInfoPtr);

private:  

  //RWHashDictionary dictionary_;
    AttributeInfoDictionary dictionary_;
};

#endif
