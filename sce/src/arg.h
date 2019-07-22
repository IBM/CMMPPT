#ifndef sce_arg_h
#define sce_arg_h

// RW2STL - inserted:
#include <string>
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
#include <vector>

// End of RW2STL - inserted includes.

//-----------------------------------------------------------------------------
// File arg.h
//
// Contains the declaration of the following classes:
//
// Arg
//    IntArg
//    FloatArg
//    StringArg
//    VectorArg
//       IntVectorArg
//       FloatVectorArg
//
// ArgList
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------

class SceArg
{
public:

  enum ArgType{ ARG_INT, ARG_FLOAT, ARG_CHARSTAR, ARG_FLOATSTAR, ARG_INTSTAR};

  virtual ~SceArg();
  SceArg &  operator = (const SceArg&);
  int operator == (const SceArg&);
  virtual SceArg * clone() const = 0;

protected:

  SceArg (){}
  SceArg (ArgType t);
  SceArg(const SceArg& arg);
  ArgType type_;
  
private:
  
};

typedef SceArg * SceArgPtr;
typedef std::vector<SceArgPtr> SceArgPtrVector;

class SceIntArg : public SceArg
{
public:
  SceIntArg(int i);
  SceIntArg(void);
  SceIntArg(const SceIntArg&  i);
  ~SceIntArg(){}
  SceIntArg &  operator = (const SceIntArg&);
  int operator == (const SceIntArg&);
  SceArg* clone()const { return new SceIntArg(*this); }
  operator int () const { return i_; }
  
private:
 
  int i_;
};

class SceFloatArg : public SceArg
{
public:
  SceFloatArg();
  SceFloatArg(float f);
   SceFloatArg(const SceFloatArg& f);
  ~SceFloatArg(){}
  SceFloatArg& operator=(const SceFloatArg&);
  int operator==(const SceFloatArg&);
  SceArg* clone()const { return new SceFloatArg(*this); }
  operator float () const {  return f_;}
  
private:

  float f_;
};

class SceStringArg : public SceArg
{
public:
  SceStringArg(const char * s);
  SceStringArg(void);
  SceStringArg(const SceStringArg & s);
  ~SceStringArg();
  SceStringArg& operator=(const SceStringArg&);
  int operator==(const SceStringArg&);
  SceArg* clone()const;
  operator const char * () const { return (char*) s_;}
  
private:
  void copy(char* s);
  void free();

  char*  s_;
};

//----------------------------------------------------------------------------
//
// SceVectorArg and its derived classes
//
//----------------------------------------------------------------------------


class SceVectorArg : public SceArg
{
public:

  virtual ~SceVectorArg();
  virtual SceArg* clone() const = 0;

protected:

  SceVectorArg (int l);

  SceVectorArg(const SceVectorArg& v);
  const int length_;
private:

  SceVectorArg & operator= (const SceVectorArg &);
  //
  // Assignment is declared private and not implemented so as to avoid
  // inadvertant assignment.

};

class SceIntVectorArg: public SceVectorArg
{
public:
  SceIntVectorArg();
  SceIntVectorArg(const int*, int);
  SceIntVectorArg(const SceIntVectorArg&);
  ~SceIntVectorArg();
  SceIntVectorArg & operator= (const SceIntVectorArg &);
  int operator== (const SceIntVectorArg &);
  SceArg * clone() const;
  operator int * () const { return iv_;}
private:

  void copy(const int*);
  void free();
  int *  iv_;
};

class SceFloatVectorArg: public SceVectorArg
{
public:
  SceFloatVectorArg();
  SceFloatVectorArg(const float*, int); 
  SceFloatVectorArg(const SceFloatVectorArg&);
  ~SceFloatVectorArg();
  SceFloatVectorArg & operator= (const SceFloatVectorArg &);
  int operator== (const SceFloatVectorArg &);
  SceArg * clone() const;
  operator float * () const { return fv_;}
  
private:

  void copy(const float*);
  void free();
  float *  fv_;
};

//---------------------------------------------------------------------------
// SceMakeVec functions.
//
// Several global functions all called SceMakeVec.
// Each of these functions constructs (on the stack) an instance of a derived
// class of VectorArg (using the arguments to the function) and returns a
// copy of it.
//---------------------------------------------------------------------------

SceIntVectorArg    SceMakeVec (const int *,          int);
SceFloatVectorArg  SceMakeVec (const float *,        int);

//---------------------------------------------------------------------------
//
// SceArgList Interface
//
//---------------------------------------------------------------------------

class SceArgList
{
public:
  SceArgList();
  SceArgList(const SceArgList&);
  SceArgList & operator= (const SceArgList &);
  int operator == (const SceArgList&);
  ~SceArgList();
 
  //Allows access to the i'th element of the list
  // Indexing begins with 1, not 0.
  const SceArg &
    operator[]
      ( size_t i )
        const;

  // Returns the # Args in this ArgList.
  size_t length () const;

  // Methods for appending to list
  
  SceArgList& operator<< (const SceArg& arg);
  SceArgList& operator<< (int i);
  SceArgList& operator<< (float f);
  SceArgList& operator<< (const char * s);

private:

  // Member Data
  void copy(SceArgPtrVector &);
  void free();
  SceArgPtrVector& list_;
};

#endif
