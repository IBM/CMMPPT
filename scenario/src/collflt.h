#ifndef LgFrCollectableFloat_h
#define LgFrCollectableFloat_h

#include <scenario/src/idnumber.h>



class LgFrCollectableFloat { 


public:

  LgFrCollectableFloat();
  LgFrCollectableFloat(float f);
  LgFrCollectableFloat(const LgFrCollectableFloat& ci);
  LgFrCollectableFloat& operator=(const LgFrCollectableFloat& ci);
  virtual ~LgFrCollectableFloat();

  virtual
    bool
       operator==(
          const LgFrCollectableFloat& c) 
             const;

  virtual 
     int
        compareTo(
           const LgFrCollectableFloat*)
              const;

  virtual 
    bool
      isEqual(
        const LgFrCollectableFloat*) 
          const;

  virtual
    float	
      value() 
        const;

  virtual
    float
      value(
        float value);

  static
    void
      test();

private:
  float value_;
  
  static float fltEps;
};	  


#endif 
