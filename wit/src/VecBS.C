//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
//
//     Source file: VecBS.C
//     Implementation of classes VectorSpec and BoundSetSpec.
//
//------------------------------------------------------------------------------

#include <Parlex.h>
#include <SymTable.h>
#include <VecBS.h>
#include <Entity.h>
#include <Parser.h>
#include <Problem.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//
//     Implementation of class VectorSpec (see VecBS.h)
//
//------------------------------------------------------------------------------

// Constructor and destructor

WitVectorSpec::WitVectorSpec (
         WitProblem *       theProblem,
         vType              type1,
         WitDRParmType::Tag type2):

      formatType_(type1),
      dataType_  (type2),
      nEntries_  (0),
      boolCVec_  (NULL),
      intCVec_   (NULL),
      dblCVec_   (NULL)
   {
   nPeriods_ = theProblem->nPeriods ();

   switch (dataType_)
      {
      case WitDRParmType::boolVector:
         {
         bvec_ = new std::vector <bool> (nPeriods_, false);
         pvec_ = new std::vector <int>  (nPeriods_, 0);

         break;
         }

      case WitDRParmType::intVector:
         {
         ivec_ = new std::vector <int> (nPeriods_, 0);
         pvec_ = new std::vector <int> (nPeriods_, 0);

         break;
         }

      case WitDRParmType::doubleVector:
         {
         dvec_ = new std::vector <double> (nPeriods_, 0.0);
         pvec_ = new std::vector <int>    (nPeriods_, 0);

         break;
         }

      default:
         {
         dataType_ = WitDRParmType::undefined;
            //
            // if the data type is invalid, set it to "DRParmType::undefined".
            // After the argument is parsed, the function WitEntity::checkArgs
            // will issue the appropriate error message and the API function
            // will not be called.
         }
      }
   }

WitVectorSpec::~WitVectorSpec ()
   {
   switch (dataType_)
      {
      case WitDRParmType::boolVector: 
         {
         delete   bvec_;
         delete   pvec_;
         delete[] boolCVec_;

         break;
         }

      case WitDRParmType::intVector: 
         {
         delete   ivec_;
         delete   pvec_;
         delete[] intCVec_;

         break;
         }

      case WitDRParmType::doubleVector:
         {
         delete   dvec_;
         delete   pvec_;
         delete[] dblCVec_;

         break;
         }
      }
   }

// Operations

// build and return pointer to WitBoolean time vector

WitBoolean * WitVectorSpec::getBoolVec (bool defVal)
   {
   witAssert (dataType_ == WitDRParmType::boolVector);

   if (boolCVec_ == NULL)
      boolCVec_ = new WitBoolean [nPeriods_];

   aSize_ = nPeriods_;
      //
      // save actual time-vector size

   switch (formatType_)
      {
      case single_:
         return getBoolVecSingle ();

      case dense_:
         return getBoolVecDense ();

      case sparse_:
         return getBoolVecSparse (defVal);

      default:
         return NULL;  
      }
   }

WitBoolean * WitVectorSpec::getBoolVecSingle ()
   {
   WitPeriod thePer;

   for (thePer = 0; thePer < nPeriods_; thePer ++)
      boolCVec_[thePer] = bvec ()[0];

   return boolCVec_;
   }

WitBoolean * WitVectorSpec::getBoolVecDense ()
   {
   WitPeriod thePer;

      // check to make sure that dense vector is well-formed
      //
   if (nval () != aSize_)
      {
      WitParser::instance ()->
         myMsgFac () ("valuesNotEqualPeriodsMsg",
            WitParser::instance ()->currentAttr ()->attrID (), 
            nval (),
            aSize_);

      WitParser::instance ()->currentAttr ()->setInvalid (true);

      return NULL;
      };  

   for (thePer = 0; thePer < nPeriods_; thePer ++)
      boolCVec_[thePer] = bvec ()[thePer];

   return boolCVec_;
   }

WitBoolean * WitVectorSpec::getBoolVecSparse (bool defVal)
   {
   WitPeriod thePer;
   int       theIdx;

      // check to make sure that sparse vector is well-formed
      //
   for (theIdx = 0; theIdx < nval (); theIdx ++)
      if (getPeriodVal (theIdx) < 0 or getPeriodVal (theIdx) >= aSize_)
         {
         WitParser::instance ()->myMsgFac () ("invalidPeriodMsg",
            WitParser::instance ()->currentAttr()->attrID (), aSize_);

         return NULL;
         };

   for (thePer = 0; thePer < nPeriods_; thePer ++)
      boolCVec_[thePer] = defVal;

   for (theIdx = 0; theIdx < nEntries_; theIdx ++)
      boolCVec_[pvec ()[theIdx]] = bvec ()[theIdx];

   return boolCVec_;
   }

// build and return pointer to integer time vector

int * WitVectorSpec::getIntVec (int defVal)
   {
   witAssert (dataType_ == WitDRParmType::intVector);

   if (intCVec_ == NULL)
      intCVec_ = new int [nPeriods_];

   aSize_ = nPeriods_;
      //
      // save actual time-vector size

   switch (formatType_)
      {
      case single_:
         return getIntVecSingle ();

      case dense_:
         return getIntVecDense ();

      case sparse_:
         return getIntVecSparse (defVal);

      default:
         return NULL;  
      }
   }

int * WitVectorSpec::getIntVecSingle ()
   {
   WitPeriod thePer;

   for (thePer = 0; thePer < nPeriods_; thePer ++)
      intCVec_[thePer] = ivec ()[0];

   return intCVec_;
   }

int * WitVectorSpec::getIntVecDense ()
   {
   WitPeriod thePer;

      // check to make sure that dense vector is well-formed
      //
   if (nval () != aSize_)
      {
      WitParser::instance ()->
         myMsgFac () ("valuesNotEqualPeriodsMsg",
            WitParser::instance ()->currentAttr ()->attrID (), 
            nval (),
            aSize_);

      WitParser::instance ()->currentAttr ()->setInvalid (true);

      return NULL;
      };  

   for (thePer = 0; thePer < nPeriods_; thePer ++)
      intCVec_[thePer] = ivec ()[thePer];

   return intCVec_;
   }

int * WitVectorSpec::getIntVecSparse (int defVal)
   {
   WitPeriod thePer;
   int       theIdx;

      // check to make sure that sparse vector is well-formed
      //
   for (theIdx = 0; theIdx < nval (); theIdx ++)
      if (getPeriodVal (theIdx) < 0 or getPeriodVal (theIdx) >= aSize_)
         {
         WitParser::instance ()->myMsgFac () ("invalidPeriodMsg",
            WitParser::instance ()->currentAttr()->attrID (), aSize_);

         return NULL;
         };

   for (thePer = 0; thePer < nPeriods_; thePer ++)
      intCVec_[thePer] = defVal;

   for (theIdx = 0; theIdx < nEntries_; theIdx ++)
      intCVec_[pvec ()[theIdx]] = ivec ()[theIdx];

   return intCVec_;
   }

   // build and return pointer to double time vector
   //
double * WitVectorSpec::getDblVec (double defVal)
   {  
   int       theIdx;
   WitPeriod thePer;

   witAssert (dataType_ == WitDRParmType::doubleVector);

   if (dblCVec_ == NULL)
      dblCVec_ = new double[nPeriods_];

   aSize_ = nPeriods_;                  // save actual time-vector size

   switch (formatType_)
      {
      case single_:
         {
         for (thePer = 0; thePer < nPeriods_; thePer ++)
            dblCVec_[thePer] = dvec ()[0];

         return dblCVec_;
         }

      case dense_:
         {
            // check to make sure that dense vector is well-formed
            //
         if (nval () != aSize_)
            {
            WitParser::instance ()->myMsgFac () ("valuesNotEqualPeriodsMsg",
               WitParser::instance ()->currentAttr()->attrID(), nval(), aSize_);

            WitParser::instance ()->currentAttr()->setInvalid (true);

            return NULL;
            };

         for (thePer = 0; thePer < nPeriods_; thePer ++)
            dblCVec_[thePer] = dvec ()[thePer];

         return dblCVec_;
         }

      case sparse_:
         {
            // check to make sure that sparse vector is well-formed
            //
         for (theIdx = 0; theIdx < nval (); theIdx ++)
            if (getPeriodVal (theIdx) < 0 or getPeriodVal (theIdx) >= aSize_)
               {
               WitParser::instance ()->myMsgFac () ("invalidPeriodMsg",
                  WitParser::instance ()->currentAttr ()->attrID (), aSize_);

               return NULL;
               };

         for (thePer = 0; thePer < nPeriods_; thePer ++)
            dblCVec_[thePer] = defVal;

         for (theIdx = 0; theIdx < nEntries_; theIdx ++)
            dblCVec_[pvec ()[theIdx]] = dvec ()[theIdx];

         return dblCVec_;
         }

      default:
         return NULL;  
      }
   }

// Accessors

WitVectorSpec::vType WitVectorSpec::formatType() const
{ return formatType_; }

WitDRParmType::Tag WitVectorSpec::dataType() const
{ return dataType_; }

int WitVectorSpec::nval() const
    {
    switch (dataType_)
       {
       case WitDRParmType::undefined:    return 0; 
       case WitDRParmType::boolVector:   return nEntries_; 
       case WitDRParmType::intVector:    return nEntries_; 
       case WitDRParmType::doubleVector: return nEntries_;
       default:                          return 0;
     }
   }

int WitVectorSpec::nPeriods() const
{ return nPeriods_; }

int WitVectorSpec::nEntries() const
{ return nEntries_; }

int WitVectorSpec::aSize() const
{ return aSize_; }

void WitVectorSpec::setFormatType(const vType type)
{ formatType_ = type; }

void WitVectorSpec::setDataType(const WitDRParmType::Tag type)
{ dataType_ = type; }

// operations

// get a value
int WitVectorSpec::getIntVal(int index)
{
  return (ivec ()[index]);
}

double WitVectorSpec::getDoubleVal(int index)
{
  return dvec ()[index];
}

int WitVectorSpec::getPeriodVal(int index)
{
  return (pvec ()[index]);
}

   // store the next value
   //
WitParseRC WitVectorSpec::addBoolVal (bool value)
   {
   witAssert (dataType_ == WitDRParmType::boolVector);

      // make the vector bigger, if necessary
      //
   if (nEntries_ >= bvec ().size ())
      bvec ().resize (bvec ().size () + VSIZE);

   bvec ()[nEntries_] = value;
      //
      // insert a boolean value

   nEntries_ ++;

   return 0;
   }

   // store the next value
   //
WitParseRC WitVectorSpec::addIntVal (int value)
   {
   witAssert (dataType_ == WitDRParmType::intVector);

      // make the vector bigger, if necessary
      //
   if (nEntries_ >= ivec ().size ())
      ivec ().resize (ivec ().size () + VSIZE);

   ivec ()[nEntries_++] = value;
      //
      // insert an integer value

   return 0;
   }

WitParseRC WitVectorSpec::addDoubleVal(double value)
{
  witAssert (dataType_ == WitDRParmType::doubleVector);

  // make the vector bigger, if necessary
  if (nEntries_ >= dvec ().size ())
    dvec ().resize (dvec ().size () + VSIZE);

  dvec ()[nEntries_++] = value; // insert a double value 
  return 0;
}

WitParseRC WitVectorSpec::addPeriodVal(int value)
{
  // make the vector bigger, if necessary
  if (nEntries_ >= pvec ().size ())
    pvec ().resize(pvec ().size () + VSIZE);

  // check for duplicate period
  for (int i = 0; i < nEntries_; i++)
    if (pvec ()[i] == value) {
      WitParser::instance ()->myMsgFac () ("duplicatePeriodMsg",
        WitParser::instance ()->currentAttr()->attrID());
      return PERIOD_DUP;
    }

   pvec ()[nEntries_] = value;
   return 0;
}

//------------------------------------------------------------------------------
//
//     Implementation of class BoundSetSpec (see VecBS.h)
//
//------------------------------------------------------------------------------

// Constructor and destructor

WitBoundSetSpec::WitBoundSetSpec(bsType type):
  boundSetType_(type),
  hardLBVec_(NULL),
  softLBVec_(NULL),
  hardUBVec_(NULL)
{
}

WitBoundSetSpec::~WitBoundSetSpec()
{
  if (hardLBVec_ != NULL) delete hardLBVec_;
  if (softLBVec_ != NULL) delete softLBVec_;
  if (hardUBVec_ != NULL) delete hardUBVec_;
}

// Operations

// Accessors

WitBoundSetSpec::bsType WitBoundSetSpec::boundSetType() const
{ return boundSetType_; }

WitVectorSpec* WitBoundSetSpec::hardLBVec() const
{ return hardLBVec_; }

WitVectorSpec* WitBoundSetSpec::softLBVec() const
{ return softLBVec_; }

WitVectorSpec* WitBoundSetSpec::hardUBVec() const
{ return hardUBVec_; }

void WitBoundSetSpec::setHardLBVec(WitVectorSpec* theVSpec)
{ hardLBVec_ = theVSpec; }

void WitBoundSetSpec::setSoftLBVec(WitVectorSpec* theVSpec)
{ softLBVec_ = theVSpec; }

void WitBoundSetSpec::setHardUBVec(WitVectorSpec* theVSpec)
{ hardUBVec_ = theVSpec; }
