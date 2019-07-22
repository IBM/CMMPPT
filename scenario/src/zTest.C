// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
// Test templates and zero-length arrays

#include <assert.h>



// RW2STL - declare (RWGVector, int)

// RW2STL - declare (RWGOrderedVector, int)

// RW2STL - implement (RWGVector, int)

// RW2STL - implement (RWGOrderedVector, int)


// RW2STL - declare (RWGVector, std::string)

// RW2STL - declare (RWGOrderedVector, std::string)

// RW2STL - implement (RWGVector, std::string)

// RW2STL - implement (RWGOrderedVector, std::string)


// RW2STL - declare (RWGVector, std::string)

// RW2STL - declare (RWGOrderedVector, std::string)

// RW2STL - implement (RWGVector, std::string)

// RW2STL - implement (RWGOrderedVector, std::string)


int
main ()
{
  // Use block to test destruction
  {
    RWTValVector<float> zerotv(0);
    RWTValVector<float> ftv(0);
    RWTValVector<int> itv(5);
    RWTValVector<RWDate> datetv(5);
    RWTValVector<long> ltv(5);
    RWDate begin;
    assert (ftv.length()==0);
    ftv.reshape(5);
    int i = 0; // Pulled out of the for below by RW2STL
    for (i=0; i<5; ++i) {
      datetv[i]=begin+i;
      ftv[i]=i;
      itv[i]=i;
      ltv[i]=i;
      assert (itv[i] == ltv[i]);
      }
    int j = 0; // Pulled out of the for below by RW2STL
    for (j=0; j<5; ++j) {
      assert (itv[j] == ltv[j]);
      cout << ftv[j] << ": " << datetv[j] << endl;
    }
  }
  // Test zero-length RWGOrderedVector, in block to test destruction
  {
    RWGOrderedVector(int) intVec(0);	// This works
  }
  // Test zero-length RWGOrderedVector of a class, in block to test destruction
  {
    RWGOrderedVector(std::string) RWCStringVec(0);
  }
  // Test zero-length RWGOrderedVector of a collectable, in block to test destruction
  // This bombs out on SunOS with Sun's C++ compiler
  {
    RWGOrderedVector(std::string) RWCollectableStringVec(0);
  }
  return 0;
}
