//------------------------------------------------------------------------------
// WIT-J C++ Header File NativeMacros.h.
//
// Contains the declaration of macros to be call from the implementations of
// native methods.
//------------------------------------------------------------------------------

#ifndef NativeMacros_h
#define NativeMacros_h

//------------------------------------------------------------------------------
// Macro NATIVE_METHOD (RetType, ClassID, methodID)
//
// Expands to the function prototype (except for the argument list) of a C++
// function that is the implmentation of a native method in the java portion of
// WIT-J.
// Arguments:
//
//    RetType:  The JNI return type
//    ClassID:  The Java class  identifier
//    methodID: The Java method identifier
//------------------------------------------------------------------------------

#define NATIVE_METHOD(RetType, ClassID, methodID)                              \
                                                                               \
   extern "C" JNIEXPORT RetType JNICALL                                        \
      Java_com_ibm_witj_ ## ClassID ## _ ## methodID                           \

//------------------------------------------------------------------------------
// Canonical Try Block Macros:
//
// The body of the implementation of each native method should consist of a
// "canonical try block". A canonical try block is a try block of the following
// form:
//
// try
//    {
//    <Contents of native method implementation>
//    }
//
// catch (...)
//    {
//    ExceptionHandler::handleAnyException (theJNIEnv);
//
//    <Return statement>
//    }
//
// where theJNIEnv is the JNI environment pointer for the native method.
//
// The return statement is a return statement without a value, if the
// native method returns void or a return statement that returns a value
// matching the return type of the native method, if the return type is not
// void. Since ExceptionHandler::handleAnyException will cause a Java exception
// to be thrown when the native method returns, the value returned by the return
// statement won't actually matter.
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Macro BEGIN_TRY_BLOCK
//
// Expands to the beginning of a canonical try block.
//------------------------------------------------------------------------------

#define BEGIN_TRY_BLOCK                                                        \
                                                                               \
   try                                                                         \
      {                                                                        \

//------------------------------------------------------------------------------
// Macro END_TRY_BLOCK_SIMPLE_RETURN (theJNIEnv)
//
// Expands to the end of a canonical try block whose return statement does not
// return a value.
// theJNIEnv is to be the JNI environment pointer for the native method.
//------------------------------------------------------------------------------

#define END_TRY_BLOCK_SIMPLE_RETURN(theJNIEnv)                                 \
                                                                               \
      }                                                                        \
                                                                               \
   catch (...)                                                                 \
      {                                                                        \
      ExceptionHandler::handleAnyException (theJNIEnv);                        \
                                                                               \
      return;                                                                  \
      }                                                                        \

//------------------------------------------------------------------------------
// Macro END_TRY_BLOCK_VALUE_RETURN (theJNIEnv, returnValue)
//
// Expands to the end of a canonical try block whose return statement returns a
// value.
// theJNIEnv is to be the JNI environment pointer for the native method.
//------------------------------------------------------------------------------

#define END_TRY_BLOCK_VALUE_RETURN(theJNIEnv, returnValue)                     \
                                                                               \
      }                                                                        \
                                                                               \
   catch (...)                                                                 \
      {                                                                        \
      ExceptionHandler::handleAnyException (theJNIEnv);                        \
                                                                               \
      return (returnValue);                                                    \
      }                                                                        \

#endif
