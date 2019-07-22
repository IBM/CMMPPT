//------------------------------------------------------------------------------
// WIT-J C++ Source file ScalarAtts.C.
//
// Contains the implementations of the following class templates:
//
//      ProbScalarAtt <Value>
//      PartScalarAtt <Value>
//    DemandScalarAtt <Value>
//       OpnScalarAtt <Value>
//    BomEntScalarAtt <Value>
//       SubScalarAtt <Value>
//    BopEntScalarAtt <Value>
//
// Contains the explicit instantiation of each of these class templates.
//
// Contains the implementation of the following class:
//
//    ProbScalarAttString
//------------------------------------------------------------------------------

#include <ScalarAtts.h>
#include       <Part.h>
#include     <Demand.h>
#include  <Operation.h>
#include   <BomEntry.h>
#include <Substitute.h>
#include   <BopEntry.h>
#include  <StringJOR.h>

//------------------------------------------------------------------------------
// Implementation of class template ProbScalarAtt.
//------------------------------------------------------------------------------

template <typename Value>
      WitJ::ProbScalarAtt <Value>::ProbScalarAtt (
            AttBldr *    theAttBldr,
            const char * theName,
            GetFuncPtr   theGetFuncPtr,
            SetFuncPtr   theSetFuncPtr):

         Att           (theAttBldr, theName, "Problem"),
         myGetFuncPtr_ (theGetFuncPtr),
         mySetFuncPtr_ (theSetFuncPtr)
   {
   EXCEPTION_ASSERT (theGetFuncPtr != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      WitJ::ProbScalarAtt <Value>::~ProbScalarAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::ProbScalarAtt <Value>::set (
         Component *,
         Value       theValue)
   {
   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (getWitRun (), theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::ProbScalarAtt <Value>::get (
         Component *,
         Value &     theValue)
   {
   myGetFuncPtr_ (getWitRun (), & theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::ProbScalarAtt <Value>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::ProbScalarAtt <Value>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return scalarTypeMatches <Value> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template PartScalarAtt.
//------------------------------------------------------------------------------

template <typename Value>
      WitJ::PartScalarAtt <Value>::PartScalarAtt (
            AttBldr *    theAttBldr,
            const char * theName,
            GetFuncPtr   theGetFuncPtr,
            SetFuncPtr   theSetFuncPtr):

         Att           (theAttBldr, theName, "Part"),
         myGetFuncPtr_ (theGetFuncPtr),
         mySetFuncPtr_ (theSetFuncPtr)
   {
   EXCEPTION_ASSERT (theGetFuncPtr != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      WitJ::PartScalarAtt <Value>::~PartScalarAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::PartScalarAtt <Value>::set (
         Component * theComp,
         Value       theValue)
   {
   Part * thePart;

   thePart = theComp->asPart ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (getWitRun (), thePart->getPartName (), theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::PartScalarAtt <Value>::get (
         Component * theComp,
         Value &     theValue)
   {
   Part * thePart;

   thePart = theComp->asPart ();

   myGetFuncPtr_ (getWitRun (), thePart->getPartName (), & theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::PartScalarAtt <Value>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::PartScalarAtt <Value>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return scalarTypeMatches <Value> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template DemandScalarAtt.
//------------------------------------------------------------------------------

template <typename Value>
      WitJ::DemandScalarAtt <Value>::DemandScalarAtt (
            AttBldr *    theAttBldr,
            const char * theName,
            GetFuncPtr   theGetFuncPtr,
            SetFuncPtr   theSetFuncPtr):

         Att           (theAttBldr, theName, "Demand"),
         myGetFuncPtr_ (theGetFuncPtr),
         mySetFuncPtr_ (theSetFuncPtr)
   {
   EXCEPTION_ASSERT (theGetFuncPtr != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      WitJ::DemandScalarAtt <Value>::~DemandScalarAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::DemandScalarAtt <Value>::set (
         Component * theComp,
         Value       theValue)
   {
   Demand * theDemand;

   theDemand = theComp->asDemand ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (
      getWitRun                (),
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::DemandScalarAtt <Value>::get (
         Component * theComp,
         Value &     theValue)
   {
   Demand * theDemand;

   theDemand = theComp->asDemand ();

   myGetFuncPtr_ (
      getWitRun                (),
      theDemand->getPartName   (),
      theDemand->getDemandName (),
    & theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::DemandScalarAtt <Value>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::DemandScalarAtt <Value>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return scalarTypeMatches <Value> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template OpnScalarAtt.
//------------------------------------------------------------------------------

template <typename Value>
      WitJ::OpnScalarAtt <Value>::OpnScalarAtt (
            AttBldr *    theAttBldr,
            const char * theName,
            GetFuncPtr   theGetFuncPtr,
            SetFuncPtr   theSetFuncPtr):

         Att           (theAttBldr, theName, "Operation"),
         myGetFuncPtr_ (theGetFuncPtr),
         mySetFuncPtr_ (theSetFuncPtr)
   {
   EXCEPTION_ASSERT (theGetFuncPtr != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      WitJ::OpnScalarAtt <Value>::~OpnScalarAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::OpnScalarAtt <Value>::set (
         Component * theComp,
         Value       theValue)
   {
   Operation * theOpn;

   theOpn = theComp->asOperation ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (getWitRun (), theOpn->getOperationName (), theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::OpnScalarAtt <Value>::get (
         Component * theComp,
         Value &     theValue)
   {
   Operation * theOpn;

   theOpn = theComp->asOperation ();

   myGetFuncPtr_ (getWitRun (), theOpn->getOperationName (), & theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::OpnScalarAtt <Value>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::OpnScalarAtt <Value>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return scalarTypeMatches <Value> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template BomEntScalarAtt.
//------------------------------------------------------------------------------

template <typename Value>
      WitJ::BomEntScalarAtt <Value>::BomEntScalarAtt (
            AttBldr *    theAttBldr,
            const char * theName,
            GetFuncPtr   theGetFuncPtr,
            SetFuncPtr   theSetFuncPtr):

         Att           (theAttBldr, theName, "BomEntry"),
         myGetFuncPtr_ (theGetFuncPtr),
         mySetFuncPtr_ (theSetFuncPtr)
   {
   EXCEPTION_ASSERT (theGetFuncPtr != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      WitJ::BomEntScalarAtt <Value>::~BomEntScalarAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::BomEntScalarAtt <Value>::set (
         Component * theComp,
         Value       theValue)
   {
   BomEntry * theBomEnt;

   theBomEnt = theComp->asBomEntry ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (
      getWitRun                   (),
      theBomEnt->getOperationName (),
      theBomEnt->getBomIndex      (),
      theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::BomEntScalarAtt <Value>::get (
         Component * theComp,
         Value &     theValue)
   {
   BomEntry * theBomEnt;

   theBomEnt = theComp->asBomEntry ();

   myGetFuncPtr_ (
      getWitRun                   (),
      theBomEnt->getOperationName (),
      theBomEnt->getBomIndex      (),
    & theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::BomEntScalarAtt <Value>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::BomEntScalarAtt <Value>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return scalarTypeMatches <Value> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template SubScalarAtt.
//------------------------------------------------------------------------------

template <typename Value>
      WitJ::SubScalarAtt <Value>::SubScalarAtt (
            AttBldr *    theAttBldr,
            const char * theName,
            GetFuncPtr   theGetFuncPtr,
            SetFuncPtr   theSetFuncPtr):

         Att           (theAttBldr, theName, "Substitute"),
         myGetFuncPtr_ (theGetFuncPtr),
         mySetFuncPtr_ (theSetFuncPtr)
   {
   EXCEPTION_ASSERT (theGetFuncPtr != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      WitJ::SubScalarAtt <Value>::~SubScalarAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::SubScalarAtt <Value>::set (
         Component * theComp,
         Value       theValue)
   {
   Substitute * theSub;

   theSub = theComp->asSubstitute ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (
      getWitRun                (),
      theSub->getOperationName (),
      theSub->getBomIndex      (),
      theSub->getSubIndex      (),
      theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::SubScalarAtt <Value>::get (
         Component * theComp,
         Value &     theValue)
   {
   Substitute * theSub;

   theSub = theComp->asSubstitute ();

   myGetFuncPtr_ (
      getWitRun                (),
      theSub->getOperationName (),
      theSub->getBomIndex      (),
      theSub->getSubIndex      (),
    & theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::SubScalarAtt <Value>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::SubScalarAtt <Value>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return scalarTypeMatches <Value> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template BopEntScalarAtt.
//------------------------------------------------------------------------------

template <typename Value>
      WitJ::BopEntScalarAtt <Value>::BopEntScalarAtt (
            AttBldr *    theAttBldr,
            const char * theName,
            GetFuncPtr   theGetFuncPtr,
            SetFuncPtr   theSetFuncPtr):

         Att           (theAttBldr, theName, "BopEntry"),
         myGetFuncPtr_ (theGetFuncPtr),
         mySetFuncPtr_ (theSetFuncPtr)
   {
   EXCEPTION_ASSERT (theGetFuncPtr != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      WitJ::BopEntScalarAtt <Value>::~BopEntScalarAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::BopEntScalarAtt <Value>::set (
         Component * theComp,
         Value       theValue)
   {
   BopEntry * theBopEnt;

   theBopEnt = theComp->asBopEntry ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (
      getWitRun                   (),
      theBopEnt->getOperationName (),
      theBopEnt->getBopIndex      (),
      theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      void WitJ::BopEntScalarAtt <Value>::get (
         Component * theComp,
         Value &     theValue)
   {
   BopEntry * theBopEnt;

   theBopEnt = theComp->asBopEntry ();

   myGetFuncPtr_ (
      getWitRun                   (),
      theBopEnt->getOperationName (),
      theBopEnt->getBopIndex      (),
    & theValue);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::BopEntScalarAtt <Value>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Value>
      bool WitJ::BopEntScalarAtt <Value>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return scalarTypeMatches <Value> (theTypeName);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of the class templates implemented in this file.
//------------------------------------------------------------------------------

template class WitJ::   ProbScalarAtt <int>;
template class WitJ::   PartScalarAtt <int>;
template class WitJ:: DemandScalarAtt <int>;
template class WitJ::    OpnScalarAtt <int>;
template class WitJ:: BomEntScalarAtt <int>;
template class WitJ::    SubScalarAtt <int>;
template class WitJ:: BopEntScalarAtt <int>;

template class WitJ::   ProbScalarAtt <double>;
template class WitJ::   PartScalarAtt <double>;
template class WitJ:: DemandScalarAtt <double>;
template class WitJ::    OpnScalarAtt <double>;
template class WitJ:: BomEntScalarAtt <double>;
template class WitJ::    SubScalarAtt <double>;
template class WitJ:: BopEntScalarAtt <double>;

//------------------------------------------------------------------------------
// Implementation of class template ProbScalarAttString.
//------------------------------------------------------------------------------

WitJ::ProbScalarAttString::ProbScalarAttString (
            AttBldr *    theAttBldr,
            const char * theName,
            GetFuncPtr   theGetFuncPtr,
            SetFuncPtr   theSetFuncPtr):

         Att           (theAttBldr, theName, "Problem"),
         myGetFuncPtr_ (theGetFuncPtr),
         mySetFuncPtr_ (theSetFuncPtr)
   {
   EXCEPTION_ASSERT (theGetFuncPtr != NULL);
   }

//------------------------------------------------------------------------------

WitJ::ProbScalarAttString::~ProbScalarAttString ()
   {
   }

//------------------------------------------------------------------------------

void WitJ::ProbScalarAttString::set (Component *, const char * theCStr)
   {
   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (getWitRun (), theCStr);
   }

//------------------------------------------------------------------------------

void WitJ::ProbScalarAttString::get (
      Component *,
      StringJOR & theStringJOR)
   {
   char * theCString;

   myGetFuncPtr_ (getWitRun (), & theCString);

   theStringJOR.makeJavaString (theCString);

   witFree (theCString);
   }

//------------------------------------------------------------------------------

bool WitJ::ProbScalarAttString::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

bool WitJ::ProbScalarAttString::valueTypeMatches (
      const std::string & theTypeName)
   {
   return (theTypeName == "String");
   }
