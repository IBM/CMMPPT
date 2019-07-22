#ifndef LGFR_DEMAND_PERIOD_PAIR_H
#define LGFR_DEMAND_PERIOD_PAIR_H
// Author: Wally

// This class is for representing demand-period pairs.

#include <scenario/src/demand.h>

// This class contains demand and period (period number)
class LGFR_DECLSPEC LgFrDemandPeriodPair {
  
       
public:

  virtual
  bool
  isEqual(const LgFrDemandPeriodPair*) 
       const;
       
  // compareTo method, uses DemandPeriodPair name to to the comparison.
  // should return 0  if this->name_ "is equal to" source->name_.
  //               >0 if this->name_ "is larger" than source->name_.
  //               <0 if this->name_ "is smaller" than source->name_.
       
  virtual int compareTo(const LgFrDemandPeriodPair*) const;
       
  // Set and Get methods
  // Demand
  void demand (const LgFrDemand&);		// Set
  const LgFrDemand& demand () const;		// Get
  // Period
  void period (const int&);			// Set
  const int& period () const;			// Get
  
  // Constructor that sets both parts
  LgFrDemandPeriodPair(const LgFrDemand& item, const int& p);
  
  // default constructor
  LgFrDemandPeriodPair();
  
  // Copy constructor
  LgFrDemandPeriodPair (const LgFrDemandPeriodPair& source);
  
  // destructor
  ~LgFrDemandPeriodPair();
  
  // Assignment 
  LgFrDemandPeriodPair& 
  operator=(const LgFrDemandPeriodPair& rhs);
  
  // Equality
  virtual
  int
  operator==(const LgFrDemandPeriodPair&)
       const;
       
  // Print DemandPeriodPair data
  virtual void print() const;
       
  // test this class
  static void test();
  
private:
  LgFrDemand        demand_;
  int               period_;
};

#endif
