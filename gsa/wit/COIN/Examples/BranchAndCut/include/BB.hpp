// Copyright (C) 2001, International Business Machines
// Corporation and others.  All Rights Reserved.
#ifndef _BB_H
#define _BB_H

class CoinPackedMatrix;
class BCP_buffer;

class BB_prob {
private:
   BB_prob(const BB_prob&);
   BB_prob& operator=(const BB_prob&);

public:
   int rownum;
   int colnum;

   // rim vectors for variables
   bool* integer;    // integrality information
   double* clb;      // lower bound
   double* cub;      // upper bound
   double* obj;      // objective coefficient

   // rim vectors for constraints
   double* rlb_core;      // lower bounds for core constraints
   double* rub_core;      // upper bounds for core constraints
   double* rlb_indexed;   // lower bounds for indexed constraints
   double* rub_indexed;   // upper bounds for indexed constraints

   // the core part of the matrix
   CoinPackedMatrix* core;
   // the part that will be the indexed cuts
   CoinPackedMatrix* indexed;

public:
   BB_prob();
   ~BB_prob();
};
   
#endif
