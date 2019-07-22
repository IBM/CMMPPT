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
#include "SbbBranchBase.hpp"


// Default Constructor
SbbObject::SbbObject() 
  :model_(NULL),
   id_(-1)
{
}

// Constructor from model
SbbObject::SbbObject(SbbModel * model)
:
  model_(model),
  id_(-1)
{
}


// Destructor 
SbbObject::~SbbObject ()
{
}

// Copy constructor 
SbbObject::SbbObject ( const SbbObject & rhs)
{
  model_ = rhs.model_;
  id_ = rhs.id_;
}

// Assignment operator 
SbbObject & 
SbbObject::operator=( const SbbObject& rhs)
{
  if (this!=&rhs) {
    model_ = rhs.model_;
    id_ = rhs.id_;
  }
  return *this;
}


// Default Constructor 
SbbBranchingObject::SbbBranchingObject()
{
  model_=NULL;
  variable_=-1;
  way_=0;
  value_=0.0;
  numberBranchesLeft_=0;
}

// Useful constructor
SbbBranchingObject::SbbBranchingObject (SbbModel * model, int variable, int way , double value)
{
  model_= model;
  variable_=variable;
  way_=way;
  value_=value;
  numberBranchesLeft_=2;
}

// Copy constructor 
SbbBranchingObject::SbbBranchingObject ( const SbbBranchingObject & rhs)
{
  model_=rhs.model_;
  variable_=rhs.variable_;
  way_=rhs.way_;
  value_=rhs.value_;
  numberBranchesLeft_=rhs.numberBranchesLeft_;
}

// Assignment operator 
SbbBranchingObject & 
SbbBranchingObject::operator=( const SbbBranchingObject& rhs)
{
  if (this != &rhs) {
    model_=rhs.model_;
    variable_=rhs.variable_;
    way_=rhs.way_;
    value_=rhs.value_;
    numberBranchesLeft_=rhs.numberBranchesLeft_;
  }
  return *this;
}

// Destructor 
SbbBranchingObject::~SbbBranchingObject ()
{
}
// Default Constructor 
SbbBranchDecision::SbbBranchDecision ()
{
}

SbbBranchDecision::~SbbBranchDecision()
{
}

  
