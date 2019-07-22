// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
//#include <iostream>
#include <iostream>

#include <scenario/src/schdDesc.h>

// RW2STL - implement (RWGVector, LgFrScheduleDescriptor)

// RW2STL - implement (RWGOrderedVector, LgFrScheduleDescriptor)



// Create and initialize maxAbbrevLength_
const int LgFrScheduleDescriptor::maxAbbrevLength_ = 15;

// Schedule descriptor's long name
const std::string &
LgFrScheduleDescriptor::name(
       LgFrNationalLanguage nationalLanguage )
const
{
  const std::string * retVal = &englishDescriptorName_;
  if ( nationalLanguage == Kana && kanaDescriptorName_ != "" )
    retVal = & kanaDescriptorName_;
  return *retVal;
}

// Schedule descriptor's name abbreviation
const std::string &
LgFrScheduleDescriptor::abbrev(
       LgFrNationalLanguage nationalLanguage )
const
{
  const std::string * retVal = &englishDescriptorAbbrev_;
  if ( nationalLanguage == Kana && kanaDescriptorAbbrev_ != "" )
    retVal = &kanaDescriptorAbbrev_;
  return *retVal;
}

int
LgFrScheduleDescriptor::compareTo(
   const LgFrScheduleDescriptor* c)
const
{
  const LgFrScheduleDescriptor * sd = (const LgFrScheduleDescriptor *) c; 
  int retVal = name().compare( sd->name() );
#ifdef DEBUG
  // If names are equal then assert that abbreviations & NLS names are equal
  if ( retVal==0 ) assert( abbrev() == sd->abbrev() );
  if ( retVal==0 ) assert( name(Kana) == sd->name(Kana) );
  if ( retVal==0 ) assert( abbrev(Kana) == sd->abbrev(Kana) );
#endif  
  return retVal;
}

bool
LgFrScheduleDescriptor::isEqual(
   const LgFrScheduleDescriptor * sd)
const
{
  return ( compareTo(sd) == 0 );
}

int
LgFrScheduleDescriptor::maxAbbrevLength()
{
    return LgFrScheduleDescriptor::maxAbbrevLength_;
}


// English Constructor.
LgFrScheduleDescriptor::LgFrScheduleDescriptor( 
   const std::string & englishDescriptorName,
   const std::string & englishDescriptorAbbrev)
:
   englishDescriptorName_  ( englishDescriptorName ),
   englishDescriptorAbbrev_( englishDescriptorAbbrev ),
   kanaDescriptorName_     ( "" ),
   kanaDescriptorAbbrev_   ( "" )
{
  // Make sure abbreviations are not longer than maxAbbrevLength
  testAbbrevLength();
} 

// English and Kana Constructor.
LgFrScheduleDescriptor::LgFrScheduleDescriptor( 
   const std::string & englishDescriptorName,
   const std::string & englishDescriptorAbbrev, 
   const std::string &    kanaDescriptorName,
   const std::string &    kanaDescriptorAbbrev)
:
   englishDescriptorName_  ( englishDescriptorName ),
   englishDescriptorAbbrev_( englishDescriptorAbbrev ),
   kanaDescriptorName_     (    kanaDescriptorName ),
   kanaDescriptorAbbrev_   (    kanaDescriptorAbbrev )
{
  // Make sure abbreviations are not longer than maxAbbrevLength
  testAbbrevLength();
}  

//test to make sure abbreviation length is not too long
void
LgFrScheduleDescriptor::testAbbrevLength()
const
{
#ifdef RW_NO_LOCALE
  if (
      englishDescriptorAbbrev_.length() >
      LgFrScheduleDescriptor::maxAbbrevLength()
      )
    std::cout <<englishDescriptorAbbrev_ <<std::endl;
  if (
      kanaDescriptorAbbrev_.length() >
      LgFrScheduleDescriptor::maxAbbrevLength()
      )
    std::cout <<kanaDescriptorAbbrev_ <<std::endl;
  assert(
         englishDescriptorAbbrev_.length() <=
         LgFrScheduleDescriptor::maxAbbrevLength()
         );
  assert(
         kanaDescriptorAbbrev_.length() <=
         LgFrScheduleDescriptor::maxAbbrevLength()
         );
#else
  if (
      englishDescriptorAbbrev_.length() >
      LgFrScheduleDescriptor::maxAbbrevLength()
      )
    std::cout <<englishDescriptorAbbrev_ <<std::endl;
  if (
      kanaDescriptorAbbrev_.length() >
      LgFrScheduleDescriptor::maxAbbrevLength()
      )
    std::cout <<kanaDescriptorAbbrev_ <<std::endl;
  assert(
         englishDescriptorAbbrev_.length() <=
         LgFrScheduleDescriptor::maxAbbrevLength()
         );
  assert(
         kanaDescriptorAbbrev_.length() <=
         LgFrScheduleDescriptor::maxAbbrevLength()
         );
#endif
}


// Copy constructor
LgFrScheduleDescriptor::LgFrScheduleDescriptor( 
   const LgFrScheduleDescriptor& source)
:
   englishDescriptorName_  ( source.englishDescriptorName_ ),
   englishDescriptorAbbrev_( source.englishDescriptorAbbrev_ ),
      kanaDescriptorName_  ( source.   kanaDescriptorName_ ),
      kanaDescriptorAbbrev_( source.   kanaDescriptorAbbrev_ )
{
  // Nothing to do here
}
  
// destructor
LgFrScheduleDescriptor::~LgFrScheduleDescriptor()
{
  // Nothing to do here
}     
  
// Assignment 
LgFrScheduleDescriptor& 
  LgFrScheduleDescriptor::operator=(const LgFrScheduleDescriptor& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
    englishDescriptorName_   = rhs.englishDescriptorName_;
    englishDescriptorAbbrev_ = rhs.englishDescriptorAbbrev_;
       kanaDescriptorName_   = rhs.   kanaDescriptorName_;
       kanaDescriptorAbbrev_ = rhs.   kanaDescriptorAbbrev_;
  }
  return *this;
}

// Constructor.
LgFrScheduleDescriptor::LgFrScheduleDescriptor( )
:
   englishDescriptorName_( ),
   englishDescriptorAbbrev_( ),
      kanaDescriptorName_( ),
      kanaDescriptorAbbrev_( )
{
  // Nothing to do here
}



// Test equality.  If string descriptors are equal then
// scheduleDescriptors are equal.
bool
   operator==(
      const LgFrScheduleDescriptor & lhs,
      const LgFrScheduleDescriptor & rhs)
{
  return lhs.isEqual( &rhs );
}


bool
   operator!=(
      const LgFrScheduleDescriptor & lhs,
      const LgFrScheduleDescriptor & rhs)
{
  return ! operator==(lhs,rhs);
}
