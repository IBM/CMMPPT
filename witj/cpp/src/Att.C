//------------------------------------------------------------------------------
// WIT-J C++ Source File Att.C.
//
// Contains the implementation of class Att.
//------------------------------------------------------------------------------

#include         <Att.h>
#include     <AttBldr.h>
#include <Coordinator.h>
#include     <Problem.h>
#include <CTVecSupply.h>

//------------------------------------------------------------------------------

WitJ::Att::~Att ()
   {
   }

//------------------------------------------------------------------------------

WitJ::Att * WitJ::Att::asAtt ()
   {
   return this;
   }

//------------------------------------------------------------------------------
// Class Att implementations of set functions.
//
// Each of these functions is called iff the Att does not represent an attribute
// of the indicated type. Each simply issues an error.
//------------------------------------------------------------------------------

void WitJ::Att::set (Component *, int)
   {
   EXCEPTION_ASSERT (false);
   }

void WitJ::Att::set (Component *, double)
   {
   EXCEPTION_ASSERT (false);
   }

void WitJ::Att::set (Component *, const char *)
   {
   EXCEPTION_ASSERT (false);
   }

void WitJ::Att::set (Component *, const int *)
   {
   EXCEPTION_ASSERT (false);
   }

void WitJ::Att::set (Component *, const double *)
   {
   EXCEPTION_ASSERT (false);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::Att::setVectorToScalar (Component * theComp, Elem theScalar)
   {
   Elem * theCTVec;
   int    nPers;
   int    thePer;

   getCTVecSupply ()->provideCTVec (theCTVec);

   nPers = getCoordinator ()->getProblem ()->getNPeriods ();

   for (thePer = 0; thePer < nPers; thePer ++)
      {
      theCTVec[thePer] = theScalar;
      }

   set (theComp, theCTVec);

   getCTVecSupply ()->reclaimCTVec (theCTVec);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of member function template setVectorToScalar (...).
//------------------------------------------------------------------------------

template void WitJ::Att::setVectorToScalar <int>    (Component *, int);
template void WitJ::Att::setVectorToScalar <double> (Component *, double);

//------------------------------------------------------------------------------
// Class Att implementations of get functions.
//
// Each of these functions is called iff the Att does not represent an attribute
// of the indicated type. Each simply issues an error.
//------------------------------------------------------------------------------

void WitJ::Att::get (Component *, int &)
   {
   EXCEPTION_ASSERT (false);
   }

void WitJ::Att::get (Component *, double &)
   {
   EXCEPTION_ASSERT (false);
   }

void WitJ::Att::get (Component *, ArrayJOR <int> &)
   {
   EXCEPTION_ASSERT (false);
   }

void WitJ::Att::get (Component *, ArrayJOR <double> &)
   {
   EXCEPTION_ASSERT (false);
   }

void WitJ::Att::get (Component *, StringJOR &)
   {
   EXCEPTION_ASSERT (false);
   }

//------------------------------------------------------------------------------

WitJ::Att::Att (
         AttBldr *    theAttBldr,
         const char * theAttName,
         const char * theCompClassName):

      JavaAccObj       (theAttBldr->getCoordinator ()),
      myAttName_       (theAttName),
      myCompClassName_ (theCompClassName)
   {
   theAttBldr->acquireAtt (this);
   }

//------------------------------------------------------------------------------
// Specialized implementation of member function Att::scalarTypeMatches <Value>.
//------------------------------------------------------------------------------

namespace WitJ
{
template <>
      bool Att::scalarTypeMatches <int> (const std::string & theTypeName)
   {
   return
         (theTypeName == "Integer")
      || (theTypeName == "Boolean");
   }
}

//------------------------------------------------------------------------------

namespace WitJ
{
template <>
      bool Att::scalarTypeMatches <double> (const std::string & theTypeName)
   {
   return (theTypeName == "Double");
   }
}

//------------------------------------------------------------------------------
// Specialized implementation of member function Att::vectorTypeMatches <Elem>.
//------------------------------------------------------------------------------

namespace WitJ
{
template <>
      bool Att::vectorTypeMatches <int> (const std::string & theTypeName)
   {
   return
         (theTypeName == "int[]")
      || (theTypeName == "boolean[]");
   }
}

//------------------------------------------------------------------------------

namespace WitJ
{
template <>
      bool Att::vectorTypeMatches <double> (const std::string & theTypeName)
   {
   return (theTypeName == "double[]");
   }
}
