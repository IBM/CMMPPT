//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "PreHelp.C"
//
// Contains the implementation of the following classes and templates:
//
//    BelowLister
//    AvSorter <SortEntry>
//------------------------------------------------------------------------------

#include <Below.h>
#include <AvSorter.h>
#include <BopEntry.h>
#include <SubEntry.h>
#include <Part.h>
#include <Operation.h>
#include <ObjStack.h>

//------------------------------------------------------------------------------
// Implementation of class BelowLister.
//------------------------------------------------------------------------------

WitBelowLister::WitBelowLister (WitProblem * theProblem):

      WitProbAssoc    (theProblem),
      isBelowCurrent_ ()
   {
   }

//------------------------------------------------------------------------------

WitBelowLister::~WitBelowLister ()
   {
   }

//------------------------------------------------------------------------------

void WitBelowLister::buildBelow ()
   {
   WitPart * thePart;

   isBelowCurrent_.allocate (myProblem (), false);

   forEachPart (thePart, myProblem ())
      buildBelow (thePart);

   isBelowCurrent_.clear  ();
   }

//------------------------------------------------------------------------------

void WitBelowLister::buildBelow (WitPart * thePart)
   {
   WitObjStack <WitPart> theParts (myProblem ());
   WitBopEntry *         theBopEnt;
   WitOperation *        theOpn;
   WitBomEntry *         theBomEnt;
   WitSubEntry *         theSub;
   WitPtrVec <WitPart>   belowListForPart;
   int                   theIdx;
   WitPart *             thePartBelow;


   theParts.push (thePart);

   isBelowCurrent_ (thePart) = true;

   forEachEl (theBopEnt, thePart->producingBopEntries ())
      if (theBopEnt->expAllowed ())
         {
         theOpn = theBopEnt->myOperation ();

         forEachEl (theBomEnt, theOpn->bom ())
            {
            mergeBelow (theBomEnt, theParts);

            forEachEl (theSub, theBomEnt->mySubEntries ())
               mergeBelow (theSub, theParts);
            }
         }

   belowListForPart.resize (theParts.nElements ());

   theIdx = -1;

   while (theParts.pop (thePartBelow))
      {
      theIdx ++;

      belowListForPart[theIdx] = thePartBelow;

      isBelowCurrent_ (thePartBelow) = false;
      }

   if (belowListForPart.length () > 1)
      belowListForPart.sort (compareParts);

   thePart->storePreBelowList (belowListForPart);
   }

//------------------------------------------------------------------------------

void WitBelowLister::mergeBelow (
      WitConsEntry *          theConsEnt,
      WitObjStack <WitPart> & theParts)
   {
   WitPtrVecItr <WitPart> theItr;
   WitPart *              thePart;

   theConsEnt->myPart ()->belowList ().attachItr (theItr);

   while (theItr.advance (thePart))
      if (not isBelowCurrent_ (thePart))
         {
         theParts.push (thePart);

         isBelowCurrent_ (thePart) = true;
         }
   }

//------------------------------------------------------------------------------

bool WitBelowLister::compareParts (WitPart * thePart1, WitPart * thePart2)
   {
   int theIdx1;
   int theIdx2;

   theIdx1 = thePart1->sortedPartIndex ();
   theIdx2 = thePart2->sortedPartIndex ();

   return (theIdx1 > theIdx2);
   }

//------------------------------------------------------------------------------
// Implementation of class template AvSorter <SortEntry>
//------------------------------------------------------------------------------

template <typename SortEntry> 
      WitAvSorter <SortEntry>::WitAvSorter (int nEntries):

         workingVec_ (nEntries)
   {
   }

//------------------------------------------------------------------------------

template <typename SortEntry> 
      WitAvSorter <SortEntry>::~WitAvSorter ()
   {
   }

//------------------------------------------------------------------------------

template <typename SortEntry> 
      void WitAvSorter <SortEntry>::sort (WitList <SortEntry> & theList)
   {
   int         theIdx;
   SortEntry * theSortEnt;
   int         nEntries;

   if (inOrder (theList))
      return;

   theIdx = -1;

   forEachEl (theSortEnt, theList)
      {
      theIdx ++;

      workingVec_[theIdx] = theSortEnt;
      }

   nEntries = theList.nElements ();

   workingVec_.sort (compareAversions, nEntries);

   theList.clear ();

   for (theIdx = 0; theIdx < nEntries; theIdx ++)
      theList.append (workingVec_[theIdx]);
   }

//------------------------------------------------------------------------------

template <typename SortEntry> 
      bool WitAvSorter <SortEntry>::inOrder (
         const WitList <SortEntry> & theList)
   {
   SortEntry * curSortEnt;
   SortEntry * prevSortEnt;

   prevSortEnt = NULL;

   forEachEl (curSortEnt, theList)
      {
      if (prevSortEnt != NULL)
         if (aversion (curSortEnt) < aversion (prevSortEnt))
            return false;

      prevSortEnt = curSortEnt;
      }

   return true;
   }

//------------------------------------------------------------------------------

template <typename SortEntry> 
      bool WitAvSorter <SortEntry>::compareAversions (
         SortEntry * theSortEnt1,
         SortEntry * theSortEnt2)
   {
   double aversion1;
   double aversion2;
   int    theIdx1;
   int    theIdx2;

   aversion1 = aversion (theSortEnt1);
   aversion2 = aversion (theSortEnt2);

   if (aversion1 < aversion2)
      return true;

   if (aversion1 > aversion2)
      return false;

   theIdx1 = theSortEnt1->mappingIndex ();
   theIdx2 = theSortEnt2->mappingIndex ();

   return (theIdx1 < theIdx2);
   }

//------------------------------------------------------------------------------

template <>
      double WitAvSorter <WitBopEntry>::aversion (WitBopEntry * theBopEnt)
   {
   return theBopEnt->expAversion ();
   }

//------------------------------------------------------------------------------

template <>
      double WitAvSorter <WitSubEntry>::aversion (WitSubEntry * theSub)
   {
   return theSub->expNetAversion ();
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template AvSorter <SortEntry>
//------------------------------------------------------------------------------

template class WitAvSorter <WitBopEntry>;
template class WitAvSorter <WitSubEntry>;
