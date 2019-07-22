// file: allocLet.h
// author:  tom

#ifndef  LGFRSCEALLOCLET_H
#define  LGFRSCEALLOCLET_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <stdlib.h>
#include <sce/src/sceDefin.h>
// #include <sce/src/mPlantH.h>
// #include <wit/src/wit.h>


class WitRun;
class LgFrMultiPlantHelper;

class LgFrSceAllocLet {  
public:




  // set/get the Amount to try 
  void    qtyDemand(const float qty);
  float  qtyDemand() const;

  // set/get the Ship Amount 
  void      qtyShip(const float qty);
  float    qtyShip() const;


  // set/get the  demandPd 
  void   demandPd(const int reqPd);
  int    demandPd() const;

  // set/get the  shipPd 
  void   shipPd(const int shipPd);
  int    shipPd() const;

  // set/get the priority 
  void   priority(const int prio);
  int    priority() const;





  // get partName and demandName
  std::string    demandName() const;
  std::string    partName() const;

  // set partName and DemandName (as char *)
 void    demandName(char * dName);
 void    partName(char * pName);

  // get partName and demandName
  // Return 1 if Shipful, 0 oWise
  int    demandName(std::string & dName);
  int    partName(std::string &   pName);
  
  // set/get the allocated flag
  int   allocated() const;
  void  allocated(int allocFlag);

  // set/get the dependent 
  void backlogDependent(LgFrSceAllocLet * dependent);
  LgFrSceAllocLet * backlogDependent();



  // create a TRACE STRING
  std::string
  traceString();


   // create a Human Readable TRACE STRING
  std::string
  hrTraceString(WitRun * const theWitRun, 
		LgFrMultiPlantHelper * mpHelper, 
		int equitCount);

   


  // print the allocLet to stdout
  void
  print();




  

  // preferred constructor 
  LgFrSceAllocLet(std::string & partName, 
	       std::string & demandName,
	       int    priority,
	       int    demandPd,
               int    shipPd,
	       float  qtyDemand,
	       int    shipLateLimit);

  // Construct from a Trace String
  LgFrSceAllocLet(std::string & traceString);



  // destructor
  virtual
  ~LgFrSceAllocLet();


  // copy constructor
  LgFrSceAllocLet(const LgFrSceAllocLet& source);
  
  // assignment operator
  LgFrSceAllocLet&
  operator=(const LgFrSceAllocLet& rhs);
  
  // default constructor
  LgFrSceAllocLet();
  
  
  
private:
  std::string partName_;
  std::string demandName_;
  int    priority_;
  int    demandPd_;
  int    shipPd_;  
  float  qtyDemand_;
  float  qtyShip_;
  int    shipLateLimit_;
  int    allocated_;            // 1=allocated, 0=unallocated
  LgFrSceAllocLet * backlogDependent_;

  // --- FUTURE USE ----
  // LgFrSceAllocLetBacklogStrategy * backlogStrategy_;      // shipLateLimit
  // int partIndex_;                                      // for user convenience
  // int demandIndex_;                                    // for user convenience
};



#endif
















