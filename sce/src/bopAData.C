// file: bopAData.C
// author: robin

#include <sce/src/bopAData.h>
#include <assert.h>



LgFrSceAppData *
LgFrSceBopAppData::clone()
const
{
  LgFrSceBopAppData * temp = new LgFrSceBopAppData(*this);

  // nothing to deep copy

  return temp;
}



// a better constructor 
LgFrSceBopAppData::LgFrSceBopAppData(float defaultPreference,
                                     int isObjectInEffectForOptExplosionFlag,
				     int expAllowed)
  :   preference_ (defaultPreference),
      isObjectInEffectForOptExplosion_ (isObjectInEffectForOptExplosionFlag),
      expAllowed_ (expAllowed)
{
  // all the work is done in initializer
}

// default constructor 
LgFrSceBopAppData::LgFrSceBopAppData()
  :   preference_ (0.0),
      isObjectInEffectForOptExplosion_ (1),
      expAllowed_ (0)
{
  assert (1 == 0); // don't use this constructor
}

// get "preference" 
const
float
LgFrSceBopAppData::preference () const
{
  return preference_;
}

// set "preference"
void
LgFrSceBopAppData::preference (const float pref)
{                                                  
  preference_ = pref ;
}

// get "in affect for opt explosion" flag
const
int
LgFrSceBopAppData::isObjectInEffectForOptExplosion () const
{
  return isObjectInEffectForOptExplosion_;
}

// set "in affect for opt explosion" flag
void
LgFrSceBopAppData::isObjectInEffectForOptExplosion(
                const int isObjectInEffectForOptExplosion)
{
  isObjectInEffectForOptExplosion_ = isObjectInEffectForOptExplosion;
}


// get "expAllowed" flag
const
int
LgFrSceBopAppData::expAllowed () const
{
  return expAllowed_;
}

// set "expAllowed" flag
void
LgFrSceBopAppData::expAllowed(
                const int expAllowed)
{
  expAllowed_ = expAllowed;
}



// assignment operator
LgFrSceBopAppData&
LgFrSceBopAppData::operator=(const LgFrSceBopAppData& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    preference_ = rhs.preference_ ;
    isObjectInEffectForOptExplosion_ = rhs.isObjectInEffectForOptExplosion_;
    expAllowed_ = rhs.expAllowed_;
  }
  return *this;
}

// copy constructor 
LgFrSceBopAppData::LgFrSceBopAppData(const LgFrSceBopAppData& source)
  :   preference_ (source.preference_),
      isObjectInEffectForOptExplosion_ (source.isObjectInEffectForOptExplosion_),
      expAllowed_ (source.expAllowed_)
{
  // nothing to do
}

// destructor
LgFrSceBopAppData::~LgFrSceBopAppData()
{
  // nothing to do, 
}
