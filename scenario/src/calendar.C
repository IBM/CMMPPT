// RW2STL -- inserted:

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif

#include <scenario/src/RWToSTLHelpers.h>
#include <string>
#include <sstream>
// End of RW2STL -- inserted includes.
// file: calendar.C
// author: tom

#include <stdlib.h>
#include <assert.h>
#include <scenario/src/calendar.h>

// fairly general constructor:
LgFrCalendar::LgFrCalendar(
	     size_t   nPeriods,
	     const LgFrDatetime&  horizonStart,
	     LgFrTimeUnit intervalUnit,
	     unsigned intervalUnitMultiplier)
: nPeriods_(nPeriods),
  startTimes_(),
  effectiveStartTimes_(),
  workUnits_(),
  intervalUnit_(intervalUnit),
  intervalUnitMultiplier_(intervalUnitMultiplier)
{
  if (nPeriods == 0) return;

  initializeOrderedVectors();
  
  // set first period
  startTimes_[0] = horizonStart;
  
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nPeriods_; i++) {
    switch (intervalUnit_)  {
    case LgFrWeek:
      startTimes_[i+1] = startTimes_[i].plusWeeks( intervalUnitMultiplier );
      break;
    case LgFrDay:
      startTimes_[i+1] = startTimes_[i].plusDays( intervalUnitMultiplier );
      break;
    case LgFrMonth:
      startTimes_[i+1] = startTimes_[i].plusMonths( intervalUnitMultiplier );
      break;
    case LgFrHour:
      startTimes_[i+1] = startTimes_[i].plusHours( intervalUnitMultiplier );
      break;
    case LgFrMinute:
      startTimes_[i+1] = startTimes_[i].plusMinutes( intervalUnitMultiplier );
      break;
    case LgFrSecond:
      startTimes_[i+1] = startTimes_[i].plusSeconds( intervalUnitMultiplier );
      break;
    case LgFrQuarter:
      startTimes_[i+1] = startTimes_[i].plusQuarters( intervalUnitMultiplier );
      break;
    default:
      break;
    }
    LgFrDatetime dt = startTimes_[i+1];

    assert(startTimes_[i+1].isValid());
  }
  this->effectivePeriodStart();
}

// a more than fairly general constructor.
// This constructor takes a vector of periodStartTimes.
// Note: This vector must have length nPeriods+1.
//       The last entry is the starting time of what would be the next period.
//       This is used to calculate the size of the last period.
// The intervalUnit and intervalUnit multiplier seem to only be used if the
// number of periods in the calendar is increased.
LgFrCalendar::LgFrCalendar(
	     size_t   nPeriods,
	     const LgFrDatetimeVector&  periodStartTimes,
	     LgFrTimeUnit intervalUnit,
	     unsigned intervalUnitMultiplier)
: nPeriods_(nPeriods),
  startTimes_(),
  effectiveStartTimes_(),
  workUnits_(),
  intervalUnit_(intervalUnit),
  intervalUnitMultiplier_(intervalUnitMultiplier)
{
  if (nPeriods == 0) return;

  initializeOrderedVectors();
  
  // set period start times
  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<=nPeriods_; i++) {    
    assert(periodStartTimes[i].isValid());
    startTimes_[i] = periodStartTimes[i];
  }
    
  this->effectivePeriodStart();
}


 // resize the calendar.  If you make it smaller, it gets
 // truncated.  If it gets bigger, then the extra periods
 // will have garbage in them.  In general, this makes the calendar
 // invalid.
void
LgFrCalendar::nPeriods(size_t nPeriods)
{
  // remember the old nPeriods
  size_t oldNPeriods = nPeriods_;


  nPeriods_ = nPeriods;
  
  // if its bigger, then fix!
  if (oldNPeriods < nPeriods)  {

      // insert new elements (ie, resize the vectors)
      LgFrDatetime aTime;
      int t = 0; // Pulled out of the for below by RW2STL
      for (t=oldNPeriods-1; t<nPeriods; t++)  {
	  startTimes_.push_back(aTime);
	  effectiveStartTimes_.push_back(aTime);
	  workUnits_.push_back(1.0);
      }
      startTimes_.push_back(aTime);
      effectiveStartTimes_.push_back(aTime);

    // fix the startDates.
    for (t=oldNPeriods-1; t<nPeriods_; t++) {
      switch (intervalUnit_)  {
        case LgFrWeek:
          startTimes_[t+1] = startTimes_[t].plusWeeks( nPeriods );
          break;
        case LgFrDay:
          startTimes_[t+1] = startTimes_[t].plusDays( nPeriods );
          break;
        case LgFrMonth:
          startTimes_[t+1] = startTimes_[t].plusMonths( nPeriods );
          break;
        case LgFrHour:
          startTimes_[t+1] = startTimes_[t].plusHours( nPeriods );
          break;
        case LgFrMinute:
          startTimes_[t+1] = startTimes_[t].plusMinutes( nPeriods );
          break;
        case LgFrSecond:
          startTimes_[t+1] = startTimes_[t].plusSeconds( nPeriods );
          break;
        case LgFrQuarter:
          startTimes_[t+1] = startTimes_[t].plusQuarters( nPeriods );
          break;
        default:
        break;
      }
      
      assert(startTimes_[t+1].isValid());
    }

    // set the workUnits to whatever they were in the last period
    for (t=oldNPeriods; t<nPeriods_; t++)
       workUnits_[t] = workUnits_[t-1];
  }
  this->effectivePeriodStart();
}

// get the nPeriods
size_t
LgFrCalendar::nPeriods()
     const
{
  return nPeriods_;
}

  // check the validity of the calendar:
bool
LgFrCalendar::isValid()
const
{
  // 0 periods is inValid
  if (nPeriods_ == 0) return false;
  

  int i = 0; // Pulled out of the for below by RW2STL
  for (i=0; i<nPeriods_; i++) {
      // make sure each startTime is valid
      if (! startTimes_[i].isValid()) return false;
      if (! effectiveStartTimes_[i].isValid()) return false;

      // make sure period startTimes are increasing
      if (startTimes_[i] > startTimes_[i+1]) return false;
      if (effectiveStartTimes_[i] > effectiveStartTimes_[i+1]) return false;

      // make sure effectiveStarts are at least as early
      // as starts
      if (effectiveStartTimes_[i] > startTimes_[i]) return false;

    }

  if (! startTimes_[nPeriods_].isValid()) return false;
  if (! effectiveStartTimes_[nPeriods_].isValid()) return false;
  if (effectiveStartTimes_[nPeriods_] > startTimes_[nPeriods_]) return false;

  return true;
}  
  
 // set the start date/time of a period
void
LgFrCalendar::periodStart(size_t period, const LgFrDatetime& startTime)
{
  assert (period < nPeriods_);
  assert(startTime.isValid());
  startTimes_[period] = startTime;
  this->effectivePeriodStart();
}
  
  // get the start date/time of a period
LgFrDatetime
LgFrCalendar::periodStart(size_t period)
const
{
  assert (period < nPeriods_);
  return startTimes_[period];
}

// set the number of workTime units of a period
void
LgFrCalendar::workUnits(size_t period, float workAmt)
{
   assert (period < nPeriods_);
   workUnits_[period] = workAmt;
 }

// get the number of workTime units of a period
float
LgFrCalendar::workUnits(size_t period)
const
{
  assert (period < nPeriods_);
  return workUnits_[period];
}


// set/get the end of Last Period Date/Time
LgFrDatetime
LgFrCalendar::horizonEnd()
     const
{
  return startTimes_[nPeriods_];
}  

void
LgFrCalendar::horizonEnd(const LgFrDatetime& theEnd)
{
  assert (theEnd.isValid());
  startTimes_[nPeriods_] = theEnd;
  this->effectivePeriodStart();
}

// set/get the interval unit identifier
LgFrTimeUnit
LgFrCalendar::timeUnitType()
     const
{
  return intervalUnit_;
}

void
LgFrCalendar::timeUnitType(LgFrTimeUnit intervalUnit)
{
  intervalUnit_ = intervalUnit;
}

// set/get the interval unit multiplier
unsigned
LgFrCalendar::intervalUnitMultiplier()
       const
{
  return intervalUnitMultiplier_;
}

void
LgFrCalendar::intervalUnitMultiplier(unsigned intervalUnitMultiplier)
{
  intervalUnitMultiplier_ = intervalUnitMultiplier;
}

void
LgFrCalendar::display()
const
{
   std::cout << "\nCalendar Display\n"; 
   int i = 0; // Pulled out of the for below by RW2STL
   for (i=0; i < this->nPeriods(); i++) 
      std::cout << "Period " << i << ": " << this->periodStart(i).getDateAsString() << "\n";
}

// Return the period in which a specified Date/Time falls.
// Returned values:
//     0 <= nPeriods()-1   ==> date is within the horizon
//     -1                  ==> date is earlier than period 0
//     nPeriods()          ==> date is later than the end of the horizon
int
LgFrCalendar::period(const LgFrDatetime& aTime)
const
{
    int i = 0; // Pulled out of the for below by RW2STL
    for (i=0; i<=nPeriods_; i++)  {
	if (aTime < startTimes_[i])
	    return i-1;
    }

  return nPeriods_;
}
 


// This routine is similar to period() except that it rounds to the 
// nearest period start date.  For example, if the PeriodStarts are
// 1/1/95, 2/1/95, and you enter 1/27/95, then it will return the 
// period for 2/1/95, since your target is more than halfway thru the
// the period.
//
//  Returns  k  such that:
//
//  startTime[k-1] + 1/2 length(k-1) <= aTime < startTime[k] + 1/2 * length(k)
//  and   -1 <= k <= nPeriods
//  
//  where:
//     length[k] is interpreted as the length of period k,
//     length[-1] = length[0],
//     length[nPeriods] = length[nPeriods - 1]
//     and startTime[-1] is interpreted as startTime[0] - length[0]
int 
LgFrCalendar::roundedPeriod(const LgFrDatetime& aTime)
const
{

    int i = 0; // Pulled out of the for below by RW2STL
    for (i=0; i<=nPeriods_; i++)   {
	if (aTime < effectiveStartTimes_[i])
	    return i-1;
	}

    // if aTime >= effectiveStartTimes_[nPeriods_]
    return nPeriods_;
}






// Translate (and round) a date into a period for purposes of 
// earlyEffectivePeriod effectivity records.
// Returns a value k, 0 <= k <= nPeriods
int 
LgFrCalendar::earlyEffectivePeriod(const LgFrDatetime& aTime)
const
{
    int early = this->roundedPeriod(aTime);
    if (early == -1)
	return 0;
    else
	return early;
}


// Translate (and round) a date into a period for purposes of 
// early effectivity records.
// Returns a value k, -1 <= k <= nPeriods - 1
int 
LgFrCalendar::lateEffectivePeriod(const LgFrDatetime& aTime)
const
{
    int late = this->roundedPeriod(aTime) - 1;
    if (late == -2)
	return -1;
    else 
	return late;
}


 // set the effective start date/time for all periods
void
LgFrCalendar::effectivePeriodStart()
{
// first consider period 0
    long diff = startTimes_[1].getValue() - startTimes_[0].getValue();
    long halfPeriodLength = diff/2;
    if (halfPeriodLength > startTimes_[0].getValue())
	    effectiveStartTimes_[0] = LgFrDatetime((unsigned long) 0);
    else 
	    effectiveStartTimes_[0] = startTimes_[0] - halfPeriodLength;

// Now do periods 1 to nPeriods_
    int i = 0; // Pulled out of the for below by RW2STL
    for (i=0; i<nPeriods_; i++)   {
	    diff = startTimes_[i+1].getValue() - startTimes_[i].getValue();
	    halfPeriodLength = diff/2;
	    effectiveStartTimes_[i+1] = startTimes_[i+1] - halfPeriodLength;
    }

}

// Initialize ordered vector.
// This is used by constructors.
void
LgFrCalendar::initializeOrderedVectors()
{
  LgFrDatetime aTime;
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods_; t++)  {
      startTimes_.push_back(aTime);
      effectiveStartTimes_.push_back(aTime);
      workUnits_.push_back(1.0);
  }
  startTimes_.push_back(aTime);
  effectiveStartTimes_.push_back(aTime);
}


float
LgFrCalendar::numWorkingDaysInPeriodOverInterval(int period, 
						 const LgFrDatetime & intervalStart, 
						 const LgFrDatetime & intervalEnd)   
const
{
  LgFrDatetime tStart = this->periodStart(period);
  LgFrDatetime tEnd;
  if (period == this->nPeriods() - 1)
    tEnd = this->horizonEnd();
  else
    tEnd = this->periodStart(period + 1);
  
  // exclusive completely
  if ((intervalStart >= tEnd) || (intervalEnd <=  tStart))
    return 0.0;
  
  // inclusive completely 
  if ( (intervalStart <= tStart) && (tEnd <=  intervalEnd))
    return this->workUnits(period);

  // inclusiveEarly && exclusiveLate
  if ( (intervalStart <= tStart)  && (tEnd > intervalEnd) )  {
    long periodOverLap = intervalEnd.getValue() - tStart.getValue();
    long periodLength = tEnd.getValue() - tStart.getValue();
    float periodOverLapFraction = (float) periodOverLap / (float) periodLength;
    return  (this->workUnits(period) * periodOverLapFraction);
  }    

  // exclusiveEarly && inclusiveLate
  if ( (intervalStart > tStart)  && (tEnd <= intervalEnd) )  {
    long periodOverLap = tEnd.getValue() - intervalStart.getValue();
    long periodLength = tEnd.getValue() - tStart.getValue();
    float periodOverLapFraction = (float) periodOverLap / (float) periodLength;
    return  (this->workUnits(period) * periodOverLapFraction);
  }    

  // inclusiveEarly && inclusiveLate
  if ( (intervalStart > tStart)  && (tEnd > intervalEnd) )  {
    long periodOverLap = intervalEnd.getValue() - intervalStart.getValue();
    long periodLength = tEnd.getValue() - tStart.getValue();
    float periodOverLapFraction = (float) periodOverLap / (float) periodLength;
    return  (this->workUnits(period) * periodOverLapFraction);
  }    

  // anything else is an error
  assert(1 == 0);
  return 0.0;


}

// calendar utility to convert a std::string date in the form
// "YYYYMMDD" to a LgFrDatetime and vice versa.
// NOTE:  the method does NOT check the date validity  you should
// check the returned LgFrDatetime with isValid()
// NOTE 2: With the new LgFrDatetime (jan '03) that directly 
// supports the YYYYMMDD format, these have been greatly trivialized.
LgFrDatetime
LgFrCalendar::yyyymmdd(const std::string & aDateString)
const
{
    return LgFrDatetime( aDateString );
}


std::string
LgFrCalendar::yyyymmdd(const LgFrDatetime & aDate)
const
{
    return aDate.getDateAsString( "%Y%m%d" );
}

    
// Return a copy in the heap
LgFrCalendar*
LgFrCalendar::clone()
     const
{
  return new LgFrCalendar(*this);
}

// Format a LgFrCalendar into a std::string.
// Return a std::string representing the calendar, in the format specified by the formatStyle.
//
// width: ios width used to format each number in vector.
//
// precision: ios precision used to format each number in vector.
//
// iosFlags: ios formating flags used to format each number in vector.
//
std::string
LgFrCalendar::format( 
		      const LgFrCalendarFormatStyle formatStyle )
const
{

    std::string retVal;

    if (formatStyle == TimeVecPeriodStarts)  {

	    int x = 0; // Pulled out of the for below by RW2STL
	    for (x = 0; x < nPeriods_; x++ ){
            retVal += periodStart(x).getDateAsString();
            retVal += ' ';
        }
    }
    else {
	    std::cerr << "LgFrCalendar::format.  Unrecognized format style.\n";
	    abort();
    }
    return retVal;

};



// ----- Default Constructors -------

// default constructor 8 periods ==> weekly buckets starting 8/1/95 
LgFrCalendar::LgFrCalendar()
: nPeriods_(8),
  startTimes_(),
  effectiveStartTimes_(),
  workUnits_(),
  intervalUnit_(LgFrWeek),
  intervalUnitMultiplier_(1)
{

// initialize the Ordered Vectors
  LgFrDatetime aTime;
  int t = 0; // Pulled out of the for below by RW2STL
  for (t=0; t<nPeriods_; t++)  {
      startTimes_.push_back(aTime);
      effectiveStartTimes_.push_back(aTime);
      workUnits_.push_back(1.0);
  }
  startTimes_.push_back(aTime);
  effectiveStartTimes_.push_back(aTime);

  // set first period
  LgFrDatetime dt;
  dt.setDate( 8, 1, 1995 );
  startTimes_[0] = dt;
  
  for (t=0; t<nPeriods_; t++) 
    startTimes_[t+1] = LgFrDatetime(startTimes_[t]) + 7;
  
  // set the effectiveStartTimes_
  this->effectivePeriodStart();

  assert(this->isValid());
}

 // copy constructor
LgFrCalendar::LgFrCalendar(const LgFrCalendar& source)
: nPeriods_(source.nPeriods_),
  startTimes_(source.startTimes_),
  effectiveStartTimes_(source.effectiveStartTimes_),
  workUnits_(source.workUnits_),
  intervalUnit_(source.intervalUnit_),
  intervalUnitMultiplier_(source.intervalUnitMultiplier_)
{
  // nothing to do
}

// assignment operator
LgFrCalendar&
LgFrCalendar::operator=(const LgFrCalendar& rhs)
{
  if (this != &rhs) {		// Check for assignment to self
    nPeriods_ = rhs.nPeriods_;
    startTimes_ = rhs.startTimes_;
    effectiveStartTimes_ = rhs.effectiveStartTimes_;
    workUnits_ = rhs.workUnits_;
    intervalUnit_ = rhs.intervalUnit_;
    intervalUnitMultiplier_ = rhs.intervalUnitMultiplier_;
  }
  return *this;
}

// destructor
LgFrCalendar::~LgFrCalendar()
{
  // nothing to do
}




#ifdef NDEBUG
#undef NDEBUG
#endif
// self-test
void
LgFrCalendar::test()
{

    assert( LgFrDatetime::unitTest() ); // Added Jan '03

    LgFrCalendarFormatStyle formatStyle = TimeVecPeriodStarts;

    LgFrCalendar myCal1 (6, LgFrDatetime("7/15/94"), LgFrMonth, 2);
    assert(myCal1.nPeriods() == 6);
    assert(myCal1.isValid());


    LgFrCalendar myCal2(9,                                       
			LgFrDatetime("12/30/94"),
			LgFrHour,
			8);

    assert(myCal2.isValid());


    LgFrCalendar myCal3(24,                                      
			LgFrDatetime("12/31/94"),
			LgFrMonth,
			1);

    assert(myCal3.isValid());

    // set period 1 to be earlier than period 0
    // and check for inValid.
    LgFrDatetime aNewTime (LgFrDatetime("12/15/94"));
    myCal3.periodStart(1, aNewTime);
    assert (aNewTime == myCal3.periodStart(1));
    assert (myCal3.isValid() == false);

    // set period 1 to be valid again
    myCal3.periodStart(1, LgFrDatetime("1/15/95"));
    assert(myCal3.isValid());

    // set horizon end to be earlier than period 0
    LgFrDatetime anEndTime (LgFrDatetime("12/15/93"));
    myCal3.horizonEnd(anEndTime);
    assert(anEndTime == myCal3.horizonEnd());

    // fix it
    LgFrDatetime goodEndTime (LgFrDatetime("12/15/99"));
    myCal3.horizonEnd(goodEndTime);
    assert(goodEndTime == myCal3.horizonEnd());
    assert (myCal3.isValid());

    assert (myCal3.period(goodEndTime) == myCal3.nPeriods());
    assert (myCal3.period(LgFrDatetime("1/16/95"))  ==  1);
    assert (myCal3.period(LgFrDatetime("1/14/94"))  == -1);
    assert (myCal3.period(LgFrDatetime("12/31/94")) ==  0);

    // myCal3.display();


    LgFrCalendar myCal4(64,                                      
			LgFrDatetime("1/1/94"),
			LgFrMonth,
			1);

    assert(myCal4.isValid());

    // increase to 12 periods and check
    myCal4.nPeriods(12);
    assert(myCal4.isValid());
    myCal4.display();
  
    // now do some funky stuff ...
    // set unitType to quarters and extend the calendar
    myCal4.timeUnitType(LgFrQuarter);
    myCal4.nPeriods(16);
    assert(myCal4.isValid());
    myCal4.display();

    LgFrCalendar myCal5(12,                                      
			LgFrDatetime("1/1/94"),
			LgFrMonth,
			1);

    assert(myCal5.isValid());

    assert(myCal5.roundedPeriod(LgFrDatetime("1/16/94")) == 0);
    assert(myCal5.roundedPeriod(LgFrDatetime("1/17/94")) == 1);
    assert(myCal5.roundedPeriod(LgFrDatetime("12/25/93")) == 0);
    assert(myCal5.roundedPeriod(LgFrDatetime("12/11/93")) == -1);
    assert(myCal5.roundedPeriod(LgFrDatetime("5/25/94")) == 5);
    assert(myCal5.roundedPeriod(LgFrDatetime("12/14/94")) == 11);
    assert(myCal5.roundedPeriod(LgFrDatetime("12/25/94")) == 12);
    assert(myCal5.roundedPeriod(LgFrDatetime("1/10/95")) == 12);
    assert(myCal5.roundedPeriod(LgFrDatetime("1/20/95")) == 12);

    assert(myCal5.earlyEffectivePeriod(LgFrDatetime("5/25/94")) == 5);
    assert(myCal5.lateEffectivePeriod( LgFrDatetime("5/25/94")) == 4);

    assert(myCal5.earlyEffectivePeriod(LgFrDatetime("12/25/93")) == 0);
    assert(myCal5.earlyEffectivePeriod(LgFrDatetime("12/25/91")) == 0);

    assert(myCal5.lateEffectivePeriod( LgFrDatetime("1/12/95")) == 11);
    assert(myCal5.lateEffectivePeriod( LgFrDatetime("1/25/95")) == 11);

    // happy new year!
    LgFrDatetime date1( "1/1/1995" );
    assert("19950101" == myCal5.yyyymmdd(date1));
    assert(date1 == myCal5.yyyymmdd("19950101"));

    // merry christmas
    LgFrDatetime date2( "12/25/1995" );
    assert("19951225" == myCal5.yyyymmdd(date2));
    assert(date2 == myCal5.yyyymmdd("19951225"));

    //Test the format method
    std::cout << "Testing the format method." << std::endl;

    std::cout << "The following should display a 1 month period, starting 1/1/96" << std::endl;
    LgFrCalendar formatCal1(1,                                     
			    LgFrDatetime("1/1/96"),
			    LgFrMonth,
			    1);
    std::cout << formatCal1.format(formatStyle) << std::endl;

    std::cout << "The following should display a 12 month period, starting 1/1/96 with a one month interval." << std::endl;
    LgFrCalendar formatCal2(12,                                     
			    LgFrDatetime("1/1/96"),
			    LgFrMonth,
			    1);
    std::cout << formatCal2.format(formatStyle) << std::endl;

    std::cout << "The following should display a 12 second periods, which should display the same date 12 times. 1/1/96" << std::endl;
    LgFrCalendar formatCal3(12,                                     
			    LgFrDatetime("1/1/96"),
			    LgFrSecond,
			    5);
    std::cout << formatCal3.format(formatStyle) << std::endl;

    std::cout << "The following should display a 12 minute periods, starting 1/1/96, which should display the same date, 12 times. 1/1/96." << std::endl;
    LgFrCalendar formatCal4(12,                                     
			    LgFrDatetime("1/1/96"),
			    LgFrMinute,
			    5);
    std::cout << formatCal4.format(formatStyle) << std::endl;

    std::cout << "The following should display 6 quarters, starting 1/1/96" << std::endl;
    LgFrCalendar formatCal5(6,                                     
			    LgFrDatetime("1/1/96"),
			    LgFrQuarter,
			    1);
    std::cout << formatCal5.format(formatStyle) << std::endl;

    // Test new constructor
    {
      LgFrDatetimeVector timeVec(4);
      timeVec[0]=LgFrDatetime("6/24/96");
      timeVec[1]=LgFrDatetime("7/2/96");
      timeVec[2]=LgFrDatetime("7/8/96");
      timeVec[3]=LgFrDatetime("8/5/96");
      LgFrCalendar cal(3,timeVec);
      std::cout << cal.format(formatStyle) << std::endl;
    }  
}


