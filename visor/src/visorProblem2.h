#ifndef VISORproblem2_h
#define VISORproblem2_h

//#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cassert>

#include <wit.h>
#include "visorProblem.h"


class VISORproblem2 : public VISORproblem
{
public:

  //----------------------------------
  // Methods for global attributes.
  //----------------------------------

  // setEquAllocPerBnd: equitable allocation period bound
  void setEqAlloPerBnd(int bnd);

  int getEqAllocPerBnd();

  //----------------------
  // Methods for Visor.
  //----------------------
  void addVisor(const std::string &name, const std::string &location);

  bool locationExists(const std::string &loc);

  void setVisorSupplyVol(const std::string &name, const std::string &location,
                         const std::vector<float> &supplyVol);

  //------------------------------------------
  // Visor name methods
  //-----------------------------------------
  std::string aggregateVisorName(const std::string &location);

  std::string aggregateOperName(const std::string &location);

  std::string noSupplyVisorName(const std::string &location);

  std::string visorPartName(const std::string &name, const std::string &location);

  std::string baseLocationName(const std::string &location);

  std::string locationFormAggregateVisorName(const std::string &location);

  std::set<std::string> getLocation();

  //----------------------
  // Demands: Hospital Request for Visor
  //----------------------
  void addVisorRequest(const std::string &name, int period, int requestedQuantity);

  bool witGetDemandExists(const std::string &visorName, const std::string &demandName);

  std::vector<std::string> getDemands();

  std::vector<float> getVisorShipVol(std::string &demandName);

  // For a specific hospital visor request get the printer locations, period and quantity that
  // is satisfying the request
  void getSubVols(const std::string &demandName, std::vector<std::string> &consPart,
                  std::vector<std::vector<float>> &subsVol);

//-------------------------------------------------------------------------
// demand (hospital) Name Methods
//-------------------------------------------------------------------------
  std::string visorForHospitalName(const std::string &demName);

  std::string aggregateVisorForHospitalName(const std::string &demName);

  std::string noSupplyForHospital(const std::string &demName);






  //--------------------
  // Solve Methods.
  //--------------------
  void solve();



  //----------------------------------------------------------------------

  // default constructor
  VISORproblem2();

  // copy constructor
  VISORproblem2(const VISORproblem2 &source);

  // assignment operator
  VISORproblem2 &
  operator=(const VISORproblem2 &rhs);

  // destructor
  ~VISORproblem2();


  // Self-test
  static void test();


  void gutsOfCopy(const VISORproblem2 &source);

  void gutsOfDestructor();


  int eqAllocPerBnd_;
  std::set<std::string> locationBaseNames_;
  std::vector<std::string> demandList_;

};


#endif

