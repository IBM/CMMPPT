// file: dmApData.h
// author: robin

#ifndef  LGFRSCEDEMANDAPPDATA_H
#define  LGFRSCEDEMANDAPPDATA_H

#include <stdlib.h>
#include <scenario/src/timeVecs.h>
#include <sce/src/baseAppData.h>

#include <scenario/src/partSchd.h>


#include <assert.h>

class LgFrSceDemandAppData :
    public LgFrSceAppData
{
public:
  virtual
  LgFrSceAppData *
  clone()
    const;



  // default constructor. Don't use.
  LgFrSceDemandAppData (); 

  // always use this constructor
  LgFrSceDemandAppData(int hasDemandVolBeenSet,
                       int hasMinDemandVolBeenSet,
		       size_t nPeriods, 
		       float backlogYieldDefault);


  // better constructor
  LgFrSceDemandAppData (size_t nPeriods, float defaultBacklogYield);

  // get "doesDemandHaveNegativeValues"
  int
  doesDemandHaveNegativeValues() const;
  
  // set "doesDemandHaveNegativeValues"
  void
  doesDemandHaveNegativeValues(int doesDemandHaveNegativeValues);

  // get "hasDemandVolBeenSet"
  int
  hasDemandVolBeenSet() const;

  // set "hasDemandVolBeenSet"
  void
  hasDemandVolBeenSet(int hasDemandVolBeenSet);

  // get "hasMinDemandVolBeenSet"
  int
  hasMinDemandVolBeenSet() const;

  // set "hasMinDemandVolBeenSet"
  void
  hasMinDemandVolBeenSet(int hasMinDemandVolBeenSet);


  // get "above minimum demand" vols
  const
  LgFrTimeVecFloat&
  aboveMinDemandVol () const ;
  
  
  // set "above minimum demand" vols
  void
  aboveMinDemandVol (const LgFrTimeVecFloat & amDemand);

  // get backlog yield                     
  const
  LgFrTimeVecFloat&  
  backlogYield () const ;

  // set backlog yield
  void
  backlogYield (const LgFrTimeVecFloat & bklgYld) ;

  // get demandVol (potentially negative)
  const
  LgFrTimeVecFloat&  
  demandVol () const ;

  // set demandVol (potentially negative)
  void
  demandVol (const LgFrTimeVecFloat & demandVol) ;

  // get allocVol (potentially negative)
  const
  LgFrTimeVecFloat&  
  allocVol () const ;

  // set allocVol (potentially negative)
  void
  allocVol (const LgFrTimeVecFloat & allocVol) ;

  // get pipPriority
  const
  LgFrTimeVecFloat&  
  pipPriority () const ;

  // set pipPriority
  void
  pipPriority (const LgFrTimeVecFloat & pipPriority) ;


  // get requestDate
  const
  std::string &
  requestDate () const ;

  // set requestDate
  void
  requestDate (const std::string & requestDate) ;


  // get a PIP Sched
  LgFrSortingPartScheduleFloat &  
  PIPSched(int pegType);

  // get PIPConsVol schedule
  LgFrSortingPartScheduleFloat &
  PIPConsVolSched();

  // get PIPConsVol schedule
  LgFrSortingPartScheduleFloat &
  PIPSupplyVolSched();


  // get PIPExecVol schedule
  LgFrSortingPartScheduleFloat &
  PIPExecVolSched();

  // get PIPInterplantVol schedule
  LgFrSortingPartScheduleFloat &
  PIPInterplantVolSched();


  // get PIPProdVol schedule
  LgFrSortingPartScheduleFloat &
  PIPProdVolSched();

  // get PIPSideVol schedule
  LgFrSortingPartScheduleFloat &
  PIPSideVolSched();


  // copy constructor
  LgFrSceDemandAppData(const LgFrSceDemandAppData& source);

  // assignment operator
  LgFrSceDemandAppData&
  operator=(const LgFrSceDemandAppData& rhs);

  // destructor
  virtual
  ~LgFrSceDemandAppData();
  
private:
  LgFrTimeVecFloat aboveMinDemandVol_ ;
  LgFrTimeVecFloat backlogYield_ ;
  LgFrTimeVecFloat demandVol_ ;
  LgFrTimeVecFloat allocVol_ ;
  LgFrTimeVecFloat pipPriority_ ;
  int hasDemandVolBeenSet_;
  int hasMinDemandVolBeenSet_;
  int doesDemandHaveNegativeValues_;
  std::string requestDate_;
  LgFrSortingPartScheduleFloat PIPConsVolSched_;  
  LgFrSortingPartScheduleFloat PIPSupplyVolSched_;  
  LgFrSortingPartScheduleFloat PIPExecVolSched_;  
  LgFrSortingPartScheduleFloat PIPInterplantVolSched_;  
  LgFrSortingPartScheduleFloat PIPProdVolSched_;  
  LgFrSortingPartScheduleFloat PIPSideVolSched_;  
};
 
#endif
