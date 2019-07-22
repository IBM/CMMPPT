// Test individual classes or groups of classes

#include <assert.h>
#include <iostream.h>

#include <scenario/src/part.h>
#include <scenario/src/sOfParts.h>



int
main ()
{
  // Testing set of parts

     // Create a set of parts and add a few parts
     LgFrSetOfParts sop;
     sop.newPart("partA");
     sop.newPart("partB");
     sop.newPart("partC");
     sop.newPart("partD");
     sop.newPart("partE");
  

     // Get pointer to first part in set
     const LgFrPart * partPtr = sop.findValue("partA");
     assert ( partPtr != NULL );


     cerr <<"About to use part ptr returned by sop.findValue" <<endl;
     LgFrPart part( *partPtr );
     cerr <<"Successfully used part ptr to make a solid part" <<endl;


     assert( part.name()=="partA" );

}
