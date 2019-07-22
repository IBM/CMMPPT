// Test individual classes or groups of classes

#include <assert.h>

#include <iostream>
#include <fstream>

#include <string>

#include <witutil/src/fullExpl.h>

// Function Prototypes. Function definitions is in this file.
void testingMessage( const char * const msg );

int
main ()
{
  testingMessage( "Testing WUtlFullMrpExploder\n" );
  WUtlFullMrpExploder::test();

  testingMessage( "All tests completed successfully\n" );
  getchar();
  return 0;
}

 
// Display message on stdout and stderr
void testingMessage( const char * const msg )
{
    std::cerr << msg;
    std::cout << std::endl <<"*****************************************"
        << std::endl << msg << std::endl;
}



