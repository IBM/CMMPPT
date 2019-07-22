//------------------------------------------------------------------------------
// WIT-J C++ Source File VectorAtts.C.
//
// Contains the implementations of the following class templates:
//
//      ProbVectorAtt <Elem>
//      PartVectorAtt <Elem>
//    DemandVectorAtt <Elem>
//       OpnVectorAtt <Elem>
//    BomEntVectorAtt <Elem>
//       SubVectorAtt <Elem>
//    BopEntVectorAtt <Elem>
//
// Contains the explicit instantiation of each of these class templates.
//------------------------------------------------------------------------------

#include <VectorAtts.h>
#include       <Part.h>
#include     <Demand.h>
#include  <Operation.h>
#include   <BomEntry.h>
#include <Substitute.h>
#include   <BopEntry.h>
#include   <ArrayJOR.h>

//------------------------------------------------------------------------------
// Implementation of class template ProbVectorAtt.
//------------------------------------------------------------------------------

template <typename Elem>
      WitJ::ProbVectorAtt <Elem>::ProbVectorAtt (
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

template <typename Elem>
      WitJ::ProbVectorAtt <Elem>::~ProbVectorAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::ProbVectorAtt <Elem>::set (
         Component *,
         const Elem * theCVec)
   {
   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (getWitRun (), theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::ProbVectorAtt <Elem>::get (
         Component *,
         ArrayJOR <Elem> & theArrayJOR)
   {
   Elem * theCVec;

   myGetFuncPtr_ (getWitRun (), & theCVec);

   theArrayJOR.copyFrom (theCVec);

   witFree (theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::ProbVectorAtt <Elem>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::ProbVectorAtt <Elem>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return vectorTypeMatches <Elem> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template PartVectorAtt.
//------------------------------------------------------------------------------

template <typename Elem>
   WitJ::PartVectorAtt <Elem>::PartVectorAtt (
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

template <typename Elem>
      WitJ::PartVectorAtt <Elem>::~PartVectorAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::PartVectorAtt <Elem>::set (
         Component *  theComp,
         const Elem * theCVec)
   {
   Part * thePart;

   thePart = theComp->asPart ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (getWitRun (), thePart->getPartName (), theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::PartVectorAtt <Elem>::get (
         Component *       theComp,
         ArrayJOR <Elem> & theArrayJOR)
   {
   Part * thePart;
   Elem * theCVec;

   thePart = theComp->asPart ();

   myGetFuncPtr_ (
      getWitRun            (),
      thePart->getPartName (),
    & theCVec);

   theArrayJOR.copyFrom (theCVec);

   witFree (theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::PartVectorAtt <Elem>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::PartVectorAtt <Elem>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return vectorTypeMatches <Elem> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template DemandVectorAtt.
//------------------------------------------------------------------------------

template <typename Elem>
      WitJ::DemandVectorAtt <Elem>::DemandVectorAtt (
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

template <typename Elem>
      WitJ::DemandVectorAtt <Elem>::~DemandVectorAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::DemandVectorAtt <Elem>::set (
         Component *  theComp,
         const Elem * theCVec)
   {
   Demand * theDemand;

   theDemand = theComp->asDemand ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (
      getWitRun                (),
      theDemand->getPartName   (),
      theDemand->getDemandName (),
      theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::DemandVectorAtt <Elem>::get (
         Component *       theComp,
         ArrayJOR <Elem> & theArrayJOR)
   {
   Demand * theDemand;
   Elem *   theCVec;

   theDemand = theComp->asDemand ();

   myGetFuncPtr_ (
      getWitRun                (),
      theDemand->getPartName   (),
      theDemand->getDemandName (),
    & theCVec);

   theArrayJOR.copyFrom (theCVec);

   witFree (theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::DemandVectorAtt <Elem>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::DemandVectorAtt <Elem>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return vectorTypeMatches <Elem> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template OpnVectorAtt.
//------------------------------------------------------------------------------

template <typename Elem>
      WitJ::OpnVectorAtt <Elem>::OpnVectorAtt (
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

template <typename Elem>
      WitJ::OpnVectorAtt <Elem>::~OpnVectorAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::OpnVectorAtt <Elem>::set (
         Component *  theComp,
         const Elem * theCVec)
   {
   Operation * theOpn;

   theOpn = theComp->asOperation ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (getWitRun (), theOpn->getOperationName (), theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::OpnVectorAtt <Elem>::get (
         Component *       theComp,
         ArrayJOR <Elem> & theArrayJOR)
   {
   Operation * theOpn;
   Elem *      theCVec;

   theOpn = theComp->asOperation ();

   myGetFuncPtr_ (
      getWitRun                (),
      theOpn->getOperationName (),
    & theCVec);

   theArrayJOR.copyFrom (theCVec);

   witFree (theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::OpnVectorAtt <Elem>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::OpnVectorAtt <Elem>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return vectorTypeMatches <Elem> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template BomEntVectorAtt.
//------------------------------------------------------------------------------

template <typename Elem>
      WitJ::BomEntVectorAtt <Elem>::BomEntVectorAtt (
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

template <typename Elem>
      WitJ::BomEntVectorAtt <Elem>::~BomEntVectorAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::BomEntVectorAtt <Elem>::set (
         Component *  theComp,
         const Elem * theCVec)
   {
   BomEntry * theBomEnt;

   theBomEnt = theComp->asBomEntry ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (
      getWitRun                   (),
      theBomEnt->getOperationName (),
      theBomEnt->getBomIndex      (),
      theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::BomEntVectorAtt <Elem>::get (
         Component *       theComp,
         ArrayJOR <Elem> & theArrayJOR)
   {
   BomEntry * theBomEnt;
   Elem *     theCVec;

   theBomEnt = theComp->asBomEntry ();

   myGetFuncPtr_ (
      getWitRun                   (),
      theBomEnt->getOperationName (),
      theBomEnt->getBomIndex      (),
    & theCVec);

   theArrayJOR.copyFrom (theCVec);

   witFree (theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::BomEntVectorAtt <Elem>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::BomEntVectorAtt <Elem>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return vectorTypeMatches <Elem> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template SubVectorAtt.
//------------------------------------------------------------------------------

template <typename Elem>
      WitJ::SubVectorAtt <Elem>::SubVectorAtt (
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

template <typename Elem>
      WitJ::SubVectorAtt <Elem>::~SubVectorAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::SubVectorAtt <Elem>::set (
         Component *  theComp,
         const Elem * theCVec)
   {
   Substitute * theSub;

   theSub = theComp->asSubstitute ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (
      getWitRun                (),
      theSub->getOperationName (),
      theSub->getBomIndex      (),
      theSub->getSubIndex      (),
      theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::SubVectorAtt <Elem>::get (
         Component *       theComp,
         ArrayJOR <Elem> & theArrayJOR)
   {
   Substitute * theSub;
   Elem *       theCVec;

   theSub = theComp->asSubstitute ();

   myGetFuncPtr_ (
      getWitRun                (),
      theSub->getOperationName (),
      theSub->getBomIndex      (),
      theSub->getSubIndex      (),
    & theCVec);

   theArrayJOR.copyFrom (theCVec);

   witFree (theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::SubVectorAtt <Elem>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::SubVectorAtt <Elem>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return vectorTypeMatches <Elem> (theTypeName);
   }

//------------------------------------------------------------------------------
// Implementation of class template BopEntVectorAtt.
//------------------------------------------------------------------------------

template <typename Elem>
      WitJ::BopEntVectorAtt <Elem>::BopEntVectorAtt (
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

template <typename Elem>
      WitJ::BopEntVectorAtt <Elem>::~BopEntVectorAtt ()
   {
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::BopEntVectorAtt <Elem>::set (
         Component *  theComp,
         const Elem * theCVec)
   {
   BopEntry * theBopEnt;

   theBopEnt = theComp->asBopEntry ();

   EXCEPTION_ASSERT (mySetFuncPtr_ != NULL);

   mySetFuncPtr_ (
      getWitRun                  (),
      theBopEnt->getOperationName (),
      theBopEnt->getBopIndex      (),
      theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      void WitJ::BopEntVectorAtt <Elem>::get (
         Component *       theComp,
         ArrayJOR <Elem> & theArrayJOR)
   {
   BopEntry * theBopEnt;
   Elem *     theCVec;

   theBopEnt = theComp->asBopEntry ();

   myGetFuncPtr_ (
      getWitRun                   (),
      theBopEnt->getOperationName (),
      theBopEnt->getBopIndex      (),
    & theCVec);

   theArrayJOR.copyFrom (theCVec);

   witFree (theCVec);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::BopEntVectorAtt <Elem>::isModifiable ()
   {
   return (mySetFuncPtr_ != NULL);
   }

//------------------------------------------------------------------------------

template <typename Elem>
      bool WitJ::BopEntVectorAtt <Elem>::valueTypeMatches (
         const std::string & theTypeName)
   {
   return vectorTypeMatches <Elem> (theTypeName);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of the class templates implemented in this file.
//------------------------------------------------------------------------------

template class WitJ::   ProbVectorAtt <int>;
template class WitJ::   PartVectorAtt <int>;
template class WitJ:: DemandVectorAtt <int>;
template class WitJ::    OpnVectorAtt <int>;
template class WitJ:: BomEntVectorAtt <int>;
template class WitJ::    SubVectorAtt <int>;
template class WitJ:: BopEntVectorAtt <int>;

template class WitJ::   ProbVectorAtt <double>;
template class WitJ::   PartVectorAtt <double>;
template class WitJ:: DemandVectorAtt <double>;
template class WitJ::    OpnVectorAtt <double>;
template class WitJ:: BomEntVectorAtt <double>;
template class WitJ::    SubVectorAtt <double>;
template class WitJ:: BopEntVectorAtt <double>;
