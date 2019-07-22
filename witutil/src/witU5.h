#ifndef WITU5_H
#define WITU5_H

#include <wit/src/wit.h>

enum {
  WUTL_CURRENT_PERIOD = -1,
  WUTL_LAST_PERIOD = 0
};


#ifdef __cplusplus
  extern "C" {
#endif
    
/* ----------    WUtlComputeDependentDemandAndPlanOrders  --------------------

   witMrp puts requirements on subassemblies when the cycle time 
   results in an insufficient amount of time to build the subassembley.
   This function fully explodes down the BOM and does not place
   requirements on the subassemblies.  It returns two part attributes.
   They are :
      fullyExplodedMrpConsVol: 
         Same as wit's mrpConsVol, only fully exploded.
      plannedOrders:
         For product parts this is the same as wit's mrpProdVol, only fully
         exploded.
         For raw parts this is the same as wit's reqVol (which is net), only
         fully exploded.

   This thing goes through the tempWitRun and computes the *FULLY
   EXPLODED* Dependent demand.  It also Computes an MRP-like plan
   orders schedule.  You can control the plan-order logic (to some
   extent) via a few pamamaters to this function.

   Usage Notes::

   1) The function will change your WitRun object, both structurally
   and data-wise.  Therefore it is highly recommended that you pass in
   a fully populated copy of the original WitRun object.  Use the
   witCopyData() api function.

   2) You create the partList vector and nParts and pass it in. This
   saves time (espcecially if you call this routine multiple times).

   3) You must create (malloc) the solution vectors that will be
   filled in by this routine.  This is, again, done for efficiency
   reasons.

   4) The solutions vectors, dependentDemand and planOrders, vector of
   float *'s, malloced to length nParts.  Then you must populate these
   vectors by malloc'ing one float vector of length nPeriods for each
   pointer.  You do not need to initialize the values.  See the sample
   code.

   5) When the routine has finished, you are responsible for free'ing
   the tempWitRun, and the solution vectors.  See the sample code.

   5) There are some special parameters that you set.  These tell the
   function what kind of "netting logic" to apply in its calculations.
   They are described below.

   6) whereToPutOutstandingReqVolForRaw: Set to WUTL_CURRENT_PERIOD, or
   WUTL_LAST_PERIOD.  For parts of type WitRAW, if you want planorders
   specified in the period that reqVol is found, then use
   WUTL_CURRENT_PERIOD.  Otherwise, use WUTL_LAST_PERIOD, in which case, the
   plan order is moved to the last period.  This is equivalent to a
   frozen-zone set at the last period.  If you want to set the frozen
   zone by part, then you must model the part as a WitPRODUCT and use
   the "cycleTime" attribute.  Call the WIT Team for details.

   7) whereToPutOutstandingReqVolForProductWithEmptyBom: Set to
   WUTL_CURRENT_PERIOD, or WUTL_LAST_PERIOD.  For parts of type WitPRODUCT that
   have NO CHILDREN components (a special case in WIT), the planOrders
   will always be placed so that they are "leadTime feasible" (ie,
   feasible with respect to cycleTime).  If there is nowhere to place
   the order such that its feasible, then how you set this paramter
   governs where the planOrder is placed.  The setting is similar to
   the parameter, whereToPutOutstandingReqVolForRaw.  (Note: this case
   would occur, roughly, under three possible cases: (1) if the global
   wit paramter "buildWhenEmpty" is is set to WitFALSE, (2) the
   cycleTime vector is ALL -1.0's, or (3) the cycleTime vector is
   nonNegative for periods 0,p and -1.0 for periods p+1,nPeriods-1 AND
   a reqVol exists in a period in the interval (p+1,nPeriods-1).
   Sorry witMrp does not look ahead.


   8) whereToPutOutstandingReqVolForProductWithBom: Set to
   WUTL_CURRENT_PERIOD, or WUTL_LAST_PERIOD.  For parts of type WitPRODUCT that
   have children, the planOrders will always be placed so that they
   are "leadTime feasible" (ie, feasible with respect to cycleTime).
   If there is nowhere to place the order such that its feasible, then
   how you set this parameter governs where the planOrder is placed.
   The setting is similar to the parameter,
   whereToPutOutstandingReqVolForRaw.  (Note: this case would occur,
   roughly, under two possible cases: (1) the cycleTime vector is ALL
   -1.0's, or (2) the cycleTime vector is nonNegative for periods 0,p
   and -1.0 for periods p+1,nPeriods-1 AND a reqVol exists in a period
   in the interval (p+1,nPeriods-1).  Sorry witMrp does not look
   ahead.


   9) resetDemandVolsToZeroWhenDone:
   use either 1, if you want the demandVols in tempWitRun
   reset to zero (including any dummy demands created here),
   OR, 0, if you don't.  Note, if you will re-use this
   tempWitRun again for this routine, then set it to 1.
   Otherwise set it to 0 (it'll go faster).           

   

   ----------------------------------------------------------------------  */


void WUtlComputeDependentDemandAndPlanOrders(
    WitRun * tempWitRun,  
    int      nParts,     
    char  ** partList,
    float ** dependentDemand,  /* Solution vector */
    float ** planOrders,       /* Solution vector */ 
    int   whereToPutOutstandingReqVolForRaw,    
    int   whereToPutOutstandingReqVolForProductWithEmptyBom,
    int   whereToPutOutstandingReqVolForProductWithBom,
    int   includeOrderCancelRecommendationInPlanOrder,
    int   resetDemandVolsToZeroWhenDone);


#ifdef __cplusplus
  }
#endif


#endif





