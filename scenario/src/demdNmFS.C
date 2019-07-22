// RW2STL -- inserted:
#include <scenario/src/RWToSTLHelpers.h>
#include <string>
// End of RW2STL -- inserted includes.
#include <iostream>

#include <scenario/src/demdNmFS.h>
#include <scenario/src/itemTimV.h>
#include <scenario/src/demand.h>
#include <scenario/src/part.h>
#include <scenario/src/machdep.h>
#include <scenario/src/idnumber.h>


// Do Filtering
bool
LgFrDemandNameFilterStrategy::exclude(
   const LgFrItemTimeVecPairFloat & itvpf)
const
{
   assert( itvpf.item().isADemand() );

#ifdef WIN32   
   const LgFrDemand (& theDemand) (itvpf.demand());
#else
   const LgFrDemand & theDemand = itvpf.demand();
#endif   

   std::string partName (theDemand.partPtr()->name());
   std::string demandName (theDemand.name());


   if ( sce::matchRegexp( partName, partRegexp_ ) == false )
      return 1;

   if ( sce::matchRegexp( demandName, demandRegexp_ ) == false )
      return 1;
   
   return 0;
}

// Get the Part Regexp as a std::string associated with this Filter
std::string
LgFrDemandNameFilterStrategy::partRE()
const
{
   return partREstring_;
}

// Get the Demand Regexp as a std::string associated with this Filter
std::string
LgFrDemandNameFilterStrategy::demandRE()
const
{
   return demandREstring_;
}

// Get the partRegexp  associated with this filter
std::string
LgFrDemandNameFilterStrategy::partRegexp()
const
{
   return partRegexp_;
}

// Get the demandRegexp  associated with this filter
std::string
LgFrDemandNameFilterStrategy::demandRegexp()
const
{
   return demandRegexp_;
}

// Make clone copy of object
LgFrFilterStrategy *
LgFrDemandNameFilterStrategy::clone()
const
{
  LgFrDemandNameFilterStrategy* newInst = 
      new LgFrDemandNameFilterStrategy(*this);
  return (newInst);
}

// Make clone copy of object with new Scenario
LgFrFilterStrategy *
LgFrDemandNameFilterStrategy::clone(LgFrScenario & newScenario)
const
{
  LgFrDemandNameFilterStrategy* newInst = 
      new LgFrDemandNameFilterStrategy(*this, newScenario);
  return (newInst);
}

// Assignment operator
LgFrDemandNameFilterStrategy &
LgFrDemandNameFilterStrategy::operator=(
			  const LgFrDemandNameFilterStrategy& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
     (this->LgFrFilterStrategy::operator= ) (rhs);
     partREstring_   = rhs.partREstring_;
     demandREstring_ = rhs.demandREstring_;
  }
  return *this;
}


// Copy constructor
LgFrDemandNameFilterStrategy::
   LgFrDemandNameFilterStrategy (
      const LgFrDemandNameFilterStrategy & source)
:  
   LgFrFilterStrategy( source ),
   id_(__LGFRDEMANDNAMEFILTERSTRATEGY),
   partREstring_(source.partREstring_),
   demandREstring_(source.demandREstring_),
   partRegexp_(source.partRegexp_),
   demandRegexp_(source.demandRegexp_)
{
}

// Copy constructor with new Scenario
LgFrDemandNameFilterStrategy::
   LgFrDemandNameFilterStrategy (
      const LgFrDemandNameFilterStrategy & source,
      LgFrScenario & newScenario )
:  
   LgFrFilterStrategy( source, newScenario ),
   id_(__LGFRDEMANDNAMEFILTERSTRATEGY),
   partREstring_(source.partREstring_),
   demandREstring_(source.demandREstring_),
   partRegexp_(source.partRegexp_),
   demandRegexp_(source.demandRegexp_)
{
}

// Preferred Constructor
LgFrDemandNameFilterStrategy::
   LgFrDemandNameFilterStrategy (
          const std::string & partREstring,
          const std::string & demandREstring)
:  
   LgFrFilterStrategy(), 
   id_(__LGFRDEMANDNAMEFILTERSTRATEGY),
   partREstring_(partREstring),
   demandREstring_(demandREstring),
   partRegexp_(partREstring),
   demandRegexp_(demandREstring)
{
   if ( !sce::checkRegexp( partRegexp_ ) )
   {
      std::cerr << "\nIllegal RWCRegexp specified for part ---> " 
           << partREstring
           << "\n";
      assert( sce::checkRegexp( partRegexp_ ) );
   }

   if ( !sce::checkRegexp( demandRegexp_ ) )
   {
      std::cerr << "\nIllegal RWCRegexp specified for demand ---> " 
           << demandREstring
           << "\n";
      assert( sce::checkRegexp( demandRegexp_ ) );
   }

}


// Default Constructor -- this will match anything
LgFrDemandNameFilterStrategy::
   LgFrDemandNameFilterStrategy ()
:  
   LgFrFilterStrategy(),
   id_(__LGFRDEMANDNAMEFILTERSTRATEGY),
   partREstring_(".*"),
   demandREstring_(".*"),   
   partRegexp_(".*"),
   demandRegexp_(".*")
{
  // Nothing to do here
}

// Destructor
LgFrDemandNameFilterStrategy::
   ~LgFrDemandNameFilterStrategy ()
{
   // Nothing to do here
}


#ifdef NDEBUG
#undef NDEBUG
#endif

// test LgFrDemandNameFilterStrategy methods.
void
LgFrDemandNameFilterStrategy::test()
{
  // Test Preferred constructor
  LgFrDemandNameFilterStrategy cs1(".*", "USA");

  // Test copy constructor
  LgFrDemandNameFilterStrategy cs2(cs1);
  assert( sce::checkRegexp( cs2.partRegexp() ));
  assert( sce::checkRegexp( cs2.demandRegexp() ));
  assert( cs2.partRE() == cs1.partRE());
  assert( cs2.demandRE() == cs1.demandRE());

  // Test assignment operator
  LgFrDemandNameFilterStrategy cs3("abc", "APG");
  cs3 = cs2;
  assert( sce::checkRegexp( cs2.partRegexp() ));
  assert( sce::checkRegexp( cs2.demandRegexp() ));
  assert( cs3.partRE() == cs2.partRE() );
  assert( cs3.demandRE() == cs2.demandRE() );

  // Test clone method
  LgFrFilterStrategy *cs4Ptr = cs1.clone();
  
  // cast the clone back down to a DemandNameFS
  LgFrDemandNameFilterStrategy *cs5Ptr = 
         (LgFrDemandNameFilterStrategy *) cs4Ptr; 
  assert(  sce::checkRegexp( cs1.partRegexp() ));
  assert(  sce::checkRegexp( cs1.demandRegexp() ));
  assert( cs1.partRE() == cs5Ptr->partRE() );
  assert( cs1.demandRE() == cs5Ptr->demandRE() );

  // Test destructor
  delete cs4Ptr;

  // Test exclude
  LgFrPart aPart("abc45");
  LgFrDemand aDemand("USA", &aPart);
  LgFrTimeVecFloat aTVF(2,2.);
  LgFrItemTimeVecPairFloat itvp1( aDemand, aTVF);
  assert( !cs1.exclude( itvp1 ) );
  assert(  cs3.exclude( itvp1 ) );

  LgFrDemandNameFilterStrategy cs6(".*45", "USA");
  assert( ! cs6.exclude( itvp1 ) );
  LgFrDemandNameFilterStrategy cs7("abc.*", "USA");
  assert( ! cs7.exclude( itvp1 ) );
  LgFrDemandNameFilterStrategy cs8("abc.*", "US.*");
  assert( ! cs8.exclude( itvp1 ) );
  LgFrDemandNameFilterStrategy cs9(".*", "APG");
  assert( cs9.exclude( itvp1 ) );

  // default constructor should let anything through
  LgFrDemandNameFilterStrategy cs10;
  assert( ! cs10.exclude( itvp1 ) );
  
}
