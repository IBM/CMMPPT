#ifndef LGFR_SCENARIO_SCHDDESC_H
#define LGFR_SCENARIO_SCHDDESC_H
// RW2STL -- inserted:
#include <string>
#include <vector>
// End of RW2STL -- inserted includes.

#include <scenario/src/generVc.h>



typedef std::string LgFrSortDescriptor;
typedef std::string LgFrFilterDescriptor;

typedef enum {English, Kana}
        LgFrNationalLanguage;

// Forward declaration


class LgFrScheduleDescriptor  
   {
     
public:

  // Schedule descriptor's long name
  const std::string &
    name(
       LgFrNationalLanguage nationalLanguage = English )
          const;

  // Schedule descriptor's name abbreviation
  const std::string &
    abbrev(
       LgFrNationalLanguage nationalLanguage = English )
          const;

  virtual
     int
        compareTo(
           const LgFrScheduleDescriptor*)
              const;

  virtual
     bool
        isEqual(
           const LgFrScheduleDescriptor * t)
              const;

  // Maximum length of a descriptor name's abbreviation
  static
    int
      maxAbbrevLength();
          
  // Enlish only Constructor.
  LgFrScheduleDescriptor( 
     const std::string & englishDescriptor,
     const std::string & englishDescriptorAbbrev );
  
  // English and Kana Constructor
  LgFrScheduleDescriptor( 
     const std::string & englishDescriptor,
     const std::string & englishDescriptorAbbrev,
     const std::string &    kanaDescriptor,
     const std::string &    kanaDescriptorAbbrev );

  // Constructor.
  LgFrScheduleDescriptor( );

  // Copy constructor
  LgFrScheduleDescriptor( 
     const LgFrScheduleDescriptor& source);
  
  // destructor
  ~LgFrScheduleDescriptor();
  
  // Assignment 
  LgFrScheduleDescriptor& 
    operator=(const LgFrScheduleDescriptor& rhs);
  
private:

  // test that abbreviation lengths are not greater than maxAbbrevLength
  virtual void testAbbrevLength() const;

  static const int maxAbbrevLength_;
  std::string englishDescriptorName_;
  std::string englishDescriptorAbbrev_;
  std::string kanaDescriptorName_;
  std::string kanaDescriptorAbbrev_;

};

// Test equality. 
bool
   operator==(
      const LgFrScheduleDescriptor &,
      const LgFrScheduleDescriptor & );

bool
   operator!=(
      const LgFrScheduleDescriptor &,
      const LgFrScheduleDescriptor & );

// RW2STL - declare (RWGVector, LgFrScheduleDescriptor)

// RW2STL - declare (RWGOrderedVector,LgFrScheduleDescriptor ) 

typedef std::vector<LgFrScheduleDescriptor> LgFrVectorScheduleDescriptor;  // RW2STL - typedef RWGVector(LgFrScheduleDescriptor) LgFrVectorScheduleDescriptor; 

typedef std::vector<LgFrScheduleDescriptor> // RW2STL - typedef RWGOrderedVector(LgFrScheduleDescriptor)

        LgFrScheduleDescriptorVector;

#endif
