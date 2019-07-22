// Copyright (C) 2002, International Business Machines
// Corporation and others.  All Rights Reserved.
#if defined(_MSC_VER)
// Turn off compiler warning about long names
#  pragma warning(disable:4786)
#endif
#include <cassert>
#include <cmath>
#include <cfloat>

#include "OsiSolverInterface.hpp"
#include "SbbModel.hpp"
#include "SbbMessage.hpp"
#include "SbbBranchActual.hpp"
#include "CoinSort.hpp"

// Default Constructor 
SbbClique::SbbClique ()
  : SbbObject(),
    numberMembers_(0),
    numberNonSOSMembers_(0),
    members_(NULL),
    type_(NULL),
    cliqueType_(-1),
    slack_(-1)
{
}

// Useful constructor (which are integer indices)
SbbClique::SbbClique (SbbModel * model, int cliqueType, int numberMembers,
	   const int * which, const char * type, int identifier,int slack)
  : SbbObject(model)
{
  id_=identifier;
  numberMembers_=numberMembers;
  if (numberMembers_) {
    members_ = new int[numberMembers_];
    memcpy(members_,which,numberMembers_*sizeof(int));
    type_ = new char[numberMembers_];
    memcpy(type_,type,numberMembers_*sizeof(char));
  } else {
    members_ = NULL;
    type_ = NULL;
  }
  // Find out how many non sos
  int i;
  numberNonSOSMembers_=0;
  for (i=0;i<numberMembers_;i++)
    if (!type_[i])
      numberNonSOSMembers_++;
  cliqueType_ = cliqueType;
  slack_ = slack;
}

// Copy constructor 
SbbClique::SbbClique ( const SbbClique & rhs)
  :SbbObject(rhs)
{
  numberMembers_ = rhs.numberMembers_;
  numberNonSOSMembers_ = rhs.numberNonSOSMembers_;
  if (numberMembers_) {
    members_ = new int[numberMembers_];
    memcpy(members_,rhs.members_,numberMembers_*sizeof(int));
    type_ = new char[numberMembers_];
    memcpy(type_,rhs.type_,numberMembers_*sizeof(char));
  } else {
    members_ = NULL;
    type_ = NULL;
  }
  cliqueType_ = rhs.cliqueType_;
  slack_ = rhs.slack_;
}

// Clone
SbbObject *
SbbClique::clone() const
{
  return new SbbClique(*this);
}

// Assignment operator 
SbbClique & 
SbbClique::operator=( const SbbClique& rhs)
{
  if (this!=&rhs) {
    SbbObject::operator=(rhs);
    delete [] members_;
    delete [] type_;
    numberMembers_ = rhs.numberMembers_;
    numberNonSOSMembers_ = rhs.numberNonSOSMembers_;
    if (numberMembers_) {
      members_ = new int[numberMembers_];
      memcpy(members_,rhs.members_,numberMembers_*sizeof(int));
      type_ = new char[numberMembers_];
      memcpy(type_,rhs.type_,numberMembers_*sizeof(char));
    } else {
      members_ = NULL;
      type_ = NULL;
    }
    cliqueType_ = rhs.cliqueType_;
    slack_ = rhs.slack_;
  }
  return *this;
}

// Destructor 
SbbClique::~SbbClique ()
{
  delete [] members_;
  delete [] type_;
}

// Infeasibility - large is 0.5
double 
SbbClique::infeasibility(int & preferredWay, double & otherWay) const
{
  int numberUnsatis=0, numberFree=0;
  int j;
  const int * integer = model_->integerVariable();
  OsiSolverInterface * solver = model_->solver();
  const double * solution = model_->currentSolution();
  const double * lower = solver->getColLower();
  const double * upper = solver->getColUpper();
  double largestValue=0.0;
  double integerTolerance = 
    model_->getDblParam(SbbModel::SbbIntegerTolerance);
  double * sort = new double[numberMembers_];

  double slackValue=0.0;
  for (j=0;j<numberMembers_;j++) {
    int sequence = members_[j];
    int iColumn = integer[sequence];
    double value = solution[iColumn];
    value = max(value, lower[iColumn]);
    value = min(value, upper[iColumn]);
    double nearest = floor(value+0.5);
    double distance = fabs(value-nearest);
    if (distance>integerTolerance) {
      if (!type_[j])
	value = 1.0-value; // non SOS
      // if slack then choose that
      if (j==slack_&&value>0.05)
	slackValue = value;
      largestValue = max(value,largestValue);
      sort[numberUnsatis++]=-value;
    } else if (upper[iColumn]>lower[iColumn]) {
      numberFree++;
    }
  }
  preferredWay=1;
  otherWay = 0.0;
  if (numberUnsatis) {
    // sort
    std::sort(sort,sort+numberUnsatis);
    for (j=0;j<numberUnsatis;j++) {
      if ((j&1)!=0)
	otherWay += -sort[j];
    }
    // Need to think more
    double value = 0.2*numberUnsatis+0.01*(numberMembers_-numberFree);
    if (fabs(largestValue-0.5)<0.1) {
      // close to half
      value +=0.1;
    }
    if (slackValue) {
      // branching on slack
      value += slackValue;
    }
    // scale other way
    otherWay *= value/(1.0-otherWay);
    delete [] sort;
    return value;
  } else {
    delete [] sort;
    return 0.0; // satisfied
  }
}

// This looks at solution and sets bounds to contain solution
void 
SbbClique::feasibleRegion()
{
  int j;
  const int * integer = model_->integerVariable();
  OsiSolverInterface * solver = model_->solver();
  const double * solution = model_->currentSolution();
  const double * lower = solver->getColLower();
  const double * upper = solver->getColUpper();
  double integerTolerance = 
    model_->getDblParam(SbbModel::SbbIntegerTolerance);
  
  for (j=0;j<numberMembers_;j++) {
    int sequence = members_[j];
    int iColumn = integer[sequence];
    double value = solution[iColumn];
    value = max(value, lower[iColumn]);
    value = min(value, upper[iColumn]);
    double nearest = floor(value+0.5);
    double distance = fabs(value-nearest);
    assert(distance<=integerTolerance);
    solver->setColLower(iColumn,nearest);
    solver->setColUpper(iColumn,nearest);
  }
}


// Creates a branching object
SbbBranchingObject * 
SbbClique::createBranch(int way) const
{
  int numberUnsatis=0;
  int j;
  int nUp=0;
  int nDown=0;
  int numberFree=numberMembers_;
  const int * integer = model_->integerVariable();
  OsiSolverInterface * solver = model_->solver();
  const double * solution = model_->currentSolution();
  const double * lower = solver->getColLower();
  const double * upper = solver->getColUpper();
  int * upList = new int[numberMembers_];
  int * downList = new int[numberMembers_];
  double * sort = new double[numberMembers_];
  double integerTolerance = 
      model_->getDblParam(SbbModel::SbbIntegerTolerance);

  double slackValue=0.0;
  for (j=0;j<numberMembers_;j++) {
    int sequence = members_[j];
    int iColumn = integer[sequence];
    double value = solution[iColumn];
    value = max(value, lower[iColumn]);
    value = min(value, upper[iColumn]);
    double nearest = floor(value+0.5);
    double distance = fabs(value-nearest);
    if (distance>integerTolerance) {
      if (!type_[j])
	value = 1.0-value; // non SOS
      // if slack then choose that
      if (j==slack_&&value>0.05)
	slackValue = value;
      value = -value; // for sort
      upList[numberUnsatis]=j;
      sort[numberUnsatis++]=value;
    } else if (upper[iColumn]>lower[iColumn]) {
      upList[--numberFree]=j;
    }
  }
  assert (numberUnsatis);
  if (!slackValue) {
    // sort
    CoinSort_2(sort,sort+numberUnsatis,upList);
    // put first in up etc
    int kWay=1;
    for (j=0;j<numberUnsatis;j++) {
      if (kWay>0) 
	upList[nUp++]=upList[j];
      else
	downList[nDown++]=upList[j];
      kWay = -kWay;
    }
    for (j=numberFree;j<numberMembers_;j++) {
      if (kWay>0)
	upList[nUp++]=upList[j];
      else
	downList[nDown++]=upList[j];
      kWay = -kWay;
    }
  } else {
    // put slack to 0 in first way
    nUp = 1;
    upList[0]=slack_;
    for (j=0;j<numberUnsatis;j++) {
      downList[nDown++]=upList[j];
    }
    for (j=numberFree;j<numberMembers_;j++) {
      downList[nDown++]=upList[j];
    }
  }
  // create object
  SbbBranchingObject * branch;
  if (numberMembers_ <=64)
     branch = new SbbCliqueBranchingObject(model_,this,way,
					 nDown,downList,nUp,upList);
  else
    branch = new SbbLongCliqueBranchingObject(model_,this,way,
					    nDown,downList,nUp,upList);
  delete [] upList;
  delete [] downList;
  delete [] sort;
  return branch;
}



// Default Constructor 
SbbSimpleInteger::SbbSimpleInteger ()
  : SbbObject(),
    sequence_(-1),
    columnNumber_(-1),
    originalLower_(0.0),
    originalUpper_(1.0)
{
}

// Useful constructor (which are integer indices)
SbbSimpleInteger::SbbSimpleInteger (SbbModel * model, int sequence,
				    int iColumn)
  : SbbObject(model)
{
  sequence_ = sequence;
  columnNumber_ = iColumn;
  originalLower_ = model_->solver()->getColLower()[iColumn];
  originalUpper_ = model_->solver()->getColUpper()[iColumn];
}

// Copy constructor 
SbbSimpleInteger::SbbSimpleInteger ( const SbbSimpleInteger & rhs)
  :SbbObject(rhs)

{
  sequence_ = rhs.sequence_;
  columnNumber_ = model_->integerVariable()[sequence_];
  originalLower_ = rhs.originalLower_;
  originalUpper_ = rhs.originalUpper_;
}

// Clone
SbbObject *
SbbSimpleInteger::clone() const
{
  return new SbbSimpleInteger(*this);
}

// Assignment operator 
SbbSimpleInteger & 
SbbSimpleInteger::operator=( const SbbSimpleInteger& rhs)
{
  if (this!=&rhs) {
    SbbObject::operator=(rhs);
    columnNumber_ = model_->integerVariable()[sequence_];
  }
  return *this;
}

// Destructor 
SbbSimpleInteger::~SbbSimpleInteger ()
{
}

// Infeasibility - large is 0.5
double 
SbbSimpleInteger::infeasibility(int & preferredWay, double & otherWay) const
{
  OsiSolverInterface * solver = model_->solver();
  const double * solution = model_->currentSolution();
  const double * lower = solver->getColLower();
  const double * upper = solver->getColUpper();
  double value = solution[columnNumber_];
  value = max(value, lower[columnNumber_]);
  value = min(value, upper[columnNumber_]);
  /*printf("%d %g %g %g %g\n",columnNumber_,value,lower[columnNumber_],
    solution[columnNumber_],upper[columnNumber_]);*/
  double nearest = floor(value+0.5);
  double integerTolerance = 
    model_->getDblParam(SbbModel::SbbIntegerTolerance);
  if (nearest>value) 
    preferredWay=1;
  else
    preferredWay=-1;
  otherWay = 1.0-fabs(value-nearest);
  if (fabs(value-nearest)<=integerTolerance) 
    return 0.0;
  else
    return fabs(value-nearest);
}

// This looks at solution and sets bounds to contain solution
void 
SbbSimpleInteger::feasibleRegion()
{
  OsiSolverInterface * solver = model_->solver();
  const double * lower = solver->getColLower();
  const double * upper = solver->getColUpper();
  const double * solution = model_->currentSolution();
  double value = solution[columnNumber_];
  value = max(value, lower[columnNumber_]);
  value = min(value, upper[columnNumber_]);

  double nearest = floor(value+0.5);
  double integerTolerance = 
    model_->getDblParam(SbbModel::SbbIntegerTolerance);
  // Scaling may have moved it a bit
  assert (fabs(value-nearest)<=100.0*integerTolerance);
  solver->setColLower(columnNumber_,nearest);
  solver->setColUpper(columnNumber_,nearest);
}

// Creates a branching object
SbbBranchingObject * 
SbbSimpleInteger::createBranch(int way) const
{
  OsiSolverInterface * solver = model_->solver();
  const double * solution = model_->currentSolution();
  const double * lower = solver->getColLower();
  const double * upper = solver->getColUpper();
  double value = solution[columnNumber_];
  value = max(value, lower[columnNumber_]);
  value = min(value, upper[columnNumber_]);
  double nearest = floor(value+0.5);
  double integerTolerance = 
    model_->getDblParam(SbbModel::SbbIntegerTolerance);
  assert (fabs(value-nearest)>integerTolerance);
  return new SbbIntegerBranchingObject(model_,sequence_,way,
					     value);
}


/* Given valid solution (i.e. satisfied) and reduced costs etc
   returns a branching object which would give a new feasible
   point in direction reduced cost says would be cheaper.
   If no feasible point returns null
*/
SbbBranchingObject * 
SbbSimpleInteger::preferredNewFeasible() const
{
  OsiSolverInterface * solver = model_->solver();
  double value = model_->currentSolution()[columnNumber_];

  double nearest = floor(value+0.5);
  double integerTolerance = 
    model_->getDblParam(SbbModel::SbbIntegerTolerance);
  assert (fabs(value-nearest)<=integerTolerance);
  double dj = solver->getObjSense()*solver->getReducedCost()[columnNumber_];
  SbbIntegerBranchingObject * object = NULL;
  if (dj>=0.0) {
    // can we go down
    if (nearest>originalLower_+0.5) {
      // yes
      object = new SbbIntegerBranchingObject(model_,sequence_,-1,
					     nearest-1.0,nearest-1.0);
    }
  } else {
    // can we go up
    if (nearest<originalUpper_-0.5) {
      // yes
      object = new SbbIntegerBranchingObject(model_,sequence_,-1,
					     nearest+1.0,nearest+1.0);
    }
  }
  return object;
}
  
/* Given valid solution (i.e. satisfied) and reduced costs etc
   returns a branching object which would give a new feasible
   point in direction opposite to one reduced cost says would be cheaper.
   If no feasible point returns null
*/
SbbBranchingObject * 
SbbSimpleInteger::notPreferredNewFeasible() const 
{
  OsiSolverInterface * solver = model_->solver();
  double value = model_->currentSolution()[columnNumber_];

  double nearest = floor(value+0.5);
  double integerTolerance = 
    model_->getDblParam(SbbModel::SbbIntegerTolerance);
  assert (fabs(value-nearest)<=integerTolerance);
  double dj = solver->getObjSense()*solver->getReducedCost()[columnNumber_];
  SbbIntegerBranchingObject * object = NULL;
  if (dj<=0.0) {
    // can we go down
    if (nearest>originalLower_+0.5) {
      // yes
      object = new SbbIntegerBranchingObject(model_,sequence_,-1,
					     nearest-1.0,nearest-1.0);
    }
  } else {
    // can we go up
    if (nearest<originalUpper_-0.5) {
      // yes
      object = new SbbIntegerBranchingObject(model_,sequence_,-1,
					     nearest+1.0,nearest+1.0);
    }
  }
  return object;
}
  
/* Bounds may be tightened - so it may be good to reset original bounds
 */
void 
SbbSimpleInteger::resetBounds()
{
  originalLower_ = model_->solver()->getColLower()[sequence_];
  originalUpper_ = model_->solver()->getColUpper()[sequence_];
}


// Default Constructor 
SbbIntegerBranchingObject::SbbIntegerBranchingObject()
  :SbbBranchingObject()
{
  down_[0] = 0.0;
  down_[1] = 0.0;
  up_[0] = 0.0;
  up_[1] = 0.0;
}

// Useful constructor
SbbIntegerBranchingObject::SbbIntegerBranchingObject (SbbModel * model, 
						      int variable, int way , double value)
  :SbbBranchingObject(model,variable,way,value)
{
  int iColumn = model_->integerVariable()[variable_];
  down_[0] = model_->solver()->getColLower()[iColumn];
  down_[1] = floor(value_);
  up_[0] = ceil(value_);
  up_[1] = model->getColUpper()[iColumn];
}
// Useful constructor for fixing
SbbIntegerBranchingObject::SbbIntegerBranchingObject (SbbModel * model, 
						      int variable, int way,
						      double lowerValue, 
						      double upperValue)
  :SbbBranchingObject(model,variable,way,lowerValue)
{
  numberBranchesLeft_=1;
  down_[0] = lowerValue;
  down_[1] = upperValue;
  up_[0] = lowerValue;
  up_[1] = upperValue;
}
  

// Copy constructor 
SbbIntegerBranchingObject::SbbIntegerBranchingObject ( const SbbIntegerBranchingObject & rhs) :SbbBranchingObject(rhs)
{
  down_[0] = rhs.down_[0];
  down_[1] = rhs.down_[1];
  up_[0] = rhs.up_[0];
  up_[1] = rhs.up_[1];
}

// Assignment operator 
SbbIntegerBranchingObject & 
SbbIntegerBranchingObject::operator=( const SbbIntegerBranchingObject& rhs)
{
  if (this != &rhs) {
    SbbBranchingObject::operator=(rhs);
    down_[0] = rhs.down_[0];
    down_[1] = rhs.down_[1];
    up_[0] = rhs.up_[0];
    up_[1] = rhs.up_[1];
  }
  return *this;
}
SbbBranchingObject * 
SbbIntegerBranchingObject::clone() const
{ 
  return (new SbbIntegerBranchingObject(*this));
}


// Destructor 
SbbIntegerBranchingObject::~SbbIntegerBranchingObject ()
{
}
void
SbbIntegerBranchingObject::branch()
{
  numberBranchesLeft_--;
  int iColumn = model_->integerVariable()[variable_];
  if (way_<0) {
    model_->solver()->setColLower(iColumn,down_[0]);
    model_->solver()->setColUpper(iColumn,down_[1]);
#ifdef SBB_DEBUG
    printf("branching down on %d\n",variable_);
#endif
    way_=1;	  // Swap direction
  } else {
    model_->solver()->setColLower(iColumn,up_[0]);
    model_->solver()->setColUpper(iColumn,up_[1]);
#ifdef SBB_DEBUG
    printf("branching up on %d\n",variable_);
#endif
    way_=-1;	  // Swap direction
  }
}
  
// Default Constructor 
SbbCliqueBranchingObject::SbbCliqueBranchingObject()
  :SbbBranchingObject()
{
  clique_ = NULL;
  downMask_[0]=0;
  downMask_[1]=0;
  upMask_[0]=0;
  upMask_[1]=0;
}

// Useful constructor
SbbCliqueBranchingObject::SbbCliqueBranchingObject (SbbModel * model,
						    const SbbClique * clique,
						    int way ,
						    int numberOnDownSide, const int * down,
						    int numberOnUpSide, const int * up)
  :SbbBranchingObject(model,clique->id(),way,0.5)
{
  clique_ = clique;
  downMask_[0]=0;
  downMask_[1]=0;
  upMask_[0]=0;
  upMask_[1]=0;
  int i;
  for (i=0;i<numberOnDownSide;i++) {
    int sequence = down[i];
    int iWord = sequence>>5;
    int iBit = sequence - 32*iWord;
    unsigned int k = 1<<iBit;
    downMask_[iWord] |= k;
  }
  for (i=0;i<numberOnUpSide;i++) {
    int sequence = up[i];
    int iWord = sequence>>5;
    int iBit = sequence - 32*iWord;
    unsigned int k = 1<<iBit;
    upMask_[iWord] |= k;
  }
}

// Copy constructor 
SbbCliqueBranchingObject::SbbCliqueBranchingObject ( const SbbCliqueBranchingObject & rhs) :SbbBranchingObject(rhs)
{
  clique_=rhs.clique_;
  downMask_[0]=rhs.downMask_[0];
  downMask_[1]=rhs.downMask_[1];
  upMask_[0]=rhs.upMask_[0];
  upMask_[1]=rhs.upMask_[1];
}

// Assignment operator 
SbbCliqueBranchingObject & 
SbbCliqueBranchingObject::operator=( const SbbCliqueBranchingObject& rhs)
{
  if (this != &rhs) {
    SbbBranchingObject::operator=(rhs);
    clique_=rhs.clique_;
    downMask_[0]=rhs.downMask_[0];
    downMask_[1]=rhs.downMask_[1];
    upMask_[0]=rhs.upMask_[0];
    upMask_[1]=rhs.upMask_[1];
  }
  return *this;
}
SbbBranchingObject * 
SbbCliqueBranchingObject::clone() const
{ 
  return (new SbbCliqueBranchingObject(*this));
}


// Destructor 
SbbCliqueBranchingObject::~SbbCliqueBranchingObject ()
{
}
void
SbbCliqueBranchingObject::branch()
{
  numberBranchesLeft_--;
  int iWord;
  int numberMembers = clique_->numberMembers();
  const int * which = clique_->members();
  const int * integerVariables = model_->integerVariable();
  int numberWords=(numberMembers+31)>>5;
  // *** for way - up means fix all those in down section
  if (way_<0) {
#ifdef FULL_PRINT
    printf("Down Fix ");
#endif
    for (iWord=0;iWord<numberWords;iWord++) {
      int i;
      for (i=0;i<32;i++) {
	unsigned int k = 1<<i;
	if ((upMask_[iWord]&k)!=0) {
	  int iColumn = which[i+32*iWord];
#ifdef FULL_PRINT
	  printf("%d ",i+32*iWord);
#endif
	  // fix weak way
	  if (clique_->type(i+32*iWord))
	    model_->solver()->setColUpper(integerVariables[iColumn],0.0);
	  else
	    model_->solver()->setColLower(integerVariables[iColumn],1.0);
	}
      }
    }
    way_=1;	  // Swap direction
  } else {
#ifdef FULL_PRINT
    printf("Up Fix ");
#endif
    for (iWord=0;iWord<numberWords;iWord++) {
      int i;
      for (i=0;i<32;i++) {
	unsigned int k = 1<<i;
	if ((downMask_[iWord]&k)!=0) {
	  int iColumn = which[i+32*iWord];
#ifdef FULL_PRINT
	  printf("%d ",i+32*iWord);
#endif
	  // fix weak way
	  if (clique_->type(i+32*iWord))
	    model_->solver()->setColUpper(integerVariables[iColumn],0.0);
	  else
	    model_->solver()->setColLower(integerVariables[iColumn],1.0);
	}
      }
    }
    way_=-1;	  // Swap direction
  }
#ifdef FULL_PRINT
  printf("\n");
#endif
}
  
// Default Constructor 
SbbLongCliqueBranchingObject::SbbLongCliqueBranchingObject()
  :SbbBranchingObject()
{
  clique_=NULL;
  downMask_=NULL;
  upMask_=NULL;
}

// Useful constructor
SbbLongCliqueBranchingObject::SbbLongCliqueBranchingObject (SbbModel * model,
							    const SbbClique * clique, 
							    int way ,
						    int numberOnDownSide, const int * down,
						    int numberOnUpSide, const int * up)
  :SbbBranchingObject(model,clique->id(),way,0.5)
{
  clique_ = clique;
  int numberMembers = clique_->numberMembers();
  int numberWords=(numberMembers+31)>>5;
  downMask_ = new unsigned int [numberWords];
  upMask_ = new unsigned int [numberWords];
  memset(downMask_,0,numberWords*sizeof(unsigned int));
  memset(upMask_,0,numberWords*sizeof(unsigned int));
  int i;
  for (i=0;i<numberOnDownSide;i++) {
    int sequence = down[i];
    int iWord = sequence>>5;
    int iBit = sequence - 32*iWord;
    unsigned int k = 1<<iBit;
    downMask_[iWord] |= k;
  }
  for (i=0;i<numberOnUpSide;i++) {
    int sequence = up[i];
    int iWord = sequence>>5;
    int iBit = sequence - 32*iWord;
    unsigned int k = 1<<iBit;
    upMask_[iWord] |= k;
  }
}

// Copy constructor 
SbbLongCliqueBranchingObject::SbbLongCliqueBranchingObject ( const SbbLongCliqueBranchingObject & rhs) :SbbBranchingObject(rhs)
{
  clique_=rhs.clique_;
  if (rhs.downMask_) {
    int numberMembers = clique_->numberMembers();
    int numberWords=(numberMembers+31)>>5;
    downMask_ = new unsigned int [numberWords];
    memcpy(downMask_,rhs.downMask_,numberWords*sizeof(unsigned int));
    upMask_ = new unsigned int [numberWords];
    memcpy(upMask_,rhs.upMask_,numberWords*sizeof(unsigned int));
  } else {
    downMask_=NULL;
    upMask_=NULL;
  }    
}

// Assignment operator 
SbbLongCliqueBranchingObject & 
SbbLongCliqueBranchingObject::operator=( const SbbLongCliqueBranchingObject& rhs)
{
  if (this != &rhs) {
    SbbBranchingObject::operator=(rhs);
    clique_=rhs.clique_;
    delete [] downMask_;
    delete [] upMask_;
    if (rhs.downMask_) {
      int numberMembers = clique_->numberMembers();
      int numberWords=(numberMembers+31)>>5;
      downMask_ = new unsigned int [numberWords];
      memcpy(downMask_,rhs.downMask_,numberWords*sizeof(unsigned int));
      upMask_ = new unsigned int [numberWords];
      memcpy(upMask_,rhs.upMask_,numberWords*sizeof(unsigned int));
    } else {
      downMask_=NULL;
      upMask_=NULL;
    }    
  }
  return *this;
}
SbbBranchingObject * 
SbbLongCliqueBranchingObject::clone() const
{ 
  return (new SbbLongCliqueBranchingObject(*this));
}


// Destructor 
SbbLongCliqueBranchingObject::~SbbLongCliqueBranchingObject ()
{
  delete [] downMask_;
  delete [] upMask_;
}
void
SbbLongCliqueBranchingObject::branch()
{
  numberBranchesLeft_--;
  int iWord;
  int numberMembers = clique_->numberMembers();
  const int * which = clique_->members();
  const int * integerVariables = model_->integerVariable();
  int numberWords=(numberMembers+31)>>5;
  // *** for way - up means fix all those in down section
  if (way_<0) {
#ifdef FULL_PRINT
    printf("Down Fix ");
#endif
    for (iWord=0;iWord<numberWords;iWord++) {
      int i;
      for (i=0;i<32;i++) {
	unsigned int k = 1<<i;
	if ((upMask_[iWord]&k)!=0) {
	  int iColumn = which[i+32*iWord];
#ifdef FULL_PRINT
	  printf("%d ",i+32*iWord);
#endif
	  // fix weak way
	  if (clique_->type(i+32*iWord))
	    model_->solver()->setColUpper(integerVariables[iColumn],0.0);
	  else
	    model_->solver()->setColLower(integerVariables[iColumn],1.0);
	}
      }
    }
    way_=1;	  // Swap direction
  } else {
#ifdef FULL_PRINT
    printf("Up Fix ");
#endif
    for (iWord=0;iWord<numberWords;iWord++) {
      int i;
      for (i=0;i<32;i++) {
	unsigned int k = 1<<i;
	if ((downMask_[iWord]&k)!=0) {
	  int iColumn = which[i+32*iWord];
#ifdef FULL_PRINT
	  printf("%d ",i+32*iWord);
#endif
	  // fix weak way
	  if (clique_->type(i+32*iWord))
	    model_->solver()->setColUpper(integerVariables[iColumn],0.0);
	  else
	    model_->solver()->setColLower(integerVariables[iColumn],1.0);
	}
      }
    }
    way_=-1;	  // Swap direction
  }
#ifdef FULL_PRINT
  printf("\n");
#endif
}
// Default Constructor 
SbbBranchDefaultDecision::SbbBranchDefaultDecision()
  :SbbBranchDecision()
{
  bestCriterion_ = 0.0;
  bestChangeUp_ = 0.0;
  bestNumberUp_ = 0;
  bestChangeDown_ = 0.0;
  bestNumberDown_ = 0;
  bestObject_ = NULL;
}

// Copy constructor 
SbbBranchDefaultDecision::SbbBranchDefaultDecision (
				    const SbbBranchDefaultDecision & rhs)
  :SbbBranchDecision()
{
  bestCriterion_ = rhs.bestCriterion_;
  bestChangeUp_ = rhs.bestChangeUp_;
  bestNumberUp_ = rhs.bestNumberUp_;
  bestChangeDown_ = rhs.bestChangeDown_;
  bestNumberDown_ = rhs.bestNumberDown_;
  bestObject_ = rhs.bestObject_;
}

SbbBranchDefaultDecision::~SbbBranchDefaultDecision()
{
}

// Clone
SbbBranchDecision * 
SbbBranchDefaultDecision::clone() const
{
  return new SbbBranchDefaultDecision(*this);
}

// Initialize i.e. before start of choosing at a node
void 
SbbBranchDefaultDecision::initialize(SbbModel * model)
{
  bestCriterion_ = 0.0;
  bestChangeUp_ = 0.0;
  bestNumberUp_ = 0;
  bestChangeDown_ = 0.0;
  bestNumberDown_ = 0;
  bestObject_ = NULL;
}


/* Returns nonzero if branching on first object is "better" than on
   second (if second NULL first wins). User can play with decision object.
   This is only used after strong branching.  The initial selection
   is done by infeasibility() for each SbbObject
   return code +1 for up branch preferred, -1 for down
   
*/
int
SbbBranchDefaultDecision::betterBranch(SbbBranchingObject * thisOne,
			    SbbBranchingObject * bestSoFar,
			    double changeUp, int numberInfeasibilitiesUp,
			    double changeDown, int numberInfeasibilitiesDown)
{
  bool beforeSolution = thisOne->model()->getSolutionCount()==
    thisOne->model()->getNumberHeuristicSolutions();;
  int betterWay=0;
  if (beforeSolution) {
    if (!bestObject_) {
      bestNumberUp_=INT_MAX;
      bestNumberDown_=INT_MAX;
    }
    // before solution - choose smallest number 
    // could add in depth as well
    int bestNumber = min(bestNumberUp_,bestNumberDown_);
    if (numberInfeasibilitiesUp<numberInfeasibilitiesDown) {
      if (numberInfeasibilitiesUp<bestNumber) {
	betterWay = 1;
      } else if (numberInfeasibilitiesUp==bestNumber) {
	if (changeUp<bestCriterion_)
	  betterWay=1;
      }
    } else if (numberInfeasibilitiesUp>numberInfeasibilitiesDown) {
      if (numberInfeasibilitiesDown<bestNumber) {
	betterWay = -1;
      } else if (numberInfeasibilitiesDown==bestNumber) {
	if (changeDown<bestCriterion_)
	  betterWay=-1;
      }
    } else {
      // up and down have same number
      bool better=false;
      if (numberInfeasibilitiesUp<bestNumber) {
	better=true;
      } else if (numberInfeasibilitiesUp==bestNumber) {
	if (min(changeUp,changeDown)<bestCriterion_)
	  better=true;;
      }
      if (better) {
	// see which way
	if (changeUp<=changeDown)
	  betterWay=1;
	else
	  betterWay=-1;
      }
    }
  } else {
    if (!bestObject_) {
      bestCriterion_=-1.0;
    }
    // got a solution
    if (changeUp<=changeDown) {
      if (changeUp>bestCriterion_)
	betterWay=1;
    } else {
      if (changeDown>bestCriterion_)
	betterWay=-1;
    }
  }
  if (betterWay) {
    bestCriterion_ = min(changeUp,changeDown);
    bestChangeUp_ = changeUp;
    bestNumberUp_ = numberInfeasibilitiesUp;
    bestChangeDown_ = changeDown;
    bestNumberDown_ = numberInfeasibilitiesDown;
    bestObject_=thisOne;
  }
  return betterWay;
}
