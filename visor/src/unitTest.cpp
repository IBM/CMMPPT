#include <assert.h>
#include <iostream>
#include <float.h>

//#include <wit.h>

#include "requestQuantity.h"
#include "printer.h"

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


  //testingMessage( "Testing ESO2problem\n" );
  //ESO2problem::test();

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
