//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2020  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Routing.C"
//
// Contains the implementation of the following classes:
//
//    RtMgr.
//    RtSite.
//    ProdRtSite.
//    ConsRtSite.
//    RtCand.
//    ProdRtCand.
//    ConsRtCand.
//------------------------------------------------------------------------------

#include <RtMgr.h>
#include <ConcRtSite.h>
#include <ConcRtCand.h>
#include <Selector.h>
#include <Problem.h>
#include <CompMgr.h>
#include <GlobalComp.h>
#include <Part.h>
#include <Operation.h>
#include <BopEntry.h>
#include <SubEntry.h>

//------------------------------------------------------------------------------
// Implementation of class RtMgr.         
//------------------------------------------------------------------------------
                                          
WitRtMgr::WitRtMgr (WitSelector * theSelector):

      WitProbAssoc       (theSelector),
      myRtSites_         (),
      myRtCands_         (),
      myRtSiteForPart_   (myProblem ()),
      myRtSiteForBomEnt_ (myProblem ()),
      myRtCandFor_       (myProblem ()),
      mySelector_        (theSelector)
   {                                      
   WitRtSite * theRtSite;

   buildRtSites ();
   buildRtCands ();

   forEachEl (theRtSite, myRtSites_)
      theRtSite->findRtCands ();

   if (selPrintLevelAt (3))
      print ();
   }                                      

//------------------------------------------------------------------------------
                                          
WitRtMgr::~WitRtMgr ()                    
   {                                      
   WitRtSite * theRtSite;

   myRtSiteForBomEnt_.clear ();
   myRtSiteForPart_  .clear ();

   forEachEl (theRtSite, myRtSites_)
      theRtSite->clearRtCands ();

   while (not myRtCands_.isEmpty ())
      delete myRtCands_.get ();

   while (not myRtSites_.isEmpty ())
      delete myRtSites_.get ();
   }                                      

//------------------------------------------------------------------------------

void WitRtMgr::buildRtSites ()
   {
   WitPart *     thePart;
   WitBomEntry * theBomEnt;
   WitRtSite *   theRtSite;

   forEachPart (thePart, myProblem ())
      {
      theRtSite = new WitProdRtSite (thePart, this);

      myRtSites_.append (theRtSite);

      myRtSiteForPart_ (thePart) = theRtSite;
      }

   forEachBomEntry (theBomEnt, myProblem ())
      {
      theRtSite = new WitConsRtSite (theBomEnt, this);

      myRtSites_.append (theRtSite);

      myRtSiteForBomEnt_ (theBomEnt) = theRtSite;
      }
   }

//------------------------------------------------------------------------------

void WitRtMgr::buildRtCands ()
   {
   WitBopEntry * theBopEnt;
   WitBomEntry * theBomEnt;
   WitSubEntry * theSub;
   WitRtCand *   theRtCand;

   forEachBopEntry (theBopEnt, myProblem ())
      {
      theRtCand = WitProdRtCand::newInstance (theBopEnt, this);

      myRtCands_.append (theRtCand);
      }

   forEachBomEntry (theBomEnt, myProblem ())
      {
      theRtCand = WitConsRtCand::newInstance (theBomEnt, this);

      myRtCands_.append (theRtCand);

      forEachEl (theSub, theBomEnt->mySubEntries ())
         {
         theRtCand = WitConsRtCand::newInstance (theSub, this);

         myRtCands_.append (theRtCand);
         }
      }

   forEachEl (theRtCand, myRtCands_)
      myRtCandFor_ (theRtCand->myBillEnt ()) = theRtCand;
   }

//------------------------------------------------------------------------------

void WitRtMgr::print ()
   {
   fprintf (msgFile (),
      "\n"
      "Routing Element Subsystem:\n"
      "\n"
      "   # Routing Sites:      %2d\n"
      "   # Routing Candidates: %2d\n"
      "\n",
      myRtSites_.nElements (),
      myRtCands_.nElements ());
   }

//------------------------------------------------------------------------------
// Implementation of class RtSite.        
//------------------------------------------------------------------------------

WitRtSite::~WitRtSite ()                  
   {                                      
   }                                      
                                          
//------------------------------------------------------------------------------

void WitRtSite::findRtCands ()
   {
   findRtCands (myRtCands_);
   }

//------------------------------------------------------------------------------

void WitRtSite::clearRtCands ()
   {
   myRtCands_.clear ();
   }

//------------------------------------------------------------------------------

void WitRtSite::printPer (WitPeriod thePer, int labelCol, int dataCol)
   {
   fprintf (msgFile (),
      "%*s%-9s %*s%d\n",
      labelCol - 1,
      "",
      perCaseID (),
      max (0, dataCol - labelCol - 10),
      "",
      thePer);
   }

//------------------------------------------------------------------------------

int WitRtSite::nInstances (WitProblem * theProblem)
   {
   return
      WitPart    ::nInstances (theProblem) + 
      WitBomEntry::nInstances (theProblem);
   }

//------------------------------------------------------------------------------

WitRtSite::WitRtSite (WitRtMgr * theRtMgr):

      WitProbAssoc  (theRtMgr),
      mappingIndex_ (theRtMgr->myRtSites ().nElements ()),
      myRtCands_    (),
      myRtMgr_      (theRtMgr)
   {
   }

//------------------------------------------------------------------------------
// Implementation of class ProdRtSite.        
//------------------------------------------------------------------------------
                                          
WitProdRtSite::WitProdRtSite (WitPart * thePart, WitRtMgr * theRtMgr):

      WitRtSite (theRtMgr),
      myPart_   (thePart)
   {                                      
   }

//------------------------------------------------------------------------------

WitProdRtSite::~WitProdRtSite ()
   {
   }

//------------------------------------------------------------------------------

bool WitProdRtSite::isForProd ()
   {
   return true;
   }

//------------------------------------------------------------------------------

void WitProdRtSite::printID (int labelCol, int dataCol)
   {
   fprintf (msgFile (),
      "%*sPart: %*s%s\n",
      labelCol - 1,
      "",
      max (0, dataCol - labelCol - 6),
      "",
      myPart_->partName ().myCstring ());
   }

//------------------------------------------------------------------------------

bool WitProdRtSite::explodeable (WitPeriod expPer)
   {
   return myPart_->explodeable (expPer);
   }

//------------------------------------------------------------------------------

const WitFlexVec <bool> & WitProdRtSite::propRtg ()
   {
   return myPart_->propRtg ();
   }

//------------------------------------------------------------------------------

bool WitProdRtSite::singleSource ()
   {
   return myPart_->singleSource ();
   }

//------------------------------------------------------------------------------

WitPart * WitProdRtSite::mrSortPart ()
   {
   return myPart_;
   }

//------------------------------------------------------------------------------

WitRtCand * WitProdRtSite::mrDefaultRtCand (WitPeriod expPer)
   {
   WitBopEntry * theBopEnt;

   if (not myPart_->explodeable (expPer))
      return NULL;

   theBopEnt = myPart_->expBopEntry (expPer);

   return myRtMgr ()->myRtCandFor (theBopEnt);
   }

//------------------------------------------------------------------------------

int WitProdRtSite::heightIdx ()
   {
   return (myPart_->sortedPartIndex () * 2) + 1;
   }

//------------------------------------------------------------------------------

void WitProdRtSite::findRtCands (WitList <WitRtCand> & theRtCands)
   {
   WitBopEntry * theBopEnt;
   WitRtCand *   theRtCand;

   theRtCands.clear ();

   forEachEl (theBopEnt, myPart_->producingBopEntries ())
      {
      theRtCand = myRtMgr ()->myRtCandFor (theBopEnt);

      theRtCands.append (theRtCand);
      }
   }

//------------------------------------------------------------------------------

const char * WitProdRtSite::perCaseID ()
   {
   return "Exp Per:";
   }

//------------------------------------------------------------------------------
// Implementation of class ConsRtSite.
//------------------------------------------------------------------------------

WitConsRtSite::WitConsRtSite (WitBomEntry * theBomEnt, WitRtMgr * theRtMgr):

      WitRtSite (theRtMgr),
      myBomEnt_ (theBomEnt)
   {
   }

//------------------------------------------------------------------------------

WitConsRtSite::~WitConsRtSite ()
   {
   }

//------------------------------------------------------------------------------

bool WitConsRtSite::isForProd ()
   {
   return false;
   }

//------------------------------------------------------------------------------

void WitConsRtSite::printID (int labelCol, int dataCol)
   {
   fprintf (msgFile (),
      "%*sBomEntry: %*s%s #%d\n",
      labelCol - 1,
      "",
      max (0, dataCol - labelCol - 10),
      "",
      myBomEnt_->myOperationName ().myCstring (),
      myBomEnt_->localIndex ());
   }

//------------------------------------------------------------------------------

bool WitConsRtSite::explodeable (WitPeriod execPer)
   {
   return myBomEnt_->inEffect (execPer);
   }

//------------------------------------------------------------------------------

const WitFlexVec <bool> & WitConsRtSite::propRtg ()
   {
   return myBomEnt_->propRtg ();
   }

//------------------------------------------------------------------------------

bool WitConsRtSite::singleSource ()
   {
   return myBomEnt_->singleSource ();
   }

//------------------------------------------------------------------------------

WitPart * WitConsRtSite::mrSortPart ()
   {
   WitOperation * theOpn;
   int            minIdx;
   WitPart *      thePart;
   WitBopEntry *  theBopEnt;

   theOpn  = myBomEnt_->myOperation ();

   minIdx  = theOpn->myCompMgr ()->nParts ();

   thePart = NULL;

   forEachEl (theBopEnt, theOpn->bop ())
      if (theBopEnt->expAllowed ())
         if (theBopEnt->myPart ()->sortedPartIndex () < minIdx)
            {
            thePart = theBopEnt->myPart ();

            minIdx  = thePart->sortedPartIndex ();
            }

   witAssert (thePart != NULL);

   return thePart;
   }

//------------------------------------------------------------------------------

WitRtCand * WitConsRtSite::mrDefaultRtCand (WitPeriod execPer)
   {
   if (myBomEnt_->inEffect (execPer))
      return myRtMgr ()->myRtCandFor (myBomEnt_);
   else
      return NULL;
   }

//------------------------------------------------------------------------------

int WitConsRtSite::heightIdx ()
   {
   int            minIdx;
   WitOperation * theOpn;
   WitBopEntry *  theBopEnt;

   minIdx = myCompMgr ()->nParts ();

   theOpn = myBomEnt_->myOperation ();

   forEachEl (theBopEnt, theOpn->bop ())
      setToMin (minIdx, theBopEnt->myPart ()->sortedPartIndex ());

   return (minIdx * 2);
   }

//------------------------------------------------------------------------------

void WitConsRtSite::findRtCands (WitList <WitRtCand> & theRtCands)
   {
   WitSubEntry * theSub;
   WitRtCand *   theRtCand;

   theRtCands.clear ();

   theRtCand = myRtMgr ()->myRtCandFor (myBomEnt_);

   theRtCands.append (theRtCand);

   forEachEl (theSub, myBomEnt_->mySubEntries ())
      {
      theRtCand = myRtMgr ()->myRtCandFor (theSub);

      theRtCands.append (theRtCand);
      }
   }

//------------------------------------------------------------------------------

const char * WitConsRtSite::perCaseID ()
   {
   return "Exec Per:";
   }

//------------------------------------------------------------------------------
// Implementation of class RtCand.        
//------------------------------------------------------------------------------
                                          
WitRtCand::~WitRtCand ()                  
   {                                      
   }                                      
                                          
//------------------------------------------------------------------------------

int WitRtCand::nInstances (WitProblem * theProblem)
   {
   return WitBillEntry::nInstances (theProblem);
   }

//------------------------------------------------------------------------------

WitBopEntry * WitRtCand::myBopEnt ()
   {
   return myBillEnt_->thisBopEnt ();
   }

//------------------------------------------------------------------------------

WitConsEntry * WitRtCand::myConsEnt ()
   {
   return myBillEnt_->thisConsEnt ();
   }

//------------------------------------------------------------------------------

WitRtCand::WitRtCand (
         WitBillEntry * theBillEnt,
         WitRtSite *    theRtSite,
         WitRtMgr *     theRtMgr):

      WitProbAssoc  (theBillEnt),
      mappingIndex_ (theRtMgr->myRtCands ().nElements ()),
      myBillEnt_    (theBillEnt),
      myRtSite_     (theRtSite)
   {
   }

//------------------------------------------------------------------------------
// Implementation of class ProdRtCand.        
//------------------------------------------------------------------------------
                                          
WitProdRtCand * WitProdRtCand::newInstance (
      WitBopEntry * theBopEnt,
      WitRtMgr *    theRtMgr)
   {
   WitRtSite * theRtSite;

   theRtSite = theRtMgr->myRtSiteFor (theBopEnt->myPart ());

   return new WitProdRtCand (theBopEnt, theRtSite, theRtMgr);
   }

//------------------------------------------------------------------------------

WitProdRtCand::WitProdRtCand (
         WitBopEntry * theBopEnt,
         WitRtSite *   theRtSite,
         WitRtMgr *    theRtMgr):

      WitRtCand (theBopEnt, theRtSite, theRtMgr),
      myBopEnt_ (theBopEnt)                 
   {
   }

//------------------------------------------------------------------------------

WitProdRtCand::~WitProdRtCand ()
   {
   }
                                          
//------------------------------------------------------------------------------

bool WitProdRtCand::usedByMr ()
   {
   return myBopEnt_->expAllowed ();
   }

//------------------------------------------------------------------------------

void WitProdRtCand::printID (int labelCol, int dataCol)
   {
   fprintf (msgFile (),
      "%*sBopEntry: %*s%s #%d\n",
      labelCol - 1,
      "",
      max (0, dataCol - labelCol - 10),
      "",
      myBopEnt_->myOperationName ().myCstring (),
      myBopEnt_->localIndex      ());
   }

//------------------------------------------------------------------------------

bool WitProdRtCand::isEligible (WitPeriod expPer)
   {
   return myBopEnt_->expEligible (expPer);
   }

//------------------------------------------------------------------------------
// Implementation of class ConsRtCand.        
//------------------------------------------------------------------------------
                                          
WitConsRtCand * WitConsRtCand::newInstance (
      WitConsEntry * theConsEnt,
      WitRtMgr *     theRtMgr)
   {
   WitRtSite * theRtSite;

   theRtSite = theRtMgr->myRtSiteFor (theConsEnt->myBomEnt ());

   return new WitConsRtCand (theConsEnt, theRtSite, theRtMgr);
   }

//------------------------------------------------------------------------------

WitConsRtCand::WitConsRtCand (
         WitConsEntry * theConsEnt,
         WitRtSite *    theRtSite,
         WitRtMgr *     theRtMgr):

      WitRtCand  (theConsEnt, theRtSite, theRtMgr),
      myConsEnt_ (theConsEnt)                 
   {
   }

//------------------------------------------------------------------------------

WitConsRtCand::~WitConsRtCand ()
   {
   }
                                          
//------------------------------------------------------------------------------

bool WitConsRtCand::usedByMr ()
   {
   WitSubEntry * theSub;

   theSub = myConsEnt_->thisSub ();

   if (theSub == NULL)
      return true;

   if (theSub->myPart ()->producingBopEntries ().isEmpty ())
      if (theSub->netAllowed ())
         if (not theSub->myBomEnt ()->singleSource ())
            return false;

   return (theSub->expAllowed ());
   }

//------------------------------------------------------------------------------

void WitConsRtCand::printID (int labelCol, int dataCol)
   {
   WitSubEntry * theSub;

   theSub = myConsEnt_->thisSub ();

   if (theSub != NULL)
      fprintf (msgFile (), 
         "%*sCons Entry:    %*sSub #%d\n",
         labelCol - 1,
         "",
         max (0, dataCol - labelCol - 15),
         "",
         theSub->localIndex ());
   else
      fprintf (msgFile (),
         "%*sCons Entry:    %*sItself\n",
         labelCol - 1,
         "",
         max (0, dataCol - labelCol - 15),
         "");

   fprintf (msgFile (),
         "%*sConsumed Part: %*s%s\n",
         labelCol - 1,
         "",
         max (0, dataCol - labelCol - 15),
         "",
         myConsEnt_->myPartName ().myCstring ());
   }

//------------------------------------------------------------------------------

bool WitConsRtCand::isEligible (WitPeriod execPer)
   {
   return myConsEnt_->inEffect (execPer);
   }
