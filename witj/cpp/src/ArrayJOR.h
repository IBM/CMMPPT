//------------------------------------------------------------------------------
// WIT-J C++ Header File ArrayJOR.h.
//
// Contains the declaration of the following classes and templetes:
//
//    ArrayJOR <E>
//    BoolArrayJOR
//    IntArrayJOR
//    DblArrayJOR
//------------------------------------------------------------------------------

#ifndef ArrayJOR_h
#define ArrayJOR_h

#include <JniClient.h>

//------------------------------------------------------------------------------
// Class template ArrayJOR <E>
//
// "Array Java Object Representation"
// Represents a Java array of primitives corresponding to the C++ type E.
//
// Class Hierarchy:
//
// JniClient
//    ArrayJOR <Boolean>
//       BoolArrayJOR
//    ArrayJOR <int>
//       IntArrayJOR
//    ArrayJOR <double>
//       DblArrayJOR
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename E>
      class ArrayJOR: public JniClient
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   virtual void copyFrom (const E * theCVec) = 0;
      //
      // Copies theCVec the represented a Java array.
      // It is assumed that theCVec is the same length as the Java array
      // (normally nPeriods). 
      // Not allowed, if the reference to the represented a Java array is NULL.

   //---------------------------------------------------------------------------
   // Protected member functions.
   //---------------------------------------------------------------------------

   protected:

   explicit ArrayJOR (
         _jarray *  theJArray,
         _jobject * theJavaAccObjJObj,
         JNIEnv *   theJNIEnv);
      //
      // Constructor.

   ~ArrayJOR ();
      //
      // Destructor.

   void * getVoidCVecCrit ();
      //
      // Calls JNI's GetPrimitiveArrayCritical (...), checks for exceptions and
      // returns the result.

   E * acquiredCTVec ();
      //
      // Returns the C time vector for this ArrayJOR, acquired from
      // myCTVecSupply_.

   void releaseJArrayCritAndCopy (void * theVoidCVec);
      //
      // Calls JNI's ReleasePrimitiveArrayCritical (...), instructing it to copy
      // back from theVoidCVec to the represented Java array, if necessary.

   void releaseJArrayCritDontCopy (void * theVoidCVec);
      //
      // Calls JNI's ReleasePrimitiveArrayCritical (...), instructing it not to
      // copy back from theVoidCVec to the represented Java array.

   inline int getLength ()
      {
      return myLength_;
      }

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   ArrayJOR              (const ArrayJOR &);
   ArrayJOR & operator = (const ArrayJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   _jarray * const myJArray_;
      //
      // A local reference to the represented Java array.
      // May be NULL.

   int myLength_;
      //
      // The length of the represented Java array.
      // 0, if myJArray_ is NULL.

   CTVecSupply * myCTVecSupply_;
      //
      // The CTVecSupply for this ArrayJOR.

   E * myCTVector_;
      //
      // The c-vector of length nPeriods owned by this ArrayJOR, if any;
      // otherwise NULL.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class BoolArrayJOR
//
// "Boolean Array Java Object Representation"
// Represents a Java boolean[].
//
// Class Hierarchy:
//
// JniClient
//    ArrayJOR <Boolean>
//       BoolArrayJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class BoolArrayJOR: public ArrayJOR <Boolean>
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit BoolArrayJOR (
         _jbooleanArray * theJBoolArray,
         _jobject *       theJavaAccObjJObj,
         JNIEnv *         theJNIEnv);
      //
      // Constructor.

   ~BoolArrayJOR ();
      //
      // Destructor.

   virtual void copyFrom (const Boolean *);
      //
      // Override from class  ArrayJOR <Boolean>

   const Boolean * getCTVec ();
      //
      // Returns the represented Java array as a c-vector.
      // It is assumed that the Java array is of length nPeriods. 
      // Not allowed, if the reference to the represented a Java array is NULL.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   BoolArrayJOR              (const BoolArrayJOR &);
   BoolArrayJOR & operator = (const BoolArrayJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class IntArrayJOR
//
// "Integer Array Java Object Representation"
// Represents a Java int[].
//
// Class Hierarchy:
//
// JniClient
//    ArrayJOR <int>
//       IntArrayJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class IntArrayJOR: public ArrayJOR <int>
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit IntArrayJOR (
         _jintArray * theJIntArray,
         _jobject *   theJavaAccObjJObj,
         JNIEnv *     theJNIEnv);
      //
      // Constructor.

   ~IntArrayJOR ();
      //
      // Destructor.

   virtual void copyFrom (const int *);
      //
      // Overrides from class  ArrayJOR <int>

   virtual const int * getCTVec ();
      //
      // See BoolArrayJOR::getCTVec ().

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   IntArrayJOR              (const IntArrayJOR &);
   IntArrayJOR & operator = (const IntArrayJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class DblArrayJOR
//
// "Double Array Java Object Representation"
// Represents a Java double[].
//
// Class Hierarchy:
//
// JniClient
//    ArrayJOR <double>
//       DblArrayJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class DblArrayJOR: public ArrayJOR <double>
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit DblArrayJOR (
         _jdoubleArray * theJDblArray,
         _jobject *      theJavaAccObjJObj,
         JNIEnv *        theJNIEnv);
      //
      // Constructor.

   ~DblArrayJOR ();
      //
      // Destructor.

   const double * getBoundCTVec ();
      //
      // Same as getCTVec (...), except that if the reference to the represented
      // Java array is NULL, then NULL is returned.

   virtual void copyFrom (const double *);
      //
      // Override from class  ArrayJOR <double>

   const double * getCTVec ();
      //
      // See BoolArrayJOR::getCTVec ().

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   DblArrayJOR              (const DblArrayJOR &);
   DblArrayJOR & operator = (const DblArrayJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.
   };
};
//
// End of namespace WitJ.

#endif
