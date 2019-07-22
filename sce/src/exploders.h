#ifndef LGFRSCEEXPLODERS
#define LGFRSCEEXPLODERS

// RW2STL - inserted:
#include <string>
// End of RW2STL - inserted includes.

#include <scenario/src/machdep.h>
#include <scenario/src/idnumber.h>
#include <wit/src/wit.h>
#include <sce/src/scenSceP.h>
#include <sce/src/sceDefin.h>

// forward declarations
class LgFrMultiPlantHelper;
class WitRun;
class LgFrSceSetOfParameters;
class LgFrSceCritList;






// + SCE Exploder Subsytem
// + =====================
// +
// + Based on Decorator Design Pattern
// +
// + Base class: LgFrSceBaseExploder
// +     1) defines public standard wit mrp on originalWitRun (no offset adjusts)
// +     2) Cannot be used with sceFSS
// +     3) localWitRun --> originalWitRun
// +     4) May only be used for standard wit MRP
// +     5) Implements some function
// + Implementation Notes:
// +    - member data is defined in Base Class (witRun, localWitRun, mpHelper, setOfParams)
// +      therefore, all decorators have this local dta as well.  The constructor of Base
// +      decorator populates these data as pointers to the one owned by concrete exploder.
// +      This is not good OO, but is more efficient, so that the decorators can get
// +      pointers to these without lots of indirection via vprl.
// +
// + Concrete Component: LgFrSceExploder
// +     1) standard wit mrp on originalWitRun (no offset adjusts)
// +     2) populates a local witRun
// +     3) All decorators must use this component
// +     2) Cannot be used with sceFSS
// +
// + Decorator Base Class: LgFrSceBaseExploderDecorator
// +
// + Concrete Decorator: LgFrSceFssableExploderDecorator
// +
// + Concrete Decorator: LgFrSceFssableExploderDecorator
// +     1) extends LgFrSceBaseExploder to do FSS work
// +
// + Concrete Decorator: LgFrSceFullExploderDecorator
// +     1) extends LgFrSceFullExploderDecorator to do full explode
// +     2) (adjusts offsets)
// +
// + Concrete Decorator: LgFrSceOptExploderDecorator
// +     1) extends LgFrSceFullExploderDecorator OR LgFrSceFssExploderDecorator
// +        to do implode based explosion
// +     2) (modifies structures for implosion)
// +     3) does witOptExplode






class LgFrSceBaseExploder {
public:

  // The "Action" method ... Just do it
  virtual int explode();

  virtual std::string title();

  // determine if this part is something that you deal with
  virtual witBoolean  exists(const std::string& part);

  // Get the four wit mrp part attributes:
  // Default implementation
#if 0
  virtual   int mrpProdVol(  const std::string& part, float ** mrpProdVol);
#endif
  virtual   int mrpExcessVol(  const std::string& part, float ** mrpExcessVol);
  virtual   int mrpResidualVol(const std::string& part, float ** mrpResidualVol);
  virtual   int reqVol(        const std::string& part, float ** reqVolPtr);
  virtual   int mrpConsVol(    const std::string& part, float ** mrpConsVolPtr);
  virtual   int mrpExecVol(    const std::string& operation, float ** mrpExecVolPtr);

  // subUsage Vol
  // must be overridden
  virtual   int subUsageVol(  const std::string& operation,
                              const int bomIndex,
                              const int subBomIndex,
                              float ** subUsageVolPtr);


  // Print out all the attributes (in "del" format)
  virtual void print(std::string& filename);


  void
  setWitInformationalMessagingExpl(WitRun * const theWitRun,
                                                  int onOrOff);



  // Constructor to be used by Client, and Exploder concrete class
  LgFrSceBaseExploder(WitRun * originalWitRun,
                      LgFrMultiPlantHelper * mpHelper,
                      LgFrSceSetOfParameters * setOfParameters);

  // constructor to be used by Decorator Base Class
  LgFrSceBaseExploder(WitRun * originalWitRun,
                      WitRun * localWitRun,
                      LgFrMultiPlantHelper * mpHelper,
                      LgFrSceSetOfParameters * setOfParameters);


  // Return Comparison Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  virtual  WitRun *                   theWitRun();
  virtual  WitRun *                   localWitRun();
  virtual  LgFrMultiPlantHelper *     mpHelper();
  virtual  LgFrSceSetOfParameters *   setOfParameters();

  virtual  int                        nPeriods();
  virtual  int                        state();
  virtual  void                       state(int state);

  void mallocZeroVec(float ** floatVecPtr);

  // default constructor
  LgFrSceBaseExploder();

  // destructor
  virtual
  ~LgFrSceBaseExploder();

protected:
  WitRun *  originalWitRun_;
  WitRun *  localWitRun_;
  LgFrMultiPlantHelper * mpHelper_;
  LgFrSceSetOfParameters * setOfParameters_;
  int nPeriods_;
  int       state_;


private:

  // copy constructor
  LgFrSceBaseExploder( const LgFrSceBaseExploder& source );

  // assignment operator
  LgFrSceBaseExploder&
  operator=(const LgFrSceBaseExploder& rhs);

  const isAReturnType id_;
};


// + ==================================
// + Concrete Component: LgFrSceExploder
// + ===================================
// +


class LgFrSceExploder : public LgFrSceBaseExploder {
public:


  // preferred constructor
  LgFrSceExploder(WitRun * originalWitRun,
                  LgFrMultiPlantHelper * mpHelper,
                  LgFrSceSetOfParameters * setOfParameters);

  virtual std::string title();

  // destructor
  ~LgFrSceExploder();

  // Return Comparison Strategy Identifier
  virtual
     isAReturnType
        isA()
           const;

  // default constructor -- don't use!
  LgFrSceExploder();

  // copy constructor  -- don't use!
  LgFrSceExploder( const LgFrSceExploder& source );

  // assignment operator -- don't use!
  LgFrSceExploder&
  operator=(const LgFrSceExploder& rhs);


private:
  void
  copyAppData();

  void
  modifyCustChoiceSupplyVols();


  const isAReturnType id_;
};


// + ==================================================
// + Decorator Base Class: LgFrSceBaseExploderDecorator
// + ==================================================
// +
// + Provides a default implementation for all public methods
// + this default is to pass the call on to myExploder


class LgFrSceBaseExploderDecorator : public LgFrSceBaseExploder {
public:

  // The "Action" method ... Just do it
  virtual int explode();






  virtual witBoolean exists(const std::string& part);

  // + Access functions are not overridden by concrete decorators
  WitRun *                  theWitRun();
  WitRun *                  localWitRun();
  LgFrMultiPlantHelper *    mpHelper();
  LgFrSceSetOfParameters *  setOfParameters();
  int                       nPeriods();
  int                       state();
  void                      state(int);

#if 0
  virtual int mrpProdVol(  const std::string& part, float ** mrpProdVol);
#endif
  virtual int mrpExcessVol(  const std::string& part, float ** mrpExcessVol);
  virtual int mrpResidualVol(const std::string& part, float ** mrpResidualVol);
  virtual int reqVol(        const std::string& part, float ** reqVolPtr);
  virtual int mrpConsVol(    const std::string& part, float ** mrpConsVolPtr);
  virtual int mrpExecVol(    const std::string& operation, float ** mrpExecVolPtr);

  // subUsage Vol
  // must be overridden
  virtual int subUsageVol(  const std::string& operation,
                            const int bomIndex,
                            const int subBomIndex,
                            float ** subUsageVolPtr);

  virtual void print(std::string& filename);

  // preferred constructor
  LgFrSceBaseExploderDecorator(LgFrSceBaseExploder * myExploder);


  // destructor
  ~LgFrSceBaseExploderDecorator();

  // Return Exploder Identifier
  virtual
     isAReturnType
        isA()
           const;

  // default constructor -- don't use!
  LgFrSceBaseExploderDecorator();

  // copy constructor  -- don't use!
  LgFrSceBaseExploderDecorator( const LgFrSceBaseExploderDecorator& source );

  // assignment operator -- don't use!
  LgFrSceBaseExploderDecorator&
  operator=(const LgFrSceBaseExploderDecorator& rhs);

protected:
  LgFrSceBaseExploder  *   myExploder_;

private:
  const isAReturnType id_;
};



// + ==================================
// + class LgFrSceFullExploderDecorator
// + ==================================
// +
// + Decorates:
// +    - the witRun with offset modifications
// +
// + Overrides:
// +    - isA()
// +    - explode()
// +    -
class LgFrSceFullExploderDecorator : public LgFrSceBaseExploderDecorator {
public:

  virtual int explode();

  virtual std::string title();

  // preferred constructor
  LgFrSceFullExploderDecorator(LgFrSceBaseExploder * myExploder);

  // destructor
  ~LgFrSceFullExploderDecorator();

  // Return Exploder Identifier
  virtual
     isAReturnType
        isA()
           const;



private:
  // internal method ... populates the witrun copy
  void decorateLocalWitRun();

  // default constructor -- don't use!
  LgFrSceFullExploderDecorator();

  // copy constructor  -- don't use!
  LgFrSceFullExploderDecorator( const LgFrSceFullExploderDecorator& source );

  // assignment operator -- don't use!
  LgFrSceFullExploderDecorator&
  operator=(const LgFrSceFullExploderDecorator& rhs);

  const isAReturnType id_;
};


// + ==================================
// + class LgFrSceSmartExploderDecorator
// + ==================================
// +
// + Decorates:
// +    - the witRun via
// +        -
// +        -
// +        -
// +        -
// +
// + Overrides:
// +    - isA()
// +    - explode()
// +    -
class LgFrSceSmartExploderDecorator : public LgFrSceBaseExploderDecorator {
public:

  virtual
  int explode();

  virtual void loadPIPDataIntoAppData(WitRun * const theWitRun);

  virtual std::string title();
#if 0
  virtual int mrpProdVol(  const std::string& part, float ** mrpProdVol);
#endif
  virtual int mrpExcessVol(  const std::string& part, float ** mrpExcessVol);
  virtual int mrpResidualVol(const std::string& part, float ** mrpResidualVol);
  virtual int reqVol(        const std::string& part, float ** reqVolPtr);
  virtual int mrpConsVol(    const std::string& part, float ** mrpConsVolPtr);
  virtual int mrpExecVol(    const std::string& operation, float ** mrpExecVolPtr);

  // subUsage Vol
  // must be overridden
  virtual int subUsageVol(  const std::string& operation,
                            const int bomIndex,
                            const int subBomIndex,
                            float ** subUsageVolPtr);



  // preferred constructor
  LgFrSceSmartExploderDecorator(LgFrSceBaseExploder * myExploder,
                                LgFrScenarioForSceSmartPointer sceScenSP,
                                LgFrMultiPlantHelper * mpHelper);

  // destructor
  ~LgFrSceSmartExploderDecorator();

  // Return Exploder Identifier
  virtual
     isAReturnType
        isA()
           const;

  virtual void print(std::string& filename);

private:
  // internal method ... populates the witrun copy
  void decorateLocalWitRun();
  void restructureForImplosionToSolveExplosion();
  void disableSubBomAccordingToAppData();


void printImpactPeriodFile(
    WitRun * const theWitRun,
    const int nPeriods,
    const std::string & fileName,
    LgFrMultiPlantHelper & mpHelper);

  void decorateLocalWitRunForPrioritizedExplode();


  // default constructor -- don't use!
  LgFrSceSmartExploderDecorator();

  // copy constructor  -- don't use!
  LgFrSceSmartExploderDecorator( const LgFrSceSmartExploderDecorator& source );

  // assignment operator -- don't use!
  LgFrSceSmartExploderDecorator&
  operator=(const LgFrSceSmartExploderDecorator& rhs);

  const isAReturnType id_;
  LgFrScenarioForSceSmartPointer sceScenSP_;
  LgFrMultiPlantHelper * mpHelper_;


};


// + ================================
// + LgFrSceFssableExploderDecorator
// + ================================
// +
// + Extends:
// +    - adds functions that extend any exploders capabilities
// +      for FSS.  Mainly, lets client modify localWitRun.
// +
// + All other access functions are passed off
class LgFrSceFssableExploderDecorator : public LgFrSceBaseExploderDecorator {
public:

  virtual int explode();

  // preferred constructor
  LgFrSceFssableExploderDecorator(LgFrSceBaseExploder * myExploder);


  virtual std::string title();
  // Put your fss-specific calls here


  virtual void setAllSupplyToZero();

  virtual void setAllDemandToZero();

  // Return Exploder Identifier
  virtual isAReturnType isA() const;

  // destructor
  ~LgFrSceFssableExploderDecorator();


private:
  // default constructor -- don't use!
  LgFrSceFssableExploderDecorator();

  // copy constructor  -- don't use!
  LgFrSceFssableExploderDecorator( const LgFrSceFssableExploderDecorator& source );

  // assignment operator -- don't use!
  LgFrSceFssableExploderDecorator&
  operator=(const LgFrSceFssableExploderDecorator& rhs);

  // set supplyVols and demandVols to the current
  // setting in originalWitRun_.
  virtual void  synchronize();


  const isAReturnType id_;
};




// + ======================================================
// + Exploder Factory -- creates the right decorated exploder
// + based on SetOfParameters.  Global Function
// + ======================================================
#if 0
// this function is in the sceScenario object.
LgFrSceBaseExploder *
createAppropriateSceExploder(WitRun * theWitRun,
                             LgFrMultiPlantHelper * mpHelper,
                             LgFrSceSetOfParameters * setOfParameters);
#endif


// These definitions follow the pattern used by Rogue Wave
#define __LGFRSCEBASEEXPLODER                0x2500
#define __LGFRSCEEXPLODER                    0x2501
#define __LGFRSCEBASEEXPLODERDECORATOR       0x2502
#define __LGFRSCEFSSABLEEXPLODERDECORATOR    0x2503
#define __LGFRSCEFULLEXPLODERDECORATOR       0x2504
#define __LGFRSCESMARTEXPLODERDECORATOR      0x2505

#endif
