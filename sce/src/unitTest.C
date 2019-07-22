#include <mcl/src/mdsPragma.h>
// RW2STL - inserted:
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
// Test individual classes or groups of classes


//#include <mcl/src/mcl.h>
#include <scenario/src/dateTime.h>
#include <scenario/src/calendar.h>
#include <scenario/src/scenDrct.h>
#include <scenario/src/schdDeSt.h>

#include <sce/src/scDeSce.h>
#include <sce/src/difSceF.h>
#include <sce/src/scBdSce.h>
#include <sce/src/scenSce.h>
#include <sce/src/scenSceP.h>
#include <sce/src/mPlantH.h>

#include <assert.h>


#include <sce/src/appData.h>
#include <sce/src/scePegging.h>

// Function Prototypes. Function definitions is in this file.
void testingMessage( const char * const msg );

int
main ()
{
  testingMessage( "Testing LgFrScePartAppData\n" );
  LgFrScePartAppData::test();

  testingMessage( "Testing LgFrSceSubBomAppData\n" );
  LgFrSceSubBomAppData::test();

  testingMessage( "Testing ScePegging\n" );
  ScePIP::test();

  testingMessage( "All tests completed successfully\n" );

#ifdef _MSC_VER
  getchar();
#endif
  return 0;
}

 
// Display message on stdout and stderr
void testingMessage( const char * const msg )
{
   std::cerr <<msg;
   std::cout <<std::endl <<"*****************************************"
        <<std::endl;
}



