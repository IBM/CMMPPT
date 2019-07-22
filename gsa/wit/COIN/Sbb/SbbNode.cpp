// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif
#include <string>
//#define SBB_DEBUG 1
//#define CHECK_CUT_COUNTS
#include <cassert>
#include <cfloat>
#define CUTS
#include "OsiSolverInterface.hpp"
#include "CoinWarmStartBasis.hpp"
#include "SbbModel.hpp"
#include "SbbNode.hpp"
#include "SbbBranchActual.hpp"
#include "OsiRowCut.hpp"
#include "OsiRowCutDebugger.hpp"
#include "OsiCuts.hpp"
#include "SbbCountRowCut.hpp"
#include "SbbMessage.hpp"
using namespace std;
#include "CglCutGenerator.hpp"
// Default Constructor 
SbbNodeInfo::SbbNodeInfo ()
  :
  numberPointingToThis_(0),
  parent_(NULL),
  owner_(NULL),
  numberCuts_(0),
  cuts_(NULL),
  numberRows_(0),
  numberBranchesLeft_(0)
{
#ifdef CHECK_NODE
  printf("SbbNodeInfo %x Constructor\n",this);
#endif
}
// Constructor given parent
SbbNodeInfo::SbbNodeInfo (SbbNodeInfo * parent)
  :
  numberPointingToThis_(2),
  parent_(parent),
  owner_(NULL),
  numberCuts_(0),
  cuts_(NULL),
  numberRows_(0),
  numberBranchesLeft_(2)
{
#ifdef CHECK_NODE
  printf("SbbNodeInfo %x Constructor from parent %x\n",this,parent_);
#endif
  if (parent_) {
    numberRows_ = parent_->numberRows_+parent_->numberCuts_;
  }
}
// Constructor given parent and owner
SbbNodeInfo::SbbNodeInfo (SbbNodeInfo * parent, SbbNode * owner)
  :
  numberPointingToThis_(2),
  parent_(parent),
  owner_(owner),
  numberCuts_(0),
  cuts_(NULL),
  numberRows_(0),
  numberBranchesLeft_(2)
{
#ifdef CHECK_NODE
  printf("SbbNodeInfo %x Constructor from parent %x\n",this,parent_);
#endif
  if (parent_) {
    numberRows_ = parent_->numberRows_+parent_->numberCuts_;
  }
}
SbbNodeInfo::~SbbNodeInfo()
{
#ifdef CHECK_NODE
  printf("SbbNodeInfo %x Destructor parent %x\n",this,parent_);
#endif
  assert(!numberPointingToThis_);
  int i;
  for (i=0;i<numberCuts_;i++) {
    if (cuts_[i]) {
      //printf("info %x nondel cut %d %x count %d\n",this,i,cuts_[i],
      //cuts_[i]->decrement(0));
    }
  }
  delete [] cuts_;
  if (owner_) 
    owner_->nullNodeInfo();
  if (parent_) {
    int numberLinks = parent_->decrement();
    //if (parent_==(SbbNodeInfo *) 0x806e9c8)
    //printf("this %x , parent %x, links left %d\n",
    //this,parent_,numberLinks);
    if (!numberLinks) 
      delete parent_;
  }
}
//#define ALLCUTS
void
SbbNodeInfo::decrementCuts(int change)
{
  int i;
  // get rid of all remaining if negative
  int changeThis;
  if (change<0)
    changeThis = numberBranchesLeft_;
  else
    changeThis = change;
 // decrement cut counts
  for (i=0;i<numberCuts_;i++) {
    if (cuts_[i]) {
      int number = cuts_[i]->decrement(changeThis);
      if (!number) {
	//printf("info %x del cut %d %x\n",this,i,cuts_[i]);
	delete cuts_[i];
	cuts_[i]=NULL;
      }
    }
  }
}
void
SbbNodeInfo::decrementParentCuts(int change)
{
  if (parent_) {
    // get rid of all remaining if negative
    int changeThis;
    if (change<0)
      changeThis = numberBranchesLeft_;
    else
      changeThis = change;
    int i;
    // Get over-estimate of space needed for basis
    CoinWarmStartBasis dummy;
    dummy.setSize(0,numberRows_+numberCuts_);
    buildRowBasis(dummy);
    /* everything is zero (i.e. free) so we can use to see
       if latest basis */
    SbbNodeInfo * thisInfo = parent_;
    while (thisInfo) 
      thisInfo = thisInfo->buildRowBasis(dummy);
    // decrement cut counts
    thisInfo = parent_;
    int numberRows=numberRows_;
    while (thisInfo) {
      for (i=thisInfo->numberCuts_-1;i>=0;i--) {
	CoinWarmStartBasis::Status status = dummy.getArtifStatus(--numberRows);
#ifdef ALLCUTS
	status = CoinWarmStartBasis::isFree;
#endif
	if (thisInfo->cuts_[i]) {
	  int number=1;
	  if (status!=CoinWarmStartBasis::basic) {
	    // tight - drop 1 or 2
	    if (change<0)
	      number = thisInfo->cuts_[i]->decrement(changeThis);
	    else
	      number = thisInfo->cuts_[i]->decrement(change);
	  }
	  if (!number) {
	    delete thisInfo->cuts_[i];
	    thisInfo->cuts_[i]=NULL;
	  }
	}
      }
      thisInfo = thisInfo->parent_;
    }
  }
}

void
SbbNodeInfo::incrementParentCuts(int change)
{
  if (parent_) {
    int i;
    // Get over-estimate of space needed for basis
    CoinWarmStartBasis dummy;
    dummy.setSize(0,numberRows_+numberCuts_);
    /* everything is zero (i.e. free) so we can use to see
       if latest basis */
    buildRowBasis(dummy);
    SbbNodeInfo * thisInfo = parent_;
    while (thisInfo) 
      thisInfo = thisInfo->buildRowBasis(dummy);
    // increment cut counts
    thisInfo = parent_;
    int numberRows=numberRows_;
    while (thisInfo) {
      for (i=thisInfo->numberCuts_-1;i>=0;i--) {
	CoinWarmStartBasis::Status status = dummy.getArtifStatus(--numberRows);
#ifdef ALLCUTS
	status = CoinWarmStartBasis::isFree;
#endif
	if (thisInfo->cuts_[i]&&status!=CoinWarmStartBasis::basic) {
	  thisInfo->cuts_[i]->increment(change);
	}
      }
      thisInfo = thisInfo->parent_;
    }
  }
}

// Add cuts
void
SbbNodeInfo::addCuts(OsiCuts & cuts, int numberToBranchOn,
		     int * whichGenerator)
{
  int numberCuts = cuts.sizeRowCuts();
  if (numberCuts) {
    int i;
    if (!numberCuts_) {
      cuts_ = new SbbCountRowCut * [numberCuts];
    } else {
      SbbCountRowCut ** temp = new SbbCountRowCut * [numberCuts+numberCuts_];
      memcpy(temp,cuts_,numberCuts_*sizeof(SbbCountRowCut *));
      delete [] cuts_;
      cuts_ = temp;
    }
    for (i=0;i<numberCuts;i++) {
      SbbCountRowCut * thisCut = new SbbCountRowCut(*cuts.rowCutPtr(i),
						    this,numberCuts_);
      //printf("info %x cut %d %x\n",this,i,thisCut);
      thisCut->increment(numberToBranchOn); 
      cuts_[numberCuts_++] = thisCut;
#ifdef SBB_DEBUG
      int n=thisCut->row().getNumElements();
#if SBB_DEBUG>1
      printf("Cut %d has %d entries, rhs %g %g =>",i,n,thisCut->lb(),
	     thisCut->ub());
#endif
      int j;
#if SBB_DEBUG>1
      const int * index = thisCut->row().getIndices();
#endif
      const double * element = thisCut->row().getElements();
      for (j=0;j<n;j++) {
#if SBB_DEBUG>1
	printf(" (%d,%g)",index[j],element[j]);
#endif
	assert(fabs(element[j])>1.00e-12);
      }
      printf("\n");
#endif
    }
  }
}

void
SbbNodeInfo::addCuts(int numberCuts, SbbCountRowCut ** cut, 
		     int numberToBranchOn)
{
  if (numberCuts) {
    int i;
    if (!numberCuts_) {
      cuts_ = new SbbCountRowCut * [numberCuts];
    } else {
      SbbCountRowCut ** temp = new SbbCountRowCut * [numberCuts+numberCuts_];
      memcpy(temp,cuts_,numberCuts_*sizeof(SbbCountRowCut *));
      delete [] cuts_;
      cuts_ = temp;
    }
    for (i=0;i<numberCuts;i++) {
      SbbCountRowCut * thisCut = cut[i];
      thisCut->setInfo(this,numberCuts_);
      //printf("info %x cut %d %x\n",this,i,thisCut);
      thisCut->increment(numberToBranchOn); 
      cuts_[numberCuts_++] = thisCut;
#ifdef SBB_DEBUG
      int n=thisCut->row().getNumElements();
#if SBB_DEBUG>1
      printf("Cut %d has %d entries, rhs %g %g =>",i,n,thisCut->lb(),
	     thisCut->ub());
#endif
      int j;
#if SBB_DEBUG>1
      const int * index = thisCut->row().getIndices();
#endif
      const double * element = thisCut->row().getElements();
      for (j=0;j<n;j++) {
#if SBB_DEBUG>1
	printf(" (%d,%g)",index[j],element[j]);
#endif
	assert(fabs(element[j])>1.00e-12);
      }
      printf("\n");
#endif
    }
  }
}

// delete cuts
void
SbbNodeInfo::deleteCuts(int numberToDelete, SbbCountRowCut ** cuts)
{
  int i;
  int j;
  int last=-1;
  for (i=0;i<numberToDelete;i++) {
    SbbCountRowCut * next = cuts[i];
    for (j=last+1;j<numberCuts_;j++) {
      if (next==cuts_[j])
	break;
    }
    if (j==numberCuts_) {
      // start from beginning
      for (j=0;j<last;j++) {
	if (next==cuts_[j])
	  break;
      }
      assert(j<last);
    }
    last=j;
    int number = cuts_[j]->decrement();
    if (!number)
      delete cuts_[j];
    cuts_[j]=NULL;
  }
  j=0;
  for (i=0;i<numberCuts_;i++) {
    if (cuts_[i])
      cuts_[j++]=cuts_[i];
  }
  numberCuts_ = j;
}

// delete cuts
void
SbbNodeInfo::deleteCuts(int numberToDelete, int * which)
{
  int i;
  for (i=0;i<numberToDelete;i++) {
    int iCut=which[i];
    int number = cuts_[iCut]->decrement();
    if (!number)
      delete cuts_[iCut];
    cuts_[iCut]=NULL;
  }
  int j=0;
  for (i=0;i<numberCuts_;i++) {
    if (cuts_[i])
      cuts_[j++]=cuts_[i];
  }
  numberCuts_ = j;
}

// Really delete a cut
void 
SbbNodeInfo::deleteCut(int whichOne)
{
  assert(whichOne<numberCuts_);
  cuts_[whichOne]=NULL;
}

SbbFullNodeInfo::SbbFullNodeInfo() :
  SbbNodeInfo(),
  basis_(CoinWarmStartBasis()),
  numberIntegers_(0),
  lower_(NULL),
  upper_(NULL)
{
}
SbbFullNodeInfo::SbbFullNodeInfo(SbbModel * model,
				 int numberRowsAtContinuous) :
  SbbNodeInfo()
{
  OsiSolverInterface * solver = model->solver();
  numberRows_ = numberRowsAtContinuous;
  numberIntegers_ = model->numberIntegers();
  int numberColumns = model->getNumCols();
  lower_ = new double [numberColumns];
  upper_ = new double [numberColumns];
  const double * lower = solver->getColLower();
  const double * upper = solver->getColUpper();
  int i;

  for (i=0;i<numberColumns;i++) {
    lower_[i]=lower[i];
    upper_[i]=upper[i];
  }
  const CoinWarmStartBasis* ws =
    dynamic_cast<const CoinWarmStartBasis*>(solver->getWarmStart());
  
  assert (ws!=NULL); // make sure not volume
  basis_ = CoinWarmStartBasis(*ws);
  delete ws;
}

SbbFullNodeInfo::SbbFullNodeInfo(const SbbFullNodeInfo & rhs) :
  SbbNodeInfo(rhs)
{  
  basis_=rhs.basis_;
  numberIntegers_=rhs.numberIntegers_;
  lower_=NULL;
  upper_=NULL;
  if (rhs.lower_!=NULL) {
    int numberColumns = basis_.getNumStructural();
    lower_ = new double [numberColumns];
    upper_ = new double [numberColumns];
    assert (upper_!=NULL);
    memcpy(lower_,rhs.lower_,numberColumns*sizeof(double));
    memcpy(upper_,rhs.upper_,numberColumns*sizeof(double));
  }
}

SbbNodeInfo * 
SbbFullNodeInfo::clone() const
{ 
  return (new SbbFullNodeInfo(*this));
}

SbbFullNodeInfo::~SbbFullNodeInfo ()
{
  delete [] lower_;
  delete [] upper_;
}

SbbNodeInfo * 
SbbFullNodeInfo::applyToModel(SbbModel * model,int mode,
			      CoinWarmStartBasis & basis,
			      SbbCountRowCut ** addCuts, int & currentNumberCuts ) const 
{
  OsiSolverInterface * solver = model->solver();
  if (mode) {
    // branch - do bounds
    int i;
    int numberColumns = model->getNumCols();
    for (i=0;i<numberColumns;i++) {
      solver->setColBounds( i,lower_[i],upper_[i]);
    }
    // move basis - but make sure size stays
    int numberRows = basis.getNumArtificial();
    basis=basis_;
    basis.resize(numberRows,numberColumns);
    for (i=0;i<numberCuts_;i++) 
      addCuts[currentNumberCuts+i]= cuts_[i];
  }
  currentNumberCuts += numberCuts_;
  assert(!parent_);
  return NULL;
}
/* Builds up row basis backwards (until original model).
   Returns NULL or previous one to apply .
   Depends on Free being 0 and impossible for cuts
*/
SbbNodeInfo * 
SbbFullNodeInfo::buildRowBasis(CoinWarmStartBasis & basis ) const 
{
  const unsigned int * saved = 
    (const unsigned int *) basis_.getArtificialStatus();
  unsigned int * now = 
    (unsigned int *) basis.getArtificialStatus();
  int number=basis_.getNumArtificial()>>4;;
  int i;
  for (i=0;i<number;i++) { 
    if (!now[i])
      now[i] = saved[i];
  }
  return NULL;
}
SbbPartialNodeInfo::SbbPartialNodeInfo() :
  SbbNodeInfo(),
  which_(NULL),
  basis_(NULL),
  numberChanged_(0),
  variables_(NULL),
  newBounds_(NULL),
  numberChangedBounds_(0)
{
}
// Constructor from current state 
SbbPartialNodeInfo::SbbPartialNodeInfo (SbbNodeInfo * parent,
					SbbNode * owner,
			    int numberChangedBounds,const int * variables,
			    const double * boundChanges, int numberChanged,
			    const unsigned int * whichBasis,
			    const unsigned int * changeBasis)
 : SbbNodeInfo(parent,owner)
{
  numberChanged_ = numberChanged;
  which_ = new unsigned int [numberChanged_];
  basis_ = new unsigned int [numberChanged_];
  int i;
  for (i=0;i<numberChanged_;i++) {
    which_[i]=whichBasis[i];
    basis_[i]=changeBasis[i];
  }

  numberChangedBounds_ = numberChangedBounds;
  variables_ = new int [numberChangedBounds_];
  newBounds_ = new double [numberChangedBounds_];

  for (i=0;i<numberChangedBounds_;i++) {
    variables_[i]=variables[i];
    newBounds_[i]=boundChanges[i];
  }
}

SbbPartialNodeInfo::SbbPartialNodeInfo(const SbbPartialNodeInfo & rhs) :
  SbbNodeInfo(rhs.parent_)
{  
  numberChanged_ = rhs.numberChanged_;
  which_ = new unsigned int [numberChanged_];
  basis_ = new unsigned int [numberChanged_];
  int i;
  for (i=0;i<numberChanged_;i++) {
    which_[i]=rhs.which_[i];
    basis_[i]=rhs.basis_[i];
  }

  numberChangedBounds_ = rhs.numberChangedBounds_;
  variables_ = new int [numberChangedBounds_];
  newBounds_ = new double [numberChangedBounds_];

  for (i=0;i<numberChangedBounds_;i++) {
    variables_[i]=rhs.variables_[i];
    newBounds_[i]=rhs.newBounds_[i];
  }
}

SbbNodeInfo * 
SbbPartialNodeInfo::clone() const
{ 
  return (new SbbPartialNodeInfo(*this));
}


SbbPartialNodeInfo::~SbbPartialNodeInfo ()
{
  delete [] which_;
  delete [] basis_;
  delete [] variables_;
  delete [] newBounds_;
}

SbbNodeInfo * 
SbbPartialNodeInfo::applyToModel(SbbModel * model,int mode,
				 CoinWarmStartBasis & basis,
				 SbbCountRowCut ** addCuts, int & currentNumberCuts ) const 
{
  OsiSolverInterface * solver = model->solver();
  if (mode) {
    // branch - do bounds
    int i;
    for (i=0;i<numberChanged_;i++) {
      unsigned int which = which_[i];
      unsigned int basisWord = basis_[i];
      unsigned int * update;
      if ((which&0x80000000)==0) {
	// structural
	update = (unsigned int *) basis.getStructuralStatus();
      } else {
	// row
	update = (unsigned int *) basis.getArtificialStatus();
	which &= 0x7fffffff;
      }
      update[which]=basisWord;
    }
    for (i=0;i<numberChangedBounds_;i++) {
      int variable = variables_[i];
      if ((variable&0x80000000)==0) {
	// lower bound changing
	solver->setColLower(variable,newBounds_[i]);
      } else {
	// upper bound changing
	solver->setColUpper(variable&0x7fffffff,newBounds_[i]);
      }
    }
    for (i=0;i<numberCuts_;i++) 
      addCuts[currentNumberCuts+i]= cuts_[i];
    currentNumberCuts += numberCuts_;
    return NULL;
  } else {
    currentNumberCuts += numberCuts_;
    return parent_;
  }
}
/* Builds up row basis backwards (until original model).
   Returns NULL or previous one to apply .
   Depends on Free being 0 and impossible for cuts
*/
SbbNodeInfo * 
SbbPartialNodeInfo::buildRowBasis(CoinWarmStartBasis & basis ) const 
{
  unsigned int * now = 
    (unsigned int *) basis.getArtificialStatus();
  int i;
  for (i=0;i<numberChanged_;i++) {
    unsigned int which = which_[i];
    if ((which&0x80000000)!=0) {
      // row
      which &= 0x7fffffff;
      if (!now[which])
	now[which]=basis_[i];
    }
  }
  return parent_;
}


SbbNode::SbbNode() :
  nodeInfo_(NULL),
  objectiveValue_(1.0e100),
  guessedObjectiveValue_(1.0e100),
  branch_(NULL),
  depth_(-1),
  numberUnsatisfied_(0)
{
#ifdef CHECK_NODE
  printf("SbbNode %x Constructor\n",this);
#endif
}

SbbNode::SbbNode(SbbModel * model,
		 SbbNode * lastNode) :
  nodeInfo_(NULL),
  objectiveValue_(1.0e100),
  guessedObjectiveValue_(1.0e100),
  branch_(NULL),
  depth_(-1),
  numberUnsatisfied_(0)
{
#ifdef CHECK_NODE
  printf("SbbNode %x Constructor from model\n",this);
#endif
  OsiSolverInterface * solver = model->solver();
  objectiveValue_ = solver->getObjSense()*solver->getObjValue();

  if (lastNode)
    lastNode->nodeInfo_->increment();
}
void
SbbNode::createInfo(SbbModel * model,
		 SbbNode * lastNode,
		 CoinWarmStartBasis & lastws,
		    const double * lastLower, const double * lastUpper,
		    int numberOldActiveCuts,int numberNewCuts)
{
  OsiSolverInterface * solver = model->solver();

  if (!lastNode) {
    // full basis (Continuous - no cuts)
    nodeInfo_=new SbbFullNodeInfo(model,solver->getNumRows());
  } else {
    
    const CoinWarmStartBasis* ws =
      dynamic_cast<const CoinWarmStartBasis*>(solver->getWarmStart());
    assert (ws!=NULL); // make sure not volume
    int numberArtificials = lastws.getNumArtificial();
    int numberColumns = solver->getNumCols();
    
    const double * lower = solver->getColLower();
    const double * upper = solver->getColUpper();

    int i;

    // expand basis back
    CoinWarmStartBasis expanded = *ws;
    int numberRowsAtContinuous = model->numberRowsAtContinuous();
    int iFull = numberRowsAtContinuous+model->currentNumberCuts()+
      numberNewCuts;
    int numberArtificialsNow = iFull;
    int maxBasisLength = ((iFull+15)>>4)+
      ((numberColumns+15)>>4);
    //printf("l %d full %d\n",maxBasisLength,iFull);
    expanded.resize(iFull,numberColumns);
#ifdef FULL_DEBUG
    printf("Before expansion\n");
    ws->print();
#endif
    int numberDropped = model->currentNumberCuts()-numberOldActiveCuts;
    int iCompact=iFull-numberDropped;
    SbbCountRowCut ** cut = model->addedCuts();
    int nPartial = model->currentNumberCuts()+numberRowsAtContinuous;
    iFull--;
    for (;iFull>=nPartial;iFull--) {
      CoinWarmStartBasis::Status status = ws->getArtifStatus(--iCompact);
      assert (status != CoinWarmStartBasis::basic);
      expanded.setArtifStatus(iFull,status);
    }
    for (;iFull>=numberRowsAtContinuous;iFull--) {
      if (cut[iFull-numberRowsAtContinuous]) {
	CoinWarmStartBasis::Status status = ws->getArtifStatus(--iCompact);
	assert (status != CoinWarmStartBasis::basic);
	expanded.setArtifStatus(iFull,status);
      } else {
	expanded.setArtifStatus(iFull,CoinWarmStartBasis::basic);
      }
    }
#ifdef FULL_DEBUG
    printf("Expanded basis\n");
    expanded.print();
#endif    
    // arrays for changes
    double  * boundChanges = new double [ 2*numberColumns];
    int * variables = new int [2*numberColumns];
    unsigned int * whichBasis = new unsigned int [maxBasisLength]; 
    unsigned int * changeBasis = new unsigned int [maxBasisLength]; 
    // check number of changes
    int numberChanged = 0;
    const unsigned int * old = 
      (const unsigned int *) lastws.getArtificialStatus();
    const unsigned int * now = 
      (const unsigned int *) expanded.getArtificialStatus();
    //printf("rows now %d, was %d\n",numberArtificialsNow,numberArtificials);
    int number1=(numberArtificials+15)>>4;;
    int number=(numberArtificialsNow+15)>>4;;
    assert(number>=number1);
    if (number==number1)
      number1--; // make sure all taken
    for (i=0;i<number1;i++) { 
      if (old[i]!=now[i]) {
	whichBasis[numberChanged]=i|0x80000000;
	changeBasis[numberChanged++]=now[i];
      }
    }
    for (;i<number;i++) { 
      whichBasis[numberChanged]=i|0x80000000;
      changeBasis[numberChanged++]=now[i];
    }
    number=(numberColumns+15)>>4;;
    old = (const unsigned int *) lastws.getStructuralStatus();
    now = (const unsigned int *) expanded.getStructuralStatus();
    for (i=0;i<number;i++) { 
      if (old[i]!=now[i]) {
	whichBasis[numberChanged]=i;
	changeBasis[numberChanged++]=now[i];
      }
    }
    int numberChangedBounds=0;
    for (i=0;i<numberColumns;i++) {
      if (lower[i]!=lastLower[i]) {
	variables[numberChangedBounds]=i;
	boundChanges[numberChangedBounds++]=lower[i];
      }
      if (upper[i]!=lastUpper[i]) {
	variables[numberChangedBounds]=i|0x80000000;
	boundChanges[numberChangedBounds++]=upper[i];
      }
#ifdef SBB_DEBUG
      if (lower[i]!=lastLower[i])
	printf("lower on %d changed from %g to %g\n",
	       i,lastLower[i],lower[i]);
      if (upper[i]!=lastUpper[i])
	printf("upper on %d changed from %g to %g\n",
	       i,lastUpper[i],upper[i]);
#endif
    }
#ifdef SBB_DEBUG
    printf("%d changed bounds, %d changed basis\n",numberChangedBounds,
	   numberChanged);
#endif
    assert (numberChangedBounds);
    // partial
    nodeInfo_ = new SbbPartialNodeInfo (lastNode->nodeInfo_,this,
					numberChangedBounds, variables,
					boundChanges, numberChanged,
					whichBasis,
					changeBasis);
    delete [] boundChanges;
    delete [] variables;
    delete [] whichBasis;
    delete [] changeBasis;
    delete ws;
  }
}
int
SbbNode::chooseBranch(SbbModel * model,
		      SbbNode * lastNode)
{
  numberUnsatisfied_ = 0;
  if (lastNode)
    depth_ = lastNode->depth_+1;
  else
    depth_ = 0;
  delete branch_;
  branch_=NULL;
  int bestPriority=INT_MAX;
  OsiSolverInterface * solver = model->solver();
  double objectiveValue = solver->getObjSense()*solver->getObjValue();
  const double * lower = solver->getColLower();
  const double * upper = solver->getColUpper();
  // Here could go cuts etc etc
  // For now just fix on reduced costs
  int anyAction=0;
  double integerTolerance = 
    model->getDblParam(SbbModel::SbbIntegerTolerance);
  int i;
  double mostAway=integerTolerance;
  bool beforeSolution = model->getSolutionCount()==0;
  int numberStrong=model->numberStrong();
  int numberObjects = model->numberObjects();
  int maximumStrong = max(min(model->numberStrong(),numberObjects),1);
  int numberColumns = model->getNumCols();
  double * saveUpper = new double[numberColumns];
  double * saveLower = new double[numberColumns];

  // Save solution in case heuristics need good solution later

  double * saveSolution = new double[numberColumns];
  memcpy(saveSolution,solver->getColSolution(),numberColumns*sizeof(double));
  // Get branching object

  SbbBranchDecision * decision = model->branchingMethod();
  if (!decision)
    decision = new SbbBranchDefaultDecision();
  decision->initialize(model); 

  typedef struct {
    SbbBranchingObject * possibleBranch; // what a branch would do
    double upMovement; // cost going up (and initial away from feasible)
    double downMovement; // cost going down
    int numberIntegerInfeasibilitiesUp; // without odd ones
    int numberObjectInfeasibilitiesUp; // just odd ones
    bool finishedUp; // true if solver finished
    int numberIterationsUp; // number of iterations in solver
    int numberIntegerInfeasibilitiesDown; // without odd ones
    int numberObjectInfeasibilitiesDown; // just odd ones
    bool finishedDown; // true if solver finished
    int numberIterationsDown; // number of iterations in solver
  } Strong;
  Strong * choice = new Strong[maximumStrong];
  for (i=0;i<numberColumns;i++) {
    saveLower[i] = lower[i];
    saveUpper[i] = upper[i];
  }
  // compute current state
  int numberIntegerInfeasibilities; // without odd ones
  int numberObjectInfeasibilities; // just odd ones
  model->feasibleSolution(
			   numberIntegerInfeasibilities,
			   numberObjectInfeasibilities);
  // at continuous
  //double continuousObjective = model->getContinuousObjective();
  //int continuousInfeasibilities = model->getContinuousInfeasibilities();

  // average cost to get rid of infeasibility
  //double averageCostPerInfeasibility = 
  //(objectiveValue-continuousObjective)/
  //(double) (abs(continuousInfeasibilities-numberIntegerInfeasibilities)+1);

  int iSmallest=0;
  // initialize distance from integer
  for (i=0;i<maximumStrong;i++) {
    choice[i].upMovement=0.0;
    choice[i].possibleBranch=NULL;
  }
  numberStrong=0;
  double saveOtherWay=0.0; // just for non-strong branching

  // A) Go through and choose variables furthest away from integer
  


  // Put in coding for forcePriority for hot starts
  assert(model->getForcePriority()<0);
  for (i=0;i<numberObjects;i++) {
    
    const SbbObject * object = model->object(i);
    int preferredWay;
    double otherWay;
    double infeasibility = object->infeasibility(preferredWay,otherWay);
    if (infeasibility>integerTolerance) {
      numberUnsatisfied_++;
      // deal with priorities
      int priorityLevel = model->priority(i);
      if (priorityLevel<bestPriority) {
	// reset
	int j;
	iSmallest=0;
	// initialize distance from integer
	for (j=0;j<maximumStrong;j++) {
	  choice[j].upMovement=0.0;
	  delete choice[j].possibleBranch;
	  choice[j].possibleBranch=NULL;
	}
	bestPriority = priorityLevel;
	mostAway=integerTolerance;
	numberStrong=0;
      } else if (priorityLevel>bestPriority) {
	continue;
      }
      if (infeasibility>mostAway) {
	//add to list
	choice[iSmallest].upMovement=infeasibility;
	choice[iSmallest].downMovement=otherWay;
	saveOtherWay=otherWay;
	delete choice[iSmallest].possibleBranch;
	choice[iSmallest].possibleBranch=object->createBranch(preferredWay);
	numberStrong = max(numberStrong,iSmallest+1);
	int j;
	iSmallest=-1;
	mostAway = 1.0e50;
	for (j=0;j<maximumStrong;j++) {
	  if (choice[j].upMovement<mostAway) {
	    mostAway=choice[j].upMovement;
	    iSmallest=j;
	  }
	}
      }
    }
  }
  for (i=0;i<numberStrong;i++) {
    choice[i].numberIntegerInfeasibilitiesUp=numberUnsatisfied_;
    choice[i].numberIntegerInfeasibilitiesDown=numberUnsatisfied_;
  }
  // B) If strong branching go through list of candidates
  // If not strong then simple
  // Number of strong branching candidates
  if (model->numberStrong()) {
    // Save basis
    CoinWarmStart * ws = solver->getWarmStart();
    // save limit
    int saveLimit;
    solver->getIntParam(OsiMaxNumIterationHotStart,saveLimit);
    if (beforeSolution)
      solver->setIntParam(OsiMaxNumIterationHotStart,10000); // go to end
    // Mark hot start
    solver->markHotStart();

    
    /* Go through and solve up and down.
       If one way infeasible return and solve,
       if both declare node infeasible */
    
    for (i=0;i<numberStrong;i++) {
      double objectiveChange;
      
      // try down
      choice[i].possibleBranch->way(-1); // down
      choice[i].possibleBranch->branch();
      solver->solveFromHotStart();
      // restore bounds
      int j;
      for (j=0;j<numberColumns;j++) {
	if (saveLower[j] != lower[j])
	  solver->setColLower(j,saveLower[j]);
	if (saveUpper[j] != upper[j])
	  solver->setColUpper(j,saveUpper[j]);
      }
      if ((solver->isProvenOptimal()||solver->isIterationLimitReached())
	  &&!solver->isDualObjectiveLimitReached()) {
	double newObjectiveValue = solver->getObjSense()*solver->getObjValue();
	objectiveChange = newObjectiveValue  - objectiveValue;
	choice[i].numberIterationsDown = solver->getIterationCount();
	if (solver->isProvenOptimal()) {
	  choice[i].finishedDown = true;
	  // See if integer solution
	  if (model->feasibleSolution(
			      choice[i].numberIntegerInfeasibilitiesDown,
			      choice[i].numberObjectInfeasibilitiesDown)) {
	    // integer solution - save
	    // set cutoff
	    model->setBestSolution(SBB_STRONGSOL,
				    solver->getObjSense()*solver->getObjValue(),
				    solver->getColSolution());
	    if (objectiveValue>model->getCutoff())
	      objectiveChange = 1.0e100; // say infeasible
	  }
	} else {
	  choice[i].finishedDown = false;
	}
      } else {
	objectiveChange = 1.0e100;
      }
      choice[i].downMovement=objectiveChange;
      
      // try up
      choice[i].possibleBranch->branch();
      solver->solveFromHotStart();
      // restore bounds
      for (j=0;j<numberColumns;j++) {
	if (saveLower[j] != lower[j])
	  solver->setColLower(j,saveLower[j]);
	if (saveUpper[j] != upper[j])
	  solver->setColUpper(j,saveUpper[j]);
      }
      if ((solver->isProvenOptimal()||solver->isIterationLimitReached())
	  &&!solver->isDualObjectiveLimitReached()) {
	double newObjectiveValue = solver->getObjSense()*solver->getObjValue();
	objectiveChange = newObjectiveValue  - objectiveValue;
	choice[i].numberIterationsUp = solver->getIterationCount();
	if (solver->isProvenOptimal()) {
	  choice[i].finishedUp = true;
	  // See if integer solution
	  if (model->feasibleSolution(
			      choice[i].numberIntegerInfeasibilitiesUp,
			      choice[i].numberObjectInfeasibilitiesUp)) {
	    // integer solution - save
	    // set cutoff
	    model->setBestSolution(SBB_STRONGSOL,
				    solver->getObjSense()*solver->getObjValue(),
				    solver->getColSolution());
	    if (objectiveValue>model->getCutoff())
	      objectiveChange = 1.0e100; // say infeasible
	  }
	} else {
	  choice[i].finishedUp = false;
	}
      } else {
	objectiveChange = 1.0e100;
      }
      choice[i].upMovement=objectiveChange;
      
      /* Possibilities are:
	 Both sides feasible 
	 Neither side feasible - exit
	 One side feasible - exit
      */
      if (choice[i].upMovement<1.0e100) {
	if(choice[i].downMovement<1.0e100) {
	  // feasible - no action
	} else {
	  // up feasible, down infeasible
	  anyAction=-1;
	  choice[i].possibleBranch->way(1);
	  choice[i].possibleBranch->branch();
	  break;
	}
      } else {
	if(choice[i].downMovement<1.0e100) {
	  // down feasible, up infeasible
	  anyAction=-1;
	  choice[i].possibleBranch->way(-1);
	  choice[i].possibleBranch->branch();
	  break;
	} else {
	  // neither side feasible
	  anyAction=-2;
	  break;
	}
      }
    }

    // Delete the snapshot
    solver->unmarkHotStart();
    solver->setIntParam(OsiMaxNumIterationHotStart,saveLimit);
    // restore basis
    solver->setWarmStart(ws);
    delete ws;

    if (anyAction>=0) {

      int numberNodes = model->getNodeCount();
      // get average cost per iteration and assume stopped ones
      // would stop after 50% more iterations at average cost??? !!! ???
      double averageCostPerIteration=0.0;
      double totalNumberIterations=1.0;
      int smallestNumberInfeasibilities=INT_MAX;
      for (i=0;i<numberStrong;i++) {
	totalNumberIterations += choice[i].numberIterationsDown +
	  choice[i].numberIterationsUp;
	averageCostPerIteration += choice[i].downMovement +
	  choice[i].upMovement;
	smallestNumberInfeasibilities= 
	  min(min(choice[i].numberIntegerInfeasibilitiesDown,
		  choice[i].numberIntegerInfeasibilitiesUp),
	      smallestNumberInfeasibilities);
      }
      if (smallestNumberInfeasibilities>=numberIntegerInfeasibilities)
	numberNodes=1000000; // switch off search for better solution
      numberNodes=1000000; // switch off anyway
      averageCostPerIteration /= totalNumberIterations;
      // all feasible - choose best bet
      branch_ = NULL;
      
      for (i=0;i<numberStrong;i++) {
	model->messageHandler()->message(SBB_STRONG,model->messages())
	  <<i<<choice[i].possibleBranch->variable()
	  <<choice[i].downMovement<<choice[i].numberIntegerInfeasibilitiesDown
	  <<choice[i].upMovement<<choice[i].numberIntegerInfeasibilitiesUp
	  <<choice[i].possibleBranch->value()
	  <<CoinMessageEol;
	int betterWay = decision->betterBranch(choice[i].possibleBranch,
					      branch_,
					      choice[i].upMovement,
					      choice[i].numberIntegerInfeasibilitiesUp,
					      choice[i].downMovement,
					      choice[i].numberIntegerInfeasibilitiesDown);
	if (betterWay) {
	  delete branch_;
	  // C) create branching object
	  branch_ = choice[i].possibleBranch;
	  choice[i].possibleBranch=NULL;
	  branch_->way(betterWay);
	}
      }
    }
  } else {
    // not strong
    // C) create branching object
    branch_ = choice[0].possibleBranch;
    choice[0].possibleBranch=NULL;
    // we could use otherWay - think a bit
  }
  if (!model->branchingMethod())
    delete decision;
    
  for (i=0;i<maximumStrong;i++)
    delete choice[i].possibleBranch;
  delete [] choice;
  delete [] saveLower;
  delete [] saveUpper;

  // restore solution
  solver->setColSolution(saveSolution);
  delete [] saveSolution;
  return anyAction;
}
SbbNode::SbbNode(const SbbNode & rhs) 
{  
#ifdef CHECK_NODE
  printf("SbbNode %x Constructor from rhs %x\n",this,&rhs);
#endif
  if (rhs.nodeInfo_)
    nodeInfo_ = rhs.nodeInfo_->clone();
  else
    nodeInfo_=NULL;
  objectiveValue_=rhs.objectiveValue_;
  guessedObjectiveValue_ = rhs.guessedObjectiveValue_;
  if (rhs.branch_)
    branch_=rhs.branch_->clone();
  else
    branch_=NULL,
  depth_ = rhs.depth_;
  numberUnsatisfied_ = rhs.numberUnsatisfied_;
}

SbbNode &
SbbNode::operator=(const SbbNode & rhs)
{
  if (this != &rhs) {
    delete nodeInfo_;
    if (nodeInfo_)
      nodeInfo_ = rhs.nodeInfo_->clone();
    else
      nodeInfo_ = NULL;
    objectiveValue_=rhs.objectiveValue_;
    guessedObjectiveValue_ = rhs.guessedObjectiveValue_;
    if (rhs.branch_)
      branch_=rhs.branch_->clone();
    else
      branch_=NULL,
    depth_ = rhs.depth_;
    numberUnsatisfied_ = rhs.numberUnsatisfied_;
  }
  return *this;
}


SbbNode::~SbbNode ()
{
#ifdef CHECK_NODE
  if (nodeInfo_) 
    printf("SbbNode %x Destructor nodeInfo %x (%d)\n",
	 this,nodeInfo_,nodeInfo_->numberPointingToThis());
  else
    printf("SbbNode %x Destructor nodeInfo %x (?)\n",
	 this,nodeInfo_);
#endif
  if (nodeInfo_) {
    nodeInfo_->nullOwner();
    int numberToDelete=nodeInfo_->numberBranchesLeft();
    //    SbbNodeInfo * parent = nodeInfo_->parent();
    if (nodeInfo_->decrement(numberToDelete)==0) {
      delete nodeInfo_;
    } else {
      //printf("node %x nodeinfo %x parent %x\n",this,nodeInfo_,parent);
      // anyway decrement parent
      //if (parent)
      ///parent->decrement(1);
    }
  }
  delete branch_;
}
// Decrement  active cut counts 
void 
SbbNode::decrementCuts(int change)
{
  if(nodeInfo_) {
    nodeInfo_->decrementCuts(change);
  }
}
void 
SbbNode::decrementParentCuts(int change)
{
  if(nodeInfo_) {
    nodeInfo_->decrementParentCuts(change);
  }
}

void
SbbNode::initializeInfo()
{
  nodeInfo_->initializeInfo(branch_->numberBranches());
}
// Nulls out node info
void 
SbbNode::nullNodeInfo()
{
  nodeInfo_=NULL;
}

int
SbbNode::branch()
{
  branch_->branch();
  return nodeInfo_->branchedOn();
}
