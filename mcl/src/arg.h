//==============================================================================
// Message Class Library
//
// (C) Copyright IBM Corp. 1997, 2005  All Rights Reserved
//==============================================================================

#ifndef mcl_arg_h
#define mcl_arg_h

//-----------------------------------------------------------------------------
// File arg.h
//
// Contains the declaration of the following classes:
//
// Arg
//    BoolArg
//    IntArg
//    FloatArg
//    DoubleArg
//    StringArg
//    MsgFragArg
//    VectorArg
//       BoolVectorArg
//       IntVectorArg
//       FloatVectorArg
//       DoubleVectorArg
//       StringVectorArg
//
// ArgList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Notes on Implementation:
//
// * All the classes including MclArg and its derived classes, are used
//   internally within MCL. There is no intended public interface for these
//   classes.
//
// * Of the class declarations in this file, MclArgList is the only one that
//   should be used outside of MCL.
//
// * Objects of type MclArgList are required as arguments to the MclFacility
//   parentheses operator which is used when issuing messages. These
//   MclArgList objects are used to hold the data values that are then 
//   printed within the framework of the message being printed.
//
// * The most compact method of creating and passing one of these argument
//   lists to the facility is by a call such as:
//
//    MclArgList() << arg1 << arg2 << etc;
//
//   which invokes the default constructor, and appends to the list using
//   the overloaded << operator.
//
// * it is also possible to index members of the argument list using the 
//   [] operator.
//
// * implementation is left up to the user, but it is important to make
//   sure that the arguments are of the correct (and unambiguous) type,
//   and also that they are in the correct order (determined by the format
//   string associated with the message being issued.
//
//-----------------------------------------------------------------------------

#include <mcl/src/defs.h>

#include <stddef.h>
#include <string>
#include <vector>
//-----------------------------------------------------------------------------

class MclFacility;
class MclMsg;
class MclMsgFrag;
class MclVectorArg;
class MclArgValueType;
class MclArg;

typedef MclArg * MclArgPtr;

//-----------------------------------------------------------------------------

class MclArg
{
public:
  virtual ~MclArg();

  virtual MclArg * clone() const = 0;

  void format (
        std::string &       formattedArg,
        const std::string & theFormat,
        MclBoolean        reqVector,
        int               argNum,
        const MclMsg *    theMsg)
        const;
     //
     // Assumes this Arg is being formatted as arg # argNum for theMsg.
     // Formats this Arg, according to theFormat and stores the result in
     // formattedArg. This Arg is required to be either a vector or a
     // non-vector depending on reqVector.

  void getValueText (
        std::string &    valueText,
        const MclMsg * theMsg)
        const;
     //
     // Sets valueText to a formatted string representing the value of
     // this Arg for testing purposes. Uses theMsg as a context.

   static void setUpClass ();
      //
      // Does initial set up of this class the first time it is called
      // during the program process.

protected:

  MclArg (const MclArgValueType & theValueType);
     //
     // myValueType_ <-- theValueType.

  MclArg(const MclArg& arg);

  virtual void getTypeName (std::string & theName) const;
     //
     // Sets theName to text indicating the type of the argument that this Arg
     // represents.

  virtual MclBoolean isaVector () const;
     //
     // Returns TRUE, iff this Arg is really a VectorArg.
     // This default implementation does the FALSE case.

  virtual void formatValidArg (
        std::string &       formattedArg,
        const std::string & theFormat,
        const MclMsg *    theMsg)
        const = 0;
     //
     // Formats this Arg, according to theFormat and stores the result in
     // formattedArg. Assumes this Arg is valid for theFormat.
     // Uses theMsg as a context.

  static void formatValidData (
        std::string &         formattedData,
        const std::string &   theFormat,
        int                 theData,
        const MclFacility & theFacility);
     //
     // Formats theData, according to theFormat and stores the result in
     // formattedData. Assumes theFormat is valid for an int.
     // Uses theFacility as a context.

  static void formatValidData (std::string &, const std::string &, float);
  static void formatValidData (std::string &, const std::string &, double);
  static void formatValidData (std::string &, const std::string &, const char *);
  static int getWidth(const std::string &);
     //
     // formatValidData for floats and strings.
     // (No Facility context needed for these.)

  static char & formatFlag (std::string & theFormat);
     //
     // Returns the format flag in theFormat, as an lvalue.

  static char formatFlag (const std::string & theFormat);
     //
     // Returns the format flag in theFormat, as a const value.

private:

  MclArg & operator= (const MclArg &);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  void vectornessError (
        MclBoolean     reqVector,
        int            argNum,
        const MclMsg * theMsg)
        const;
     //
     // Assumes this Arg is being formatted as arg # argNum for theMsg.
     // Reports an error indicating that is Arg is or is not a vector,
     // contrary to reqVector.

  void flagError (
        char           formatFlag,
        int            argNum,
        const MclMsg * theMsg)
        const;
     //
     // Assumes this Arg is being formatted as arg # argNum for theMsg.
     // Reports an error indicating a mismatch between formatFlag and the type
     // of this Arg.

  const MclArgValueType & myValueType_;
     //
     // The ArgValueType for this Arg.
};

class MclBoolArg : public MclArg
{
public:
  MclBoolArg(bool);
  ~MclBoolArg(){}
  MclArg* clone()const { return new MclBoolArg(*this); }

private:

  MclBoolArg& operator=(const MclBoolArg&);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  MclBoolArg(const MclBoolArg&  i);

  virtual void formatValidArg (
     std::string &,
     const std::string &,
     const MclMsg *)
     const;

  const bool myBool_;
};

class MclIntArg : public MclArg
{
public:
  MclIntArg(int i);
  ~MclIntArg(){}
  MclArg* clone()const { return new MclIntArg(*this); }

private:

  MclIntArg& operator=(const MclIntArg&);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  MclIntArg(const MclIntArg&  i);

  virtual void formatValidArg (
     std::string &,
     const std::string &,
     const MclMsg *)
     const;

  const int i_;
};

class MclFloatArg : public MclArg
{
public:
  MclFloatArg(float f);
  ~MclFloatArg(){}
  MclArg* clone()const { return new MclFloatArg(*this); }

private:

  MclFloatArg& operator=(const MclFloatArg&);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  MclFloatArg(const MclFloatArg& f);

  virtual void formatValidArg (
     std::string &,
     const std::string &,
     const MclMsg *)
     const;

  const float f_;
};

class MclDoubleArg: public MclArg
   {
   public:

      MclDoubleArg (double theDouble);

      ~MclDoubleArg ();

      MclArg * clone () const;

   private:

      MclDoubleArg & operator= (const MclDoubleArg &);
         //
         // Assignment is declared private and not implemented so as to avoid
         // inadvertant assignment.

      MclDoubleArg (const MclDoubleArg & theDouble);

      virtual void formatValidArg (
         std::string &,
         const std::string &,
         const MclMsg *)
         const;

      const double myDouble_;
   };

class MclStringArg : public MclArg
{
public:
  MclStringArg(const char * s);
  ~MclStringArg();
  MclArg* clone()const;
private:

  MclStringArg& operator=(const MclStringArg&);
     //
     // Assignment is declared private and not implemented so as to avoid
     // inadvertant assignment.

  MclStringArg(const MclStringArg & s);

  virtual void formatValidArg (
     std::string &,
     const std::string &,
     const MclMsg *)
     const;

  const char * const string_;
};

class MclMsgFragArg: public MclArg
   {
   public:

      MclMsgFragArg (const MclMsgFrag * theFrag);

      ~MclMsgFragArg ();

      MclArg * clone () const;

   private:

      MclMsgFragArg& operator= (const MclMsgFragArg &);
         //
         // Assignment is declared private and not implemented so as to avoid
         // inadvertant assignment.

      MclMsgFragArg (const MclMsgFragArg & s);

      virtual void formatValidArg (
         std::string &,
         const std::string &,
         const MclMsg *)
         const;

      const MclMsgFrag * const myFrag_;
   };

//----------------------------------------------------------------------------
//
// MclVectorArg and its derived classes
//
//----------------------------------------------------------------------------


class MclVectorArg : public MclArg
{
public:

  virtual ~MclVectorArg();
  virtual MclArg* clone() const = 0;

protected:

  MclVectorArg (int l, const MclArgValueType & theValueType);
     //
     // length_ <-- l
      // Arg     <-- theValueType

  MclVectorArg(const MclVectorArg& v);

  virtual void formatElem (
        std::string &         formattedElem,
        const std::string &   theFormat,
        int                 theIndex,
        const MclFacility & theFacility)
        const = 0;
     //
     // Formats the element of this VectorArg indexed by theIndex, according
     // to theFormat and stores the result in formattedElem.
     // Uses theFacility as a context.

private:

  MclVectorArg & operator= (const MclVectorArg &);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  virtual void getTypeName (std::string &) const;

  virtual MclBoolean isaVector () const;

  virtual void formatValidArg (
     std::string &,
     const std::string &,
     const MclMsg *)
     const;

  const size_t length_;
};

class MclBoolVectorArg: public MclVectorArg
{
public:
  MclBoolVectorArg(const bool*, int);
  MclBoolVectorArg(const MclBoolVectorArg&);
  ~MclBoolVectorArg();
  MclArg * clone() const;

private:

  MclBoolVectorArg & operator= (const MclBoolVectorArg &);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  virtual void formatElem (
     std::string &,
     const std::string &,
     int,
     const MclFacility &)
     const;

  const bool * const boolVec_;
};

class MclIntVectorArg: public MclVectorArg
{
public:
  MclIntVectorArg(const int*, int);
  MclIntVectorArg(const MclIntVectorArg&);
  ~MclIntVectorArg();
  MclArg * clone() const;

private:

  MclIntVectorArg & operator= (const MclIntVectorArg &);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  virtual void formatElem (
     std::string &,
     const std::string &,
     int,
     const MclFacility &)
     const;

  const int * const intVec_;
};

class MclFloatVectorArg: public MclVectorArg
{
public:
  MclFloatVectorArg(const float*, int); 
  MclFloatVectorArg(const MclFloatVectorArg&);
  ~MclFloatVectorArg();
  MclArg * clone() const;

private:

  MclFloatVectorArg & operator= (const MclFloatVectorArg &);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  virtual void formatElem (
     std::string &,
     const std::string &,
     int,
     const MclFacility &)
     const;

  const float * const floatVec_;
};

class MclDoubleVectorArg: public MclVectorArg
   {
   public:

      MclDoubleVectorArg (const double *, int); 

      MclDoubleVectorArg (const MclDoubleVectorArg &);

      ~MclDoubleVectorArg ();

      MclArg * clone () const;

   private:

      MclDoubleVectorArg & operator= (const MclDoubleVectorArg &);
         //
         // Assignment is declared private and not implemented so as to avoid
         // inadvertant assignment.

      virtual void formatElem (
         std::string &,
         const std::string &,
         int,
         const MclFacility &)
         const;

      const double * const doubleVec_;
   };

class MclStringVectorArg: public MclVectorArg
{
public:
  MclStringVectorArg(const char* const *, int);
  MclStringVectorArg(const MclStringVectorArg&);
  ~MclStringVectorArg();
  MclArg * clone() const;

private:

  MclStringVectorArg & operator= (const MclStringVectorArg &);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

  virtual void formatElem (
     std::string &,
     const std::string &,
     int,
     const MclFacility &)
     const;

  const char * const * const strVec_;
};

//---------------------------------------------------------------------------
// MclMakeVec functions.
//
// Several global functions all called MclMakeVec.
// Each of these functions constructs (on the stack) an instance of a derived
// class of VectorArg (using the arguments to the function) and returns a
// copy of it.
//---------------------------------------------------------------------------

MclBoolVectorArg   MclMakeVec (const bool *,         int); 
MclIntVectorArg    MclMakeVec (const int *,          int); 
MclFloatVectorArg  MclMakeVec (const float *,        int);
MclStringVectorArg MclMakeVec (const char * const *, int);

//---------------------------------------------------------------------------
//
// MclArgList Interface
//
//---------------------------------------------------------------------------

class MclArgList
{
public:
   MclArgList();
  ~MclArgList();
 
  //Allows access to the i'th element of the list
  // Indexing begins with 1, not 0.
  const MclArg &
    operator[]
      ( size_t i )
        const;

  // Returns the # Args in this ArgList.
  size_t length () const;

  // Methods for appending to list
  
  MclArgList& operator<< (const MclArg& arg);
  MclArgList& operator<< (bool);
  MclArgList& operator<< (int i);
  MclArgList& operator<< (float f);
  MclArgList& operator<< (const char * s);
  MclArgList& operator<< (const MclMsgFrag * theFrag);
  MclArgList& operator<< (const std::string& s);
 
private:

  // Copy ctor is declared private and not implemented so as to avoid
  // inadvertant copying.  
  MclArgList(const MclArgList&);


  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.
  MclArgList & operator= (const MclArgList &);

  // Member Data

  std::vector<MclArgPtr> list_;
};

#endif
