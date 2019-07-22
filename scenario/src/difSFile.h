#ifndef LgFrDataInterfaceSpecialPurposeFromFile_h
#define LgFrDataInterfaceSpecialPurposeFromFile_h

// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.
// This subclass of LgFrDataInterfaceSpecialPurpose is used to read data from a a WIT data-file.
// For now (8/28/94), it should only be used with LgFrInitialProblemFromFile
// because it is not actually able to parse WIT files.  The scenario-builder
// should insure this by making sure that it is only used in scenario's that
// use LgFrInitialProblemFromFile.

// Note: right now this class is a subclass of LgFrDataInterfaceSpecialPurpose so that
// if a client uses the normal LgFrDataInterface interface, it will not get
// any errors.


#include <wit/src/wit.h>
#include <scenario/src/difSP.h>


class LgFrDataInterfaceSpecialPurposeFromFile : 
                      public LgFrDataInterfaceSpecialPurpose
{
public:

  // Return problem title
  virtual
    std::string
      title(const LgFrDataInterfaceEssentials* = 0) const;

  // Set problem title
  virtual
    void
      title(std::string t);

  // Returns the file name of the wit.data file that the initial
  // parameters are coming from.
  virtual
    std::string
      fileName()
	const;

  // Read the WIT data file specified by fileName_ into wr
  virtual
  void
  readDataFile (WitRun * const wr);

  // Do any massaging of the data that should be done after reading it from
  // the wit file
  virtual
  void
  massageWitRun (WitRun * const wr);

  //--------------------------------------------------------------------
  //
  //   Methods to manage resources
  //
  //-------------------------------------------------------------------

  // Free up extra resources
  virtual
    void
      deleteExtraResources();

  // Make a clone copy of this object
  virtual
    LgFrDataInterfaceAbstract *
        clone()
           const;

  // Destructor
  virtual
     ~LgFrDataInterfaceSpecialPurposeFromFile();

  // Assignment operator.
  LgFrDataInterfaceSpecialPurposeFromFile&
    operator=(
	      const LgFrDataInterfaceSpecialPurposeFromFile& rhs);

  // Default Constructor. 
  LgFrDataInterfaceSpecialPurposeFromFile();

  // Another constructor
  // Constructor to Create an instance for the file named in the first parameter.
  // This is the constructor that most instances of this class should be made with,
  // since, except for assignment, there is no way to assign a set a fileName
  // in a LgFrDataInterfaceSpecialPurposeFromFile excpet by assigning 
  // another LgFrDataInterfaceSpecialPurposeFromFile to it.
  LgFrDataInterfaceSpecialPurposeFromFile(const std::string& fileName);

  // Copy constructor
  LgFrDataInterfaceSpecialPurposeFromFile(
      const LgFrDataInterfaceSpecialPurposeFromFile& source);

  // Tests this class.  Only returns if all tests passed
  static
    void
      test ();

private:
  // The name of the wit.data file the initial problem comes from
  std::string fileName_;
  
};

#endif
