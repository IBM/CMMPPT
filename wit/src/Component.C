//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Component.C"
//
// Contains the implementation of the following classes:
//
//    Component
//    DelComp
//    Node
//------------------------------------------------------------------------------

#include <Node.h>
#include <DelCompItr.h>
#include <OptComp.h>
#include <Part.h>
#include <Demand.h>
#include <Operation.h>
#include <BomEntry.h>
#include <SubEntry.h>
#include <BopEntry.h>
#include <DataWrit.h>
#include <Pre.h>
#include <HeurAllMgr.h>
#include <OptStarter.h>
#include <Post.h>
#include <FSS.h>
#include <BoundSet.h>
#include <OrigMrp.h>
#include <StochImpMgr.h>
#include <IVRealArg.h>
#include <MsgFrag.h>
#include <MsgFac.h>

//------------------------------------------------------------------------------
// Implementation of class Component.
//------------------------------------------------------------------------------

WitComponent::~WitComponent ()
   {
   witAssert (myCompMgr ()->isDeleting (this));
   }

//------------------------------------------------------------------------------

void WitComponent::setAppData (void * theData)
   {
   if (myStochImpMgr ()->stochMode ())
       myStochImpMgr ()->prepSetStochModeAttr ("appData");

   appData_ = theData;
   }

//------------------------------------------------------------------------------

void WitComponent::writeData ()
   {
   writeDataID ();

   writeDataAttrs ();

   myDataWriter ()->writeEndCommand ();
   }

//------------------------------------------------------------------------------

void WitComponent::writeMultiObjVecs (WitObjective *)
   {
   }

//------------------------------------------------------------------------------

void WitComponent::writeSolnData ()
   {
   }

//------------------------------------------------------------------------------

void WitComponent::purgeCompLists ()
   {
   witAssert (myCompMgr ()->purging ());

   purgeCompListsSpec ();
   }

//------------------------------------------------------------------------------

void WitComponent::reindexBoundSet ()
   {
   witAssert (myBoundSet () != NULL);

   myBoundSet ()->reindex ();
   }

//------------------------------------------------------------------------------

WitComponent::WitComponent (WitCompMgr * theCompMgr):

      WitProbAssoc (theCompMgr->myProblem ()),
      appData_     (NULL)
   {
   myPreprocessor ()->unpreprocess ();
   }

//------------------------------------------------------------------------------

void WitComponent::prepSetUnpreAttr ()
   {
   myProblem ()->prepSetUnpreAttr ();
   }

//------------------------------------------------------------------------------

void WitComponent::prepSetUnpostAttr ()
   {
   myProblem ()->prepSetUnpostAttr ();
   }

//------------------------------------------------------------------------------

void WitComponent::prepSetNonUnpostAttr ()
   {
   myProblem ()->prepSetNonUnpostAttr ();
   }

//------------------------------------------------------------------------------

void WitComponent::clearDepLists ()
   {
   witAssert (myCompMgr ()->clearingDepLists ());

   clearDepListsSpec ();
   }

//------------------------------------------------------------------------------

WitBoundSet * WitComponent::myBoundSet ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

void WitComponent::unpreprocess ()
   {
   }

//------------------------------------------------------------------------------

void WitComponent::clearOptSoln ()
   {
   }

//------------------------------------------------------------------------------

void WitComponent::clearStochSoln ()
   {
   witAssert (myStochImpMgr ()->clearingSoln ());
   }

//------------------------------------------------------------------------------

WitDelComp * WitComponent::thisDelComp ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitNode * WitComponent::thisNode ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitPart * WitComponent::thisPart ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitMaterial * WitComponent::thisMat ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitCapacity * WitComponent::thisCap ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitOperation * WitComponent::thisOpn ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitConsEntry * WitComponent::thisConsEnt ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitBomEntry * WitComponent::thisBomEnt ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitSubEntry * WitComponent::thisSub ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitBopEntry * WitComponent::thisBopEnt ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

WitDemand * WitComponent::thisDemand ()
   {
   return NULL;
   }

//------------------------------------------------------------------------------

void WitComponent::writeDataAttrs ()
   {
   }

//------------------------------------------------------------------------------

void WitComponent::copyAttrsFrom (WitComponent * theComp)
   {
   witAssert (this != theComp);
   }

//------------------------------------------------------------------------------

void WitComponent::purgeCompListsSpec ()
   {
   }

//------------------------------------------------------------------------------

void WitComponent::clearDepListsSpec ()
   {
   }

//------------------------------------------------------------------------------
// Implementation of class DelComp.
//------------------------------------------------------------------------------

WitDelComp::~WitDelComp ()
   {
   }

//------------------------------------------------------------------------------

void WitDelComp::setSelForDel (bool theValue)
   {
   prepSetNonUnpostAttr ();

   selForDel_ = theValue;
   }

//------------------------------------------------------------------------------

void WitDelComp::identifyOrAutoSel ()
   {
   WitDelComp * prereq1;
   WitDelComp * prereq2;

   witAssert (myCompMgr ()->purging ());

   if (selForDel ())
      {
      if (myMsgFac ().mayIssue ("userSelForDelMsg"))
         myMsgFac () ("userSelForDelMsg");

      identify ();

      myMsgFac ().divide ();
      }
   else
      {
      findPrereqs   (prereq1, prereq2);
      autoSelForDel (prereq1);
      autoSelForDel (prereq2);
      }
   }

//------------------------------------------------------------------------------

void WitDelComp::identify ()
   {
   if (myMsgFac ().mayIssue ("objectTypeMsg"))
      myMsgFac () ("objectTypeMsg", objTypeMsgFrag ());

   identifyInstance ();
   }

//------------------------------------------------------------------------------

WitDelComp * WitDelComp::thisDelComp ()
   {
   return this;
   }

//------------------------------------------------------------------------------

void WitDelComp::display ()
   {
   if (selForDel ())
      if (myMsgFac ().mayIssue ("selForDelDdMsg"))
         myMsgFac () ("selForDelDdMsg");
   }

//------------------------------------------------------------------------------

void WitDelComp::prepSetScenSpecAttr (const char * theAttName)
   {
   if (myStochImpMgr ()->stochMode ())
      myStochImpMgr ()->prepSetScenSpecAttr (theAttName, this);

   myProblem ()->resetSoln ();
   }

//------------------------------------------------------------------------------
// Macro implementPurgeSelComps (CompClass)
//
// Implements DelComp::purgeSelComps (List <CompClass> &).
//
// CompClass must be derived from DelComp.
//------------------------------------------------------------------------------

#define implementPurgeSelComps(CompClass)                                      \
                                                                               \
void WitDelComp::purgeSelComps (WitList <CompClass> & theList)                 \
   {                                                                           \
   CompClass * theDerComp;                                                     \
   bool        selCompFound;                                                   \
                                                                               \
   selCompFound = false;                                                       \
                                                                               \
   forEachEl (theDerComp, theList)                                             \
      if (theDerComp->selForDel ())                                            \
         selCompFound = true;                                                  \
                                                                               \
   if (not selCompFound)                                                       \
      return;                                                                  \
                                                                               \
   WitList <CompClass> purgedList;                                             \
                                                                               \
   while (not theList.isEmpty ())                                              \
      {                                                                        \
      theDerComp = theList.get ();                                             \
                                                                               \
      if (not theDerComp->selForDel ())                                        \
         purgedList.append (theDerComp);                                       \
      }                                                                        \
                                                                               \
   theList = purgedList;                                                       \
   }                                                                           \

//------------------------------------------------------------------------------

implementPurgeSelComps (WitPart)
implementPurgeSelComps (WitDemand)
implementPurgeSelComps (WitOperation)
implementPurgeSelComps (WitBomEntry)
implementPurgeSelComps (WitSubEntry)
implementPurgeSelComps (WitBopEntry)

//------------------------------------------------------------------------------

WitDelComp::WitDelComp (WitCompMgr * theCompMgr):

      WitComponent (theCompMgr),

      selForDel_   (defSelForDel ())
   {
   if (myCompMgr ()->myDelCompItr ()->active ())
      myMsgFac () ("createWhileItrActSmsg");
   }

//------------------------------------------------------------------------------

void WitDelComp::writeDataAttrs ()
   {
   WitComponent::writeDataAttrs ();

   myDataWriter ()->writeBool (
        "selForDel",
         selForDel (),
      defSelForDel ());
   }

//------------------------------------------------------------------------------

void WitDelComp::copyAttrsFrom (WitDelComp * theDelComp)
   {
   WitComponent::copyAttrsFrom (theDelComp);

   selForDel_ = theDelComp->selForDel ();
   }

//------------------------------------------------------------------------------

void WitDelComp::findPrereqs (WitDelComp * & prereq1, WitDelComp * & prereq2)
   {
   prereq1 = NULL;
   prereq2 = NULL;
   }

//------------------------------------------------------------------------------

void WitDelComp::autoSelForDel (WitDelComp * prereq)
   {
   if (prereq != NULL)
      if (prereq->selForDel ())
         if (not selForDel ())
            {
            selForDel_ = true;

            if (myMsgFac ().mayIssue ("autoSelForDelMsg"))
               myMsgFac () ("autoSelForDelMsg");

            identify ();

            if (myMsgFac ().mayIssue ("selPrereqMsg"))
               myMsgFac () ("selPrereqMsg");

            prereq->identify ();

            myMsgFac ().divide ();
            }
   }

//------------------------------------------------------------------------------
// Implementation of class Node.
//------------------------------------------------------------------------------

WitNode::~WitNode ()
   {
   }

//------------------------------------------------------------------------------

int WitNode::nInstances (WitProblem * theProblem)
   {
   return theProblem->myCompMgr ()->nNodes ();
   }

//------------------------------------------------------------------------------

void WitNode::storeCompMgrNodeName (const char * theName)
   {
   witAssert (myCompMgr ()->settingNodeName ());

   nodeName_ = theName;
   }

//------------------------------------------------------------------------------

WitNode * WitNode::thisNode ()
   {
   return this;
   }

//------------------------------------------------------------------------------

const WitString & WitNode::nodeName (const WitNode * theNode)
   {
   return theNode->nodeName ();
   }

//------------------------------------------------------------------------------

WitNode::WitNode (const char * theName, WitCompMgr * theCompMgr):

      WitDelComp (theCompMgr),

      nodeName_  (theName)
   {
   witAssert (not isNullCstring (theName));

   myProblem ()->allocTVecs ();
   }

//------------------------------------------------------------------------------

void WitNode::writeDataAttrs ()
   {
   WitDelComp::writeDataAttrs ();
   }

//------------------------------------------------------------------------------

void WitNode::copyAttrsFrom (WitNode * theNode)
   {
   WitDelComp::copyAttrsFrom (theNode);
   }
