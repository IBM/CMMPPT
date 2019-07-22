
// RW2STL - inserted:
#include <ostream>
#include <scenario/src/RWToSTLHelpers.h>
// End of RW2STL - inserted includes.
//-----------------------------------------------------------------------------
// File arg.C
//
// Contains the declaration and implementation of the following classes:
//
// ArgPtrVector
// 
// Contains the implementation of the following classes:
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

#include <assert.h>
#include <iostream>
#include <sce/src/arg.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

//---------------------------------------------------------------------------
// Rogue Wave Declarations and Implementations
//---------------------------------------------------------------------------

// RW2STL - declare   (RWGVector,        SceArgPtr)

// RW2STL - declare   (RWGOrderedVector, SceArgPtr)


// RW2STL - implement (RWGVector,        SceArgPtr)

// RW2STL - implement (RWGOrderedVector, SceArgPtr)


//-----------------------------------------------------------------------------
// class ArgPtrVector.
//
// Wrapper (by inheritance) for RWGOrderedVector(SceArgPtr).
// The purpose of this class is to avoid the need to #include rw/gordvec.h in
// arg.h. It allows ArgList to use a forward reference to this class, rather
// than a forward reference to class RWGOrderedVector(SceArgPtr), which would
// not be possible, because it is a generic class.
// All Implementation is inline.
//-----------------------------------------------------------------------------

/*
class SceArgPtrVector: public RWGOrderedVector(SceArgPtr)
   {
   public:

      inline SceArgPtrVector (size_t capac):
            RWGOrderedVector(SceArgPtr) (capac)
         {
         }

   private:

      SceArgPtrVector             (const SceArgPtrVector &);
 
         //
         // Copy ctor is declared private and not implemented
         // so as to prevent inadvertant copying and assignment.
   };
*/
//-----------------------------------------------------------------------------
//
// SceArg Implementation
//
//-----------------------------------------------------------------------------


SceArg::~SceArg ()
   {
   }


SceArg::SceArg (const SceArg & theArg) : type_(theArg.type_)
   {
   }

SceArg::SceArg (ArgType t) : type_(t)
{
}

SceArg &  SceArg::operator = (const SceArg& arg)
{
  assert(type_ == arg.type_);
  if (this == &arg)
    return *this;

  switch (type_)
    {
    case SceArg::ARG_INT:
      (SceIntArg&) *this = (SceIntArg&) arg;
      break;
    case SceArg::ARG_FLOAT:
      (SceFloatArg&) *this = (SceFloatArg&) arg;
      break;
    case SceArg::ARG_CHARSTAR:
      (SceStringArg&) *this = (SceStringArg&) arg;
      break;
    case SceArg::ARG_FLOATSTAR:
      (SceFloatVectorArg&) *this = (SceFloatVectorArg&) arg;
      break;
    case SceArg::ARG_INTSTAR:
      (SceIntVectorArg&) *this = (SceIntVectorArg&) arg;
      break;
    }
  return *this;
}

int SceArg::operator == (const SceArg& arg)
{
  assert(type_ == arg.type_);
  if (this != &arg)
    return 0;

  switch (type_)
    {
    case SceArg::ARG_INT:
      return (SceIntArg&) *this == (SceIntArg&) arg;
    case SceArg::ARG_FLOAT:
      (SceFloatArg&) *this == (SceFloatArg&) arg;
    case SceArg::ARG_CHARSTAR:
      (SceStringArg&) *this == (SceStringArg&) arg;
    case SceArg::ARG_FLOATSTAR:
      (SceFloatVectorArg&) *this == (SceFloatVectorArg&) arg;
    case SceArg::ARG_INTSTAR:
      (SceIntVectorArg&) *this == (SceIntVectorArg&) arg;
    default:
      assert(0);
    }
  return 0;
}

//-----------------------------------------------------------------------------
//
// SceIntArg Implementation
//
//-----------------------------------------------------------------------------

SceIntArg::SceIntArg(void):
  SceArg(ARG_INT),
  i_    (0)
{
}

SceIntArg::SceIntArg(int i):
  SceArg(ARG_INT),
  i_    (i)
{
}

SceIntArg::SceIntArg(const SceIntArg&  i):
  SceArg(i),
  i_    (i.i_)
{
}

SceIntArg & SceIntArg::operator = (const SceIntArg& i)
{
  assert(type_ == i.type_);
  i_ = i.i_;
  return *this;
}

int SceIntArg::operator == (const SceIntArg& i)
{
  assert(type_ == i.type_);
  return (i_ == i.i_) ? 1 : 0;
}

//-----------------------------------------------------------------------------
//
// SceFloatArg Implementation
//
//-----------------------------------------------------------------------------

SceFloatArg::SceFloatArg():
  SceArg(ARG_FLOAT),
  f_    (0.0)
{
}


SceFloatArg::SceFloatArg(float f):
  SceArg(ARG_FLOAT),
  f_    (f)
{
}

SceFloatArg::SceFloatArg(const SceFloatArg& f):
  SceArg(f),
  f_ (f.f_)
{
}

SceFloatArg & SceFloatArg::operator = (const SceFloatArg& f)
{
  assert(type_ == f.type_);
  f_ = f.f_;
  return *this;
}

int SceFloatArg::operator == (const SceFloatArg& f)
{
  assert(type_ == f.type_);
  return (f_ == f.f_) ? 1 : 0;
}
//-----------------------------------------------------------------------------
//
// SceStringArg Implementation
//
//-----------------------------------------------------------------------------

SceStringArg::SceStringArg(void):
  SceArg(ARG_CHARSTAR)
{
 s_ = new char[1];
 s_[0] = '\0';
 
}

SceStringArg::SceStringArg(const char * s):
  SceArg(ARG_CHARSTAR)
{
  assert(s);
  free();
  copy((char*)s);
 
}

SceArg* SceStringArg::clone()const
{
  return new SceStringArg(*this);
}

SceStringArg::~SceStringArg()
{
  free();
}

SceStringArg::SceStringArg(const SceStringArg & source)
  :SceArg(source)
{
  free();
  copy(source.s_);
  
}

void SceStringArg::copy(char * s)
{
  if (s)
    {
      free();
      s_ = new char[strlen(s) + 1];
      assert(s_);
      if (s_)
        strcpy(s_,s);
    }
}

void SceStringArg::free()
{
  if (s_)
    delete [] s_;
  s_ = 0;
}

SceStringArg&  SceStringArg::operator=(const SceStringArg& s)
{
  assert(type_ == s.type_);
  free();
  copy(s.s_);
  return *this;
}

int  SceStringArg::operator==(const SceStringArg& s)
{
  assert(type_ == s.type_);
  return (strcmp(s_, s.s_) == 0 ? 1 : 0);
}


//-----------------------------------------------------------------------------
//
// SceVectorArg Implementation
//
//-----------------------------------------------------------------------------

SceVectorArg::SceVectorArg (int l):
   SceArg (),
   length_(l)
{
  // do nothing
}

SceVectorArg::~SceVectorArg()
{
}

SceVectorArg::SceVectorArg(const SceVectorArg& v):
   SceArg(v),
   length_(v.length_)
{
  // do nothing
}


//-----------------------------------------------------------------------------
//
// SceIntVectorArg Implementation
//
//-----------------------------------------------------------------------------

SceIntVectorArg::SceIntVectorArg():
  SceVectorArg(0),
  iv_(0)
{
  type_ = ARG_INTSTAR;
}

SceIntVectorArg::SceIntVectorArg(const int* v, int l):
   SceVectorArg(l)
{
  assert(v);
  type_ = ARG_INTSTAR;
  free();
  copy(v);
}

SceIntVectorArg::~SceIntVectorArg()
{
  free();
}


SceArg * SceIntVectorArg::clone() const
{
  SceArg * newInst = new SceIntVectorArg(*this);
  return newInst;
}

SceIntVectorArg::SceIntVectorArg(const SceIntVectorArg& iv):
   SceVectorArg (iv)
{
 
   type_ = ARG_INTSTAR; 
   free();
   copy(iv_);
}

SceIntVectorArg &  SceIntVectorArg::operator= (const SceIntVectorArg & iv)
{
  assert(type_ == iv.type_);
  free();
  copy(iv.iv_);
  return *this;
}

int  SceIntVectorArg::operator== (const SceIntVectorArg & iv)
{
  assert(iv_ && iv.iv_);
  assert(type_ == iv.type_);
   int i = 0; // Pulled out of the for below by RW2STL
   for (i = 0; i < length_; i++)
     {
       if (iv_[i] != iv.iv_[i])
         return 0;
     }
   return 1;
}

void SceIntVectorArg::copy(const int* iv)
{
  if (iv)
    {
      free();
      iv_ = new int[length_];
      assert(iv_);
      int i = 0; // Pulled out of the for below by RW2STL
      for (i=0; i < length_; i++)
        {
          iv_[i] = iv[i];
        }
    }
 
}

void SceIntVectorArg::free()
{
  if (iv_)
    delete [] iv_;
  iv_ = 0;
}

//-----------------------------------------------------------------------------
//
// SceFloatVectorArg Implementation
//
//-----------------------------------------------------------------------------

SceFloatVectorArg::SceFloatVectorArg():
   SceVectorArg(0),
   fv_(0)
{
  type_ = ARG_FLOATSTAR;
}


SceFloatVectorArg::SceFloatVectorArg(const float* v, int l):
   SceVectorArg(l)
{
  assert(v);
  type_ = ARG_FLOATSTAR;
  free();
  copy(v);
}

SceFloatVectorArg::~SceFloatVectorArg()
{
  free();
}


SceArg * SceFloatVectorArg::clone() const
{
  SceArg * newInst = new SceFloatVectorArg(*this);
  return newInst;
}

SceFloatVectorArg::SceFloatVectorArg(const SceFloatVectorArg& fv):
   SceVectorArg (fv)
{
   type_ = ARG_FLOATSTAR;
   free();
   copy(fv.fv_);
}

SceFloatVectorArg &  SceFloatVectorArg::operator= (const SceFloatVectorArg & fv)
{
  assert(type_ == fv.type_);
  free();
  copy(fv.fv_);
  return *this;
}

int  SceFloatVectorArg::operator== (const SceFloatVectorArg & fv)
{
  assert(fv_ && fv.fv_);
  assert(type_ == fv.type_);
   int i = 0; // Pulled out of the for below by RW2STL
   for (i = 0; i < length_; i++)
     {
       if (fv_[i] != fv.fv_[i])
         return 0;
     }
   return 1;
}

void SceFloatVectorArg::copy(const float* fv)
{
  if (fv)
    {
      free();
      fv_ = new float[length_];
      assert(fv_);
      int i = 0; // Pulled out of the for below by RW2STL
      for (i=0; i < length_; i++)
        {
          fv_[i] = fv[i];
        }
    }
}

void SceFloatVectorArg::free()
{
  if (fv_)
    delete [] fv_;
  fv_ = 0;
}


//---------------------------------------------------------------------------
// Implementation of SceMakeVec functions.
//---------------------------------------------------------------------------

SceIntVectorArg SceMakeVec (const int * theIntVector, int length)
   {
   return SceIntVectorArg (theIntVector, length);
   }

SceFloatVectorArg SceMakeVec (const float * theFloatVector, int length)
   {
   return SceFloatVectorArg (theFloatVector, length);
   }


//---------------------------------------------------------------------------
//
// SceArgList Implementation
//
//---------------------------------------------------------------------------

// Default constructor
SceArgList::SceArgList():
   list_ (* new SceArgPtrVector (10))
{
  assert(&list_);
}

SceArgList::SceArgList(const SceArgList& al):list_ (* new SceArgPtrVector (10))
{
  copy(al.list_);
}


// Destructor
SceArgList::~SceArgList()
{
  free();
}


// operator[]
const SceArg &
SceArgList::operator[]( size_t i )
const
{
  return * (list_[i-1]);
}

size_t SceArgList::length () const
   {
   return list_.size ();
   }

// operator<<

SceArgList& SceArgList::operator<<(const SceArg & arg)
{
  SceArg * a = arg.clone();
  assert(a);
  list_.push_back(a);
  return *this;
}

SceArgList& SceArgList::operator<<(int i)
{
  return (*this) << SceIntArg(i);
}

SceArgList& SceArgList::operator<<(float f)
{
  return (*this) << SceFloatArg(f);
}

SceArgList& SceArgList::operator<<(const char * string)
{
  return (*this) << SceStringArg(string);
  
}

SceArgList & SceArgList::operator= (const SceArgList & al)
{
  free();
  list_ = * new SceArgPtrVector(10);
  copy(al.list_);
  return *this;
}

int SceArgList::operator == (const SceArgList& al)
{
  if (this && &al)
    {
      if (length() == al.length())
        {
          int i = 0; // Pulled out of the for below by RW2STL
          for (i = 0; i < length(); i++)
            {
              if (list_[i] == al.list_[i])
                continue;
              else
                return 0;            
            }
          return 1;
        }
    }
  return 0;
}

void SceArgList::copy(SceArgPtrVector& av)
{   

  assert(&list_);
  size_t i = 0; // Pulled out of the for below by RW2STL
  for ( i=0; i<av.size(); i++ )
    {
      SceArg* pa = av[i] -> clone();
      assert(pa);
      list_.push_back(pa);
    }
}

void SceArgList::free()

{
  if (&list_)
   {
      size_t i = 0; // Pulled out of the for below by RW2STL
      for ( i=0; i<list_.size(); i++ )
        {
          if (list_[i] != 0)
            delete list_[i];
        }

      delete &list_;
   }
}


