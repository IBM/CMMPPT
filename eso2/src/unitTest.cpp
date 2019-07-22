#if defined(_MSC_VER)
#pragma warning(disable:4786)
#pragma warning(disable:4503)
#endif

#include <assert.h>
#include <iostream>
#include <float.h>

//#include <wit.h>

#include "altData.h"
#include "applyScaleFactorLocData.h"
#include "altOverrideData.h"
#include "altExcludeLocData.h"
#include "mtmData.h"
#include "mtmInterplantData.h"
#include "featureData.h"
#include "featureXrefData.h"
#include "featureConversionData.h"
#include "featureConversionOverrideData.h"
#include "loosePartData.h"
#include "partDemandData.h"
#include "mtmDemanBomData.h"
#include "alternatePartData.h"
#include "partInterplantData.h"
#include "globalAttrData.h"
#include "probAppData.h"
#include "mtmAppData.h"
#include "featureXrefAppData.h"
#include "eso2Problem.h"
#include "pipSequenceTriple.h"
#include "partDemandAppData.h"

// Function Prototypes. Function definitions is in this file.
void testingMessage( const char * const msg );



int
main (int argc, char * argv[])
{

  testingMessage( "Testing ESO2pipSequenceTriple\n" );
  ESO2pipSequenceTriple::test();
  testingMessage( "Testing ESO2partDemandAppData\n" );
  ESO2partDemandAppData::test();

  testingMessage( "Testing ESO2altData\n" );
  ESO2altData::test();

  testingMessage( "Testing ESO2applyScaleFactorLocData\n" );
  ESO2applyScaleFactorLocData::test();

  testingMessage( "Testing ESO2altDataIterator\n" );
  ESO2altDataIterator::test();

  testingMessage( "Testing ESO2altOverrideData\n" );
  ESO2altOverrideData::test();

  testingMessage( "Testing ESO2altOverrideDataIterator\n" );
  ESO2altOverrideDataIterator::test();

  testingMessage( "Testing ESO2altExcludeLocData\n" );
  ESO2altExcludeLocData::test();

  testingMessage( "Testing ESO2altExcludeLocDataIterator\n" );
  ESO2altExcludeLocDataIterator::test();

  testingMessage( "Testing ESO2mtmData\n" );
  ESO2mtmData::test();

  testingMessage( "Testing ESO2mtmDataIterator\n" );
  ESO2mtmDataIterator::test();

  testingMessage( "Testing ESO2featureData\n" );
  ESO2featureData::test();

  testingMessage( "Testing ESO2featureXrefData\n" );
  ESO2featureXrefData::test();

  testingMessage( "Testing ESO2featureConversionData\n" );
  ESO2featureConversionData::test();

  testingMessage( "Testing ESO2featureConversionOverrideData\n" );
  ESO2featureConversionOverrideData::test();

  testingMessage( "Testing ESO2featureDataIterator\n" );
  ESO2featureDataIterator::test();

  testingMessage( "Testing ESO2loosePartData\n" );
  ESO2loosePartData::test();

  testingMessage( "Testing ESO2loosePartIterator\n" );
  ESO2loosePartDataIterator::test();

  testingMessage( "Testing ESO2partDemandData\n" );
  ESO2partDemandData::test();

  testingMessage( "Testing ESO2partDemandDataIterator\n" );
  ESO2partDemandDataIterator::test();

  testingMessage( "Testing ESO2mtmDemanBomData\n" );
  ESO2mtmDemanBomData::test();

  testingMessage( "Testing ESO2mtmDemanBomDataIterator\n" );
  ESO2mtmDemanBomDataIterator::test();

#if 0
  testingMessage( "Testing ESO2alternatePartData\n" );
  ESO2alternatePartData::test();

  testingMessage( "Testing ESO2alternatePartDataIterator\n" );
  ESO2alternatePartDataIterator::test();
#endif

  testingMessage( "Testing ESO2partInterplantData\n" );
  ESO2partInterplantData::test();

  testingMessage( "Testing ESO2interplantIterator\n" );
  ESO2partInterplantDataIterator::test();

  testingMessage( "Testing ESO2globalAttrData\n" );
  ESO2globalAttrData::test();

  testingMessage( "Testing ESO2probAppData\n" );
  ESO2probAppData::test();

  testingMessage( "Testing ESO2featureXrefAppData\n" );
  ESO2featureXrefAppData::test();

  testingMessage( "Testing ESO2mtmAppData\n" );
  ESO2mtmAppData::test();

  testingMessage( "Testing ESO2problem\n" );
  ESO2problem::test();

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
