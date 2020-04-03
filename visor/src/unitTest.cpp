#include <assert.h>
#include <iostream>
#include <float.h>

//#include <wit.h>

#include "requestQuantity.h"
#include "printer.h"
#include "onHandMaterial.h"
#include "visorProblem1.h"
#include "visorProblem2.h"
#include "globalAttrData.h"

// Function Prototypes. Function definitions is in this file.
void testingMessage( const char * const msg );



int
main (int argc, char * argv[])
{

  testingMessage( "Testing VISORrequestQuantity\n" );
  VISORrequestQuantity::test();

  testingMessage( "Testing VISORrequestQuantityIterator\n" );
  VISORrequestQuantityIterator::test();

  testingMessage( "Testing VISORprinter\n" );
  VISORprinter::test();

  testingMessage( "Testing VISORprinterIterator\n" );
  VISORprinterIterator::test();

  testingMessage( "Testing VISORonHandMaterial\n" );
  VISORonHandMaterial::test();

  testingMessage( "Testing VISORonHandMaterialIterator\n" );
  VISORonHandMaterialIterator::test();

  testingMessage( "Testing VISORproblem1\n" );
  VISORproblem1::test();

  testingMessage( "Testing VISORproblem2\n" );
  VISORproblem2::test();

  testingMessage( "Testing VISORglobalAttrData\n" );
  VISORglobalAttrData::test();

  testingMessage( "All tests completed successfully\n" );

  return 0;
}


// Display message on stdout and stderr
void testingMessage( const char * const msg )
{
	std::cerr <<msg;
  //cout <<endl <<"*****************************************"
  //     <<endl <<msg <<endl;
}
