//==============================================================================
// Constrained Materials Management and Production Planning Tool
//
// (C) Copyright IBM Corp. 1993, 2012  All Rights Reserved
//==============================================================================

#ifndef TimingH
#define TimingH

//------------------------------------------------------------------------------
// Header file: "Timing.h"
//
// Contains the declaration of the following classes:
//
//    Timer
//    TimingSection
//
// These classes constitute the timing subsystem of WIT and are used only in
// stand-alone mode.
//------------------------------------------------------------------------------

#include <Str.h>
#include <List.h>

class WitTimingSection;

//------------------------------------------------------------------------------
// class Timer.
// Owns and manages the TimingSections of a stand-alone run.
//------------------------------------------------------------------------------

class WitTimer
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitTimer (WitMsgFacility & theMsgFac);
         //
         // myMsgFac_ <-- theMsgFac.

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitTimer ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (WitMsgFacility &, myMsgFac)

      //------------------------------------------------------------------------
      // Static public member functions.
      //------------------------------------------------------------------------

      static void enterSection (const char * theName);
         //
         // Causes timing to enter the TimingSection matching theName.
         // No-op, when not in stand-alone mode.

      static void leaveSection (const char * theName);
         //
         // Causes timing to leave the current TimingSection, which should
         // match theName.
         // No-op, when not in stand-alone mode.

      static double getTimeAndChargeToCurrent ();
         //
         // Charges timeSincePrev () to the current TimingSection.
         // Returns timeSincePrev ().
         // No-op (returns 0), when not in stand-alone mode.

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void include (WitTimingSection * theSection);
         //
         // Includes theSection in allSections_.

      double timeSincePrev ();
         //
         // Returns process time since previous time timeSincePrev () was
         // called. Sets prevTime_ to current value of cpuTime ().

      void display ();
         //
         // Displays timing data at the end of a stand-alone run.

      void reset ();
         //
         // Re-initializes timing: starts timing over again.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      void addTime (const char * theName1, const char * theName2);
         //
         // Adds the time charged to the TimingSection for theName2 to
         //      the time charged to the TimingSection for theName1.

      static WitTimingSection * namedSection (const char * theName);
         //
         // Returns the TimingSection whose name is theName.
         // Generates an error, if no such TimingSection exists.
         // Uses a linear search.
         // Returns NULL, if uniqueTimer_ is NULL.

      double cpuTime () const;
         //
         // Returns the current cpu time, in seconds.

      inline static WitTimer * uniqueTimer ()
         {
         witAssert (uniqueTimer_ != NULL);

         return uniqueTimer_;
         }

      noCopyCtorAssign (WitTimer);

      //------------------------------------------------------------------------
      // Static private member data.
      //------------------------------------------------------------------------

      static WitTimer * uniqueTimer_;
         //
         // The unique instance of the class in stand-alone mode.
         // NULL, when not in stand-alone mode.

      //------------------------------------------------------------------------
      // Other private member data.
      //------------------------------------------------------------------------

      WitMsgFacility & myMsgFac_;
         //
         // The MsgFacility for this Timer.

      double prevTime_;
         //
         // Value returned from cpuTime () the last time timeSincePrev was
         // called, converted to seconds.

      WitList <WitTimingSection> allSections_;
         //
         // The list of all TimingSections, in the order in which they are
         // to appear in the output.

      WitList <WitTimingSection> activeSections_;
         //
         // The LIFO stack of TimingSections entered, but not yet exited.
   };

//------------------------------------------------------------------------------
// class TimingSection.
// Represents a section of the code for timing purposes. At any time during a
// stand-alone run, the CPU time is "charged" to one of the TimingSections.
// At the end of a run, a table is printed showing how much time was charged
// to each TimingSection.
//------------------------------------------------------------------------------

class WitTimingSection
   {
   public:

      //------------------------------------------------------------------------
      // Constructor functions.
      //------------------------------------------------------------------------

      WitTimingSection (
            WitTimer * theTimer,
            const char * sectionName,
            const char * sectionTitle);
         //
         // myTimer_ <-- theTimer
         // name_    <-- sectionName
         // title_   <-- sectionTitle

      //------------------------------------------------------------------------
      // Destructor function.
      //------------------------------------------------------------------------

      ~WitTimingSection ();

      //------------------------------------------------------------------------
      // Data access functions.
      //------------------------------------------------------------------------

      accessFunc (const WitString &, name)
      accessFunc (bool,              wasTimed)
      accessFunc (double,            timeCharged)

      //------------------------------------------------------------------------
      // Other public member functions.
      //------------------------------------------------------------------------

      void chargeTime ();
         //
         // Adds timeSincePrev () to timeCharged ().

      void chargeTime (double incTime);
         //
         // Adds incTime to timeCharged ().

      void reset ();
         //
         // Re-initializes this TimingSection.

      void display () const;
         //
         // Displays the time spent in this TimingSection.

   private:

      //------------------------------------------------------------------------
      // Private member functions.
      //------------------------------------------------------------------------

      noCopyCtorAssign (WitTimingSection);

      //------------------------------------------------------------------------
      // Private member data.
      //------------------------------------------------------------------------

      const WitString name_;
         //
         // The name of this TimingSection, used to identify it in the
         // source code.

      WitTimer * const myTimer_;
         //
         // The Timer to which this TimingSection belongs.

      const WitString title_;
         //
         // Title used when displaying this TimingSection.

      bool wasTimed_;
         //
         // true iff this TimingSection has been timed at least once.

      double timeCharged_;
         //
         // The CPU time charged to this TimingSection.
   };

#endif
