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
#include <cmath>
#include <cfloat>

#include "OsiSolverInterface.hpp"
#include "CoinWarmStartBasis.hpp"
#include "CoinPackedMatrix.hpp"
#include "SbbCompareActual.hpp"
#include "SbbBranchActual.hpp"
#include "SbbHeuristic.hpp"
#include "SbbModel.hpp"
#include "SbbMessage.hpp"
#include "OsiRowCut.hpp"
#include "OsiColCut.hpp"
#include "OsiRowCutDebugger.hpp"
#include "OsiCuts.hpp"
#include "SbbCountRowCut.hpp"
#include "SbbCutGenerator.hpp"
// include Probing
#include "CglProbing.hpp"
// include Presolve from Clp
#include "Presolve.hpp"
// Temporary 
#include "OsiClpSolverInterface.hpp"
// Time

#include  <time.h>
#if !defined(_MSC_VER)
#include <sys/times.h>
#include <sys/resource.h>
#include <unistd.h>
#endif

static double cpuTime()
{
  double cpu_temp;
#if defined(_MSC_VER)
  unsigned int ticksnow;        /* clock_t is same as int */
  
  ticksnow = (unsigned int)clock();
  
  cpu_temp = (double)((double)ticksnow/CLOCKS_PER_SEC);
#else
  struct rusage usage;
  getrusage(RUSAGE_SELF,&usage);
  cpu_temp = usage.ru_utime.tv_sec;
  cpu_temp += 1.0e-6*((double) usage.ru_utime.tv_usec);
#endif
  return cpu_temp;
}


using namespace std;

#include <vector>

// Heap/priority queue
class tree : public std::vector <SbbNode *> {
  SbbCompare comparison_;
public:
  tree() {};
  void setComparison(SbbCompareBase  &compare) {
    comparison_.test_ = &compare;
    make_heap(begin(), end(), comparison_);
  };
  SbbNode * top() {return front();};
  void push(SbbNode * x) {
    push_back(x);
    push_heap(begin(), end(), comparison_);
  };
  void pop() {
    pop_heap(begin(), end(), comparison_);
    pop_back();
  };
  // delete all on tree with objective at least this
  void cleanTree(SbbModel * model, double cutoff)
  {
    // Do from deepest
    int j;
    int nNodes = size();
    SbbNode ** nodeArray = new SbbNode * [nNodes];
    int * depth = new int [nNodes];
    int k=0;
    int kDelete=nNodes;
    for (j=0;j<nNodes;j++) {
      SbbNode * node = top();
      pop();
      if (node->objectiveValue()>=cutoff) {
	nodeArray[--kDelete] = node;
	depth[kDelete] = node->depth();
      } else {
	nodeArray[k++]=node;
      }
    }
    for (j=0;j<k;j++) {
      push(nodeArray[j]);
    }
    CoinSort_2(depth+kDelete,depth+nNodes,nodeArray+kDelete);
    for (j=nNodes-1;j>=kDelete;j--) {
      SbbNode * node = nodeArray[j];
      CoinWarmStartBasis lastws;
      
      model->addCuts1(node, lastws);
      // Decrement cut counts 
      int numberLeft = node->nodeInfo()->numberBranchesLeft();
      int i;
      for (i=0;i<model->currentNumberCuts();i++) {
	// take off node
	CoinWarmStartBasis::Status status = 
	  lastws.getArtifStatus(i+model->numberRowsAtContinuous());
	if (status != CoinWarmStartBasis::basic&&
	    model->addedCuts()[i]) {
	  if (!model->addedCuts()[i]->decrement(numberLeft))
	    delete model->addedCuts()[i];
	}
      }
      // node should not have anything pointing to it
      node->nodeInfo()->throwAway();
      delete node;
    }
    delete [] nodeArray;
    delete [] depth;
  };
};
//-------------------------------------------------------------------
// Returns the greatest common denominator of two 
// positive integers, a and b, found using Euclid's algorithm 
//-------------------------------------------------------------------
static int gcd(int a, int b) 
{
  int remainder = -1;
  // make sure a<=b (will always remain so)
  if(a > b) {
    // Swap a and b
    int temp = a;
    a = b;
    b = temp;
  }
  // if zero then gcd is nonzero (zero may occur in rhs of packed)
  if (!a) {
    if (b) {
      return b;
    } else {
      printf("**** gcd given two zeros!!\n");
      abort();
    }
  }
  while (remainder) {
    remainder = b % a;
    b = a;
    a = remainder;
  }
  return b;
}



// Invoke enumeration algorithm
void 
SbbModel::branchAndBound() 
{
  status_ = 0;
  findIntegers(false);
  synchronizeModel(); // make sure everything that needs solver has it
  // start time
  double time1 = cpuTime();
  // solve LP
  bool feasible = resolve();

#ifdef SBB_DEBUG
  std::string problemName;
  solver_->getStrParam(OsiProbName,problemName);
  printf("Problem name - %s\n",problemName.c_str());
  solver_->activateRowCutDebugger(problemName.c_str());
#endif
  bestObjective_=1.0e50;
  numberSolutions_=0;
  numberHeuristicSolutions_=0;
  double cutoff=1.0e50;
  assert(solver_->getDblParam(OsiDualObjectiveLimit,cutoff));
  double direction = solver_->getObjSense();
  if (cutoff<1.0e20)
    cutoff *= direction; // Don't change default
  if (fabs(cutoff)>bestObjective_)
    cutoff=bestObjective_;
  solver_->setDblParam(OsiDualObjectiveLimit,cutoff);
  int iColumn;
  int numberColumns = getNumCols();
  // Get space for current solution if necessary
  if (!currentSolution_)
    currentSolution_ = new double[numberColumns];
  if (feasible) {
    // create a copy of solver
    continuousSolver_ = solver_->clone();

    const double * objective = getObjCoefficients();
    // Get increment in solutions - could do better than this
    {
      const double * lower = getColLower();
      const double * upper = getColUpper();
      double maximumCost=0.0;
      bool possibleMultiple=true;
      for (iColumn=0;iColumn<numberColumns;iColumn++) {
	if (upper[iColumn]>lower[iColumn]+1.0e-8) {
	  if( isInteger(iColumn)) 
	    maximumCost = max(maximumCost,fabs(objective[iColumn]));
	  else if (objective[iColumn]) 
	    possibleMultiple=false;
	}
      }
      if (possibleMultiple) {
	int increment=0; 
	double multiplier = 2520.0;
	while (10.0*multiplier*maximumCost<1.0e8)
	  multiplier *= 10.0;
	for (iColumn=0;iColumn<numberColumns;iColumn++) {
	  if (upper[iColumn]>lower[iColumn]+1.0e-8) {
	    if( isInteger(iColumn)&&objective[iColumn]) {
	      double value = fabs(objective[iColumn])*multiplier;
	      int nearest = (int) floor(value+0.5);
	      if (fabs(value-floor(value+0.5))>1.0e-8) {
		increment=0;
		break; // no good
	      } else if (!increment) {
		// first
		increment=nearest;
	      } else {
		increment = gcd(increment,nearest);
	      }
	    }
	  }
	}
        if (increment) {
	  double value = increment;
	  value /= multiplier;
	  if (value*0.999>dblParam_[SbbCutoffIncrement]) {
	    messageHandler()->message(SBB_INTEGERINCREMENT,messages())
	      <<value
	      <<CoinMessageEol;
	    dblParam_[SbbCutoffIncrement]=value*0.999;
	  }
	}
      }
    }
    // Space for type of cuts
    int maximumWhich=1000;
    int * whichGenerator = new int[maximumWhich];
    // save number of rows
    numberRowsAtContinuous_ = getNumRows();
    int currentNumberCuts=0;
    maximumNumberCuts_=0;
    currentNumberCuts_=0;
    delete [] addedCuts_;
    addedCuts_ = NULL;
    
    // maximum depth for tree walkback
    maximumDepth_=10;
    delete [] walkback_;
    walkback_ = new SbbNodeInfo * [maximumDepth_];
    // Whether variables fixed by strong branching
    bool resolved=false;
    
    // Stuff so can do incremental nodes
    double * lowerBefore = new double [numberColumns];
    double * upperBefore = new double [numberColumns];
    // With some thought I should be able to get rid of lastws and use basis_
    CoinWarmStartBasis lastws;
    
    // empty tree
    tree branchingTree;
    // various node selection criteria
    SbbCompareObjective compareObjective;
    SbbCompareDepth compareDepth;
    // An example of a more sophisticated criterion
    SbbCompareDefault compareDefault(dblParam_[SbbInfeasibilityWeight]);
    // Start with depth first unless user overrides
    if (!nodeCompare_)
      branchingTree.setComparison(compareDepth);
    else
      branchingTree.setComparison(*nodeCompare_);
    
    // Do root cuts etc
    SbbNode * newNode = NULL;
    // save continuous information
    SbbNode * continuousNode = new SbbNode;
    continuousNode->createInfo(this,NULL,lastws,NULL,NULL,0,0);
    continuousNode->setObjectiveValue(solver_->getObjValue());

    OsiCuts cuts;
    int anyAction=-1;
    int numberOldActiveCuts=0;
    int numberNewCuts = 0;
    // First - check if feasible
    {
      int iObject;
      int preferredWay;
      double otherWay;
      int numberUnsatisfied=0;
      double integerTolerance = 
	getDblParam(SbbModel::SbbIntegerTolerance);
      // Move solution to a safe place
      memcpy(currentSolution_,solver_->getColSolution(),
	     numberColumns*sizeof(double));
      for (iObject=0;iObject<numberObjects_;iObject++) {
	double infeasibility = object_[iObject]->infeasibility(preferredWay,
							       otherWay);
	if (infeasibility>integerTolerance) 
	  numberUnsatisfied++;
      }
      if (numberUnsatisfied) {
	feasible = solveWithCuts(cuts,maximumCutPassesAtRoot_,
				 NULL,numberOldActiveCuts,numberNewCuts,
				 maximumWhich, whichGenerator);
      }
    }
    currentNumberCuts_=numberNewCuts;
    // ** this version always adds cuts first
    if (feasible) {
      newNode = new SbbNode;
      newNode->setObjectiveValue(direction*solver_->getObjValue());
      anyAction=-1;
      while(anyAction==-1) {
	anyAction = newNode->chooseBranch(this,NULL);
	if (anyAction==-1) {
	  feasible=resolve();
	  resolved=true;
	  /*printf("Resolve as something fixed, Obj value %g %d rows\n",
		 solver_->getObjValue(),
		 solver_->getNumRows());*/
	  if (!feasible)
	    anyAction=-2;
	}
	if (anyAction==-2) {
	  // infeasible
	  delete newNode;
	  newNode=NULL;
	  feasible=false;
	}
      }
    }
    if (feasible&&newNode->variable()>=0) {
      if (resolved) {
	// cuts may have gone slack
	takeOffCuts(cuts,whichGenerator,numberOldActiveCuts,numberNewCuts);
#ifdef CHECK_CUT_COUNTS
	{
	  printf("Number of rows at end (only active cuts) %d\n",
		 numberRowsAtContinuous_+numberNewCuts+numberOldActiveCuts);
	  const CoinWarmStartBasis* ws =
	    dynamic_cast<const CoinWarmStartBasis*>(solver_->getWarmStart());
	  ws->print();
	  delete ws;
	}
#endif
      }
      newNode->createInfo(this,NULL,lastws,NULL,NULL,
			  numberOldActiveCuts,numberNewCuts);
      const CoinWarmStartBasis* ws =
	dynamic_cast<const CoinWarmStartBasis*>(solver_->getWarmStart());
      lastws = *ws;
      basis_=*ws;
      delete ws;
      // add cuts and mark as needed N times
      newNode->nodeInfo()->addCuts(cuts,newNode->numberBranches(),
				   whichGenerator);
    }     
    // Continuous data to be used later
    continuousObjective_=0.0;
    continuousInfeasibilities_=0;
    if (newNode) {
      continuousObjective_=newNode->objectiveValue();
      continuousInfeasibilities_=newNode->numberUnsatisfied();
    }
    if (0) {
      CoinPackedMatrix * rowCopy=NULL;
      rowCopy = new CoinPackedMatrix(*solver_->getMatrixByRow());
      int nRows=solver_->getNumRows(); 
      const double * lower = solver_->getRowLower();
      const double * upper = solver_->getRowUpper();
      const int * column = rowCopy->getIndices();
      const int * rowStart = rowCopy->getVectorStarts();
      const int * rowLength = rowCopy->getVectorLengths(); 
      const double * rowElements = rowCopy->getElements();
      int i;
      FILE * fp=fopen("p0033.cuts","w");
      for (i= numberRowsAtContinuous_;i<nRows;i++) {
	fprintf(fp,"%g %g %d ",lower[i],upper[i],rowLength[i]);
	int j;
	for (j=rowStart[i];j<rowStart[i]+rowLength[i];j++)
	  fprintf(fp,"%d %g ",column[j],rowElements[j]);
	fprintf(fp,"\n");
      }
      fclose(fp);
    }
    {
      // bound may have changed so reset in objects
      int i;
      for (i=0;i<numberObjects_;i++)
	object_[i]->resetBounds();
    }
    // For printing totals
    numberIterations_=0;
    numberNodes_ =0;
    bool stoppedOnGap=false;

    assert (!newNode||newNode->objectiveValue()<getCutoff());
    if (newNode) {
      if (newNode->variable()>=0) {
	newNode->initializeInfo();
	branchingTree.push(newNode);
#ifdef CHECK_NODE
        printf("Node %x on tree\n",newNode);
#endif
      } else {
	// continuous is integer
	setBestSolution(SBB_SOLUTION,newNode->objectiveValue(),
			solver_->getColSolution());
	delete newNode;
	newNode=NULL;
      }
    }

    if (printFrequency_<=0) {
      printFrequency_=1000;
      if (getNumCols()>2000)
	printFrequency_=100;
    }

    double bestValue=0.0;
    // while until nothing on stack
    while (!branchingTree.empty()) {
      // Clean tree if cutoff changed
      if (cutoff> getCutoff()) {
	// Do from deepest
	branchingTree.cleanTree(this, getCutoff());
	if (!nodeCompare_) {
	  if (!compareDefault.getWeight()) {
	    // set to get close to this
	    double costPerInteger = 
	      (bestObjective_-continuousObjective_)/
	      ((double) continuousInfeasibilities_);
	    compareDefault.setWeight(0.98*costPerInteger);
	    /*printf("Setting weight per infeasibility to %g\n",
	      0.98*costPerInteger);*/
	  }
	  branchingTree.setComparison(compareDefault);
	  //branchingTree.setComparison(compareObjective);
	} else {
	  nodeCompare_->newSolution(this);
	  nodeCompare_->newSolution(this,continuousObjective_,
				    continuousInfeasibilities_);
	  branchingTree.setComparison(*nodeCompare_);
	}
	if (branchingTree.empty())
	  break; // finished
      }
      cutoff = getCutoff();
      // allow user to get bored
      if ((numberNodes_%1000)==0&&nodeCompare_) 
	nodeCompare_->every1000Nodes( this, numberNodes_);
      if ((numberNodes_%printFrequency_)==0) {
	// Summary
	int j;
	int nNodes = branchingTree.size();
	bestValue = 1.0e100;
	for (j=0;j<nNodes;j++) {
	  SbbNode * node = branchingTree[j];
	  if (node->objectiveValue()<bestValue)
	    bestValue = node->objectiveValue();
	}
	messageHandler()->message(SBB_STATUS,messages())
	  <<numberNodes_<<nNodes<<bestObjective_<<bestValue
	  <<CoinMessageEol;
	// See if can stop on gap
	if (bestObjective_-bestValue<dblParam_[SbbAllowableGap]) {
	  stoppedOnGap=true;
	}
      }
      //#define CHECK_NODE_FULL
#ifdef CHECK_NODE_FULL
      // check tree counts okay
      {
	printf("*** CHECKING tree after %d nodes\n",numberNodes_);
	int j;
	int nNodes = branchingTree.size();
#define MAXINFO 1000
	int * count = new int [MAXINFO];
	SbbNodeInfo ** info = new SbbNodeInfo * [MAXINFO];
	int nInfo=0;
	for (j=0;j<nNodes;j++) {
	  SbbNode * node = branchingTree[j];
	  SbbNodeInfo * nodeInfo = node->nodeInfo(); 
	  while (nodeInfo) {
	    int k;
	    for (k=0;k<nInfo;k++)
	      if (nodeInfo==info[k])
		break;
	    if (k==nInfo) {
	      assert (nInfo<MAXINFO);
	      nInfo++;
	      info[k]=nodeInfo;
	      count[k]=0;
	    }
	    nodeInfo=nodeInfo->parent();
	  }
	}
	for (j=0;j<nInfo;j++) {
	  SbbNodeInfo * nodeInfo = info[j];
	  nodeInfo=nodeInfo->parent();
	  if (nodeInfo) {
	    int k;
	    for (k=0;k<nInfo;k++)
	      if (nodeInfo==info[k])
		break;
	    assert (k<nInfo);
	    count[k]++;
	  }
	}
	for (j=0;j<nInfo;j++) {
	  SbbNodeInfo * nodeInfo = info[j];
	  if (nodeInfo) {
	    int k;
	    for (k=0;k<nInfo;k++)
	      if (nodeInfo==info[k])
		break;
	    printf("Nodeinfo %x - %d left, %d count\n",
		   nodeInfo,
		   nodeInfo->numberBranchesLeft(),
		   nodeInfo->numberPointingToThis());
	    assert (nodeInfo->numberPointingToThis()==count[k]+nodeInfo->numberBranchesLeft());
	  }
	}
	delete [] count;
	delete [] info;
      }
#endif
#ifdef CHECK_CUT_COUNTS
      // check validity of cut counts on tree
      {
	printf("*** CHECKING cuts after %d nodes\n",numberNodes_);
	int j;
	int nNodes = branchingTree.size();
	for (j=0;j<nNodes;j++) {
	  SbbNode * node = branchingTree[j];
	  SbbNodeInfo * nodeInfo = node->nodeInfo(); 
	  while (nodeInfo) {
	    int k;
	    for (k=0;k<nodeInfo->numberCuts();k++) {
	      SbbCountRowCut * cut = nodeInfo->cuts()[k];
	      if (cut)
		cut->tempNumber_=0;
	    }
	    nodeInfo = nodeInfo->applyToModel(this,0,lastws,NULL,
					      currentNumberCuts);
	  }
	}
	for (j=0;j<nNodes;j++) {
	  SbbNode * node = branchingTree[j];
	  SbbNodeInfo * nodeInfo = node->nodeInfo(); 
	  printf("Node %d %x (info %x) var %d way %d obj %g",j,node,
		 node->nodeInfo(),node->variable(),node->way(),
		 node->objectiveValue());
	  int change=node->nodeInfo()->numberBranchesLeft();
	  assert(change);
	  // get cut list and basis
	  addCuts1(node,lastws);
	  int i;
	  //printf("node %x\n",node);
          //lastws.print();
	  for (i=0;i<currentNumberCuts_;i++) {
	    CoinWarmStartBasis::Status status = 
	      lastws.getArtifStatus(i+numberRowsAtContinuous_);
	    if (status != CoinWarmStartBasis::basic&&addedCuts_[i]) {
	      addedCuts_[i]->tempNumber_+=change;
	    }
	    /*if (addedCuts_[i]==(SbbCountRowCut *) 0x809c8c0)
	      printf("\nXX 0x809c8c0 node %d(%d), status %d, change %d now %d\n",
	      j,i,status,change,addedCuts_[i]->tempNumber_);*/
	  }
	  while (nodeInfo) {
	    nodeInfo = nodeInfo->applyToModel(this,0,lastws,NULL,
					      currentNumberCuts);
	    if (nodeInfo)
	      printf(" -> %x",nodeInfo);
	  }
	  printf("\n");
	}
	for (j=0;j<nNodes;j++) {
	  SbbNode * node = branchingTree[j];
	  SbbNodeInfo * nodeInfo = node->nodeInfo(); 
	  
	  while (nodeInfo) {
	    int k;
	    for (k=0;k<nodeInfo->numberCuts();k++) {
	      SbbCountRowCut * cut = nodeInfo->cuts()[k];
	      if (cut&&cut->tempNumber_>=0) {
		if (cut->tempNumber_!=cut->numberPointingToThis()) 
		  printf("mismatch %x %d %x %d %d\n",
			 nodeInfo,k,
			 cut,cut->tempNumber_,cut->numberPointingToThis());
		else
		  printf("   match %x %d %x %d %d\n",
			 nodeInfo,k,
			 cut,cut->tempNumber_,cut->numberPointingToThis());
		cut->tempNumber_=-1;
	      }
	    }
	    nodeInfo = nodeInfo->applyToModel(this,0,lastws,NULL,
					      currentNumberCuts);
	  }
	}
      }
#endif
      // last node
      SbbNode * node = branchingTree.top();
      branchingTree.pop();
      /* std::cout<<"Choosing "
	       <<node->objectiveValue()<<" "<<node->numberUnsatisfied()<<" "<<node->depth()
	       <<" "<<node->variable()<<std::endl; */
#ifdef CHECK_NODE
      printf("Node %x popped from tree - %d left, %d count\n",node,
	     node->nodeInfo()->numberBranchesLeft(),
	     node->nodeInfo()->numberPointingToThis());
#endif
      // get cuts anyway in case we want to delete them
      SbbNodeInfo * nodeInfo = node->nodeInfo();
      newNode=NULL;
      if (!addCuts(node, lastws)) {
	// node is okay
	int i;
	// now save situation for node differences
	const double * lower = getColLower();
	const double * upper = getColUpper();
	for (i=0;i<numberColumns;i++) {
	  lowerBefore[i]= lower[i];
	  upperBefore[i]= upper[i];
	}
	// do branching variable
	bool deleteNode;
	// Set bounds (and push back node if more to come)
	if ( node->branch()) {
	  branchingTree.push(node);
	  deleteNode=false;
#ifdef CHECK_NODE
	  printf("Node %x pushed back on tree - %d left, %d count\n",node,
		 nodeInfo->numberBranchesLeft(),
		 nodeInfo->numberPointingToThis());
#endif
	} else {
	  deleteNode=true;
	}
	// solve
#ifdef SBB_DEBUG
	const OsiRowCutDebugger * debugger = solver_->getRowCutDebugger();
	if (debugger) {
	  if(debugger->onOptimalPath(*solver_))
	    printf("On optimal path\n");
	  else
	    printf("Not on optimal path\n");
	}
#endif
	// empty cuts
        cuts = OsiCuts();
	currentNumberCuts = solver_->getNumRows()-
	  numberRowsAtContinuous_;
	feasible = solveWithCuts(cuts,maximumCutPasses_,node,
				 numberOldActiveCuts,numberNewCuts,
		     maximumWhich, whichGenerator);
	int anyAction;
	resolved=false;
	double totalTime = cpuTime()-time1;
	if (numberNodes_<intParam_[SbbMaxNumNode]&&
	    numberSolutions_<intParam_[SbbMaxNumSol]&&
	    totalTime<dblParam_[SbbMaximumSeconds]&&
	    !stoppedOnGap) 
	{
	  if (feasible) {
	    newNode = new SbbNode;
	    newNode->setObjectiveValue(direction*solver_->getObjValue());
	    // Do branching
	    anyAction =-1;
	    while (anyAction==-1) {
	      anyAction=newNode->chooseBranch(this,node);
	      if (anyAction==-1) {
		feasible=resolve();
		resolved=true;
		if (feasible) {
		  newNode->setObjectiveValue(direction*solver_->getObjValue());
		} else {
		  anyAction=-2;
		}
	      }
	    }
	    if (anyAction>=0) {
	      if (resolved) {
		// cuts may have gone slack
		takeOffCuts(cuts,whichGenerator,numberOldActiveCuts,numberNewCuts);
#ifdef CHECK_CUT_COUNTS
		{
		  printf("Number of rows at end (only active cuts) %d\n",
			 numberRowsAtContinuous_+numberNewCuts+numberOldActiveCuts);
		  const CoinWarmStartBasis* ws =
		    dynamic_cast<const CoinWarmStartBasis*>(solver_->getWarmStart());
		  ws->print();
		  delete ws;
		}
#endif
	      }
	      newNode->createInfo(this,node,
				  lastws,lowerBefore,upperBefore,
	      numberOldActiveCuts,numberNewCuts);
	      if (newNode->numberUnsatisfied())
		  newNode->nodeInfo()->addCuts(cuts,newNode->numberBranches(),
					       whichGenerator);
	    }
	  } else {
	    anyAction=-2;
	  }
	  if (anyAction==-2) {
	    // infeasible
	    delete newNode;
	    newNode=NULL;
	    anyAction=0;
	    // Decrement cut counts 
	    for (i=0;i<currentNumberCuts_;i++) {
	      // take off node
	      if (addedCuts_[i]) {
		if (!addedCuts_[i]->decrement(1))
		  delete addedCuts_[i];
	      }
	    }
	  } else {
	    nodeInfo->increment();
	  }
	  assert (!newNode||newNode->objectiveValue()<=getCutoff());
	  if (newNode) {
	    if (newNode->variable()>=0) {
	      handler_->message(SBB_BRANCH,messages_)
		<<numberNodes_
		<<newNode->objectiveValue()<<newNode->numberUnsatisfied()<<newNode->depth()
		<<newNode->variable()
		<<CoinMessageEol;
	      // Increment cut counts (taking off current)
	      int numberLeft = newNode->numberBranches();
	      for (i=0;i<currentNumberCuts_;i++) {
		if (addedCuts_[i]) {
#ifdef CHECK_CUT_COUNTS
		  printf("Count on cut %x increased by %d\n",addedCuts_[i],
			 numberLeft-1);
#endif
		  addedCuts_[i]->increment(numberLeft-1);
		}
	      }
	      
	      // see if we can get solution by heuristics
	      double estimatedValue=1.0e100;
	      int found=-1;
	      // double check current optimal
	      solver_->resolve();
	      double * newSolution = new double [numberColumns];
	      double heuristicValue=getCutoff();
	      int iHeuristic;
	      for (iHeuristic=0;iHeuristic<numberHeuristics_;iHeuristic++) {
		double saveValue=heuristicValue;
		int ifSol = heuristic_[iHeuristic]->solution(heuristicValue,
						    newSolution);
		if (ifSol>0) {
		  // better solution found
		  found=iHeuristic;
		} else if (ifSol<0) {
		  // just returning an estimate
		  estimatedValue = min(heuristicValue,estimatedValue);
		  heuristicValue = saveValue;
		}
	      }
	      if (found>=0) {
		// better solution save
		setBestSolution(SBB_ROUNDING,heuristicValue,
				newSolution);
	      }
	      delete [] newSolution;

	      newNode->setGuessedObjectiveValue(estimatedValue);
	      // push on stack
	      branchingTree.push(newNode);
#ifdef CHECK_NODE
	      printf("Node %x pushed on tree c\n",newNode);
#endif
	    } else {
	      // Decrement cut counts 
	      for (i=0;i<currentNumberCuts_;i++) {
		// take off node
		if (addedCuts_[i]) {
		  if (!addedCuts_[i]->decrement(1))
		    delete addedCuts_[i];
		}
	      }
	      // integer solution - save

	      // set cutoff
	      setBestSolution(SBB_SOLUTION,newNode->objectiveValue(),
				solver_->getColSolution());
              assert(nodeInfo->numberPointingToThis()<=2);
	      nodeInfo->increment();// stop deletion of node as done later
	      delete newNode;
	      nodeInfo->decrement();
	    }
	  }
	  if (deleteNode) 
	    delete node;
	} else {
	  // maximum nodes - exit
	  branchingTree.cleanTree(this, -DBL_MAX);
	  if (stoppedOnGap) {
	    messageHandler()->message(SBB_GAP,messages())
	      <<bestObjective_-bestValue
	      <<dblParam_[SbbAllowableGap]
	      <<CoinMessageEol;
	    status_ = 0;
	  } else if (isNodeLimitReached()) {
	    handler_->message(SBB_MAXNODES,messages_)
	      <<CoinMessageEol;
	    status_ = 1;  
	  } else if (totalTime<dblParam_[SbbMaximumSeconds])
	  {
		  handler_->message(SBB_MAXTIME,messages_)
	      <<CoinMessageEol;

	  }
	  else{
	    handler_->message(SBB_MAXSOLS,messages_)
	      <<CoinMessageEol;
	    status_ = 1;  
	  }
	  break;
	}
	// take off rows 
	int numberToDelete = getNumRows() - numberRowsAtContinuous_;
	if (numberToDelete) {
	  int * delRows = new int[numberToDelete];
	  int i;
	  for (i=0;i<numberToDelete;i++) {
	    delRows[i]=i+numberRowsAtContinuous_;
	  }
	  solver_->deleteRows(numberToDelete,delRows);
	  delete [] delRows;
	}
      } else {
	// throw away all node information
	delete node;
      }
    }
    delete [] whichGenerator;
    if (!status_)
      handler_->message(SBB_END_GOOD,messages_)
	<<bestObjective_
	<<numberIterations_
	<<numberNodes_
	<<CoinMessageEol;
    else
      handler_->message(SBB_END,messages_)
	<<numberIterations_
	<<numberNodes_
	<<CoinMessageEol;
    if (bestSolution_) {
      // we have a solution - restore
      // make sure continuous solution okay
      setBestSolution(SBB_SOLUTION,bestObjective_,bestSolution_,true);
      delete solver_;
      solver_=continuousSolver_->clone();

      // reset cutoff
      setCutoff(bestObjective_+max(1.0e-3,1.0e-7*fabs(bestObjective_)));
      solver_->resolve();
      if (!solver_->isProvenOptimal()) {
	solver_->messageHandler()->setLogLevel(2);
	solver_->initialSolve();
      }
      //printf("Obj value after fixing to values %g status %s\n",
      //     solver_->getObjValue(), 
      //     (solver_->isProvenOptimal() ? "Optimal" : "Odd status"));
    }
    delete continuousNode;
    delete [] lowerBefore;
    delete [] upperBefore;
    delete [] walkback_;
    walkback_ = NULL;
    delete [] addedCuts_;
    addedCuts_=NULL;
    delete continuousSolver_;
    continuousSolver_=NULL;
  } else {
    handler_->message(SBB_INFEAS,messages_)
    <<CoinMessageEol;
    status_ = 1;
  }
  // destroy global cuts
  globalCuts_= OsiCuts();
}
// Solve initial LP relaxation 
void 
SbbModel::initialSolve() 
{
  assert (solver_);
  solver_->initialSolve();
}
// Default Constructor
SbbModel::SbbModel() 

:
  solver_(NULL),
  continuousSolver_(NULL),
  defaultHandler_(true),
  basis_(),
  bestObjective_(DBL_MAX),
  bestSolution_(NULL),
  currentSolution_(NULL),
  minimumDrop_(1.0e-4),
  numberSolutions_(0),
  forcePriority_(-1),
  numberHeuristicSolutions_(0),
  numberNodes_(0),
  numberIterations_(0),
  status_(0),
  numberIntegers_(0),
  numberRowsAtContinuous_(0),
  maximumNumberCuts_(0),
  currentNumberCuts_(0),
  maximumDepth_(0),
  walkback_(NULL),
  addedCuts_(NULL),
  integerVariable_(NULL),
  strategy_(0),
  numberStrong_(5),
  printFrequency_(0),
  numberCutGenerators_(0),
  generator_(NULL),
  numberHeuristics_(0),
  heuristic_(NULL),
  numberObjects_(0),
  object_(NULL),
  originalColumns_(NULL),
  priority_(NULL),
  howOftenGlobalScan_(1),
  continuousObjective_(DBL_MAX),
  continuousInfeasibilities_(INT_MAX),
  maximumCutPassesAtRoot_(20),
  maximumCutPasses_(10)
{
  intParam_[SbbMaxNumNode] = 9999999;
  intParam_[SbbMaxNumSol] = 9999999;

  dblParam_[SbbIntegerTolerance] = 1e-6;
  dblParam_[SbbInfeasibilityWeight] = 0.0;
  dblParam_[SbbCutoffIncrement] = 1e-5;
  dblParam_[SbbAllowableGap] = 1.0e-10;
  dblParam_[SbbMaximumSeconds] = 1.0e100;
  nodeCompare_=NULL;
  branchingMethod_=NULL;

  handler_ = new CoinMessageHandler();
  handler_->setLogLevel(2);
  messages_ = SbbMessage();
}

// Constructor from solver
SbbModel::SbbModel(const OsiSolverInterface &rhs)
:
  continuousSolver_(NULL),
  defaultHandler_(true),
  basis_(),
  bestObjective_(DBL_MAX),
  minimumDrop_(1.0e-4),
  numberSolutions_(0),
  forcePriority_(-1),
  numberHeuristicSolutions_(0),
  numberNodes_(0),
  numberIterations_(0),
  status_(0),
  numberRowsAtContinuous_(0),
  maximumNumberCuts_(0),
  currentNumberCuts_(0),
  maximumDepth_(0),
  walkback_(NULL),
  addedCuts_(NULL),
  strategy_(0),
  numberStrong_(5),
  printFrequency_(0),
  numberCutGenerators_(0),
  generator_(NULL),
  numberHeuristics_(0),
  heuristic_(NULL),
  numberObjects_(0),
  object_(NULL),
  originalColumns_(NULL),
  priority_(NULL),
  howOftenGlobalScan_(-1),
  continuousObjective_(DBL_MAX),
  continuousInfeasibilities_(INT_MAX),
  maximumCutPassesAtRoot_(20),
  maximumCutPasses_(10)
{
  intParam_[SbbMaxNumNode] = 9999999;
  intParam_[SbbMaxNumSol] = 9999999;

  dblParam_[SbbIntegerTolerance] = 1e-6;
  dblParam_[SbbInfeasibilityWeight] = 0.0;
  dblParam_[SbbCutoffIncrement] = 1e-5;
  dblParam_[SbbAllowableGap] = 1.0e-10;
  dblParam_[SbbMaximumSeconds] = 1.0e100;

  nodeCompare_=NULL;
  branchingMethod_=NULL;

  handler_ = new CoinMessageHandler();
  handler_->setLogLevel(2);
  messages_ = SbbMessage();
  solver_ = rhs.clone();
  // Fill integer region
  numberIntegers_=0;
  int numberColumns = solver_->getNumCols();
  bestSolution_ = NULL; // to say no solution found
  int iColumn;
  if (numberColumns) {
    // Space for current solution
    currentSolution_ = new double[numberColumns];
    for (iColumn=0;iColumn<numberColumns;iColumn++) {
      if( solver_->isInteger(iColumn)) 
	numberIntegers_++;
      
    }
  } else {
    // empty model
    currentSolution_=NULL;
  }
  if (numberIntegers_) {
    integerVariable_ = new int [numberIntegers_];
    numberIntegers_=0;
    for (iColumn=0;iColumn<numberColumns;iColumn++) {
      if( solver_->isInteger(iColumn)) 
	integerVariable_[numberIntegers_++]=iColumn;
    
    }
  } else {
    integerVariable_ = NULL;
  }
}

// assign solver
void
SbbModel::assignSolver(OsiSolverInterface * solver)
{
  solver_ = solver;
  // Fill integer region
  numberIntegers_=0;
  int numberColumns = solver_->getNumCols();
  int iColumn;
  for (iColumn=0;iColumn<numberColumns;iColumn++) {
    if( solver_->isInteger(iColumn)) 
      numberIntegers_++;
    
  }
  if (numberIntegers_) {
    integerVariable_ = new int [numberIntegers_];
    numberIntegers_=0;
    for (iColumn=0;iColumn<numberColumns;iColumn++) {
      if( solver_->isInteger(iColumn)) 
	integerVariable_[numberIntegers_++]=iColumn;
    
    }
  } else {
    integerVariable_ = NULL;
  }
  abort(); // need ownership flag to stop delete at end
}

//  
SbbModel::SbbModel(const SbbModel & rhs)
:
  continuousSolver_(NULL),
  defaultHandler_(rhs.defaultHandler_),
  basis_(rhs.basis_),
  bestObjective_(rhs.bestObjective_),
  minimumDrop_(rhs.minimumDrop_),
  numberSolutions_(rhs.numberSolutions_),
  forcePriority_(rhs.forcePriority_),
  numberHeuristicSolutions_(rhs.numberHeuristicSolutions_),
  numberNodes_(rhs.numberNodes_),
  numberIterations_(rhs.numberIterations_),
  status_(rhs.status_),
  strategy_(rhs.strategy_),
  numberStrong_(rhs.numberStrong_),
  printFrequency_(rhs.printFrequency_),
  howOftenGlobalScan_(rhs.howOftenGlobalScan_),
  continuousObjective_(rhs.continuousObjective_),
  continuousInfeasibilities_(rhs.continuousInfeasibilities_),
  maximumCutPassesAtRoot_(rhs.maximumCutPassesAtRoot_),
  maximumCutPasses_( rhs.maximumCutPasses_)
{
  intParam_[SbbMaxNumNode] = rhs.intParam_[SbbMaxNumNode];
  intParam_[SbbMaxNumSol] = rhs.intParam_[SbbMaxNumSol];
  dblParam_[SbbIntegerTolerance] = rhs.dblParam_[SbbIntegerTolerance];
  dblParam_[SbbInfeasibilityWeight] = rhs.dblParam_[SbbInfeasibilityWeight];
  dblParam_[SbbCutoffIncrement] = rhs.dblParam_[SbbCutoffIncrement]; 
  dblParam_[SbbAllowableGap] = rhs.dblParam_[SbbAllowableGap]; 
  dblParam_[SbbMaximumSeconds] = rhs.dblParam_[SbbMaximumSeconds];
  if (defaultHandler_) {
    handler_ = new CoinMessageHandler();
    handler_->setLogLevel(2);
  } else {
    handler_ = rhs.handler_;
  }
  messageHandler()->setLogLevel(rhs.messageHandler()->logLevel());
  numberCutGenerators_ = rhs.numberCutGenerators_;
  if (numberCutGenerators_) {
    generator_ = new SbbCutGenerator * [numberCutGenerators_];
    int i;
    for (i=0;i<numberCutGenerators_;i++) {
      generator_[i]=new SbbCutGenerator(*rhs.generator_[i]);
    }
  } else {
    generator_=NULL;
  }
  globalCuts_ = rhs.globalCuts_;
  numberHeuristics_ = rhs.numberHeuristics_;
  if (numberHeuristics_) {
    heuristic_ = new SbbHeuristic * [numberHeuristics_];
    memcpy(heuristic_,rhs.heuristic_,
	   numberHeuristics_*sizeof(SbbHeuristic *));
  } else {
    heuristic_=NULL;
  }
  numberObjects_=rhs.numberObjects_;
  if (numberObjects_) {
    object_ = new SbbObject * [numberObjects_];
    int i;
    for (i=0;i<numberObjects_;i++) 
      object_[i]=(rhs.object_[i])->clone();
  } else {
    object_=NULL;
  }
  if (rhs.priority_) {
    priority_= new int [numberObjects_];
    memcpy(priority_,rhs.priority_,numberObjects_*sizeof(int));
  } else {
    priority_=NULL;
  }
  if (rhs.originalColumns_) {
    int numberColumns = solver_->getNumCols();
    originalColumns_= new int [numberColumns];
    memcpy(originalColumns_,rhs.originalColumns_,numberColumns*sizeof(int));
  } else {
    originalColumns_=NULL;
  }
  nodeCompare_=rhs.nodeCompare_;
  branchingMethod_=rhs.branchingMethod_;
  messages_ = rhs.messages_;
  solver_ = rhs.solver_->clone();
  messageHandler()->setLogLevel(rhs.messageHandler()->logLevel());
  numberIntegers_=rhs.numberIntegers_;
  if (numberIntegers_) {
    integerVariable_ = new int [numberIntegers_];
    memcpy(integerVariable_,rhs.integerVariable_,numberIntegers_*sizeof(int));
  } else {
    integerVariable_ = NULL;
  }
  if (rhs.bestSolution_) {
    int numberColumns = solver_->getNumCols();
    bestSolution_ = new double[numberColumns];
    memcpy(bestSolution_,rhs.bestSolution_,numberColumns*sizeof(double));
  } else {
    bestSolution_=NULL;
  }
  if (rhs.currentSolution_) {
    int numberColumns = solver_->getNumCols();
    currentSolution_ = new double[numberColumns];
    memcpy(currentSolution_,rhs.currentSolution_,numberColumns*sizeof(double));
  } else {
    currentSolution_=NULL;
  }
  numberRowsAtContinuous_ = rhs.numberRowsAtContinuous_;
  maximumNumberCuts_=rhs.maximumNumberCuts_;
  currentNumberCuts_=rhs.currentNumberCuts_;
  maximumDepth_= rhs.maximumDepth_;
  // These are only used as temporary arrays so need not be filled
  if (maximumNumberCuts_) {
    addedCuts_ = new SbbCountRowCut * [maximumNumberCuts_];
  } else {
    addedCuts_ = NULL;
  }
  if (maximumDepth_)
    walkback_ = new SbbNodeInfo * [maximumDepth_];
  else
    walkback_ = NULL;
  synchronizeModel();
}
  
// Assignment operator 
SbbModel & 
SbbModel::operator=(const SbbModel& rhs)
{
  if (this!=&rhs) {
    if (defaultHandler_) {
      delete handler_;
      handler_ = NULL;
    }
    delete solver_;
    continuousSolver_=NULL;
    basis_ = rhs.basis_;
    bestObjective_ = rhs.bestObjective_;
    delete [] bestSolution_;
    if (rhs.bestSolution_) {
      int numberColumns = solver_->getNumCols();
      bestSolution_ = new double[numberColumns];
      memcpy(bestSolution_,rhs.bestSolution_,numberColumns*sizeof(double));
    } else {
      bestSolution_=NULL;
    }
    if (rhs.currentSolution_) {
      int numberColumns = solver_->getNumCols();
      currentSolution_ = new double[numberColumns];
      memcpy(currentSolution_,rhs.currentSolution_,numberColumns*sizeof(double));
    } else {
      currentSolution_=NULL;
    }
    minimumDrop_ = rhs.minimumDrop_;
    numberSolutions_=rhs.numberSolutions_;
    forcePriority_=rhs.forcePriority_;
    numberHeuristicSolutions_=rhs.numberHeuristicSolutions_;
    numberNodes_ = rhs.numberNodes_;
    numberIterations_ = rhs.numberIterations_;
    status_ = rhs.status_;
    strategy_ = rhs.strategy_;
    numberStrong_ = rhs.numberStrong_;
    printFrequency_ = rhs.printFrequency_;
    howOftenGlobalScan_=rhs.howOftenGlobalScan_;
    continuousObjective_=rhs.continuousObjective_;
    continuousInfeasibilities_ = rhs.continuousInfeasibilities_;
    maximumCutPassesAtRoot_ = rhs.maximumCutPassesAtRoot_;
    maximumCutPasses_ = rhs.maximumCutPasses_;
    defaultHandler_ = rhs.defaultHandler_;
    intParam_[SbbMaxNumNode] = rhs.intParam_[SbbMaxNumNode];
    intParam_[SbbMaxNumSol] = rhs.intParam_[SbbMaxNumSol];
    dblParam_[SbbIntegerTolerance] = rhs.dblParam_[SbbIntegerTolerance];
    dblParam_[SbbInfeasibilityWeight] = rhs.dblParam_[SbbInfeasibilityWeight];
    dblParam_[SbbCutoffIncrement] = rhs.dblParam_[SbbCutoffIncrement]; 
    dblParam_[SbbAllowableGap] = rhs.dblParam_[SbbAllowableGap]; 
    dblParam_[SbbMaximumSeconds] = rhs.dblParam_[SbbMaximumSeconds];
    if (defaultHandler_) {
      handler_ = new CoinMessageHandler();
      handler_->setLogLevel(2);
    } else {
      handler_ = rhs.handler_;
    }
    messageHandler()->setLogLevel(rhs.messageHandler()->logLevel());
    globalCuts_ = rhs.globalCuts_;
    int i;
    for (i=0;i<numberCutGenerators_;i++)
      delete generator_[i];
    delete [] generator_;
    delete [] heuristic_;
    numberCutGenerators_ = rhs.numberCutGenerators_;
    if (numberCutGenerators_) {
      generator_ = new SbbCutGenerator * [numberCutGenerators_];
      int i;
      for (i=0;i<numberCutGenerators_;i++) {
	generator_[i]=new SbbCutGenerator(*rhs.generator_[i]);
      }
    } else {
      generator_=NULL;
    }
    numberHeuristics_ = rhs.numberHeuristics_;
    if (numberHeuristics_) {
      heuristic_ = new SbbHeuristic * [numberHeuristics_];
      memcpy(heuristic_,rhs.heuristic_,
	     numberHeuristics_*sizeof(SbbHeuristic *));
    } else {
      heuristic_=NULL;
    }
    for (i=0;i<numberObjects_;i++)
      delete object_[i];
    delete [] object_;
    numberObjects_=rhs.numberObjects_;
    if (numberObjects_) {
      object_ = new SbbObject * [numberObjects_];
      int i;
      for (i=0;i<numberObjects_;i++) 
	object_[i]=(rhs.object_[i])->clone();
    } else {
      object_=NULL;
    }
    delete [] priority_;
    if (rhs.priority_) {
      priority_= new int [numberObjects_];
      memcpy(priority_,rhs.priority_,numberObjects_*sizeof(int));
    } else {
      priority_=NULL;
    }
    delete [] originalColumns_;
    if (rhs.originalColumns_) {
      int numberColumns = solver_->getNumCols();
      originalColumns_= new int [numberColumns];
      memcpy(originalColumns_,rhs.originalColumns_,numberColumns*sizeof(int));
    } else {
      originalColumns_=NULL;
    }
    nodeCompare_=rhs.nodeCompare_;
    branchingMethod_=rhs.branchingMethod_;
    messages_ = rhs.messages_;
    solver_ = rhs.solver_->clone();
    delete [] integerVariable_;
    numberIntegers_=rhs.numberIntegers_;
    if (numberIntegers_) {
      integerVariable_ = new int [numberIntegers_];
      memcpy(integerVariable_,rhs.integerVariable_,
	     numberIntegers_*sizeof(int));
    } else {
      integerVariable_ = NULL;
    }
    numberRowsAtContinuous_ = rhs.numberRowsAtContinuous_;
    maximumNumberCuts_=rhs.maximumNumberCuts_;
    currentNumberCuts_=rhs.currentNumberCuts_;
    maximumDepth_= rhs.maximumDepth_;
    delete [] addedCuts_;
    delete [] walkback_;
    // These are only used as temporary arrays so need not be filled
    if (maximumNumberCuts_) {
      addedCuts_ = new SbbCountRowCut * [maximumNumberCuts_];
    } else {
      addedCuts_ = NULL;
    }
    if (maximumDepth_)
      walkback_ = new SbbNodeInfo * [maximumDepth_];
    else
      walkback_ = NULL;
    synchronizeModel();
  }
  return *this;
}
  
// Destructor 
SbbModel::~SbbModel ()
{
  if (defaultHandler_) {
    delete handler_;
    handler_ = NULL;
  }
  delete solver_;
  delete [] bestSolution_;
  delete [] currentSolution_;
  delete [] integerVariable_;
  int i;
  for (i=0;i<numberCutGenerators_;i++)
    delete generator_[i];
  delete [] generator_;
  delete [] heuristic_;
  delete [] addedCuts_;
  delete [] walkback_;
  for (i=0;i<numberObjects_;i++)
    delete object_[i];
  delete [] object_;
  delete [] priority_;
  delete [] originalColumns_;
  delete continuousSolver_;
}
// Are there a numerical difficulties?
bool 
SbbModel::isAbandoned() const
{
  return status_ == 2;
}
// Is optimality proven?
bool 
SbbModel::isProvenOptimal() const
{
  if (!status_ && bestObjective_<1.0e30)
    return true;
  else
    return false;
}
// Is  infeasiblity proven (or none better than cutoff)?
bool 
SbbModel::isProvenInfeasible() const
{
  if (!status_ && bestObjective_==1.0e30)
    return true;
  else
    return false;
}
// Node limit reached?
bool 
SbbModel::isNodeLimitReached() const
{
  return numberNodes_>=intParam_[SbbMaxNumNode];
}
// Solution limit reached?
bool 
SbbModel::isSolutionLimitReached() const
{
  return numberSolutions_>=intParam_[SbbMaxNumSol];
}
// Set language
void 
SbbModel::newLanguage(CoinMessages::Language language)
{
  messages_ = SbbMessage(language);
}
void 
SbbModel::setNumberStrong(int number)
{
  if (number<0)
    numberStrong_=0;
   else
    numberStrong_=number;
}
// Add one generator
void 
SbbModel::addCutGenerator(CglCutGenerator * generator,
			  int howOften, const char * name,
			  bool normal, bool atSolution,
			  bool whenInfeasible)
{
  SbbCutGenerator ** temp = generator_;
  generator_ = new SbbCutGenerator * [numberCutGenerators_+1];
  memcpy(generator_,temp,numberCutGenerators_*sizeof(SbbCutGenerator *));
  generator_[numberCutGenerators_++]= 
    new SbbCutGenerator(this,generator, howOften, name,
			normal,atSolution,whenInfeasible);
							  
}
// Add one heuristic
void 
SbbModel::addHeuristic(SbbHeuristic * generator)
{
  SbbHeuristic ** temp = heuristic_;
  heuristic_ = new SbbHeuristic * [numberHeuristics_+1];
  memcpy(heuristic_,temp,numberHeuristics_*sizeof(SbbHeuristic *));
  delete [] temp;
  heuristic_[numberHeuristics_++]=generator;
}
void SbbModel::addCuts1(SbbNode * node, 
		      CoinWarmStartBasis & lastws)
{
  // get cuts anyway so we can delete
  int i;
  int nNode=0;
  int numberColumns = getNumCols();
  SbbNodeInfo * nodeInfo = node->nodeInfo();
  // take off cuts - see note below for later efficiency
  int currentNumberCuts=solver_->getNumRows()-numberRowsAtContinuous_;
  int * which = new int[currentNumberCuts];
  for (i=0;i<currentNumberCuts;i++)
    which[i]=i+numberRowsAtContinuous_;
  solver_->deleteRows(currentNumberCuts,which);
  delete [] which;
  currentNumberCuts=0;
  //assert(solver_->getNumRows()==numberRowsAtContinuous_);
  while (nodeInfo) {
    /* when working then just unwind until where new node
       joins old node (for cuts?) */
    //printf("nNode = %d, nodeInfo = %x\n",nNode,nodeInfo);
    walkback_[nNode++]=nodeInfo;
    nodeInfo = nodeInfo->applyToModel(this,0,lastws,NULL,
				      currentNumberCuts);
    if (nNode==maximumDepth_) {
      maximumDepth_ *= 2;
      SbbNodeInfo ** temp = new SbbNodeInfo * [maximumDepth_];
      for (i=0;i<nNode;i++) 
	temp[i] = walkback_[i];
      delete [] walkback_;
      walkback_ = temp;
    }
  }
  currentNumberCuts_=currentNumberCuts;
  nodeInfo = node->nodeInfo();
  if (currentNumberCuts>=maximumNumberCuts_) {
    maximumNumberCuts_ = currentNumberCuts;
    delete [] addedCuts_;
    addedCuts_ = new SbbCountRowCut * [maximumNumberCuts_];
  }
  // make sure basis large enough
  lastws.setSize(numberColumns,
		 numberRowsAtContinuous_ + currentNumberCuts);
  currentNumberCuts=0;
  //int saveNumberNodes =nNode;
  while (nNode) {
    --nNode;
    walkback_[nNode]->applyToModel(this,1,lastws,addedCuts_,
				      currentNumberCuts);
  }
}
int SbbModel::addCuts(SbbNode * node, 
		      CoinWarmStartBasis & lastws)
{
  // get cuts anyway so we can delete
  addCuts1(node,lastws);
  int i;
  int numberColumns = getNumCols();
  SbbNodeInfo * nodeInfo = node->nodeInfo();
  double cutoff = bestObjective_-dblParam_[SbbCutoffIncrement];
  int currentNumberCuts=currentNumberCuts_;
  if (node->objectiveValue() < cutoff) {
    int numberToAdd = 0;
    OsiRowCut * addCuts;
    if ( currentNumberCuts==0 ) addCuts = NULL;
    else addCuts = new OsiRowCut [currentNumberCuts];
    // collapse basis and cuts 
#ifdef CHECK_CUT_COUNTS
    printf("Number of rows (including inactive cuts) %d\n",
	   numberRowsAtContinuous_+currentNumberCuts);
    lastws.print();
#endif
    for (i=0;i<currentNumberCuts;i++) {
      CoinWarmStartBasis::Status status = 
	lastws.getArtifStatus(i+numberRowsAtContinuous_);
      if (status != CoinWarmStartBasis::basic&&addedCuts_[i]) {
#ifdef CHECK_CUT_COUNTS
	printf("Using cut %d %x as row %d\n",i,addedCuts_[i],
	       numberRowsAtContinuous_+numberToAdd);
#endif
	lastws.setArtifStatus(numberToAdd+numberRowsAtContinuous_,status);
	addCuts[numberToAdd++]= OsiRowCut (*addedCuts_[i]);
      } else {
#ifdef CHECK_CUT_COUNTS
	printf("Dropping cut %d %x\n",i,addedCuts_[i]);
#endif
	addedCuts_[i]=NULL;
      }
    }
#if 0
    int numberRowsNow=numberRowsAtContinuous_+numberToAdd;
    basis_.setSize(numberColumns,
		   numberRowsNow);
    memcpy(basis_.getStructuralStatus(),lastws.getStructuralStatus(),
	   ((numberColumns+3)>>2)*sizeof(char));
#else
    int numberRowsNow=numberRowsAtContinuous_+numberToAdd;
    lastws.resize(numberRowsNow,numberColumns);
    basis_=lastws;
#ifdef FULL_DEBUG
    printf("B after addCuts1\n");
    basis_.print();
#endif
    solver_->setWarmStart(&lastws);
#endif
#if 0
    if ((numberNodes_%printFrequency_)==0) {
      printf("Objective %g, depth %d, unsatisfied %d\n",
	     node->objectiveValue(),
	     node->depth(),node->numberUnsatisfied());
    }
#endif
    // apply cuts
    solver_->applyRowCuts(numberToAdd,addCuts);
    delete [] addCuts;
    numberNodes_++;
    return 0;
  } else {
#if 1
    // take off cuts for remaining branches for this node
    int i;
    int numberLeft = nodeInfo->numberBranchesLeft();
    for (i=0;i<currentNumberCuts;i++) {
      // take off node
      if (addedCuts_[i]) {
	if (!addedCuts_[i]->decrement(numberLeft)) {
	  delete addedCuts_[i];
	  addedCuts_[i]=NULL;
	}
      }
    }
#endif
    return 1;
  }
}
// Solve with cuts
// This version takes off redundant cuts from node
// Returns true if feasible
bool 
SbbModel::solveWithCuts(OsiCuts & cuts, int numberTries,SbbNode * node,
			int & numberOldActiveCuts, int & numberNewCuts,
		     int & maximumWhich, int * & whichGenerator)
{
  bool feasible=true;
  int lastNumberCuts=0;
  numberNewCuts = 0;
  double lastObjective = -1.0e100;
  int violated=0;
  int numberRowsAtStart = solver_->getNumRows();
  numberOldActiveCuts = numberRowsAtStart-numberRowsAtContinuous_;
  int numberColumns = solver_->getNumCols();
#ifdef SBB_DEBUG
  bool onOptimalPath=false;
  const OsiRowCutDebugger * debugger = solver_->getRowCutDebugger();
  if (debugger) 
    onOptimalPath = (debugger->onOptimalPath(*solver_));

#endif
  // solve problem
  feasible=resolve();
  double direction = solver_->getObjSense();
  double startObjective = solver_->getObjValue()*direction;
  int * countColumnCuts = NULL;
  int * countRowCuts = NULL;
  // If numberTries is negative - ignore minimum drop
  double minimumDrop=minimumDrop_;
  if (numberTries<0) {
    numberTries=-numberTries;
    minimumDrop=-1.0;
  }
#define SCANCUTS 100  
  bool fullScan=false;
  if ((numberNodes_%SCANCUTS)==0) {
    fullScan=true;
    countColumnCuts = new int[numberCutGenerators_+numberHeuristics_];
    countRowCuts = new int[numberCutGenerators_+numberHeuristics_];
    memset(countColumnCuts,0,
	   (numberCutGenerators_+numberHeuristics_)*sizeof(int));
    memset(countRowCuts,0,
	   (numberCutGenerators_+numberHeuristics_)*sizeof(int));
  }
#ifdef SBB_DEBUG
  printf("Obj value %g (%d) %d rows\n",solver_->getObjValue(),
	 solver_->isProvenOptimal(),
	 solver_->getNumRows());
#endif
  int numberPasses=0;
  double primalTolerance = 1.0e-7;
  if (feasible) {
    do {
      numberPasses++;
      numberTries--;
      OsiCuts theseCuts;
      const double * lower = solver_->getColLower();
      const double * upper = solver_->getColUpper();
      const double * solution = solver_->getColSolution();
      const double * reducedCost = solver_->getReducedCost();
      // Fix on reduced costs
      double cutoff;
      solver_->getDblParam(OsiDualObjectiveLimit,cutoff);
      double gap = cutoff - solver_->getObjValue()*direction;
      double integerTolerance = 
	getDblParam(SbbModel::SbbIntegerTolerance);
      int i;
      int numberFixed=0;
      for (i=0;i<numberIntegers_;i++) {
	int iColumn = integerVariable_[i];
	// Check all solvers same on maximization
	double djValue =direction*reducedCost[iColumn];
	if (upper[iColumn]-lower[iColumn]>integerTolerance) {
	  if (solution[iColumn]<lower[iColumn]+integerTolerance&&
	      djValue>gap) {
	    solver_->setColUpper(iColumn,lower[iColumn]);
	    numberFixed++;
	  } else if (solution[iColumn]>upper[iColumn]-integerTolerance&&
		     -djValue>gap) {
	    solver_->setColLower(iColumn,upper[iColumn]);
	    numberFixed++;
	  }
	}
      }
      
      // Generate cuts here
      // If CglProbing then should be first as can fix continuous
      // also allow heuristics

      double * newSolution = new double [numberColumns];
      double heuristicValue=getCutoff();
      int found=-1; // no solution found

      if (numberPasses==1&&howOftenGlobalScan_>0&&
	  (numberNodes_%howOftenGlobalScan_)==0) {
	// look at cuts to see if marked as global
	const double * solution = solver_->getColSolution();
	int numberCuts = globalCuts_.sizeColCuts();
	for (i=0;i<numberCuts;i++) {
	  const OsiColCut * thisCut = globalCuts_.colCutPtr(i);
	  // see if cuts off solution
	  if (thisCut->violated(solution)>primalTolerance) {
	    // add cut
	    printf("Global cut added - violation %g\n",
		   thisCut->violated(solution));
	    theseCuts.insert(*thisCut);
	  }
	}
	numberCuts = globalCuts_.sizeRowCuts();
	for (i=0;i<numberCuts;i++) {
	  const OsiRowCut * thisCut = globalCuts_.rowCutPtr(i);
	  // see if cuts off solution
	  if (thisCut->violated(solution)>primalTolerance) {
	    // add cut
	    printf("Global cut added - violation %g\n",
		   thisCut->violated(solution));
	    theseCuts.insert(*thisCut);
	  }
	}
      }
      for (i=0;i<numberCutGenerators_+numberHeuristics_;i++) {
	int numberRowCutsBefore = theseCuts.sizeRowCuts();
	int numberColumnCutsBefore = theseCuts.sizeColCuts();
	if (i<numberCutGenerators_) {
	  bool mustResolve = 
	      generator_[i]->generateCuts(theseCuts,fullScan);
	  if (mustResolve) {
	    feasible=resolve();
	    if (!feasible)
	      break;
	  }
	} else {
	  // see if heuristic will do anything
	  double saveValue=heuristicValue;
	  int ifSol = 
	    heuristic_[i-numberCutGenerators_]->solution(
							 heuristicValue,
							 newSolution,
							 theseCuts);
	  if (ifSol>0) {
	    // better solution found
	    found=i;
	  } else if (ifSol<0) {
	    heuristicValue = saveValue;
	  }
	}
	int numberRowCutsAfter = theseCuts.sizeRowCuts();
	int numberColumnCutsAfter = theseCuts.sizeColCuts();
#ifdef SBB_DEBUG
	if (onOptimalPath) {
	  int k;
	  for (k=numberRowCutsBefore;k<numberRowCutsAfter;k++) {
	    OsiRowCut thisCut = theseCuts.rowCut(k);
	    assert(!debugger->invalidCut(thisCut));
	  }
	}
#endif
	int numberBefore = numberRowCutsBefore+numberColumnCutsBefore
	  + lastNumberCuts;
	int numberAfter = numberRowCutsAfter+numberColumnCutsAfter
	  + lastNumberCuts;
	if (numberAfter>maximumWhich) {
	  maximumWhich = max(maximumWhich*2+100,numberAfter);
	  int * temp = new int[2*maximumWhich];
	  memcpy(temp,whichGenerator,numberBefore*sizeof(int));
	  delete [] whichGenerator;
	  whichGenerator=temp;
	}
	int j;
	if (fullScan) {
	  // counts
	  countRowCuts[i] += numberRowCutsAfter-numberRowCutsBefore;
	  countColumnCuts[i] += 
	    theseCuts.sizeColCuts()-numberColumnCutsBefore;
	}
	
	for (j=numberRowCutsBefore;j<numberRowCutsAfter;j++) {
	  whichGenerator[numberBefore++]=i;
	  const OsiRowCut * thisCut = theseCuts.rowCutPtr(j);
	  if (thisCut->globallyValid()) {
	    // add to global list
	    globalCuts_.insert(*thisCut);
	  }
	}
	for (j=numberColumnCutsBefore;j<numberColumnCutsAfter;j++) {
	  whichGenerator[numberBefore++]=i;
	  const OsiColCut * thisCut = theseCuts.colCutPtr(j);
	  if (thisCut->globallyValid()) {
	    // add to global list
	    globalCuts_.insert(*thisCut);
	  }
	}
      }
      if (found>=0) {
	// better solution save
	setBestSolution(SBB_ROUNDING,heuristicValue,
			newSolution);
	// update cutoff
	cutoff = getCutoff();
      }
      delete [] newSolution;
    
      // switch on to get all cuts printed
#if 0
      theseCuts.printCuts();
#endif
      int numberColumnCuts = theseCuts.sizeColCuts();
      int numberRowCuts = theseCuts.sizeRowCuts();
      violated = numberRowCuts + numberColumnCuts;
      if (numberColumnCuts) {

#ifdef SBB_DEBUG
	double * oldLower = new double [numberColumns];
	double * oldUpper = new double [numberColumns];
	memcpy(oldLower,lower,numberColumns*sizeof(double));
	memcpy(oldUpper,upper,numberColumns*sizeof(double));
#endif
	for (i=0;i<numberColumnCuts;i++) {
	  const OsiColCut * thisCut = theseCuts.colCutPtr(i);
	  const CoinPackedVector & lbs = thisCut->lbs();
	  const CoinPackedVector & ubs = thisCut->ubs();
	  int j;
	  int n;
	  const int * which;
	  const double * values;
	  n = lbs.getNumElements();
	  which = lbs.getIndices();
	  values = lbs.getElements();
	  for (j=0;j<n;j++) {
	    int iColumn = which[j];
	    double value=solution[iColumn];
#ifdef SBB_DEBUG
	    printf("%d %g %g %g %g\n",iColumn,oldLower[iColumn],
		   solution[iColumn],oldUpper[iColumn],values[j]);
#endif
	    solver_->setColLower(iColumn,values[j]);
	    if (value<values[j]-integerTolerance)
	      violated = -1;
	    if (values[j]>upper[iColumn]+integerTolerance) {
	      // infeasible
	      violated = -2;
	      break;
	    }
	  }
	  n = ubs.getNumElements();
	  which = ubs.getIndices();
	  values = ubs.getElements();
	  for (j=0;j<n;j++) {
	    int iColumn = which[j];
	    double value=solution[iColumn];
#ifdef SBB_DEBUG
	    printf("%d %g %g %g %g\n",iColumn,oldLower[iColumn],
		   solution[iColumn],oldUpper[iColumn],values[j]);
#endif
	    solver_->setColUpper(iColumn,values[j]);
	    if (value>values[j]+integerTolerance)
	      violated = -1;
	    if (values[j]<lower[iColumn]-integerTolerance) {
	      // infeasible
	      violated = -2;
	      break;
	    }
	  }
	}
#ifdef SBB_DEBUG
	delete [] oldLower;
	delete [] oldUpper;
#endif
      }
      if (violated==-2) {
	// infeasible
	feasible = false;
	break;
      }
      // add in current set of cuts and add to list
      int numberRowsNow = solver_->getNumRows();
      int numberToAdd = theseCuts.sizeRowCuts();
      numberNewCuts = lastNumberCuts+numberToAdd;
      // make sure basis large enough
      basis_.setSize(numberColumns,
		     numberRowsAtStart + numberNewCuts);
      const CoinWarmStartBasis* ws =
	dynamic_cast<const CoinWarmStartBasis*>(solver_->getWarmStart());
      assert (ws!=NULL); // make sure not volume
      memcpy(basis_.getStructuralStatus(),ws->getStructuralStatus(),
	     ((numberColumns+3)>>2)*sizeof(char));
      memcpy(basis_.getArtificialStatus(),ws->getArtificialStatus(),
	       ((numberRowsNow+3)>>2)*sizeof(char));
      if (numberRowCuts||numberColumnCuts) {
	// add in new ones and copy
	
	OsiRowCut * addCuts = new OsiRowCut [numberToAdd];
	for (i=0;i<numberToAdd;i++) {
	  addCuts[i]= theseCuts.rowCut(i);
	}
	solver_->applyRowCuts(numberToAdd,addCuts);
// AJK this caused a memory fault on Win32
//	delete [] addCuts;
	// add to collection
	for (i=0;i<numberToAdd;i++) {
	  cuts.insert(theseCuts.rowCut(i));
	}
	for (i=0;i<numberToAdd;i++) {
	  basis_.setArtifStatus(numberRowsNow+i,CoinWarmStartBasis::basic);
	}
	feasible=resolve();
	//printf("Obj value after cuts %g %d rows\n",solver_->getObjValue(),
	//     solver_->getNumRows());
      } else {
	// no cuts
	numberTries=0;
      }
      if (feasible) {
	takeOffCuts(cuts, whichGenerator, numberOldActiveCuts, numberNewCuts);
	numberRowsAtStart = numberOldActiveCuts + numberRowsAtContinuous_;
	lastNumberCuts=numberNewCuts;
	if (direction*solver_->getObjValue()<lastObjective+minimumDrop&&
	    numberPasses>=3)
	  numberTries = 0;
	if (!(numberRowCuts+numberColumnCuts)||
	    !solver_->getIterationCount()) {
	  delete ws;
	  ws=NULL;
	  break; // no new cuts or no iterations
	}
	lastObjective = direction*solver_->getObjValue();
      }
      // If not feasible take off all cuts
      if (!feasible) {
	int i;
	for (i=0;i<currentNumberCuts_;i++) {
	  // take off node
	  if (addedCuts_[i]) {
	    if (!addedCuts_[i]->decrement())
	      delete addedCuts_[i];
	    addedCuts_[i]=NULL;
	  }
	}
	numberTries=0;
      }
      delete ws;
    } while (numberTries);
  }

  if (feasible&&fullScan&&numberCutGenerators_) {
    // Root node or every so often - see what to turn off
    int i;
    double thisObjective = solver_->getObjValue()*direction;
    double totalCuts=0.0;
    for (i=0;i<numberCutGenerators_;i++) 
      totalCuts += countRowCuts[i] + 5.0*countColumnCuts[i];
    if (!numberNodes_)
      handler_->message(SBB_ROOT,messages_)
	<<numberNewCuts
	<<startObjective<<thisObjective
	<<numberPasses
	<<CoinMessageEol;
    int * count = new int[numberCutGenerators_];
    memset(count,0,numberCutGenerators_*sizeof(int));
    for (i=0;i<numberNewCuts;i++) 
      count[whichGenerator[i]]++;
    double small = (0.5* totalCuts) /
      ((double) numberCutGenerators_);
    for (i=0;i<numberCutGenerators_;i++) {
      int howOften = generator_[i]->howOften();
      if (howOften<-99)
	continue;
      if (howOften<0||howOften>=1000000) {
	// If small number switch mostly off
	double thisCuts = countRowCuts[i] + 5.0*countColumnCuts[i];
	if (!thisCuts||howOften==-99) {
	  if (howOften==-99)
	    howOften=-100;
	  else
	    howOften=1000000+SCANCUTS; // wait until next time
	} else if (thisCuts<small) {
	  int k= (int) sqrt(small/thisCuts);
	  howOften=k+1000000;
	} else {
	  howOften=1+1000000;
	}
      }
      generator_[i]->setHowOften(howOften);
      int newFrequency=generator_[i]->howOften()%1000000;
      if (handler_->logLevel()>1||!numberNodes_)
	handler_->message(SBB_GENERATOR,messages_)
	  <<i
	  <<generator_[i]->cutGeneratorName()
	  <<countRowCuts[i]<<count[i]
	  <<countColumnCuts[i]
	  <<newFrequency
	  <<CoinMessageEol;
    }
    delete [] count;
  }
  delete [] countRowCuts;
  delete [] countColumnCuts;
  //exit(0);
  if (feasible) {
#ifdef CHECK_CUT_COUNTS
    printf("Number of rows at end (only active cuts) %d\n",
	   numberRowsAtContinuous_+numberNewCuts+numberOldActiveCuts);
    const CoinWarmStartBasis* ws =
      dynamic_cast<const CoinWarmStartBasis*>(solver_->getWarmStart());
    ws->print();
    delete ws;
#endif
  }
#ifdef SBB_DEBUG
  if (onOptimalPath)
    assert(feasible);
#endif
  return feasible;
}
// Take off cuts
void
SbbModel::takeOffCuts(OsiCuts & cuts, int * whichGenerator,
		     int & numberOldActiveCuts, int & numberNewCuts)
{
  int totalNumberCuts = numberNewCuts+numberOldActiveCuts;
  int * which = new int[totalNumberCuts];
  int numberToDelete=0, numberToDeleteInOriginal=0;
  // Take off inactive cuts
  int i;
  const CoinWarmStartBasis* ws =
    dynamic_cast<const CoinWarmStartBasis*>(solver_->getWarmStart());
  int * slack = new int[numberNewCuts];
  // Ones already in problem
  int originalCut=0;
  for (i=0;i<numberOldActiveCuts;i++) {
    CoinWarmStartBasis::Status status = 
      ws->getArtifStatus(i+numberRowsAtContinuous_);
    // find next on list
    while (!addedCuts_[originalCut])
      originalCut++;
    if (status == CoinWarmStartBasis::basic) {
      which[numberToDeleteInOriginal++]=i+numberRowsAtContinuous_;
      // take off node
      if (!addedCuts_[originalCut]->decrement())
	delete addedCuts_[originalCut];
      addedCuts_[originalCut]=NULL;
      originalCut++;
    } else {
      originalCut++;
    }
  }
  int numberRowsAtStart = numberOldActiveCuts+numberRowsAtContinuous_;
  int k=0;
  for (i=0;i<numberNewCuts;i++) {
    CoinWarmStartBasis::Status status = 
      ws->getArtifStatus(i+numberRowsAtStart);
    if (status == CoinWarmStartBasis::basic) {
      which[numberToDelete+numberToDeleteInOriginal] = i+numberRowsAtStart;
      slack[numberToDelete++]=i;
    } else {
      // save which generator did it
      whichGenerator[k++]=whichGenerator[i];
    }
  }
  // adjust 
  numberNewCuts -= numberToDelete;
  numberOldActiveCuts -= numberToDeleteInOriginal;
  // do backwards for eraseRowCut
  for (i=numberToDelete-1;i>=0;i--) {
    int iCut = slack[i];
    cuts.eraseRowCut(iCut);
  }
  delete [] slack;
  // delete cuts in problem
  solver_->deleteRows(numberToDelete+numberToDeleteInOriginal,which);
  delete ws;
  delete [] which;
}

bool
SbbModel::resolve()
{
  // We may have deliberately added in violated cuts - check to avoid message
  int iRow;
  int numberRows = solver_->getNumRows();
  const double * rowLower = solver_->getRowLower();
  const double * rowUpper = solver_->getRowUpper();
  bool feasible=true;
  for (iRow= numberRowsAtContinuous_;iRow<numberRows;iRow++) {
    if (rowLower[iRow]>rowUpper[iRow]+1.0e-8)
      feasible=false;
  }
  if (feasible) {
    solver_->resolve();
    numberIterations_ += getIterationCount();
    //if (solver_->isIterationLimitReached())
    //solver_->initialSolve(); // fudge for clp
    feasible =  (solver_->isProvenOptimal()&&
	    !solver_->isDualObjectiveLimitReached());
  }
  return feasible;
}
/* Set up objects.  Only do ones whose length is in range.
   If makeEquality true then a new model may be returned if
   modifications had to be made, otherwise "this" is returned.

   Could use Probing at continuous to extend objects
*/
SbbModel * 
SbbModel::findCliques(bool makeEquality,
		      int atLeastThisMany, int lessThanThis, int defaultValue)
{
  // No objects are allowed to exist
  assert(numberObjects_==numberIntegers_);
  CoinPackedMatrix matrixByRow(*solver_->getMatrixByRow());
  int numberRows = solver_->getNumRows();
  int numberColumns = solver_->getNumCols();

  // We may want to add columns
  int numberSlacks=0;
  int * rows = new int[numberRows];
  double * element =new double[numberRows];

  int iRow;

  findIntegers(true);
  numberObjects_=numberIntegers_;

  int numberCliques=0;
  SbbObject ** object = new SbbObject * [numberRows];
  int * which = new int[numberIntegers_];
  char * type = new char[numberIntegers_];
  int * lookup = new int[numberColumns];
  int i;
  for (i=0;i<numberColumns;i++) 
    lookup[i]=-1;
  for (i=0;i<numberIntegers_;i++) 
    lookup[integerVariable_[i]]=i;

  // Statistics
  int totalP1=0,totalM1=0;
  int numberBig=0,totalBig=0;
  int numberFixed=0;

  // Row copy
  const double * elementByRow = matrixByRow.getElements();
  const int * column = matrixByRow.getIndices();
  const int * rowStart = matrixByRow.getVectorStarts();
  const int * rowLength = matrixByRow.getVectorLengths();

  // Column lengths for slacks
  const int * columnLength = solver_->getMatrixByCol()->getVectorLengths();

  const double * lower = getColLower();
  const double * upper = getColUpper();
  const double * rowLower = getRowLower();
  const double * rowUpper = getRowUpper();

  for (iRow=0;iRow<numberRows;iRow++) {
    int numberP1=0, numberM1=0;
    int j;
    double upperValue=rowUpper[iRow];
    double lowerValue=rowLower[iRow];
    bool good=true;
    int slack = -1;
    for (j=rowStart[iRow];j<rowStart[iRow]+rowLength[iRow];j++) {
      int iColumn = column[j];
      int iInteger=lookup[iColumn];
      if (upper[iColumn]-lower[iColumn]<1.0e-8) {
	// fixed
	upperValue -= lower[iColumn]*elementByRow[j];
	lowerValue -= lower[iColumn]*elementByRow[j];
	continue;
      } else if (upper[iColumn]!=1.0||lower[iColumn]!=0.0) {
	good = false;
	break;
      } else if (iInteger<0) {
	good = false;
	break;
      } else {
	if (columnLength[iColumn]==1)
	  slack = iInteger;
      }
      if (fabs(elementByRow[j])!=1.0) {
	good=false;
	break;
      } else if (elementByRow[j]>0.0) {
	which[numberP1++]=iInteger;
      } else {
	numberM1++;
	which[numberIntegers_-numberM1]=iInteger;
      }
    }
    int iUpper = (int) floor(upperValue+1.0e-5);
    int iLower = (int) ceil(lowerValue-1.0e-5);
    int state=0;
    if (upperValue<1.0e6) {
      if (iUpper==1-numberM1)
	state=1;
      else if (iUpper==-numberM1)
	state=2;
      else if (iUpper<-numberM1)
	state=3;
    }
    if (!state&&lowerValue>-1.0e6) {
      if (-iLower==1-numberP1)
	state=-1;
      else if (-iLower==-numberP1)
	state=-2;
      else if (-iLower<-numberP1)
	state=-3;
    }
    if (good&&state) {
      if (abs(state)==3) {
	// infeasible
	numberObjects_=-1;
	break;
      } else if (abs(state)==2) {
	// we can fix all
	numberFixed += numberP1+numberM1;
	if (state>0) {
	  // fix all +1 at 0, -1 at 1
	  for (i=0;i<numberP1;i++)
	    solver_->setColUpper(integerVariable_[which[i]],0.0);
	  for (i=0;i<numberM1;i++)
	    solver_->setColLower(integerVariable_[which[numberIntegers_-i-1]],
				 1.0);
	} else {
	  // fix all +1 at 1, -1 at 0
	  for (i=0;i<numberP1;i++)
	    solver_->setColLower(integerVariable_[which[i]],1.0);
	  for (i=0;i<numberM1;i++)
	    solver_->setColUpper(integerVariable_[which[numberIntegers_-i-1]],
				 0.0);
	}
      } else {
	int length = numberP1+numberM1;
	if (length>=atLeastThisMany&&length<lessThanThis) {
	  // create object
	  bool addOne=false;
	  int objectType;
	  if (iLower==iUpper) {
	    objectType=1;
	  } else {
	    if (makeEquality) {
	      objectType=1;
	      element[numberSlacks]=state;
	      rows[numberSlacks++]=iRow;
	      addOne=true;
	    } else {
	      objectType=0;
	    }
	  }
	  if (state>0) {
	    totalP1 += numberP1;
	    totalM1 += numberM1;
	    for (i=0;i<numberP1;i++)
	      type[i]=1;
	    for (i=0;i<numberM1;i++) {
	      which[numberP1]=which[numberIntegers_-i-1];
	      type[numberP1++]=0;
	    }
	  } else {
	    totalP1 += numberM1;
	    totalM1 += numberP1;
	    for (i=0;i<numberP1;i++)
	      type[i]=0;
	    for (i=0;i<numberM1;i++) {
	      which[numberP1]=which[numberIntegers_-i-1];
	      type[numberP1++]=1;
	    }
	  }
	  if (addOne) {
	    // add in slack
	    which[numberP1]=numberIntegers_+numberSlacks-1;
	    slack = numberP1;
	    type[numberP1++]=1;
	  } else if (slack>=0) {
	    for (i=0;i<numberP1;i++) {
	      if (which[i]==slack) {
		slack=i;
	      }
	    }
	  }
	  object[numberCliques++] = new SbbClique(this,objectType,numberP1,
					      which,type,
					       1000000+numberCliques,slack);
	} else if (numberP1+numberM1>=lessThanThis) {
	  // too big
	  numberBig++;
	  totalBig += numberP1+numberM1;
	}
      }
    }
  }
  delete [] which;
  delete [] type;
  delete [] lookup;
  if (numberCliques<0) {
    printf("*** Problem infeasible\n");
  } else {
    if (numberCliques)
      printf("%d cliques of average size %g found, %d P1, %d M1\n",
	     numberCliques,
	     ((double)(totalP1+totalM1))/((double) numberCliques),
	     totalP1,totalM1);
    else
      printf("No cliques found\n");
    if (numberBig)
      printf("%d large cliques (>= %d) found, total %d\n",
	     numberBig,lessThanThis,totalBig);
    if (numberFixed)
      printf("%d variables fixed\n",numberFixed);
  }
  if (numberCliques>0&&numberSlacks&&makeEquality) {
    printf("adding %d integer slacks\n",numberSlacks);
    // add variables to make equality rows
    int * temp = new int[numberIntegers_+numberSlacks];
    memcpy(temp,integerVariable_,numberIntegers_*sizeof(int));
    // Get new model
    SbbModel * newModel = new SbbModel(*this);
    OsiSolverInterface * newSolver = newModel->solver();
    for (i=0;i<numberSlacks;i++) {
      temp[i+numberIntegers_]=i+numberColumns;
      int iRow = rows[i];
      double value = element[i];
      double lowerValue = 0.0;
      double upperValue = 1.0;
      double objValue  = 0.0;
      CoinPackedVector column(1,&iRow,&value);
      newSolver->addCol(column,lowerValue,upperValue,objValue);
      // set integer
      newSolver->setInteger(numberColumns+i);
      if (value >0)
	newSolver->setRowLower(iRow,rowUpper[iRow]);
      else
	newSolver->setRowUpper(iRow,rowLower[iRow]);
    }
    // replace list of integers
    for (i=0;i<newModel->numberObjects_;i++)
      delete newModel->object_[i];
    newModel->numberObjects_ = 0;
    delete [] newModel->object_;
    newModel->object_=NULL;
    newModel->findIntegers(true); //Set up all integer objects
    if (priority_) {
      // old model had priorities
      delete [] newModel->priority_;
      newModel->priority_ = new int[newModel->numberIntegers_+numberCliques];
      memcpy(newModel->priority_,priority_,numberIntegers_*sizeof(int));
      for (i=numberIntegers_;i<newModel->numberIntegers_+numberCliques;i++)
	newModel->priority_[i]=defaultValue;
    }
    if (originalColumns_) {
      // old model had originalColumns
      delete [] newModel->originalColumns_;
      newModel->originalColumns_ = new int[numberColumns+numberSlacks];
      memcpy(newModel->originalColumns_,originalColumns_,numberColumns*sizeof(int));
      // mark as not in previous model
      for (i=numberColumns;i<numberColumns+numberSlacks;i++)
	newModel->originalColumns_[i]=-1;
    }
    delete [] rows;
    delete [] element;
    newModel->addObjects(numberCliques,object);
    for (;i<numberCliques;i++) 
      delete object[i];
    delete [] object;
    newModel->synchronizeModel();
    return newModel;
  } else {
    if (numberCliques>0) {
      addObjects(numberCliques,object);
      for (;i<numberCliques;i++) 
	delete object[i];
      synchronizeModel();
    }
    delete [] object;
    if (priority_) {
      // model had priorities
      int * temp = new int[numberIntegers_+numberCliques];
      memcpy(temp,priority_,numberIntegers_*sizeof(int));
      delete [] priority_;
      priority_=temp;
      for (i=numberIntegers_;i<numberIntegers_+numberCliques;i++)
	priority_[i]=defaultValue;
    }
    delete [] rows;
    delete [] element;
    return this;
  }
}
// Pass in priorities (added for Kurt Spielberg)
void 
SbbModel::passInPriorities(const int * priorities,bool ifObject, int defaultValue)
{
  findIntegers(false);
  int i;
  if (!priority_) {
    priority_ = new int[numberObjects_];
    for (i=0;i<numberObjects_;i++)
      priority_[i]=defaultValue;
  }
  if (priorities) {
    if (ifObject)
      memcpy(priority_+numberIntegers_,priorities,
	     (numberObjects_-numberIntegers_)*sizeof(int));
    else
      memcpy(priority_,priorities,numberIntegers_*sizeof(int));
  }
}
// Delete all object information
void 
SbbModel::deleteObjects()
{
  delete [] priority_;
  priority_=NULL;
  int i;
  for (i=0;i<numberObjects_;i++)
    delete object_[i];
  delete [] object_;
  object_ = NULL;
  numberObjects_=0;
  findIntegers(true);
}
// Makes all model pointers current
void SbbModel::synchronizeModel()
{
  int i;
  for (i=0;i<numberHeuristics_;i++) 
    heuristic_[i]->setModel(this);
  for (i=0;i<numberObjects_;i++)
    object_[i]->setModel(this);
  for (i=0;i<numberCutGenerators_;i++)
    generator_[i]->refreshModel(this);
}
// Fill in integers and create objects
void 
SbbModel::findIntegers(bool startAgain)
{
  assert(solver_);

  if (numberIntegers_&&!startAgain&&object_)
    return;
  delete [] integerVariable_;
  numberIntegers_=0;
  int numberColumns = getNumCols();
  int iColumn;
  for (iColumn=0;iColumn<numberColumns;iColumn++) {
    if( isInteger(iColumn)) 
      numberIntegers_++;
  }
  if (numberIntegers_) {
    if (!object_) {
      object_ = new SbbObject * [numberIntegers_];
      memset(object_,0,numberIntegers_*sizeof(SbbObject *));
      numberObjects_=numberIntegers_;
    }
    integerVariable_ = new int [numberIntegers_];
    numberIntegers_=0;
    for (iColumn=0;iColumn<numberColumns;iColumn++) {
      if( solver_->isInteger(iColumn)) {
	delete object_[numberIntegers_];
	object_[numberIntegers_]=new SbbSimpleInteger(this,
						  numberIntegers_,
						  iColumn);
	integerVariable_[numberIntegers_++]=iColumn;
      }
    }
  } else {
    handler_->message(SBB_NOINT,messages_)
      <<CoinMessageEol;
    return;
  }
}
/* Add in any object information (objects are cloned - owner can delete
   originals */
void 
SbbModel::addObjects(int numberObjects, SbbObject ** objects)
{
  int newNumberObjects= numberObjects+numberObjects_;
  SbbObject ** temp  = new SbbObject * [newNumberObjects];
  int i;
  for (i=0;i<numberObjects_;i++) 
    temp[i]=object_[i];
  for (;i<newNumberObjects;i++) { 
    temp[i]=objects[i-numberObjects_]->clone();
    temp[i]->setModel(this);
  }
  delete [] object_;
  object_ = temp;
  numberObjects_ = newNumberObjects;
}
/* Call this from anywhere when solution found.
   Solution is number columns in size */
void
SbbModel::setBestSolution(SBB_Message how,
			  double objectiveValue, const double * solution,bool fixVariables)
{
  bestObjective_ = objectiveValue;
  int numberColumns = solver_->getNumCols();
  if (!bestSolution_)
    bestSolution_ = new double[numberColumns];

  // swap solvers
  OsiSolverInterface * saveSolver = solver_;
  solver_ = continuousSolver_;
  // move solution to continuous copy
  solver_->setColSolution(solution);
  // Put current solution in safe place
  memcpy(currentSolution_,solver_->getColSolution(),
	 numberColumns*sizeof(double));

  // save original bounds
  double * saveUpper = new double[numberColumns];
  double * saveLower = new double[numberColumns];
  memcpy(saveUpper,getColUpper(),numberColumns*sizeof(double));
  memcpy(saveLower,getColLower(),numberColumns*sizeof(double));

  // clean bounds
  int i;
  for (i=0;i<numberObjects_;i++)
    object_[i]->feasibleRegion();
  // solve with all slack basis so fixed will be clean
  CoinWarmStartBasis slack;
  solver_->setWarmStart(&slack);
  solver_->initialSolve();
  assert(solver_->isProvenOptimal());

  // redo solution
  solution = solver_->getColSolution();
  
  // Column copy
  const double * element = solver_->getMatrixByCol()->getElements();
  const int * row = solver_->getMatrixByCol()->getIndices();
  const int * columnStart = solver_->getMatrixByCol()->getVectorStarts();
  const int * columnLength = solver_->getMatrixByCol()->getVectorLengths();

  const double * rowLower = solver_->getRowLower();
  const double * rowUpper = solver_->getRowUpper();

  int numberRows = solver_->getNumRows();

  // get row activities
  double * rowActivity = new double[numberRows];
  memset(rowActivity,0,numberRows*sizeof(double));

  // First move solution across - then clean up
  // This is for virtuous solvers which invalidate solution
  // when bound changed
  memcpy(bestSolution_,solution,numberColumns*sizeof(double));
  int iColumn;
  for (iColumn=0;iColumn<numberColumns;iColumn++) {
    double value = bestSolution_[iColumn];
    // clean up - should be no need
    value = max(value, saveLower[iColumn]);
    value = min(value, saveUpper[iColumn]);
    if (!fixVariables) {
      solver_->setColLower(iColumn,saveLower[iColumn]);
      solver_->setColUpper(iColumn,saveUpper[iColumn]);
    }
    bestSolution_[iColumn] = value;
    if (value) {
      int j;
      for (j=columnStart[iColumn];
	   j<columnStart[iColumn]+columnLength[iColumn];j++) {
	int iRow=row[j];
	rowActivity[iRow] += value*element[j];
      }
    }
  }

  delete [] saveLower;
  delete [] saveUpper;

  // check was feasible - if not adjust (cleaning may move)
  double primalTolerance;
  assert(solver_->getDblParam(OsiPrimalTolerance,primalTolerance));
  for (i=0;i<numberRows;i++) {
    if(rowActivity[i]<rowLower[i]) {
      assert (rowActivity[i]>rowLower[i]-10.0*primalTolerance);
    } else if(rowActivity[i]>rowUpper[i]) {
      assert (rowActivity[i]<rowUpper[i]+10.0*primalTolerance);
    }
  }
  delete [] rowActivity;
  // now see if anyone wants to generate cuts
  OsiCuts theseCuts;
  for (i=0;i<numberCutGenerators_;i++) {
    if (generator_[i]->atSolution())
      generator_[i]->generateCuts(theseCuts,true);
  }
  int numberCuts = theseCuts.sizeColCuts();
  for (i=0;i<numberCuts;i++) {
    const OsiColCut * thisCut = theseCuts.colCutPtr(i);
    if (thisCut->globallyValid()) {
      // add to global list
      globalCuts_.insert(*thisCut);
    }
  }
  numberCuts = theseCuts.sizeRowCuts();
  for (i=0;i<numberCuts;i++) {
    const OsiRowCut * thisCut = globalCuts_.rowCutPtr(i);
    if (thisCut->globallyValid()) {
      // add to global list
      globalCuts_.insert(*thisCut);
    }
  }
  // back to true solver
  solver_=saveSolver;
  double cutoff = bestObjective_-dblParam_[SbbCutoffIncrement];
  double direction = solver_->getObjSense();
  setCutoff(cutoff*direction);

  handler_->message(how,messages_)
    <<bestObjective_<<numberIterations_
    <<numberNodes_
    <<CoinMessageEol;
  if (how==SBB_ROUNDING)
    numberHeuristicSolutions_++;
    numberSolutions_++;
}
/* Test if solution is feasible.
   Sets number of infeasibilities for normal and odd
*/
bool 
SbbModel::feasibleSolution(int & numberIntegerInfeasibilities,
			int & numberObjectInfeasibilities) const
{
  int numberUnsatisfied=0;
  double sumUnsatisfied=0.0;
  int preferredWay;
  double otherWay;
  double integerTolerance = getIntegerTolerance();
  int j;
  // Put current solution in safe place
  memcpy(currentSolution_,solver_->getColSolution(),
	 solver_->getNumCols()*sizeof(double));
  for (j=0;j<numberIntegers_;j++) {
    const SbbObject * object = object_[j];
    double infeasibility = object->infeasibility(preferredWay,otherWay);
    if (infeasibility>integerTolerance) {
      numberUnsatisfied++;
      sumUnsatisfied += infeasibility;
    }
  }
  numberIntegerInfeasibilities = numberUnsatisfied;
  for (;j<numberObjects_;j++) {
    const SbbObject * object = object_[j];
    double infeasibility = object->infeasibility(preferredWay,otherWay);
    if (infeasibility>integerTolerance) {
      numberUnsatisfied++;
      sumUnsatisfied += infeasibility;
    }
  }
  numberObjectInfeasibilities = numberUnsatisfied-numberIntegerInfeasibilities;
  return (!numberUnsatisfied);
}
/* For all vubs see if we can tighten bounds by solving Lp's
   type - 0 just vubs
   1 all (could be very slow)
   -1 just vubs where variable away from bound
   Returns false if not feasible
*/
bool 
SbbModel::tightenVubs(int type, bool allowMultipleBinary, double useCutoff)
{

  CoinPackedMatrix matrixByRow(*solver_->getMatrixByRow());
  int numberRows = solver_->getNumRows();
  int numberColumns = solver_->getNumCols();

  int iRow,iColumn;

  // Row copy
  //const double * elementByRow = matrixByRow.getElements();
  const int * column = matrixByRow.getIndices();
  const int * rowStart = matrixByRow.getVectorStarts();
  const int * rowLength = matrixByRow.getVectorLengths();

  const double * colUpper = solver_->getColUpper();
  const double * colLower = solver_->getColLower();
  //const double * rowUpper = solver_->getRowUpper();
  //const double * rowLower = solver_->getRowLower();

  const double * objective = solver_->getObjCoefficients();
  //double direction = solver_->getObjSense();
  const double * colsol = solver_->getColSolution();

  int numberVub=0;
  int * continuous = new int[numberColumns];
  if (type>=0) {
    double * sort = new double[numberColumns];
    for (iRow=0;iRow<numberRows;iRow++) {
      int j;
      int numberBinary=0;
      int numberUnsatisfiedBinary=0;
      int numberContinuous=0;
      int iCont=-1;
      double weight=1.0e30;
      for (j=rowStart[iRow];j<rowStart[iRow]+rowLength[iRow];j++) {
	int iColumn = column[j];
	if (colUpper[iColumn]-colLower[iColumn]>1.0e-8) {
	  if (solver_->isFreeBinary(iColumn)) {
	    numberBinary++;
	    /* For sort I make naive assumption:
	       x - a * delta <=0 or
	       -x + a * delta >=0
	    */
	    if (colsol[iColumn]>colLower[iColumn]+1.0e-6&&
		colsol[iColumn]<colUpper[iColumn]-1.0e-6) {
	      numberUnsatisfiedBinary++;
	      weight = min(weight,fabs(objective[iColumn]));
	    }
	  } else {
	    numberContinuous++;
	    iCont=iColumn;
	  }
	}
      }
      if (numberContinuous==1&&numberBinary) {
	if (numberBinary==1||allowMultipleBinary) {
	  // treat as vub
	  if (!numberUnsatisfiedBinary)
	    weight=-1.0; // at end
	  sort[numberVub]=-weight;
	  continuous[numberVub++] = iCont;
	}
      }
    }
    if (type>0) {
      // take so many
      CoinSort_2(sort,sort+numberVub,continuous);
      numberVub = min(numberVub,type);
    }
    delete [] sort;
  } else {
    for (iColumn=0;iColumn<numberColumns;iColumn++) 
      continuous[iColumn]=iColumn;
    numberVub=numberColumns;
  }
  bool feasible = tightenVubs(numberVub,continuous,useCutoff);
  delete [] continuous;

  return feasible;
}
// This version is just handed a list of variables
bool 
SbbModel::tightenVubs(int numberSolves, const int * which,
		      double useCutoff)
{

  int numberColumns = solver_->getNumCols();

  int iColumn;
  
  double saveCutoff;
  OsiSolverInterface * solver = solver_;
  assert (solver_->getDblParam(OsiDualObjectiveLimit,saveCutoff));
  
  double * objective = new double[numberColumns];
  memcpy(objective,solver_->getObjCoefficients(),numberColumns*sizeof(double));
  double direction = solver_->getObjSense();
  
  // add in objective if there is a cutoff
  if (useCutoff<1.0e30) {
    // get new version of model
    solver = solver_->clone();
    CoinPackedVector newRow;
    for (iColumn=0;iColumn<numberColumns;iColumn++) {
      solver->setObjCoeff(iColumn,0.0); // zero out in new model
      if (objective[iColumn]) 
	newRow.insert(iColumn,direction * objective[iColumn]);
      
    }
    solver->addRow(newRow,-COIN_DBL_MAX,useCutoff);
    // signal no objective
    delete [] objective;
    objective=NULL;
  }
  solver->setDblParam(OsiDualObjectiveLimit,DBL_MAX);


  bool * vub = new bool [numberColumns];
  int iVub;

  // mark vub columns
  for (iColumn=0;iColumn<numberColumns;iColumn++) 
    vub[iColumn]=false;
  for (iVub=0;iVub<numberSolves;iVub++) 
    vub[which[iVub]]=true;
  OsiCuts cuts;
  // First tighten bounds anyway if CglProbing there
  CglProbing* generator = NULL;
  int iGen;
  for (iGen=0;iGen<numberCutGenerators_;iGen++) {
    generator = dynamic_cast<CglProbing*>(generator_[iGen]->generator());
    if (generator)
      break;
  }
  int numberFixed=0;
  int numberTightened=0;
  int numberFixedByProbing=0;
  int numberTightenedByProbing=0;
  int printFrequency = (numberSolves+19)/20; // up to 20 messages
  int save[4];
  if (generator) {
    // set to cheaper and then restore at end
    save[0]=generator->getMaxPass();
    save[1]=generator->getMaxProbe();
    save[2]=generator->getMaxLook();
    save[3]=generator->rowCuts();
    generator->setMaxPass(1);
    generator->setMaxProbe(10);
    generator->setMaxLook(50);
    generator->setRowCuts(0);
    
    // Probing - return tight column bounds
    generator->generateCutsAndModify(*solver,cuts);
    const double * tightLower = generator->tightLower();
    const double * lower = solver->getColLower();
    const double * tightUpper = generator->tightUpper();
    const double * upper = solver->getColUpper();
    for (iColumn=0;iColumn<numberColumns;iColumn++) {
      double newUpper = tightUpper[iColumn];
      double newLower = tightLower[iColumn];
      if (newUpper<upper[iColumn]-1.0e-8*(fabs(upper[iColumn])+1)||
	  newLower>lower[iColumn]+1.0e-8*(fabs(lower[iColumn])+1)) {
	if (newUpper<newLower) {
	  fprintf(stderr,"Problem is infeasible\n");
	  return false;
	}
	if (newUpper==newLower) {
	  numberFixed++;
	  numberFixedByProbing++;
	  solver->setColLower(iColumn,newLower);
	  solver->setColUpper(iColumn,newUpper);
	} else if (vub[iColumn]) {
	  numberTightened++;
	  numberTightenedByProbing++;
	  if (!solver->isInteger(iColumn)) {
	    // relax
	    newLower=max(lower[iColumn],
				    newLower
				    -1.0e-8*(fabs(lower[iColumn])+1));
	    newUpper=min(upper[iColumn],
				    newUpper
				    +1.0e-8*(fabs(upper[iColumn])+1));
	  }
	  solver->setColLower(iColumn,newLower);
	  solver->setColUpper(iColumn,newUpper);
	}
      }
    }
  }
  CoinWarmStart * ws = solver->getWarmStart();
  double * solution = new double [numberColumns];
  memcpy(solution,solver->getColSolution(),numberColumns*sizeof(double));
  for (iColumn=0;iColumn<numberColumns;iColumn++) 
    solver->setObjCoeff(iColumn,0.0);
  //solver->messageHandler()->setLogLevel(2);
  for (iVub=0;iVub<numberSolves;iVub++) {
    iColumn = which[iVub];
    int iTry;
    for (iTry=0;iTry<2;iTry++) {
      double saveUpper = solver->getColUpper()[iColumn];
      double saveLower = solver->getColLower()[iColumn];
      double value;
      if (iTry==1) {
	// try all way up
	solver->setObjCoeff(iColumn,-1.0);
      } else {
	// try all way down
	solver->setObjCoeff(iColumn,1.0);
      }
      solver->initialSolve();
      value = solver->getColSolution()[iColumn];
      bool change=false;
      if (iTry==1) {
	if (value<saveUpper-1.0e-4) {
	  if (solver->isInteger(iColumn)) {
	    value = floor(value+0.00001);
	  } else {
	    // relax a bit
	    value=min(saveUpper,value+1.0e-8*(fabs(saveUpper)+1));
	  }
	  if (value-saveLower<1.0e-7) 
	    value = saveLower; // make sure exactly same
	  solver->setColUpper(iColumn,value);
	  saveUpper=value;
	  change=true;
	}
      } else {
	if (value>saveLower+1.0e-4) {
	  if (solver->isInteger(iColumn)) {
	    value = ceil(value-0.00001);
	  } else {
	    // relax a bit
	    value=max(saveLower,value-1.0e-8*(fabs(saveLower)+1));
	  }
	  if (saveUpper-value<1.0e-7) 
	    value = saveUpper; // make sure exactly same
	  solver->setColLower(iColumn,value);
	  saveLower=value;
	  change=true;
	}
      }
      solver->setObjCoeff(iColumn,0.0);
      if (change) {
	if (saveUpper==saveLower) 
	  numberFixed++;
	else
	  numberTightened++;
	int saveFixed=numberFixed;
	
	int jColumn;
	if (generator) {
	  // Probing - return tight column bounds
	  cuts = OsiCuts();
	  generator->generateCutsAndModify(*solver,cuts);
	  const double * tightLower = generator->tightLower();
	  const double * lower = solver->getColLower();
	  const double * tightUpper = generator->tightUpper();
	  const double * upper = solver->getColUpper();
	  for (jColumn=0;jColumn<numberColumns;jColumn++) {
	    double newUpper = tightUpper[jColumn];
	    double newLower = tightLower[jColumn];
	    if (newUpper<upper[jColumn]-1.0e-8*(fabs(upper[jColumn])+1)||
		newLower>lower[jColumn]+1.0e-8*(fabs(lower[jColumn])+1)) {
	      if (newUpper<newLower) {
		fprintf(stderr,"Problem is infeasible\n");
		return false;
	      }
	      if (newUpper==newLower) {
		numberFixed++;
		numberFixedByProbing++;
		solver->setColLower(jColumn,newLower);
		solver->setColUpper(jColumn,newUpper);
	      } else if (vub[jColumn]) {
		numberTightened++;
		numberTightenedByProbing++;
		if (!solver->isInteger(jColumn)) {
		  // relax
		  newLower=max(lower[jColumn],
			       newLower
			       -1.0e-8*(fabs(lower[jColumn])+1));
		  newUpper=min(upper[jColumn],
			       newUpper
			       +1.0e-8*(fabs(upper[jColumn])+1));
		}
		solver->setColLower(jColumn,newLower);
		solver->setColUpper(jColumn,newUpper);
	      }
	    }
	  }
	}
	if (numberFixed>saveFixed) {
	  // original solution may not be feasible
	  // go back to true costs to solve if exists
	  if (objective) {
	    for (jColumn=0;jColumn<numberColumns;jColumn++) 
	      solver->setObjCoeff(jColumn,objective[jColumn]);
	  }
	  solver->setColSolution(solution);
	  solver->setWarmStart(ws);
	  solver->resolve();
	  if (!solver->isProvenOptimal()) {
	    fprintf(stderr,"Problem is infeasible\n");
	    return false;
	  }
	  delete ws;
	  ws = solver->getWarmStart();
	  memcpy(solution,solver->getColSolution(),
		 numberColumns*sizeof(double));
	  for (jColumn=0;jColumn<numberColumns;jColumn++) 
	    solver->setObjCoeff(jColumn,0.0);
	}
      }
      solver->setColSolution(solution);
      solver->setWarmStart(ws);
    }
    if (iVub%printFrequency==0) 
      handler_->message(SBB_VUB_PASS,messages_)
	<<iVub+1<<numberFixed<<numberTightened
	<<CoinMessageEol;
  }
  handler_->message(SBB_VUB_END,messages_)
    <<numberFixed<<numberTightened
    <<CoinMessageEol;
  delete ws;
  delete [] solution;
  // go back to true costs to solve if exists
  if (objective) {
    for (iColumn=0;iColumn<numberColumns;iColumn++) 
      solver_->setObjCoeff(iColumn,objective[iColumn]);
    delete [] objective;
  }
  delete [] vub;
  if (generator) {
    /*printf("Probing fixed %d and tightened %d\n",
	   numberFixedByProbing,
	   numberTightenedByProbing);*/
    if (generator_[iGen]->howOften()==-1&&
	(numberFixedByProbing+numberTightenedByProbing)*5>
	(numberFixed+numberTightened))
      generator_[iGen]->setHowOften(1000000+1);
    generator->setMaxPass(save[0]);
    generator->setMaxProbe(save[1]);
    generator->setMaxLook(save[2]);
    generator->setRowCuts(save[3]);
  }

  if (solver!=solver_)
    delete solver;
  solver_->setDblParam(OsiDualObjectiveLimit,saveCutoff);
  return true;
}
/* 
   Do Integer Presolve. Returns new model.
   I have to work out cleanest way of getting solution to
   original problem at end.  So this is very preliminary.
*/
SbbModel * 
SbbModel::integerPresolve()
{
  status_ = 0;
  // solve LP
  bool feasible = resolve();

  SbbModel * newModel = NULL;
  if (feasible) {

    // get a new model
    newModel = new SbbModel(*this);
    newModel->messageHandler()->setLogLevel(messageHandler()->logLevel());

    feasible = newModel->integerPresolveThisModel(solver_);
  }
  if (!feasible) {
    handler_->message(SBB_INFEAS,messages_)
    <<CoinMessageEol;
    status_ = 1;
    delete newModel;
    return NULL;
  } else {
    newModel->synchronizeModel(); // make sure everything that needs solver has it
    return newModel;
  }
}
/* 
   Do Integer Presolve - destroying current model
*/
bool 
SbbModel::integerPresolveThisModel(OsiSolverInterface * originalSolver)
{
  status_ = 0;
  // solve LP
  bool feasible = resolve();

  bestObjective_=1.0e50;
  numberSolutions_=0;
  numberHeuristicSolutions_=0;
  double cutoff=1.0e50;
  assert(solver_->getDblParam(OsiDualObjectiveLimit,cutoff));
  double direction = solver_->getObjSense();
  if (cutoff<1.0e20)
    cutoff *= direction; // Don't change default
  if (fabs(cutoff)>bestObjective_)
    cutoff=bestObjective_;
  solver_->setDblParam(OsiDualObjectiveLimit,cutoff);
  int iColumn;
  int numberColumns = getNumCols();
  int originalNumberColumns = numberColumns;
  int numberPasses=0;
  synchronizeModel(); // make sure everything that needs solver has it
  // just point to solver_
  delete continuousSolver_;
  continuousSolver_ = solver_;
  // get a copy of original so we can fix bounds
  OsiSolverInterface * cleanModel = originalSolver->clone();
#ifdef SBB_DEBUG
  std::string problemName;
  cleanModel->getStrParam(OsiProbName,problemName);
  printf("Problem name - %s\n",problemName.c_str());
  cleanModel->activateRowCutDebugger(problemName.c_str());
  const OsiRowCutDebugger * debugger = cleanModel->getRowCutDebugger();
#endif

  // array which points from original columns to presolved
  int * original = new int[numberColumns];
  // arrays giving bounds - only ones found by probing 
  // rest will be found by presolve
  double * originalLower = new double[numberColumns];
  double * originalUpper = new double[numberColumns];
  {
    const double * lower = getColLower();
    const double * upper = getColUpper();
    for (iColumn=0;iColumn<numberColumns;iColumn++) {
      original[iColumn]=iColumn;
      originalLower[iColumn] = lower[iColumn];
      originalUpper[iColumn] = upper[iColumn];
    }
  }
  findIntegers(true);
  // save original integers
  int * originalPriority = NULL;
  int * originalIntegers = new int[numberIntegers_];
  int originalNumberIntegers = numberIntegers_;
  memcpy(originalIntegers,integerVariable_,numberIntegers_*sizeof(int));

  if (priority_) {
    originalPriority = new int[numberIntegers_];
    memcpy(originalPriority,priority_,numberIntegers_*sizeof(int));
    delete [] priority_;
    priority_=NULL;
  }
  while (numberPasses<20) {
   
    numberPasses++;
    numberSolutions_=0;
    // this will be set false to break out of loop with presolved problem
    bool doIntegerPresolve=(numberPasses!=20);
    
    // Current number of free integer variables
    // Get increment in solutions
    {
      const double * objective = cleanModel->getObjCoefficients();
      const double * lower = cleanModel->getColLower();
      const double * upper = cleanModel->getColUpper();
      double maximumCost=0.0;
      bool possibleMultiple=true;
      int numberChanged=0;
      for (iColumn=0;iColumn<originalNumberColumns;iColumn++) {
	if (originalUpper[iColumn]>originalLower[iColumn]) {
	  if( cleanModel->isInteger(iColumn)) {
	    maximumCost = max(maximumCost,fabs(objective[iColumn]));
	  } else if (objective[iColumn]) {
	    possibleMultiple=false;
	  }
	}
	if (originalUpper[iColumn]<upper[iColumn]) {
#ifdef SBB_DEBUG
	  printf("Changing upper bound on %d from %g to %g\n",
		 iColumn,upper[iColumn],originalUpper[iColumn]);
#endif
	  cleanModel->setColUpper(iColumn,originalUpper[iColumn]);
	  numberChanged++;
	}
	if (originalLower[iColumn]>lower[iColumn]) {
#ifdef SBB_DEBUG
	  printf("Changing lower bound on %d from %g to %g\n",
		 iColumn,lower[iColumn],originalLower[iColumn]);
#endif
	  cleanModel->setColLower(iColumn,originalLower[iColumn]);
	  numberChanged++;
	}
      }
      // if first pass - always try
      if (numberPasses==1)
	numberChanged += 1;
      if (possibleMultiple) {
	int increment=0; 
	double multiplier = 2520.0;
	while (10.0*multiplier*maximumCost<1.0e8)
	  multiplier *= 10.0;
	for (iColumn=0;iColumn<originalNumberColumns;iColumn++) {
	  if (originalUpper[iColumn]>originalLower[iColumn]) {
	    if( isInteger(iColumn)&&objective[iColumn]) {
	      double value = fabs(objective[iColumn])*multiplier;
	      int nearest = (int) floor(value+0.5);
	      if (fabs(value-floor(value+0.5))>1.0e-8) {
		increment=0;
		break; // no good
	      } else if (!increment) {
		// first
		increment=nearest;
	      } else {
		increment = gcd(increment,nearest);
	      }
	    }
	  }
	}
	if (increment) {
	  double value = increment;
	  value /= multiplier;
	  if (value*0.999>dblParam_[SbbCutoffIncrement]) {
	    messageHandler()->message(SBB_INTEGERINCREMENT,messages())
	      <<value
	      <<CoinMessageEol;
	    dblParam_[SbbCutoffIncrement]=value*0.999;
	  }
	}
      }
      if (!numberChanged) {
	doIntegerPresolve=false; // not doing any better
      }
    }
#ifdef SBB_DEBUG
    if (debugger) 
      assert(debugger->onOptimalPath(*cleanModel));
#endif
    // do presolve - for now just clp but easy to get osi interface
    OsiClpSolverInterface * clpSolver 
      = dynamic_cast<OsiClpSolverInterface *> (cleanModel);
    assert (clpSolver);
    ClpSimplex * clp = clpSolver->getModelPtr();
    Presolve pinfo;
    ClpSimplex * model2 = pinfo.presolvedModel(*clp,1.0e-8);
    if (!model2) {
      // presolve found to be infeasible
      feasible=false;
    } else {
      // update original array
      const int * originalColumns = pinfo.originalColumns();
      // just slot in new solver
      OsiClpSolverInterface * temp = new OsiClpSolverInterface(model2);
      numberColumns = temp->getNumCols();
      for (iColumn=0;iColumn<originalNumberColumns;iColumn++)
	original[iColumn]=-1;
      for (iColumn=0;iColumn<numberColumns;iColumn++)
	original[originalColumns[iColumn]]=iColumn;
      // copy parameters
      temp->copyParameters(*solver_);
      delete solver_;
      solver_ = temp;
      setCutoff(cutoff);
      deleteObjects();
      synchronizeModel(); // make sure everything that needs solver has it
      // just point to solver_
      continuousSolver_ = solver_;
      feasible=resolve();
      if (!feasible||!doIntegerPresolve) break;
      // see if we can get solution by heuristics
      int found=-1;
      int iHeuristic;
      double * newSolution = new double [numberColumns];
      double heuristicValue=getCutoff();
      for (iHeuristic=0;iHeuristic<numberHeuristics_;iHeuristic++) {
	double saveValue=heuristicValue;
	int ifSol = heuristic_[iHeuristic]->solution(heuristicValue,
						     newSolution);
	if (ifSol>0) {
	  // better solution found
	  found=iHeuristic;
	} else if (ifSol<0) {
	  heuristicValue = saveValue;
	}
      }
      if (found>=0) {
	// better solution save
	setBestSolution(SBB_ROUNDING,heuristicValue,
			newSolution);
	// update cutoff
	cutoff = getCutoff();
      }
      delete [] newSolution;
      // Space for type of cuts
      int maximumWhich=1000;
      int * whichGenerator = new int[maximumWhich];
      // save number of rows
      numberRowsAtContinuous_ = getNumRows();
      maximumNumberCuts_=0;
      currentNumberCuts_=0;
      delete [] addedCuts_;
      addedCuts_ = NULL;
      
      // maximum depth for tree walkback
      maximumDepth_=10;
      delete [] walkback_;
      walkback_ = new SbbNodeInfo * [maximumDepth_];
      
      OsiCuts cuts;
      int numberOldActiveCuts=0;
      int numberNewCuts = 0;
      feasible = solveWithCuts(cuts,maximumCutPassesAtRoot_,
			       NULL,numberOldActiveCuts,numberNewCuts,
			       maximumWhich, whichGenerator);
      currentNumberCuts_=numberNewCuts;
      delete [] whichGenerator;
      delete [] walkback_;
      walkback_ = NULL;
      delete [] addedCuts_;
      addedCuts_=NULL;
      if (feasible) {
	// fix anything in original which integer presolve fixed
	// for now just integers
	const double * lower = solver_->getColLower();
	const double * upper = solver_->getColUpper();
	int i;
	for (i=0;i<originalNumberIntegers;i++) {
	  iColumn = originalIntegers[i];
	  int jColumn = original[iColumn];
	  if (jColumn>=0) {
	    if (upper[jColumn]<originalUpper[iColumn]) 
	      originalUpper[iColumn]  = upper[jColumn];
	    if (lower[jColumn]>originalLower[iColumn]) 
	      originalLower[iColumn]  = lower[jColumn];
	  }
	}
      }
    }
    if (!feasible||!doIntegerPresolve) {
      break;
    }
  }
  //solver_->writeMps("xx");
  delete cleanModel;
  // create new priorities and save list of original columns
  if (originalPriority) {
    priority_ = new int[numberIntegers_];
    int i;
    int number=0;
    // integer variables are in same order if they exist at all
    for (i=0;i<originalNumberIntegers;i++) {
      iColumn = originalIntegers[i];
      int newColumn=original[iColumn];
      if (newColumn>=0) 
	priority_[number++]=originalPriority[i];
    }
    assert (number==numberIntegers_);
    delete [] originalPriority;
  }
  delete [] originalIntegers;
  numberColumns = getNumCols();
  delete [] originalColumns_;
  originalColumns_ = new int[numberColumns];
  numberColumns=0;
  for (iColumn=0;iColumn<originalNumberColumns;iColumn++) {
    int jColumn = original[iColumn];
    if (jColumn>=0) 
      originalColumns_[numberColumns++]=iColumn;
  }
  delete [] original;
  delete [] originalLower;
  delete [] originalUpper;
  
  deleteObjects();
  synchronizeModel(); // make sure everything that needs solver has it
  continuousSolver_=NULL;
  currentNumberCuts_=0;
  return feasible;
}
// Put back information into original model - after integerpresolve 
void 
SbbModel::originalModel(SbbModel * presolvedModel)
{
  solver_->copyParameters(*(presolvedModel->solver_));
  bestObjective_ = presolvedModel->bestObjective_;
  delete [] bestSolution_;
  findIntegers(true);
  if (presolvedModel->bestSolution_) {
    int numberColumns = getNumCols();
    int numberOtherColumns = presolvedModel->getNumCols();
    bestSolution_ = new double[numberColumns];
    // set up map
    int * back = new int[numberColumns];
    int i;
    for (i=0;i<numberColumns;i++)
      back[i]=-1;
    for (i=0;i<numberOtherColumns;i++)
      back[presolvedModel->originalColumns_[i]]=i;
    int iColumn;
    // set ones in presolved model to values
    double * otherSolution = presolvedModel->bestSolution_;
    for (i=0;i<numberIntegers_;i++) {
      iColumn = integerVariable_[i];
      int jColumn = back[iColumn];
      if (jColumn>=0) {
	double value=floor(otherSolution[jColumn]+0.5);
	solver_->setColLower(iColumn,value);
	solver_->setColUpper(iColumn,value);
      }
    }
#if 0
    // ** looks as if presolve needs more intelligence
    // do presolve - for now just clp but easy to get osi interface
    OsiClpSolverInterface * clpSolver 
      = dynamic_cast<OsiClpSolverInterface *> (solver_);
    assert (clpSolver);
    ClpSimplex * clp = clpSolver->getModelPtr();
    Presolve pinfo;
    ClpSimplex * model2 = pinfo.presolvedModel(*clp,1.0e-8);
    model2->primal(1);
    pinfo.postsolve(true);
    const double * solution = solver_->getColSolution();
    for (i=0;i<numberIntegers_;i++) {
      iColumn = integerVariable_[i];
      double value=floor(solution[iColumn]+0.5);
      solver_->setColLower(iColumn,value);
      solver_->setColUpper(iColumn,value);
    }
#else
    // for now give up
    int save = numberCutGenerators_;
    numberCutGenerators_=0;
    branchAndBound();
    numberCutGenerators_=save;
    
#endif
    // solve problem
    resolve();
    // should be feasible
    int numberIntegerInfeasibilities;
    int numberObjectInfeasibilities;
    assert(feasibleSolution(numberIntegerInfeasibilities,
			    numberObjectInfeasibilities));
  } else {
    bestSolution_=NULL;
  }
  numberSolutions_=presolvedModel->numberSolutions_;
  numberHeuristicSolutions_=presolvedModel->numberHeuristicSolutions_;
  numberNodes_ = presolvedModel->numberNodes_;
  numberIterations_ = presolvedModel->numberIterations_;
  status_ = presolvedModel->status_;
  synchronizeModel();
} 
// Pass in Message handler (not deleted at end)
void 
SbbModel::passInMessageHandler(CoinMessageHandler * handler)
{
  if (defaultHandler_) {
    delete handler_;
    handler_ = NULL;
  }
  defaultHandler_=false;
  handler_=handler;
}

