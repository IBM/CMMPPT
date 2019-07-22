//------------------------------------------------------------------------------
// WIT-J C++ Header File ArrayListJOR.h.
//
// Contains the declaration of the following classes and templates:
//
//    ArrayListJOR
//    IntALJOR
//    DblALJOR
//    StringALJOR
//    CompALJOR <C>
//    DemandALJOR
//    PeggingTripleALJOR
//------------------------------------------------------------------------------

#ifndef ArrayListJOR_h
#define ArrayListJOR_h

#include <JniClient.h>

#include    <vector>

//------------------------------------------------------------------------------
// Class ArrayListJOR
//
// "ArrayList Java Object Representation"
// Represents a Java ArrayList <E> for some element type, E.
//
// An instance of this class stores a local reference to the represented Java
// ArrayList. Thus an instance of this class should always be destroyed
// by the time the native method in which it was created returns.
//
// Class Hierarchy:
//
// JniClient
//    ArrayListJOR
//       IntALJOR
//       DblALJOR
//       StringALJOR
//       CompALJOR <C>
//       DemandALJOR
//       PeggingTripleALJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class ArrayListJOR: public JniClient
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   static void setUpJni (JNIEnv *);
      //
      // Sets up the interaction between this class and JNI.

   void clearAndAllocate (int minCapacity);
      //
      // Removes all of the elements from the represented Java ArrayList <E>.
      // Increases the storage capacity of the represented Java ArrayList <E> to
      // at least minCapacity.

   //---------------------------------------------------------------------------
   // Protected member functions.
   //---------------------------------------------------------------------------

   protected:

   explicit ArrayListJOR (_jobject * theArrayListJObj, JNIEnv *);
      //
      // Constructor.
      // theArrayListJObj should be a local reference to a Java ArrayList <E>
      // for some element type, E.

   ~ArrayListJOR ();
      //
      // Destructor.

   int size ();
      //
      // Returns the # of elements in the represented Java ArrayList <E>.

   void add (_jobject * theJObject);
      //
      // Appends the Java Object referenced by theJObject to the end of the
      // represented Java ArrayList <E>.
      // The Java Object referenced by theJObject must be of type E.

   _jobject * get (int theIdx);
      //
      // Creates and returns a local reference to the Java Object of type E
      // stored by the represented Java ArrayList <E> at theIdx.
      // Performs range checking on theIdx.
                                                               
   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   ArrayListJOR              (const ArrayListJOR &);
   ArrayListJOR & operator = (const ArrayListJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   static _jmethodID *          clearJMeth_;
   static _jmethodID * ensureCapacityJMeth_;
   static _jmethodID *           sizeJMeth_;
   static _jmethodID *            addJMeth_;
   static _jmethodID *            getJMeth_;
      //
      // The JNI method IDs for the following Java class ArrayList methods:
      //    clear
      //    ensureCapacity
      //    size
      //    add
      //    get

   //---------------------------------------------------------------------------

   _jobject * const myArrayListJObj_;
      //          
      // A local reference to the represented Java ArrayList.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class IntALJOR
//
// "Integer ArrayList Java Object Representation"
// Represents a Java ArrayList <Integer>.
//
// Class Hierarchy:
//
// JniClient
//    ArrayListJOR
//       IntALJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class IntALJOR: public ArrayListJOR
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   static void setUpJni (JNIEnv *);
      //
      // Sets up the interaction between this class and JNI.

   explicit IntALJOR (_jobject * theIntArrayListJObj, JNIEnv *);
      //
      // Constructor.
      // theIntArrayListJObj must reference a Java ArrayList <Integer>.

   ~IntALJOR ();
      //
      // Destructor.

   void copyFrom (const int * theIntCVec, int theLength);
      //
      // Copies theIntCVec into the represented Java ArrayList <Integer>,
      // where theLength is taken to be the length of theIntCVec.

   void copyInto (std::vector <int> & theIntVec);
      //
      // Copies the represented Java ArrayList <Integer> into theIntVec.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   IntALJOR              (const IntALJOR &);
   IntALJOR & operator = (const IntALJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   static _jmethodID * intValueJMeth_;
      //
      // The JNI method ID for the intValue method of Java class Integer.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class DblALJOR
//
// "Double ArrayList Java Object Representation"
// Represents a Java ArrayList <Double>.
//
// Class Hierarchy:
//
// JniClient
//    ArrayListJOR
//       DblALJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class DblALJOR: public ArrayListJOR
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   static void setUpJni (JNIEnv *);
      //
      // Sets up the interaction between this class and JNI.

   explicit DblALJOR (_jobject * theDblArrayListJObj, JNIEnv *);
      //
      // Constructor.
      // theDblArrayListJObj must reference a Java ArrayList <Double>.

   ~DblALJOR ();
      //
      // Destructor.

   void copyFrom (const double * theDblCVec, int theLength);
      //
      // Copies theDblCVec into the represented Java ArrayList <Double>,
      // where theLength is taken to be the length of theDblCVec.

   void copyInto (std::vector <double> & theDblVec);
      //
      // Copies the represented Java ArrayList <Double> into theDblVec.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   DblALJOR              (const DblALJOR &);
   DblALJOR & operator = (const DblALJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   static _jmethodID * doubleValueJMeth_;
      //
      // The JNI method ID for the doubleValue method of Java class Double.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class StringALJOR
//
// "String ArrayList Java Object Representation"
// Represents a Java ArrayList <String>.
//
// Class Hierarchy:
//
// JniClient
//    ArrayListJOR
//       StringALJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class StringALJOR: public ArrayListJOR
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit StringALJOR (_jobject * theStringArrayListJObj, JNIEnv *);
      //
      // Constructor.
      // theStringArrayListJObj must reference a Java ArrayList <String>.

   ~StringALJOR ();
      //
      // Destructor.

   void copyFrom (const char * const * theCStrCVec, int theLength);
      //
      // Copies theCStrCVec into the represented Java ArrayList <String>,
      // where theLength is taken to be the length of theCStrCVec.

   void copyInto (StringVec & theStringVec);
      //
      // Copies the represented Java ArrayList <String> into theStringVec.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   StringALJOR              (const StringALJOR &);
   StringALJOR & operator = (const StringALJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class template CompALJOR <C>
//
// "Component ArrayList Java Object Representation"
// Represents a Java ArrayList <C extends Component>.
// C is to be a C++ class derived from Component.
//
// Class Hierarchy:
//
// JniClient
//    ArrayListJOR
//       CompALJOR <C>
//------------------------------------------------------------------------------

namespace WitJ
{
template <typename C>
      class CompALJOR: public ArrayListJOR
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   explicit CompALJOR (_jobject * theCompArrayListJObj, JNIEnv *);
      //
      // Constructor.
      // theCompArrayListJObj must reference a Java
      // ArrayList <C extends Component>.

   ~CompALJOR ();
      //
      // Destructor.

   void add (C * theC);
      //
      // Appends the instance Java class C corresponding to theC to the end of
      // the represented Java ArrayList <C>.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   CompALJOR              (const CompALJOR &);
   CompALJOR & operator = (const CompALJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class DemandALJOR
//
// "Demand ArrayList Java Object Representation"
// Represents a Java ArrayList <Demand>.
//
// Class Hierarchy:
//
// JniClient
//    ArrayListJOR
//       DemandALJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class DemandALJOR: public ArrayListJOR
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   static void setUpJni (JNIEnv *);
      //
      // Sets up the interaction between this class and JNI.

   explicit DemandALJOR (_jobject * theDemandArrayListJObj, JNIEnv *);
      //
      // Constructor.
      // theDemandArrayListJObj must reference a Java ArrayList <Demand>.

   ~DemandALJOR ();
      //
      // Destructor.

   void populateIDVecs (
         std::vector <const char *> & thePartNameVec,
         std::vector <const char *> & theDemandNameVec);
      //
      // Populates thePartNameVec and theDemandNameVec with the partNames and
      // demandNames of the Demands in the Java ArrayList <Demand> represented
      // by this DemandALJOR.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   static _jmethodID * getDemandCORJMeth_;
      //
      // The JNI method ID for the getDemandCOR method of Java class Demand.

   private:

   DemandALJOR              (const DemandALJOR &);
   DemandALJOR & operator = (const DemandALJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.
   };
};
//
// End of namespace WitJ.

//------------------------------------------------------------------------------
// Class PeggingTripleALJOR
//
// "PeggingTriple ArrayList Java Object Representation"
// Represents a Java ArrayList <PeggingTriple <C>> for some Java Component class
// C.
//
// Class Hierarchy:
//
// JniClient
//    ArrayListJOR
//       PeggingTripleALJOR
//------------------------------------------------------------------------------

namespace WitJ
{
class PeggingTripleALJOR: public ArrayListJOR
   {
   //---------------------------------------------------------------------------
   // Public member functions.
   //---------------------------------------------------------------------------

   public:

   static void setUpJni (JNIEnv *);
      //
      // Sets up the interaction between this class and JNI.

   explicit PeggingTripleALJOR (_jobject * theTripleListJObj, JNIEnv *);
      //
      // Constructor.
      // theTripleListJObj must reference a Java ArrayList <PeggingTriple <C>>.

   ~PeggingTripleALJOR ();
      //
      // Destructor.

   void add (Component * theComp, int thePeriod, double theVolume);
      //
      // theComp must be an instance of the C++ class corresponding to the Java
      // class C for this PeggingTripleALJOR.
      // Constructs a Java PeggingTriple <C> from the Java C instance
      // corresponding to theComp along with thePeriod and theVolume and appends
      // the new PeggingTriple <C> to the represented Java
      // ArrayList <PeggingTriple <C>>.

   //---------------------------------------------------------------------------
   // Private member functions.
   //---------------------------------------------------------------------------

   private:

   PeggingTripleALJOR              (const PeggingTripleALJOR &);
   PeggingTripleALJOR & operator = (const PeggingTripleALJOR &);
      //
      // Not implemented; prevents accidental copying and assignment.

   //---------------------------------------------------------------------------
   // Private member data.
   //---------------------------------------------------------------------------

   static _jclass * ourPeggingTripleJCl_;
      //
      // A global reference to the (raw) WIT-J/Java class PeggingTriple.

   static _jmethodID * ctorPeggingTripleJMeth_;
      //
      // The _jmethodID of the ctor for WIT-J/Java class PeggingTriple.
   };
};
//
// End of namespace WitJ.

#endif
