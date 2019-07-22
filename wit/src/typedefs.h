//==============================================================================
// IBM Confidential
//
// OCO Source Materials
//
// Watson Implosion Technology
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//
// The Source code for this program is not published or otherwise divested of
// its trade secrets, irrespective of what has been deposited with the U. S.
// Copyright office.
//==============================================================================

#ifndef typedefs_h
#define typedefs_h

//------------------------------------------------------------------------------
// Header file: "typedefs.h"
// Typedefs, forward declarations, and some global constants.
//------------------------------------------------------------------------------

#include <macs.h>

#include <stdio.h>
#include <string.h>
#include <math.h>

//------------------------------------------------------------------------------
// Namespace NonClass.
//
// Namespace for various names in WIT that are not for classes, but would
// otherwise be in the global namespace, e.g.:
//
//    enums
//    enum values
//    non-member functions
//
// Contributions to this namespace are made in various header files of WIT.
//------------------------------------------------------------------------------

namespace WitNonClass
   {
   }

using namespace WitNonClass;

//------------------------------------------------------------------------------
// Forward declarations.
//------------------------------------------------------------------------------

class WitAbortApiExc;
class WitAbsLink;
class WitAbsLinkMgr;
class WitAbsList;
class WitAbsObjStItr;
class WitAbsScenInputAtt;
class WitAbsScenSolnAtt;
class WitAbsStItr;
class WitAltPt;
class WitApiCall;
class WitApiMgr;
class WitAsapBaDir;
class WitAsapMeDir;
class WitAttribute;
class WitAvailSched;
class WitBaCand;
class WitBaDir;
class WitBaMat;
class WitBaMgr;
class WitBaPt;
class WitBelowLister;
class WitBillEntry;
class WitBomEntry;
class WitBopEntry;
class WitBoundPair;
class WitBoundSet;
class WitBoundedVar;
class WitCapacity;
class WitChoiceParam;
class WitCoeff;
class WitCompMgr;
class WitComponent;
class WitConsEntry;
class WitCplexMgr;
class WitCpxParSpec;
class WitCpxParSpecMgr;
class WitCumShipVar;
class WitDLSortEl;
class WitDataReader;
class WitDataRepos;
class WitDataWriter;
class WitDblFlexVec;
class WitDelComp;
class WitDelCompItr;
class WitDemand;
class WitDetAltPt;
class WitDetImpOP;
class WitDetOptImpMgr;
class WitDetVar;
class WitDoubleParam;
class WitEntity;
class WitEqAllPerf;
class WitEqAllocator;
class WitExecVar;
class WitExpRest;
class WitExtOptMgr;
class WitFSS;
class WitFeasChkr;
class WitFixedPegEl;
class WitFixedPer;
class WitFlexVAsst;
class WitFlowMonitor;
class WitFssMrpExp;
class WitGlobalComp;
class WitHeurAllMgr;
class WitHeurAtor;
class WitHeurCritList;
class WitHeurImpPerf;
class WitHeurImploder;
class WitHeurModifier;
class WitISRealArg;
class WitIVRealArg;
class WitInputID;
class WitIntParam;
class WitIssueMsgExc;
class WitMaterial;
class WitMeCand;
class WitMeDir;
class WitMeMgr;
class WitMePt;
class WitMeSitePt;
class WitMrCand;
class WitMrCoord;
class WitMrMgr;
class WitMrPt;
class WitMrSelMgr;
class WitMrSite;
class WitMsg;
class WitMsgArg;
class WitMsgFacility;
class WitMsgFrag;
class WitMsgMgr;
class WitMultiObjCon;
class WitMultiObjMgr;
class WitMultiObjVar;
class WitMultiObjVecIR;
class WitNode;
class WitNodeTable;
class WitNonSubVar;
class WitNstnBaDir;
class WitNstnMeDir;
class WitNtbvVar;
class WitOSRealArg;
class WitOTDARealArg;
class WitOVRAElem;
class WitOVRealArg;
class WitObjLink;
class WitObjLinkMgr;
class WitObjStageMgr;
class WitObjVec;
class WitObjective;
class WitOperation;
class WitOptComp;
class WitOptCon;
class WitOptProblem;
class WitOptStarter;
class WitOptVC;
class WitOptVar;
class WitOrigMrpExp;
class WitOutputDisplayer;
class WitParam;
class WitParamMgr;
class WitPart;
class WitPclBldr;
class WitPclEl;
class WitPegEl;
class WitPegger;
class WitPenExMgr;
class WitPerStageMgr;
class WitPipBuilder;
class WitPipMgr;
class WitPipPartReqMgr;
class WitPipPgg;
class WitPipReqBasis;
class WitPipReqMgr;
class WitPipReqSpec;
class WitPipSeqMgr;
class WitPostprocessor;
class WitPrAltPt;
class WitPrCoord;
class WitPrMgr;
class WitPrSelMgr;
class WitPrSelPt;
class WitPreprocessor;
class WitProblem;
class WitReqPt;
class WitReqPtMgr;
class WitReqSched;
class WitResourceCon;
class WitRtAnalyzer;
class WitRtCand;
class WitRtMgr;
class WitRtSite;
class WitRun;
class WitSaeMgr;
class WitScenAttMgr;
class WitScenGroup;
class WitScenMgr;
class WitScenario;
class WitScrapVar;
class WitSelCand;
class WitSelMgr;
class WitSelPt;
class WitSelector;
class WitSession;
class WitSglObjVecIR;
class WitSglSrcMgr;
class WitSglSrcSite;
class WitShipCon;
class WitShipVar;
class WitSlbCon;
class WitSlbvVar;
class WitSolnWriter;
class WitSplitCntr;
class WitSplitPt;
class WitSplitter;
class WitSsrCand;
class WitSsrMgr;
class WitSsrPt;
class WitStBddVar;
class WitStCumShipVar;
class WitStExecVar;
class WitStNonSubVar;
class WitStResCon;
class WitStScrapVar;
class WitStShipCon;
class WitStShipVar;
class WitStSlbvVar;
class WitStStockVar;
class WitStSubCon;
class WitStSubVar;
class WitStage;
class WitStageMgr;
class WitStochAtt;
class WitStochAttMgr;
class WitStochImpMgr;
class WitStochImpOP;
class WitStochLoc;
class WitStochModeMgr;
class WitStochOptMgr;
class WitStochVar;
class WitStockVar;
class WitString;
class WitStringParam;
class WitSubCon;
class WitSubEntry;
class WitSubVar;
class WitTbvCon;
class WitTimer;
class WitTripLink;
class WitTripLinkMgr;
class WitVariant;

//------------------------------------------------------------------------------
// Forward declaration templates.
//------------------------------------------------------------------------------

template <typename Elem>                        class WitList;
template <typename Elem>                        class WitVector;
template <typename Elem>                        class WitTVec;
template <typename Elem>                        class WitPtrVec;
template <typename Elem>                        class WitPtrVecItr;
template <typename Elem>                        class WitPtrTVec;
template <typename Elem>                        class WitRef;

template <typename Elem>                        class WitFlexVec;

template <typename DomEl,   typename RangeEl>   class WitMapping;
template <typename DomEl,   typename RangeEl>   class WitSchedule;
template <typename DomEl,   typename RangeEl>   class WitPtrMap;
template <typename DomEl,   typename RangeEl>   class WitPtrSched;

template <typename ObjByPtr>                    class WitObjStItr;
template <typename ObjByPtr>                    class WitObjStack;

template                   <typename ObjByVal>  class WitPairLink;
template                   <typename ObjByVal>  class WitPairLinkMgr;

template <typename ObjByPtr, typename ObjByVal> class WitPairStItr;
template <typename ObjByPtr, typename ObjByVal> class WitPairStack;

template <typename ObjByPtr>                    class WitTripStack;

template <typename Elem>                        class WitAbsPVecCompFtor;

template <typename DerNode>                     class WitDerNodeTable;

template <typename SrcComp>                     class WitPipAttPgg;
template <typename SrcComp, typename RcpComp>   class WitPipRcpPgg;
template                   <typename RcpComp>   class WitPipRcpFac;
template                   <typename RcpComp>   class WitPipIncPgg;
template                   <typename RcpComp>   class WitPipReqCList;
template                   <typename RcpComp>   class WitPipReqClient;

template <typename SortEntry>                   class WitAvSorter;

template <typename Entry>                       class WitExecPerSched;

template <typename Owner>                       class WitScenInputAtt;
template <typename Owner>                       class WitScenSolnAtt;
template <typename Owner>                       class WitScenAtt;

namespace std
   {
   template <class T1, class T2> struct pair;
   };

//------------------------------------------------------------------------------
// Useful constants.
//------------------------------------------------------------------------------

#define FLOAT_EPSILON 1.0E-06

//------------------------------------------------------------------------------
// NET_TOL, the tolerance used for netting and explosions.
//------------------------------------------------------------------------------

#define NET_TOL 1.0E-08

//------------------------------------------------------------------------------
// SEARCH_TOL, the tolerance used by the heuristic for search.
//------------------------------------------------------------------------------

#define SEARCH_TOL 1.0E-03

//------------------------------------------------------------------------------
// FEAS_TOL, the tolerance used for feasibility tests.
//------------------------------------------------------------------------------

#define FEAS_TOL 0.001

//------------------------------------------------------------------------------

typedef int WitPeriod;
   //
   // Used to index into arrays of size nPeriods.

typedef const char * WitAttr;
typedef const char * WitFileFormat;
typedef int          WitReturnCode;
typedef int          WitBoolean;
   //
   // Typedefs used by the API.
   // Equivalent to witAttr etc., but internal typedefs normally start with
   // upper case.

typedef const char * WitMsgID;
   //
   // Declaring a variable to be of type MsgID indicates that it is a
   // const char * intended to be used as the ID string for a Msg.

typedef const char * WitMsgFragID;
   //
   // Declaring a variable to be of type MsgFragID indicates that it is a
   // const char * intended to be used as the ID string for a MsgFrag.

//------------------------------------------------------------------------------
// Typedefs for template classes.
//------------------------------------------------------------------------------

typedef class WitObjStack  <WitRtCand>               WitRtCandStack;

typedef class WitPairStack <WitPart,      WitPeriod> WitPartPerStack;
typedef class WitPairStack <WitOperation, WitPeriod> WitOpnPerStack;
typedef class WitPairStack <WitDemand,    WitPeriod> WitDemPerStack;
typedef class WitPairStack <WitRtCand,    WitPeriod> WitRtCandPerStack;

typedef class WitPairStack <WitSelPt,        double> WitSelPtDblStack;
typedef class WitPairStack <WitSelCand,      double> WitSelCandDblStack;
typedef class WitPairStack <WitRtCand,       double> WitRtCandDblStack;
typedef class WitPairStack <WitFixedPer,     double> WitPerDblStack;

typedef class WitPairStItr <WitDemand,    WitPeriod> WitDemPerItr;
typedef class WitPairStItr <WitOperation, WitPeriod> WitOpnPerItr;

typedef class WitTripStack <WitPart>                 WitPartTripStack;
typedef class WitTripStack <WitOperation>            WitOpnTripStack;
typedef class WitTripStack <WitBopEntry>             WitBopEntTripStack;
typedef class WitTripStack <WitSubEntry>             WitSubTripStack;
typedef class WitTripStack <WitDemand>               WitDemTripStack;

typedef class WitPtrVecItr <WitCoeff>                WitCoeffItr;

typedef class WitPairStack <WitPipReqBasis,  double> WitPipReqList;
typedef class WitPairStItr <WitPipReqBasis,  double> WitPipReqListItr;
typedef class WitObjStack  <WitPipReqBasis>          WitPipReqBasisStack;
typedef class WitTripStack <WitPipReqBasis>          WitPipReqSeq;

//------------------------------------------------------------------------------

#endif
