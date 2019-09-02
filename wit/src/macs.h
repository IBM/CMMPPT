//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef macs_h
#define macs_h

//------------------------------------------------------------------------------
// Header file: "macs.h"
// Macro definitions.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// DEBUG_MODE
//
// true if and only if DEBUG is defined.
//------------------------------------------------------------------------------

#ifdef DEBUG

#define DEBUG_MODE true

#else

#define DEBUG_MODE false

#endif

//------------------------------------------------------------------------------
// ifDebug
//
// Expands to debugCode, iff WIT was complied in DEBUG mode.
//------------------------------------------------------------------------------

#ifdef DEBUG

#define ifDebug(debugCode)  debugCode

#else

#define ifDebug(debugCode)

#endif

//------------------------------------------------------------------------------
// NumberElem
// Returns number of elements in an array
//------------------------------------------------------------------------------

#define NumberElem(arr) \
   (static_cast <unsigned int> (sizeof(arr) / sizeof(arr[0])))

//------------------------------------------------------------------------------
// noCopyCtorAssign
//
// The purpose of this macro is to prevent the compiler from generating a
// copy constructor and assignment operator for classes whose instances are
// not to be copied or assigned. This macro should be invoked in the private
// section of the declaration of class TheClass. Its effect is to declare a
// copy ctor and an assignment operator for TheClass. These two functions
// should not be implemented. If copying or assignment is then done
// inadvertantly, the result will be either a syntax error or a link-time
// error. See Meyers, Item 27.
//------------------------------------------------------------------------------

#define noCopyCtorAssign(TheClass)                                             \
                                                                               \
   TheClass              (const TheClass &);                                   \
   TheClass & operator = (const TheClass &)                                    \

//------------------------------------------------------------------------------
// noCopyCtor
//
// Same as noCopyCtorAssign, but only applies to the copy ctor.
//------------------------------------------------------------------------------

#define noCopyCtor(TheClass)                                                   \
                                                                               \
   TheClass (const TheClass &)                                                 \

//------------------------------------------------------------------------------
// defaultValue
//
// Declares and implements an inline default value function for an attribute.
// This macro is designed to be used within a class declaration.
// typedFunction is *two* tokens: the type of the default value function and
// the name of the function.
// theValue is the default value.
// Thus
//
//     defaultValue (double defOffset, 0.0)
//
// declares and implements a double-valued default value function, defOffset,
// returning 0.0 as the default value.
//------------------------------------------------------------------------------

#define defaultValue(typedFunction, theValue)                                  \
                                                                               \
   inline static typedFunction ()                                              \
      {                                                                        \
      return theValue;                                                         \
      }                                                                        \

//------------------------------------------------------------------------------
// accessFunc
//
// Declares and implements an access function for an attribute. This macro is
// designed to be used within a class declaration.
// theAttr is the name of the attribute. The name data member storing the
// attribute is given by appending "_" to the attribute name. ReturnType is the
// return type of the access function. If theAttr is of type "AttrType", then
// ReturnType should normally be either "AttrType" or "const AttrType &".
//------------------------------------------------------------------------------

#define accessFunc(ReturnType, theAttr)                                        \
                                                                               \
   inline ReturnType theAttr () const                                          \
      {                                                                        \
      return theAttr ## _;                                                     \
      }                                                                        \

//------------------------------------------------------------------------------
// accessFunc2
//
// Same as accessFunc, except that the return type is given by:
//    ReturnType1, ReturnType2
// In other words, the return type contains a comma.
// For example, the return type might be:
//    const WitSchedule <WitDemand, double> &
//------------------------------------------------------------------------------

#define accessFunc2(ReturnType1, ReturnType2, theAttr)                         \
                                                                               \
   inline ReturnType1, ReturnType2 theAttr () const                            \
      {                                                                        \
      return theAttr ## _;                                                     \
      }                                                                        \

//------------------------------------------------------------------------------
// accessFuncNonConst2
//
// Same as accessFunc2, except the function is not const.
//------------------------------------------------------------------------------

#define accessFuncNonConst2(ReturnType1, ReturnType2, theAttr)                 \
                                                                               \
   inline ReturnType1, ReturnType2 theAttr ()                                  \
      {                                                                        \
      return theAttr ## _;                                                     \
      }                                                                        \

//------------------------------------------------------------------------------
// accessNonNull
//
// Same as accessFunc, except that theAttr is a pointer which is requires
// (via witAssert) to be non-NULL.
//------------------------------------------------------------------------------

#define accessNonNull(ReturnType, theAttr)                                     \
                                                                               \
   inline ReturnType theAttr () const                                          \
      {                                                                        \
      witAssert (theAttr ## _ != NULL);                                        \
                                                                               \
      return theAttr ## _;                                                     \
      }                                                                        \

//------------------------------------------------------------------------------
// Macro accessWrapper
//
// This macro is designed to be used within a class declaration.
// Declares and implements an access function for an attribute as a wrapper
// around an access function with the same name from another class.
// theFunc is the name of the function.
// ReturnType its return type.
// theObject is the object whose function is being wrapped.
//------------------------------------------------------------------------------

#define accessWrapper(ReturnType, theFunc, theObject)                          \
                                                                               \
   inline ReturnType theFunc () const                                          \
      {                                                                        \
      return theObject->theFunc ();                                            \
      }                                                                        \

//------------------------------------------------------------------------------
// Macro updateAccess
//
// Declares and implements a static member function that provides update access
// to a data member of a class.
// Arguments:
//    DataType: The type of the data member
//    theData:  The name of the data member is given by theData followed by _.
//              The name of the function is given by theData.
//    Owner:    The name of the class that owns the data member.
//
// This macro should be used within the declaration of a class that is a friend
// of class Owner.
//------------------------------------------------------------------------------

#define updateAccess(DataType, theData, Owner)                                 \
                                                                               \
   inline static DataType & theData (Owner * theOwner)                         \
      {                                                                        \
      return theOwner->theData ## _;                                           \
      }                                                                        \

//------------------------------------------------------------------------------
// Macro decMappingIndex
//
// This macro is designed to be used within the declaration of class TheClass.
//
// Declares and implements the access function for TheClass's mapping index.
//
// Also declares and implements a function which
// verifies that TheClass has its own mapping index function, rather than just 
// inheriting one: When verifyMappingIndex is invoked on a pointer to an 
// instance of TheClass, if TheClass doesn't have its own version of 
// verifyMappingIndex, a syntax error will result.
//------------------------------------------------------------------------------

#define decMappingIndex(TheClass)                                              \
                                                                               \
   inline int mappingIndex () const                                            \
      {                                                                        \
      return mappingIndex_;                                                    \
      }                                                                        \
                                                                               \
   inline static void verifyMappingIndex (TheClass * &)                        \
      {                                                                        \
      }                                                                        \

//------------------------------------------------------------------------------
// name2 (a, b)
//
// Concatinates symbols a and b.
//------------------------------------------------------------------------------

#define  name2(a,b) _name2 (a,b)
   //
   // Forces the args to be evaluated here.

#define _name2(a,b) a##b

#endif
