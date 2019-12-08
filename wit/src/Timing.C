//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

//------------------------------------------------------------------------------
// Source file: "Timing.C"
//
// Contains the implementation of the following classes:
//
//    Timer
//    TimingSection
//------------------------------------------------------------------------------

#include <Timing.h>
#include <MsgFac.h>
#include <ApiMgr.h>

#include <time.h>

//------------------------------------------------------------------------------
// Implementation of class Timer.
//------------------------------------------------------------------------------

WitTimer * WitTimer::uniqueTimer_ = NULL;

//------------------------------------------------------------------------------

WitTimer::WitTimer (WitMsgFacility & theMsgFac):

      myMsgFac_       (theMsgFac),
      prevTime_       (cpuTime ()),
      allSections_    (),
      activeSections_ ()
   {
   stronglyAssert (uniqueTimer_ == NULL);

   uniqueTimer_ = this;

   new WitTimingSection (this, "input",      "Input Reading");
   new WitTimingSection (this, "default",    "Default Output");
   new WitTimingSection (this, "preproc",    "Preprocessing");
   new WitTimingSection (this, "heuristic",  "Heuristic");
   new WitTimingSection (this, "opt-prob",   "Opt. Prob. Management");
   new WitTimingSection (this, "coin",       "COIN");
   new WitTimingSection (this, "cplex",      "CPLEX");
   new WitTimingSection (this, "postproc",   "Postprocessing");
   new WitTimingSection (this, "pip",        "P.I. Pegging");
   new WitTimingSection (this, "mrp",        "MRP");
   new WitTimingSection (this, "fss",        "FSS");
   new WitTimingSection (this, "freeMemory", "Memory Deallocation");
   new WitTimingSection (this, "other",      "Other CPU");
   new WitTimingSection (this, "standard",   "Standard CPU");
   new WitTimingSection (this, "extra",      "Non-default Output");
   new WitTimingSection (this, "total",      "Total CPU");
   }

//------------------------------------------------------------------------------

WitTimer::~WitTimer ()
   {
   while (not allSections_.isEmpty ())
      delete allSections_.get ();

   stronglyAssert (uniqueTimer_ == this);

   uniqueTimer_ = NULL;
   }

//------------------------------------------------------------------------------

void WitTimer::enterSection (const char * theName)
   {
   WitTimingSection * theSection;

   if (uniqueTimer_ == NULL)
      return;

   if (not uniqueTimer ()->activeSections_.isEmpty ())
      uniqueTimer ()->activeSections_.first ()->chargeTime ();

   theSection = namedSection (theName);

   stronglyAssert (theSection != NULL);

   uniqueTimer ()->activeSections_.prepend (theSection);
   }

//------------------------------------------------------------------------------

void WitTimer::leaveSection (const char * theName)
   {
   WitTimingSection * theSection;

   if (uniqueTimer_ == NULL)
      return;

   stronglyAssert (not uniqueTimer ()->activeSections_.isEmpty ());

   theSection = uniqueTimer ()->activeSections_.get ();

   stronglyAssert (theSection->name () == theName);

   theSection->chargeTime ();
   }

//------------------------------------------------------------------------------

double WitTimer::getTimeAndChargeToCurrent ()
   {
   double timeSP;

   if (uniqueTimer_ == NULL)
      return 0.0;

   stronglyAssert (not uniqueTimer ()->activeSections_.isEmpty ());

   timeSP = uniqueTimer ()->timeSincePrev ();
                            
   uniqueTimer ()->activeSections_.first ()->chargeTime (timeSP);

   return timeSP;
   }

//------------------------------------------------------------------------------

void WitTimer::include (WitTimingSection * theSection)
   {
   allSections_.append (theSection);
   }

//------------------------------------------------------------------------------

double WitTimer::timeSincePrev ()
   {
   double curTime;
   double delta;

   curTime   = cpuTime ();
   delta     = curTime - prevTime_;
   prevTime_ = curTime;

   namedSection ("total")->chargeTime (delta);

   return delta;
   }

//------------------------------------------------------------------------------

void WitTimer::display ()
   {
   WitTimingSection * theSection;

   stronglyAssert (activeSections_.isEmpty ());

   namedSection ("standard")->chargeTime (
        namedSection ("total")->timeCharged ()
      - namedSection ("extra")->timeCharged ());

   myMsgFac () ("cpuTimesMsg");

   forEachEl (theSection, allSections_)
      theSection->display ();
   }

//------------------------------------------------------------------------------

void WitTimer::reset ()
   {
   WitTimingSection * theSection;

   stronglyAssert (activeSections_.isEmpty ());

   timeSincePrev ();

   forEachEl (theSection, allSections_)
      theSection->reset ();
   }

//------------------------------------------------------------------------------

void WitTimer::addTime (const char * theName1, const char * theName2)
   {
   WitTimingSection * theSection1;
   WitTimingSection * theSection2;

   theSection1 = namedSection (theName1);
   theSection2 = namedSection (theName2);

   if (theSection2->wasTimed ())
      theSection1->chargeTime (theSection2->timeCharged ());
   }

//------------------------------------------------------------------------------

WitTimingSection * WitTimer::namedSection (const char * theName)
   {
   if (uniqueTimer_ == NULL)
      return NULL;

   WitTimingSection * theSection;

   forEachEl (theSection, uniqueTimer_->allSections_)
      if (theSection->name () == theName)
         return theSection;

   stronglyAssert (false);

   return NULL;  // Never executed; Makes the compiler happy.
   }

//------------------------------------------------------------------------------
// cpuTime ()
//
// The implementation on RS/6000             uses getrusage (...).
// The implementation on all other platforms uses clock ()
//------------------------------------------------------------------------------

#ifdef RS6K

#include <sys/time.h>
#include <sys/resource.h>

double WitTimer::cpuTime () const
   {
   struct rusage theRusage;
   int           rc;
   double        seconds;
   double        microSeconds;

   rc           = getrusage (RUSAGE_SELF, & theRusage);

   witAssert (rc == 0);

   seconds      = static_cast <double> (theRusage.ru_utime.tv_sec);
   microSeconds = static_cast <double> (theRusage.ru_utime.tv_usec);

   return (seconds + microSeconds / 1.0e+6);
   }

#else

double WitTimer::cpuTime () const
   {
    return
         static_cast <double> (clock ())
       / static_cast <double> (CLOCKS_PER_SEC);
   }

#endif

//------------------------------------------------------------------------------
// Implementation of class TimingSection.
//------------------------------------------------------------------------------

WitTimingSection::WitTimingSection (
         WitTimer * theTimer,
         const char * sectionName,
         const char * sectionTitle):

      name_        (sectionName),
      myTimer_     (theTimer),
      title_       (sectionTitle),
      wasTimed_    (false),
      timeCharged_ (0.0)
   {
   stronglyAssert (strlen (sectionTitle) <= 21);

   myTimer_->include (this);
   }

//------------------------------------------------------------------------------

WitTimingSection::~WitTimingSection ()
   {
   }

//------------------------------------------------------------------------------

void WitTimingSection::chargeTime (double incTime)
   {
   timeCharged_ += incTime;

   wasTimed_     = true;
   }

//------------------------------------------------------------------------------

void WitTimingSection::chargeTime ()
   {
   chargeTime (myTimer_->timeSincePrev ());
   }

//------------------------------------------------------------------------------

void WitTimingSection::reset ()
   {
   wasTimed_    = false;
   timeCharged_ = 0.0;
   }

//------------------------------------------------------------------------------

void WitTimingSection::display () const
   {
   if (not wasTimed_)
      return;

   char titleString[100];

   sprintf (titleString, "%s time:", title_.myCstring ());

   myTimer_->myMsgFac () ("sectionTimeMsg", titleString, timeCharged ());
   }
