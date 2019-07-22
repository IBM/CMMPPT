#ifndef  LGFRCALENDAR_H
#define  LGFRCALENDAR_H
// RW2STL -- inserted:
#include <string>
// End of RW2STL -- inserted includes.

// file: calendar.h
// author: tom


#include <iostream>
#include <scenario/src/dateTime.h>

#include <scenario/src/generVc.h>

//  Calendar object:  Used to map real dates/times to periods.
//  It maintins a vector of periodStartTimes using LgFrDatetime.
//  LgFrDatetime works hand-in-hand with LgFrDatetime.  Holidays or plant
//  shutdowns are NOT considered by this object.  However, it
//  does provide a user-settable vector of floats that can be
//  used to store the number of workingTimeUnits available
//  during each period.  This data is NOT used by the calendar
//  and is only provided for the convenience of the client.
//  In addition, the Calendar object defines an enumerated type
//  for distinguishing timeUnits.  This is only used to make a
//  convenient means for constructing an instance with fixed
//  interval time periods.  The object will retain the timeUnitType
//  but only for the convenience of the client.

typedef enum {LgFrQuarter, LgFrMonth, LgFrDay, LgFrWeek, LgFrHour,
              LgFrMinute, LgFrSecond}
        LgFrTimeUnit;

typedef enum {TimeVecPeriodStarts}
        LgFrCalendarFormatStyle;

class LgFrCalendar {

public:


  // fairly general constructor:
  LgFrCalendar(size_t         nPeriods,
	       const LgFrDatetime&  horizonStart,
	       LgFrTimeUnit   intervalUnit = LgFrWeek,
	       unsigned       intervalUnitMultiplier = 1);
  // Create a calendar with "nPeriods", the first period beginning at
  // horizonStart, and each subsequent period beginning exactly
  // intervalUnitMultiplier intervalUnits after its predecessor.
  // Note: since months and quarters don't necessarily match up, the
  // convention is used that each subsequent period begins on the
  // same dayNumber of the month.  If this causes an invalid date,
  // say, Feb 30, then the dayNumber is "rewound" to a valid one.
  // Examples:
  //   LgFrCalendar myCal(6,
  //                      LgFrDatetime(LgFrDatetime("7/15/94")),
  //                      LgFrMonth,
  //                      2);
  // 6 periods, 2 month intervals, period 0 starts 7/15/94 
  //
  //   LgFrCalendar myCal(9,                                       
  //                      LgFrDatetime(LgFrDatetime("7/15/94"), "08:00"),
  //                      LgFrHour,
  //                      8);
  // 9 periods, 8-hour shifts, the first shift starts at 8am on 7/15/94


  // a more than fairly general constructor.
  // This constructor takes a vector of periodStartTimes.
  // Note: This vector must have length nPeriods+1.
  //       The last entry is the starting time of what would be the next period.
  //       This is used to calculate the size of the last period.
  // The intervalUnit and intervalUnit multiplier seem to only be used if the
  // number of periods in the calendar is increased.
  LgFrCalendar(
	       size_t   nPeriods,
	       const LgFrDatetimeVector&  periodStartTimes,
	       LgFrTimeUnit   intervalUnit = LgFrWeek,
	       unsigned       intervalUnitMultiplier = 1);
  
  // Return the period in which a specified Date/Time falls.
  // Returned values:
  //     0 <= nPeriods()-1   ==> date is within the horizon
  //     -1                  ==> date is earlier than period 0
  //     nPeriods()          ==> date is later than the end of the horizon
  virtual
  int
  period(const LgFrDatetime&)
    const;


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
  roundedPeriod(const LgFrDatetime& aTime)
    const;


  // Mapping dates to periods:  EARLY and LATE EFFECTIVITY DATES
  // A note about effectivity dates.  These methods, earlyEffectivePeriod() and 
  // lateEffectivePeriod() will round a date(time) to a period.  The following 
  // is assumed about the dates:  
  //   --  An early effective date is INCLUSIVE    (consistent with WIT)
  //   --  A  late  effective date is EXCLUSIVE    (not consistent with WIT)
  //   --  The early effective period is INCLUSIVE (consistent with WIT)
  //   --  The late  effective period is INCLUSIVE (consistent with WIT)
  // 
  // Translate (and round) a date into a period for purposes of 
  // early effectivity records.  
  // Returns k st   0 <= k ,+ nPeriods
  virtual
  int 
  earlyEffectivePeriod(const LgFrDatetime& aTime)
    const;


  // Translate (and round) a date into a period for purposes of 
  // late effectivity records.
  // Returns k st   -1 <= k <= nPeriods - 1
  virtual
  int 
  lateEffectivePeriod(const LgFrDatetime& aTime)
    const;



  // resize the calendar.  If you make it smaller, it gets
  // truncated.  If it gets bigger, then it will be extended.
  // Extending the calendar simply uses the calendars setting of
  // timeUnitType and intervalUnitMultiplier and then makes
  // the first  new periodStartDate offset that amount from
  // the periodStartDate of the last period of the original
  // calendar.  If you want the horizon end date to be other than
  // the offset fromthe last period start date, then you must set it
  // "manually" with the horizonEnd() method.  Although, this method
  // should return a valid calendar (assuming the existing calendar was
  // already valid), it is not guaranteed and you  should check it 
  // with the  isValid() method.
  virtual
  void
  nPeriods(size_t nPeriods);

  // get the nPeriods
  virtual
  size_t
  nPeriods()
    const;

  // check the validity of the calendar:
  virtual
  bool
  isValid()
    const;

  // set the start date/time of a period
  virtual
  void
  periodStart(size_t period, const LgFrDatetime& startTime);

  // get the start date/time of a period
  virtual
  LgFrDatetime
  periodStart(size_t period)
    const;

  // get the number of workTime units of a period
  virtual
  void
  workUnits(size_t period, float workAmt);

  // get the number of workTime units of a period
  virtual
  float
  workUnits(size_t period)
    const;


  float
  numWorkingDaysInPeriodOverInterval(int period, 
				     const LgFrDatetime & intervalStart, 
				     const LgFrDatetime & intervalEnd)
    const;




  // calendar utility to convert a std::string date in the form
  // "YYYYMMDD" to a LgFrDatetime and vice versa.
  // NOTE:  the method does check the date validity  you should
  // check the returned LgFrDatetime with isValid()
  virtual
  LgFrDatetime
  yyyymmdd(const std::string & aDateString)
    const;

  virtual
  std::string
  yyyymmdd(const LgFrDatetime & aDate)
    const;

#if 0
  // calendar utility to convert a std::string date in the form
  // "ddmonyy" (e.g. 24 Mar 98) to a LgFrDatetime and vice versa.
  // NOTE:  the method does NOT check the date validity.
  // Check the returned LgFrDatetime with isValid().
  virtual
  LgFrDatetime
  ddmonyy(const std::string & aDateString)
    const;

  virtual
  std::string
  ddmonyy(const LgFrDatetime & aDate)
    const;
#endif


  // set/get the end of Last Period Date/Time
  virtual
  LgFrDatetime
  horizonEnd()
    const;

  virtual
  void
  horizonEnd(const LgFrDatetime& theEnd);

  // set/get the interval unit identifier
  virtual
  LgFrTimeUnit
  timeUnitType()
    const;

  virtual
  void
  timeUnitType(LgFrTimeUnit);

  // set/get the interval unit multiplier
  unsigned
  intervalUnitMultiplier()
    const;

  void
  intervalUnitMultiplier(unsigned intervalUnitMultiplier);


  // return a (legal) time,  numMonthsOut in the future from a
  // given date (Utility function)
  LgFrDatetime
  addMonths(const LgFrDatetime& aTime, unsigned numMonthsOut,
			  unsigned newDay)
    const;


  // Return a copy in the heap
  LgFrCalendar*
  clone()
    const;

  // default constructor 0 periods ==> generates an invalid
  LgFrCalendar();

  // copy constructor
  LgFrCalendar(const LgFrCalendar& source);

  // assignment operator
  LgFrCalendar&
  operator=(const LgFrCalendar& rhs);

  // destructor
  virtual
  ~LgFrCalendar();

  // simple cout of calendar start dates
  // Use the features of LgFrDatetime and LgFrDatetime to create
  // fancier displays.     
  virtual     
  void
  display()
    const;

  // Format a LgFrCalendar into a std::string.
  // Return a std::string representing the calendar, in the format
  //        specified by the formatStyle.
  //
  // width: ios width used to format each number in vector.
  //
  // precision: ios precision used to format each number in vector.
  //
  // iosFlags: ios formating flags used to format each number in vector.
  //
  std::string format( 
		   const LgFrCalendarFormatStyle formatStyle )/*,
		   const int width = 10,
		   const int precision = 1,
		   const long iosFlags = ios::fixed | std::showpoint | std::right )*/
    const;


  // self-test
  static void test();

private:


  // set the effective start date/time for all periods
  virtual
  void
  effectivePeriodStart();

  // Initizlize member data ordered vectors. Used by constructors.
  virtual
  void
  initializeOrderedVectors();
  
  // number of periods 
  size_t            nPeriods_;

  // vector of period starting times/dates
  LgFrDatetimeVector    startTimes_;


  // vector of period startingTimes for Rounding
  LgFrDatetimeVector    effectiveStartTimes_;

  // vector of period workUnits available per period
  LgFrFloatVector   workUnits_;

  // interval Unit identifier
  LgFrTimeUnit      intervalUnit_;

  // interval Unit multiplier
  unsigned          intervalUnitMultiplier_;


};


#endif
