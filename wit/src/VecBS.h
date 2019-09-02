//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef VecBSH
#define VecBSH

//------------------------------------------------------------------------------
//
//     Source file: VecBS.h
//
//     Contains the declaration of classes VectorSpec and BoundSetSpec
//------------------------------------------------------------------------------

#include <Util.h>

#include <vector>

#define VSIZE 25

//------------------------------------------------------------------------------
//
//     Definition of class VectorSpec to implement
//     "vector formats":
//
//          DRParmType::boolVector
//          DRParmType::intVector
//          DRParmType::doubleVector
//
//     When a Wit function requires a vector format, a vector (of the
//     required datatype) is constructed and a pointer to the vector is
//     passed to the function.  The length of the vector is a parameter	
//     and it's usually equal to myProblem->nPeriods()
//------------------------------------------------------------------------------

class WitVectorSpec
{

public:

  enum vType {undefined_, single_, sparse_, dense_};

  // Constructors and destructors
  WitVectorSpec();
  WitVectorSpec(WitProblem * theProblem, vType, WitDRParmType::Tag);
  ~WitVectorSpec();

  // Operations
  int      getIntVal(int i);     // where i= index into integer vector
  double   getDoubleVal(int i);  //   0, 1, ..., nval_-1
  int      getPeriodVal(int i);

  WitParseRC   addBoolVal   (bool);
  WitParseRC   addIntVal    (int);
  WitParseRC   addDoubleVal (double);
  WitParseRC   addPeriodVal (int);

  WitBoolean * getBoolVec       (bool    defVal);
  WitBoolean * getBoolVecSingle ();
  WitBoolean * getBoolVecDense  ();
  WitBoolean * getBoolVecSparse (bool    defVal);

  int *        getIntVec        (int    defVal);
  int *        getIntVecSingle  ();
  int *        getIntVecDense   ();
  int *        getIntVecSparse  (int    defVal);

  double *     getDblVec        (double defVal);

  // Accessors
  vType              formatType() const;
  WitDRParmType::Tag dataType() const;
  int                nval() const;
  int                nPeriods() const;
  int                nEntries() const;
  int                aSize() const;

  void     setFormatType(const vType);
  void     setDataType(const WitDRParmType::Tag);

private:
  vType              formatType_;    // how the vector format is specified

  WitDRParmType::Tag dataType_;      // should be one of the following:
                                     //    DRParmType::boolVector
                                     //    DRParmType::intVector
                                     //    DRParmType::doubleVector

  int                nPeriods_;      // number of periods
  int                nEntries_;      // number of specified values 
  int                aSize_;         // actual size of the time vector

  std::vector <int> *    pvec_;      // a vector of "periods"
  std::vector <bool> *   bvec_;      // a vector of booleans
  std::vector <int> *    ivec_;      // a vector of integers
  std::vector <double> * dvec_;      // a vector of doubles

     // pointers to the actual vectors to be passed to the API functions.
     // The vector to be constructed depends on the datatype.
     //
  WitBoolean * boolCVec_;
  int *        intCVec_;
  double *     dblCVec_;

     // Convenient-access functions for the vectors.
     //
  inline std::vector <int> & pvec ()
     {
     witAssert (pvec_ != NULL);

     return * pvec_;
     }

  inline std::vector <bool> & bvec ()
     {
     witAssert (bvec_ != NULL);

     return * bvec_;
     }

  inline std::vector <int> & ivec ()
     {
     witAssert (ivec_ != NULL);

     return * ivec_;
     }

  inline std::vector <double> & dvec ()
     {
     witAssert (dvec_ != NULL);

     return * dvec_;
     }
};

//------------------------------------------------------------------------------
//
//     Declaration of class BoundSetSpec to implement BoundSet
//     attributes.  Each BoundSetSpec object may be of type execBounds,
//     stockBounds or cumShipBounds, and may include any or all of
//     the following in any order:
//
//          hardLB <vector format>
//          softLB <vector format>
//          hardUB <vector format>
//
//     When a Wit function requires a bound set, a BoundSetSpec object is
//     constructed, which in turn contains 3 pointers to VectorSpec objects
//     (of the required datatype).  A pointer to the BoundSetSpec object is
//     passed to the function.  The length of each vector is equal to
//     myProblem->nPeriods()
//------------------------------------------------------------------------------

class WitBoundSetSpec
{

public:

  typedef enum bsType {undefined, exec, stock, cumShip} bsType;
  enum bounds {hardlb, softlb, hardub};

  // Constructors and destructors
  WitBoundSetSpec();
  WitBoundSetSpec(bsType);
  ~WitBoundSetSpec();

  // Operations

  // Accessors
  bsType         boundSetType() const;
  WitVectorSpec* hardLBVec() const;
  WitVectorSpec* softLBVec() const;
  WitVectorSpec* hardUBVec() const;

  void           setHardLBVec(WitVectorSpec*);
  void           setSoftLBVec(WitVectorSpec*);
  void           setHardUBVec(WitVectorSpec*);

private:

  bsType         boundSetType_;   // how the bound set is specified

  WitVectorSpec  *hardLBVec_;     // pointer to hardLB vector
  WitVectorSpec  *softLBVec_;     // pointer to softLB vector
  WitVectorSpec  *hardUBVec_;     // pointer to hardUB vector
};

#endif
