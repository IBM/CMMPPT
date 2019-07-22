
#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
#include <scenario/src/attTable.h>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>

LgFrAttributeValueType
LgFrAttributeTable::attributeValueType(const std::string& attrName,
                                       const LgFrItemSubType& itemSubType,
                                       const LgFrItemType& itemType)
     const
{
    AttributeInfoDictionary::const_iterator it = dictionary_.find( attrName );
    
    if( it != dictionary_.end() )
    {
        LgFrGSlistAttributeInformation* pv = it->second;

        for( LgFrGSlistAttributeInformation::iterator it2 = pv->begin(); it2 != pv->end(); it2++ )
            if( (*it2)->itemSubType() == itemSubType &&
                (*it2)->itemType() == itemType )
            {
              return ( (*it2)->attributeValueType() );
            }
    }
    return unknown;
}

LgFrPtrToValueExtractionFunction
LgFrAttributeTable::getAttributeFunction(
    const std::string& attrName,
    const LgFrItemSubType& itemSubType,
    const LgFrItemType& itemType)
const
{
    AttributeInfoDictionary::const_iterator it = dictionary_.find( attrName );
    
    if( it != dictionary_.end() )
    {
        LgFrGSlistAttributeInformation* pv = it->second;

        for( LgFrGSlistAttributeInformation::iterator it2 = pv->begin(); it2 != pv->end(); it2++ )
        {
            LgFrAttributeInformation* pAI = *it2;
            if( pAI->itemSubType() == itemSubType && pAI->itemType() == itemType )
            {
              return ( pAI->valueFunctionPtr() );
            }
        }
    }
    return 0;
}

void
LgFrAttributeTable::registerFunction(
                           const std::string& attributeName,
                           const LgFrItemSubType& itemSubType,
                           const LgFrItemType& itemType,
                           const LgFrAttributeValueType& attrValueType,
                           const LgFrPtrToValueExtractionFunction& valFuncPtr)
{
  LgFrAttributeInformation attrInfo
      (itemType, itemSubType, attrValueType, valFuncPtr);

  insert(attributeName, attrInfo);
}

void
LgFrAttributeTable::unregisterFunction(
                          const std::string& attributeName,
                          const LgFrItemSubType& itemSubType,
                          const LgFrItemType& itemType)
{
  LgFrAttributeInformation attrInfo(itemType, itemSubType);
  unregisterFunction(attributeName,attrInfo);
}


// Constructor

LgFrAttributeTable::LgFrAttributeTable ()
{
// nothing to do here
}

// Copy constructor(deep copy)
LgFrAttributeTable::LgFrAttributeTable (const LgFrAttributeTable& copy)
{
  // For each key and value pair in copy's dictionary_, create a clone and 
  // insert the clone key/clone value pair to this->dictionary_.  
    for( AttributeInfoDictionary::const_iterator it1 = copy.dictionary_.begin();
            it1 != copy.dictionary_.end();
            it1++ )
    {
        const std::string& key = it1->first;
        LgFrGSlistAttributeInformation* pVec = it1->second;

        LgFrGSlistAttributeInformation* pNewVec = new LgFrGSlistAttributeInformation;

        for( LgFrGSlistAttributeInformation::iterator it2 = pVec->begin(); it2 != pVec->end(); it2++ )
            pNewVec->push_back( new LgFrAttributeInformation( *(*it2) ) );

        AttrInfoDictPair p( key, pNewVec );

        dictionary_.insert( p );
    }
}

// Destructor
LgFrAttributeTable::~LgFrAttributeTable()
{
    for( AttributeInfoDictionary::iterator iter = dictionary_.begin();
         iter != dictionary_.end();
         iter++ )
    {
        delete iter->second;
    }
}

// PRIVATE:

void
LgFrAttributeTable::unregisterFunction(
                                       const std::string& attrName,
                           const LgFrAttributeInformation& attrInfo)
{
    AttributeInfoDictionary::iterator it = dictionary_.find( attrName );
    
    if( it != dictionary_.end() )
    {
        LgFrGSlistAttributeInformation* pv = it->second;

        for( LgFrGSlistAttributeInformation::iterator it2 = pv->begin(); it2 != pv->end(); it2++ )
            if( (*it2)->itemSubType() == attrInfo.itemSubType() &&
                (*it2)->itemType() == attrInfo.itemType() )
            {
              pv->erase( it2 );
              break;
            }
    }
}

// Insert new LgFrAttributeInformation into the dictionary_.
void
LgFrAttributeTable::insert(	const std::string& k, const LgFrAttributeInformation& attrInfoPtr)
{
    AttributeInfoDictionary::iterator it = dictionary_.find( k );
    LgFrGSlistAttributeInformation* pVec = 0;

    if( it != dictionary_.end() )
    {
        pVec = it->second;
    }
    else
    {
        pVec = new LgFrGSlistAttributeInformation;

        AttrInfoDictPair p( k, pVec );
        dictionary_.insert( p );
    }

    pVec->push_back( new LgFrAttributeInformation( attrInfoPtr ) );

}








