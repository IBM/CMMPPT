#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
#include <math.h>

#include <wit/src/wit.h>
#include <scenario/src/difSFile.h>
#include <scenario/src/scenario.h>


// Return problem title
std::string
LgFrDataInterfaceSpecialPurposeFromFile::title(
    const LgFrDataInterfaceEssentials* difEssenPtr)
const
{
  return fileName();
}

// Set problem title
void
LgFrDataInterfaceSpecialPurposeFromFile::title(
    std::string t /* not used */
    )
{
// do nothing here since we don't want to change the filename.
}

std::string
LgFrDataInterfaceSpecialPurposeFromFile::fileName()
     const
{
  return fileName_;
}


void
LgFrDataInterfaceSpecialPurposeFromFile::readDataFile (WitRun * const wr)
{
  witReadData (wr, fileName_.c_str());
}

void
LgFrDataInterfaceSpecialPurposeFromFile::massageWitRun (WitRun * const )
{
}


// Free up any resources
void
LgFrDataInterfaceSpecialPurposeFromFile::deleteExtraResources()
{
  // Nothing to do here in base class
}



// Make copy of DIF without making it belong to a scenario
LgFrDataInterfaceAbstract *
LgFrDataInterfaceSpecialPurposeFromFile::clone()
const
{
  return new LgFrDataInterfaceSpecialPurposeFromFile(*this);
}

// Destructor
LgFrDataInterfaceSpecialPurposeFromFile::~LgFrDataInterfaceSpecialPurposeFromFile()
{
   // Nothing to do here
}

// Assignment operator.
LgFrDataInterfaceSpecialPurposeFromFile&
LgFrDataInterfaceSpecialPurposeFromFile::operator=(
  const LgFrDataInterfaceSpecialPurposeFromFile& rhs)
{
  if (this != &rhs) {           // Check for assignment to self
     (this->LgFrDataInterfaceSpecialPurpose::operator= ) (rhs);
     this->fileName_ = rhs.fileName_;
  }
  return *this;
}

// Another constructor
LgFrDataInterfaceSpecialPurposeFromFile::LgFrDataInterfaceSpecialPurposeFromFile(const std::string& fileName)
:
  LgFrDataInterfaceSpecialPurpose(),
  fileName_(fileName)  
{
  // Nothing to do here
}

// Copy constructor
LgFrDataInterfaceSpecialPurposeFromFile::LgFrDataInterfaceSpecialPurposeFromFile(
  const LgFrDataInterfaceSpecialPurposeFromFile& source)
:
  LgFrDataInterfaceSpecialPurpose(source),
  fileName_(source.fileName_)
{
   // Nothing to do here
}

// Default Constructor
LgFrDataInterfaceSpecialPurposeFromFile::LgFrDataInterfaceSpecialPurposeFromFile()
:
  fileName_()
{
}


#ifdef NDEBUG
#undef NDEBUG
#endif
// Tests this class.  Only returns if all tests passed
void
LgFrDataInterfaceSpecialPurposeFromFile::test ()
{
  char* fn = "../../../../wit/wit-data/examples/diner12/wit.data";

  // Test default constructor
  LgFrDataInterfaceSpecialPurposeFromFile tdif1;

  // Test filename constructor
  LgFrDataInterfaceSpecialPurposeFromFile tdif2 (fn);
  assert( tdif1.fileName() != tdif2.fileName() );

  // Test copy constructor
  LgFrDataInterfaceSpecialPurposeFromFile tdif3( tdif1 );
  assert( tdif1.fileName() == tdif3.fileName() );

  // Test clone method that takes a pointer
  LgFrDataInterfaceAbstract * dif4Ptr= tdif2.clone( );
  LgFrDataInterfaceSpecialPurposeFromFile * tdif4Ptr = (LgFrDataInterfaceSpecialPurposeFromFile *)dif4Ptr;
  assert( tdif2.fileName() == tdif4Ptr->fileName() );

  // Test destructor
  delete tdif4Ptr;

  // Test assignment
  tdif3 = tdif2;
  assert( tdif3.fileName() == tdif2.fileName() );
  assert( tdif3.fileName() != tdif1.fileName() );

  assert( tdif1.title() == tdif1.fileName() );

}
