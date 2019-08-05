#ifndef SCEPEGGING
#define SCEPEGGING


#include <scenario/src/RWToSTLHelpers.h>



#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <wit/src/wit.h>
#include <string.h>

// forward declarations
class LgFrMultiPlantHelper;



//---------------------------------------------------------------------------
// Define a SCECollectable class that contains the pair const char * and an int
//---------------------------------------------------------------------------
class SceConstCharStar_Int //: public SCECollectable 
{
// RW2STL -   RWDECLARE_COLLECTABLE(SceConstCharStar_Int);
public:

  const char * getCharStar()const{return charStar_;};
  void setCharStar(const char * cs){charStar_=cs;};

  int getInt()const{return int_;};
  void setInt(int i){int_=i;};

  int compareTo(const SceConstCharStar_Int* col) const
  {  
    const SceConstCharStar_Int* rhs = (const SceConstCharStar_Int*)col;
    int compareString = strcmp(getCharStar(),rhs->getCharStar());
    if ( compareString != 0 ) return compareString;
    if (getInt() == rhs->getInt()) return 0;
    return getInt() > rhs->getInt() ? 1 : -1;
  };
  
  virtual bool operator==(const SceConstCharStar_Int& c) const
  { return isEqual(&c); };
  
  bool isEqual(const SceConstCharStar_Int* col) const
  {  
    return compareTo(col) == 0;
  }

  // Constructors
  SceConstCharStar_Int():charStar_(0),int_(0){};
  SceConstCharStar_Int(const char * cs, int i):charStar_(cs),int_(i){};

  
  SceConstCharStar_Int(const SceConstCharStar_Int& ci):charStar_(ci.charStar_),int_(ci.int_){};
  SceConstCharStar_Int& operator=(const SceConstCharStar_Int& ci)
  {
    if (this != &ci) {           // Check for assignment to self
//      (this->SCECollectable::operator= ) (ci);
      charStar_ = ci.charStar_;
      int_ = ci.int_;
    }
    return *this;
  };

  virtual ~SceConstCharStar_Int(){};
private:
  const char * charStar_;
  int int_;
};



//---------------------------------------------------------------------------
// Define a SCECollectable class that contains a double
//---------------------------------------------------------------------------
class SceCollectableDouble { //: public SCECollectable {


public:

  SceCollectableDouble():dbl_(0.0){};
  SceCollectableDouble(double f):dbl_(f){};
  SceCollectableDouble(const SceCollectableDouble& ci):dbl_(ci.dbl_){};
  SceCollectableDouble& operator=(const SceCollectableDouble& ci)
  {
    if (this != &ci) {           // Check for assignment to self
//      (this->SCECollectable::operator= ) (ci);
      dbl_ = ci.dbl_;
    }
    return *this;
  };

  virtual ~SceCollectableDouble(){};


  virtual int compareTo(const SceCollectableDouble*c)const
  { 
    const SceCollectableDouble* rhs = (const SceCollectableDouble*)c;
    
    return
      (
      // did not build because fabs was not defined but dblEps_ was comemnted out,
      // so the expression was replaced by a simpler equivalent.
      dbl() == rhs->dbl() //( fabs( dbl() - rhs->dbl() ) <= /*dblEps_*/ 0.0 )
      ? 0
      : ( ( dbl() < rhs->dbl() )
      ? -1 
      : 1
      )
      );
  }
  
  virtual bool operator==(const SceCollectableDouble& c) const
  { return isEqual(&c); };
  
  virtual bool isEqual(const SceCollectableDouble* c) const
  { 
    return compareTo(c) == 0;
  }

  virtual double	dbl() const {return dbl_;};

  virtual double dbl(double value){dbl_=value;return value;};

private:
  double dbl_; 
  
  //static double dblEps_;
};	  

//double SceCollectableDouble::dblEps_ = .00001;

struct CompareSceConstCharStar_IntStruct 
{
    bool operator()(const SceConstCharStar_Int& p, const SceConstCharStar_Int& q) const
    {
        return p.compareTo( &q ) < 0 ? true : false;
    }
};

typedef std::map<SceConstCharStar_Int,
                 SceCollectableDouble,
                 CompareSceConstCharStar_IntStruct> PartNamePeriodConsVolDictionary;
typedef std::pair<SceConstCharStar_Int,SceCollectableDouble> PartNamePeriodConsVolPair;

class LgFrSortingPartScheduleFloat;



class ScePIP {
public:
  
  //----------------------------------------------------------
  // For a a given demand over a range of shipment periods,
  // return all the parts consumed to satisfy these shipments.
  //
  // The wit global attribute perfPIP must be true,
  // because this method uses witGetDemandExecVolPIP
  // and witGetDemandSubVolPIP, which require perfPIP
  // to be true.  
  // 
  // It is anticipated that SCE will use this function with
  // firstShipPeriod=0 and lastShipPeriod=nPeriods-1
  // 
  // The SCE definition of consVol is the wit consVol plus
  // shipVol (sce::ConsVol = wit::consVol + wit::shipVol)
  // This method will return either definition based on the
  // the value of the parameter sceConsVol.
  //
  // If ignoreSubstituteUsage is true, then the consVols returned
  // is the consumpution of parts if primary parts had been used
  // rather than substitutes.  The consVols are computed from
  // execVols even though there might be substitute consumption.
  //
  // The user of this method must free the returned storage
  // using the following code or the equivalent:
  //   for( int cp=0; cp<nConsParts; cp++ ) free(consPartList[cp]);
  //   free(consPartList);
  //   free(consPerList);
  //   free(consVolList);
  //----------------------------------------------------------


  static
  witReturnCode getDemandConsVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   int * lenLists,
   char * * * partNameList,
   int * * consPeriodList,
   float * * PIPConsVolList
   );

  //----------------------------------------------------------
  // This method is the same as the one above except that
  // the computed consVol is returned in an LgFr 
  // Part Scehdule (ps).
  //
  // The computed consVols are added to the ps.
  // That is the ps is not cleared.
  //----------------------------------------------------------
  static
  witReturnCode getDemandConsVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & ps,
   bool sceConsVol = false,
   bool ignoreSubstituteUsage = true 
   );

  static
  witReturnCode getDemandConsVolPIP (
    WitRun * const wr,
    const char * const partName,
    const char * const demandName,
    const int firstShipPeriod,
    const int lastShipPeriod,
    bool sceConsVol,
    bool ignoreSubstituteUsage,
    PartNamePeriodConsVolDictionary & consDict
    ); 










  static
  witReturnCode getSupplyVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   int * lenLists,
   char * * * partNameList,
   int * * consPeriodList,
   float * * PIPConsVolList
   );

  //----------------------------------------------------------
  // This method is the same as the one above except that
  // the computed consVol is returned in an LgFr 
  // Part Scehdule (ps).
  //
  // The computed consVols are added to the ps.
  // That is the ps is not cleared.
  //----------------------------------------------------------
  static
  witReturnCode getSupplyVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & ps
   );

  static
  witReturnCode getSupplyVolPIP (
    WitRun * const wr,
    const char * const partName,
    const char * const demandName,
    const int firstShipPeriod,
    const int lastShipPeriod,
    PartNamePeriodConsVolDictionary & consDict
    ); 





  //----------------------------------------------------------
  // This method is the same as the one above except that
  // the computed consVol is returned in an LgFr 
  // Part Scehdule (ps).
  //
  // The computed consVols are added to the ps.
  // That is the ps is not cleared.
  //----------------------------------------------------------
  static
  witReturnCode getProdVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & ps
   );

  static
  witReturnCode getProdVolPIP (
    WitRun * const wr,
    const char * const partName,
    const char * const demandName,
    const int firstShipPeriod,
    const int lastShipPeriod,
    PartNamePeriodConsVolDictionary & consDict
    ); 



  //----------------------------------------------------------
  // This method is the same as the one above except that
  // the computed consVol is returned in an LgFr 
  // Part Scehdule (ps).
  //
  // The computed consVols are added to the ps.
  // That is the ps is not cleared.
  //----------------------------------------------------------
  static
  witReturnCode getSideVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & ps
   );

  static
  witReturnCode getSideVolPIP (
    WitRun * const wr,
    const char * const partName,
    const char * const demandName,
    const int firstShipPeriod,
    const int lastShipPeriod,
    PartNamePeriodConsVolDictionary & consDict
    ); 







  //----------------------------------------------------------
  // Return the execVol's PIP to demand in specified period.
  // The execVols are returned in a PartSchedule.
  // This is a bit wierd.
  // It should be an OperaionSchedule, but this object does
  // not exist.
  //
  // The computed execVols are added to opSched.
  // That is the opSched is not cleared.
  //----------------------------------------------------------
  static
  witReturnCode getDemandExecVolPIP (
   WitRun * const wr,
   const char * const partName,
   const char * const demandName,
   const int firstShipPeriod,
   const int lastShipPeriod,
   LgFrSortingPartScheduleFloat & opSched
   );

  static
  witReturnCode getDemandInterplantVolPIP ( 
					   WitRun * const wr,
					   LgFrMultiPlantHelper * mpHelper,
					   const char * const partName,
					   const char * const demandName,
					   const int firstShipPeriod,
					   const int lastShipPeriod,
					   LgFrSortingPartScheduleFloat & os,
					   bool supplierPerspectiveInterplantPeg
					   );

  
  
  // constructor
  ScePIP();
  
  
  // destructor
  ~ScePIP();
  
  // Self-test
  static void test();
  
  
private:
  
  // copy constructor  -- don't use!
  ScePIP( const ScePIP& source );
  
  // assignment operator -- don't use!
  ScePIP&
    operator=(const ScePIP& rhs);
  
  static
  void updateConsDict(
    PartNamePeriodConsVolDictionary & consDict,
    char * consumedPartName,
    int consPeriod,
    double deltaConsVol);
  
  static
  void computeConsVolAndUpdateConsDict(
    WitRun * wr,
    char * consumedPartName,
    float * consRate,
    float fallout,
    int * impactPeriod,
    float execOrSubVol,
    int execPeriod,
    bool increment,
    PartNamePeriodConsVolDictionary & consDict );
  
  static
  void updateConsDictFromExecOrSubVolAndBom(
    WitRun * wr, 
    const char * opName,
    int bomEntryIndex,
    int execPeriod,
    float execOrSubVol,
    bool isExecVol,
    PartNamePeriodConsVolDictionary & consDict);    
  
  static
  witReturnCode getDemandExecVolPIP(
    WitRun * const wr,
    const char * const partName,
    const char * const demandName,
    const int firstShipPeriod,
    const int lastShipPeriod,
    PartNamePeriodConsVolDictionary & execDict
    );

  static
  witReturnCode getDemandInterplantVolPIP (
					   WitRun * const wr,
					   LgFrMultiPlantHelper * mpHelper,
					   const char * const partName,
					   const char * const demandName,
					   const int firstShipPeriod,
					   const int lastShipPeriod,
					   PartNamePeriodConsVolDictionary & execDict,
					   bool supplierPerspectiveInterplantPeg
					   );


  
};





#endif





