// file: bopAData.h
// author: robin

#ifndef  LGFRSCEBOPAPPDATA_H
#define  LGFRSCEBOPAPPDATA_H

#include <stdlib.h>
#include <sce/src/sceDefin.h>
#include <scenario/src/timeVecs.h>
#include <sce/src/baseAppData.h>
#include <assert.h>

class LgFrSceBopAppData   :
    public LgFrSceAppData

{
public:

  virtual
  LgFrSceAppData *
  clone()
    const;


  // default constructor. Don't use.
  LgFrSceBopAppData (); 

  // better constructor
  LgFrSceBopAppData (float defaultPreference, 
		     int isObjectInEffectForOptExplosionFlag,
		     int expAllowed);

  // get "preference"
  const
  float
  preference () const ;
  
  // set "preference"
  void
  preference (const float pref);

  // get "in affect for opt explosion" flag
  const
  int
  isObjectInEffectForOptExplosion () const ;

  // set "in affect for opt explosion" flag
  void
  isObjectInEffectForOptExplosion(const int isObjectInEffectForOptExplosionFlag);


  // get "expAllowed" flag
  const
  int
  expAllowed () const;

  // set "expAllowed" flag
  void
  expAllowed(const int expAllowed);

  
  // copy constructor
  LgFrSceBopAppData(const LgFrSceBopAppData& source);

  // assignment operator
  LgFrSceBopAppData&
  operator=(const LgFrSceBopAppData& rhs);

  // destructor
  virtual
  ~LgFrSceBopAppData();
  
private:
  // The preference on the bop is used to determine
  // which bop entry to explode in witMrp and the hueristic.
 
  // The bop entry corresponding to the
  // "regular" build has preference 0.0
  // The lower the preferences (negatives are ok), the more important. 
  // With the lp, the "preference" is interpreted as a cost.
  // Default = 0.0
  float preference_ ;
  int isObjectInEffectForOptExplosion_;  
  int expAllowed_;  
};
 
#endif
