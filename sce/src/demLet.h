// file: allocLet.h
// author:  tom

#ifndef  LGFRSCEDEMLET_H
#define  LGFRSCEDEMLET_H

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <stdlib.h>
#include <vector>
#include <sce/src/sceDefin.h>


// class WitRun;
// class LgFrMultiPlantHelper;

class LgFrSceDemLet {  
public:
  // gets
  std::string partName() const;
  std::string witPartName() const;
  std::string witDemandName() const;
  std::string sourceLoc() const;
  std::string customerLoc() const;
  std::string demandClass() const;
  std::string demandLevel() const;
  std::string partClass() const;
  int         demandPd() const;
  float       demandVol() const;
  int         priority() const;
  std::vector<int> * ruleSignature();

  // sets
  void  partName(std::string & partName);
  void  witPartName(std::string & witPartName);
  void  witDemandName(std::string & witDemandName);
  void  sourceLoc(std::string & sourceLoc);
  void  customerLoc(std::string & customerLoc);
  void  demandClass(std::string & demandClass);
  void  demandLevel(std::string & demandLevel);
  void  partClass(std::string & partClass);
  void  demandPd(int demandPd);
  void  demandVol(float demandVol);
  void  priority(int priority);
  void  ruleSignature(std::vector<int> & ruleSig);

  /*
  // create a TRACE STRING
  std::string
  traceString();


   // create a Human Readable TRACE STRING
  std::string
  hrTraceString(WitRun * const theWitRun, 
		LgFrMultiPlantHelper * mpHelper, 
		int equitCount);

  */
   


  // print the demLet to stdout
  void
  print();




  

  // preferred constructor 
  LgFrSceDemLet(std::string & partName, 
		std::string & witPartName, 
		std::string & witDemandName, 
		std::string & sourceLoc,
		std::string & customerLoc,
		std::string & demandClass,
		std::string & demandLevel,
		std::string & partClass,
		int    demandPd,
		float  demandVol);


  // Construct from a Trace String
  LgFrSceDemLet(std::string & traceString);



  // destructor
  virtual
  ~LgFrSceDemLet();


  // copy constructor
  LgFrSceDemLet(const LgFrSceDemLet& source);
  
  // assignment operator
  LgFrSceDemLet&
  operator=(const LgFrSceDemLet& rhs);
  
  // default constructor
  LgFrSceDemLet();
  
  
  
private:
  std::string partName_;
  std::string witPartName_;
  std::string witDemandName_;
  std::string sourceLoc_;
  std::string customerLoc_;
  std::string demandClass_;
  std::string demandLevel_;
  std::string partClass_;
  int    demandPd_;
  float  demandVol_;
  int    priority_;
  std::vector<int> ruleSignature_;
};



#endif
















