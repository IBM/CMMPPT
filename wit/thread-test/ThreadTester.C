//------------------------------------------------------------------------------
// Program     :    ThreadTester.
// C++ Source File: ThreadTester.C.
//
// Contains the C++ implementations of the native methods of Java class
// ThreadTester.
//------------------------------------------------------------------------------

#include <wit.h>

#include <jni.h>

//------------------------------------------------------------------------------
// Two globally declared WitRuns.
// These WitRun pointers are declared globally so that the WitRuns can be
// created in one native call and then used in a different native call
// (probably in a different Thread).
//------------------------------------------------------------------------------

WitRun * globWitRunA;
WitRun * globWitRunB;

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_newRunA (JNIEnv *, jobject)
   {
   witNewRun (& globWitRunA);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_initializeA (JNIEnv *, jobject)
   {
   witInitialize (globWitRunA);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_readRch818A (JNIEnv *, jobject)
   {
   witReadData (globWitRunA, "rch818.data");
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_readEastmanA (JNIEnv *, jobject)
   {
   witReadData (globWitRunA, "eastman.data");
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_heurImplodeA (JNIEnv *, jobject)
   {
   witHeurImplode (globWitRunA);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_optImplodeA (JNIEnv *, jobject)
   {
   witOptImplode (globWitRunA);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_standAloneA (JNIEnv *, jobject)
   {
   char * argv[] = {"", "run.params"};

   witExecStandAlone (globWitRunA, 2, argv);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_deleteRunA (JNIEnv *, jobject)
   {
   witDeleteRun (globWitRunA);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_newRunB (JNIEnv *, jobject)
   {
   witNewRun (& globWitRunB);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_initializeB (JNIEnv *, jobject)
   {
   witInitialize (globWitRunB);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_copyDataBA (JNIEnv *, jobject)
   {
   witCopyData (globWitRunB, globWitRunA);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_copyPartDataBA (JNIEnv *, jobject)
   {
   witAddPart      (globWitRunB, "Part1", WitMATERIAL);
   witCopyPartData (globWitRunB, "Part1", globWitRunA, "0000002G2800");
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_readRch818B (JNIEnv *, jobject)
   {
   witReadData (globWitRunB, "rch818.data");
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_optImplodeB (JNIEnv *, jobject)
   {
   witOptImplode (globWitRunB);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_standAloneB (JNIEnv *, jobject)
   {
   char * argv[] = {"", "run.params"};

   witExecStandAlone (globWitRunB, 2, argv);
   }

//------------------------------------------------------------------------------

extern "C" JNIEXPORT void JNICALL
      Java_ThreadTester_deleteRunB (JNIEnv *, jobject)
   {
   witDeleteRun (globWitRunB);
   }
