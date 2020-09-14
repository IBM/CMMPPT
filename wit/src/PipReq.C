//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "PipReq.C"
//
// Contains the implementation of the following classes and templates:
//
//    PipReqSpec
//    PipReqBasis
//    PipReqCList  <RcpComp>
//    PipReqClient <RcpComp>, with explicit instantiation
//------------------------------------------------------------------------------

#include <PipReqMgr.h>
#include <PipReqSpec.h>
#include <PipReqBasis.h>
#include <PipReqCList.h>
#include <PipReqClient.h>
#include <PipRcpFac.h>
#include <GlobalComp.h>
#include <Demand.h>
#include <Operation.h>

//------------------------------------------------------------------------------
// Implementation of class PipReqSpec.
//------------------------------------------------------------------------------

WitPipReqSpec::WitPipReqSpec ():
      myBasis_     (NULL),
      myIncReqVol_ (0.0)
   {
   }

//------------------------------------------------------------------------------

WitPipReqSpec::~WitPipReqSpec ()
   {
   }

//------------------------------------------------------------------------------

void WitPipReqSpec::setData (WitPipReqBasis * theBasis, double incReqVol)
   {
   witAssert (incReqVol >= 0.0);

   myBasis_     = theBasis;
   myIncReqVol_ = incReqVol;
   }

//------------------------------------------------------------------------------

void WitPipReqSpec::getData (WitPipReqBasis * & theBasis, double & incReqVol)
   {
   theBasis  = myBasis_;
   incReqVol = myIncReqVol_;
   }

//------------------------------------------------------------------------------
// Implementation of class PipReqBasis.
//------------------------------------------------------------------------------

WitPipReqBasis::WitPipReqBasis (
         WitPipReqClient <WitDemand> * theDemClient,
         WitProblem *                  theProblem):

      myDemCList_ (NULL),
      myOpnCList_ (NULL)
   {
   WitPtrVec <WitPipReqClient <WitDemand>    > theDemClientVec (1);
   WitPtrVec <WitPipReqClient <WitOperation> > theOpnClientVec (0);

   theDemClientVec[0] = theDemClient;

   myDemCList_        = new WitPipReqCList <WitDemand>    (theDemClientVec);
   myOpnCList_        = new WitPipReqCList <WitOperation> (theOpnClientVec);
   }

//------------------------------------------------------------------------------

WitPipReqBasis::WitPipReqBasis (
         WitPipReqBasis *                 theBasis,
         WitPipReqClient <WitOperation> * theOpnClient):

      myDemCList_ (NULL),
      myOpnCList_ (NULL)
   {
   WitPtrVec <WitPipReqClient <WitOperation> > newOpnClientVec;

   myDemCList_ =
      new WitPipReqCList <WitDemand> (theBasis->myDemCList_->myClientVec ());
   
   formByInsertion (
      newOpnClientVec,
      theBasis->myOpnCList_->myClientVec (),
      theOpnClient);
   
   myOpnCList_ = new WitPipReqCList <WitOperation> (newOpnClientVec);
   }

//------------------------------------------------------------------------------

WitPipReqBasis::WitPipReqBasis (
         const WitPipReqBasisStack & theBases,
         WitProblem *                theProblem):

      myDemCList_ (NULL),
      myOpnCList_ (NULL)
   {
   myDemCList_ = new WitPipReqCList <WitDemand>    (theBases, theProblem);
   myOpnCList_ = new WitPipReqCList <WitOperation> (theBases, theProblem);
   }

//------------------------------------------------------------------------------

WitPipReqBasis::~WitPipReqBasis ()
   {
   delete myOpnCList_;
   delete myDemCList_;
   }

//------------------------------------------------------------------------------

bool WitPipReqBasis::belongsBefore (WitPipReqBasis * theBasis)
   {
   if (myDemCList_->isEquivalentTo (theBasis->myDemCList_))
      {
      return myOpnCList_->belongsBefore (theBasis->myOpnCList_);
      }
   else
      {
      return myDemCList_->belongsBefore (theBasis->myDemCList_);
      }
   }

//------------------------------------------------------------------------------

bool WitPipReqBasis::isEquivalentTo (WitPipReqBasis * theBasis)
   {
   return
         myDemCList_->isEquivalentTo (theBasis->myDemCList_)
      && myOpnCList_->isEquivalentTo (theBasis->myOpnCList_);
   }

//------------------------------------------------------------------------------

void WitPipReqBasis::print (FILE * theFile, const char * leadingBlanks)
   {
   fprintf (theFile,
      "%sReq Clients:\n", leadingBlanks);

   myDemCList_->print (theFile, leadingBlanks);
   myOpnCList_->print (theFile, leadingBlanks);
   }

//------------------------------------------------------------------------------

void WitPipReqBasis::attachClientItr (
      WitPtrVecItr <WitPipReqClient <WitDemand> > & theDemClientItr)
   {
   myDemCList_->attachClientItr (theDemClientItr);
   }

//------------------------------------------------------------------------------

void WitPipReqBasis::attachClientItr (
      WitPtrVecItr <WitPipReqClient <WitOperation> > & theOpnClientItr)
   {
   myOpnCList_->attachClientItr (theOpnClientItr);
   }

//------------------------------------------------------------------------------

void WitPipReqBasis::formByInsertion (
            WitPtrVec <WitPipReqClient <WitOperation> > & newOpnClientVec,
      const WitPtrVec <WitPipReqClient <WitOperation> > & oldOpnClientVec,
            WitPipReqClient <WitOperation> *              newOpnClient)
   {
   int                                            newLength;
   int                                            newIdx;
   WitPtrVecItr <WitPipReqClient <WitOperation> > oldOpnClientItr;
   WitPipReqClient <WitOperation> *               oldOpnClient;

   witAssert (newOpnClient != NULL);

   newLength = oldOpnClientVec.length () + 1;

   newOpnClientVec.resize (newLength);

   newIdx    = 0;

   oldOpnClientVec.attachItr (oldOpnClientItr);

   while (oldOpnClientItr.advance (oldOpnClient))
      {
      if (newOpnClient != NULL)
         if (newOpnClient->mySeqNo () < oldOpnClient->mySeqNo ())
            {
            newOpnClientVec[newIdx ++] = newOpnClient;

            newOpnClient               = NULL;
            }

      newOpnClientVec[newIdx ++] = oldOpnClient;
      }

   if (newOpnClient != NULL)
      newOpnClientVec[newIdx ++] = newOpnClient;

   witAssert (newIdx == newLength);
   }

//------------------------------------------------------------------------------
// Implementation of class template PipReqCList <RcpComp>.
//------------------------------------------------------------------------------

template <typename RcpComp> 
      WitPipReqCList <RcpComp>::WitPipReqCList (
            const WitPtrVec <WitPipReqClient <RcpComp> > & theClientVec):

         myClientVec_ (theClientVec.length ())
   {
   myClientVec_ = theClientVec;
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      WitPipReqCList <RcpComp>::WitPipReqCList (
            const WitPipReqBasisStack & theBases,
            WitProblem *                theProblem):

         myClientVec_ ()
   {
   WitObjStack <WitPipReqClient <RcpComp> > theClients (theProblem);

   getClientsFromBases     (theClients, theBases);

   loadClientVec           (theClients);

   myClientVec_.sort       (compareClients);

   getUniqueClientsFromVec (theClients);

   loadClientVec           (theClients);
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      WitPipReqCList <RcpComp>::~WitPipReqCList ()
   {
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      bool WitPipReqCList <RcpComp>::belongsBefore (
         WitPipReqCList <RcpComp> * theReqCList)
   {
   int minLength;
   int theIdx;
   int theSeqNo1;
   int theSeqNo2;

   minLength = min (length (), theReqCList->length ());

   for (theIdx = 0; theIdx < minLength; theIdx ++)
      {
      theSeqNo1 =              myClientVec_[theIdx]->mySeqNo ();
      theSeqNo2 = theReqCList->myClientVec_[theIdx]->mySeqNo ();

      if (theSeqNo1 < theSeqNo2)
         return true;

      if (theSeqNo1 > theSeqNo2)
         return false;
      }

   return (length () > theReqCList->length ());
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      bool WitPipReqCList <RcpComp>::isEquivalentTo (
         WitPipReqCList <RcpComp> * theReqCList)
   {
   int theIdx;

   if (length () != theReqCList->length ())
      return false;

   for (theIdx = 0; theIdx < length (); theIdx ++)
      {
      if (myClientVec_[theIdx] != theReqCList->myClientVec_[theIdx])
         return false;
      }

   return true;
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      void WitPipReqCList <RcpComp>::print (
         FILE *       theFile,
         const char * leadingBlanks)
   {
   WitPtrVecItr <WitPipReqClient <RcpComp> > theClientItr;
   WitPipReqClient <RcpComp> *               theClient;

   myClientVec_.attachItr (theClientItr);

   while (theClientItr.advance (theClient))
      theClient->print (theFile, leadingBlanks);
   }

//------------------------------------------------------------------------------

template <typename RcpComp>
      void WitPipReqCList <RcpComp>::attachClientItr (
         WitPtrVecItr <WitPipReqClient <RcpComp> > & theClientItr)
   {
   myClientVec_.attachItr (theClientItr);
   }

//------------------------------------------------------------------------------

template <typename RcpComp>
      void WitPipReqCList <RcpComp>::getClientsFromBases (
         WitObjStack <WitPipReqClient <RcpComp> > & theClients,
         const WitPipReqBasisStack &                theBases)
   {
   WitObjStItr <WitPipReqBasis>              theBasisItr;
   WitPipReqBasis *                          theBasis;
   WitPtrVecItr <WitPipReqClient <RcpComp> > theClientItr;
   WitPipReqClient <RcpComp> *               theClient;

   theBases.attachItr (theBasisItr);

   while (theBasisItr.advance (theBasis))
      {
      theBasis->attachClientItr (theClientItr);

      while (theClientItr.advance (theClient))
         theClients.push (theClient);
      }
   }

//------------------------------------------------------------------------------

template <typename RcpComp>
      void WitPipReqCList <RcpComp>::loadClientVec (
         WitObjStack <WitPipReqClient <RcpComp> > & theClients)
   {
   int                         nClients;
   int                         theIdx;
   WitPipReqClient <RcpComp> * theClient;

   nClients = theClients.nElements ();

   myClientVec_.resize (nClients);

   theIdx   = nClients;

   while (theClients.pop (theClient))
      myClientVec_[-- theIdx] = theClient;
   }

//------------------------------------------------------------------------------

template <typename RcpComp>
      bool WitPipReqCList <RcpComp>::compareClients (
         WitPipReqClient <RcpComp> * theClient1,
         WitPipReqClient <RcpComp> * theClient2)
   {
   return (theClient1->mySeqNo () < theClient2->mySeqNo ());
   }

//------------------------------------------------------------------------------

template <typename RcpComp>
      void WitPipReqCList <RcpComp>::getUniqueClientsFromVec (
         WitObjStack <WitPipReqClient <RcpComp> > & theClients)
   {
   WitPipReqClient <RcpComp> *               prevClient;
   WitPtrVecItr <WitPipReqClient <RcpComp> > theClientItr;
   WitPipReqClient <RcpComp> *               theClient;

   prevClient = NULL;

   myClientVec_.attachItr (theClientItr);

   while (theClientItr.advance (theClient))
      {
      if (theClient == prevClient)
         continue;

      theClients.push (theClient);

      prevClient = theClient;
      }
   }

//------------------------------------------------------------------------------
// Implementation of class template PipReqClient <RcpComp>.
//------------------------------------------------------------------------------

template <typename RcpComp> 
      WitPipReqClient <RcpComp>::WitPipReqClient (
            RcpComp * theRcpComp,
            WitPeriod rcpPer,
            int       theSeqNo):

         myRcpComp_ (theRcpComp),
         myRcpPer_  (rcpPer),
         mySeqNo_   (theSeqNo)
   {
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      WitPipReqClient <RcpComp>::~WitPipReqClient ()
   {
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      void WitPipReqClient <RcpComp>::getData (
         RcpComp * & theRcpComp,
         WitPeriod & rcpPer)
   {
   theRcpComp = myRcpComp_;
   rcpPer     = myRcpPer_;
   }

//------------------------------------------------------------------------------

template <typename RcpComp> 
      void WitPipReqClient <RcpComp>::print (
         FILE *       theFile,
         const char * leadingBlanks)
   {
   RcpComp * theRcpComp;
   WitPeriod rcpPer;
   
   getData (theRcpComp, rcpPer);

   fprintf (theFile,
      "   %s%s %s, Per %d, Seq #%d\n",
      leadingBlanks,
      WitPipRcpFac <RcpComp>::getRcpCompLabel (),
      WitPipRcpFac <RcpComp>::getRcpCompName (theRcpComp),
      rcpPer,
      mySeqNo_);
   }

//------------------------------------------------------------------------------
// Explicit instantiation of class template PipReqClient <RcpComp>.
//------------------------------------------------------------------------------

template class WitPipReqClient <WitDemand>;
template class WitPipReqClient <WitOperation>;
