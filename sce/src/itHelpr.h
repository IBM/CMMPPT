// file: itHelpr.h
// author: robin

#ifndef  LGFRSCEITERATIVEIMPHELPER_H
#define  LGFRSCEITERATIVEIMPHELPER_H


#include <wit/src/wit.h>


// forward reference



// Iterative Implosion Helper class helps to do what needs to be done when
// iteratively imploding

class LgFrSceIterativeImpHelper {

public:
  // Make a working copy of a witRun and set the messaging
  // NOTE: it is up to the client to delete the working copy
  WitRun *
  copyWitRunWithMsging( WitRun * const theOrigWitRun,
                        int printInformationalWitMessages,
                        const char * witMessageFilename);


  // get demand lists
  // NOTE: calling object is responsible for memory clean-up
  void
  getDemandLists( WitRun    * theWitRun,
                  int         nParts,
                  char    **  partList,
                  int     **  nDemandsPtr,
                  char  ****  demandListPtr );

  // copy demand vols, so we can clean-up after we muck around
  // NOTE: caller is responsible for memory management: free up the allDemandsVolsPtr
  void
  getDemandVols ( WitRun * theWitRun_,
                   int nPeriods,
                   int nParts,
                   char ** partList,
                   int *  nDemandsList,
                   char *** allDemandsList,
                   float **** allDemandsVolsPtr );

  //copy ship late limits
  // NOTE: calling object is responsible for memory clean-up
  void
  getShipLateLimits ( WitRun * theWitRun,
                       int nPeriods,
                       int nParts,
                       char ** partList,
                       int *  nDemandsList,
                       char *** allDemandsList,
                       int *** allShipLateLimitsPtr );
  
  
  // Create a vector of minLotSize's, and a vector of booleans that describe
  // whether a part has a min lot size, and a global flag indicating whether
  // ANY part has a minLotSize.  These vectors must be freed by the client! 
  // 
  void
  getMinLotSizeFlagsAndVols ( WitRun       * theWitRun,
                              int            nOperations,
                              int            nPeriods,
                              char        ** operationList,
                              witBoolean   * isMinLotSizeUsedAnywherePtr,
                              witBoolean  ** isMinLotSizeUsedForOperationPtr ,
                              float ***  minLotSizesPtr);

  void
  getSupplyVols ( WitRun       * theWitRun,
                  int            nParts,
                  char        ** partList,
                  float ***  allSupplyVols);
  

  // initialize the structures that hold the different critical list information
  // that is from each time an implosion is done
  //
  // USE: the consolidateCriticalList member function will take these structures
  //      and combine them into one critical list, filtering out duplicates, and
  //      freeing the un-consolidated memory.
  //
  // NOTE: calling object is responsible for memory clean-up
  void
  initGlobalCriticalStructures ( int       nImplosions,
                                 int    ** tieCritLenPtr,
                                 int   *** tieCritPeriodPtr, 
                                 char **** tieCritListPtr );

  // getNBom - create a vector of length nOperations to hold the number of Bom entrees
  //           for each operation
  // NOTE: caller is responsible for memory clean-up
  void
  getNBom ( WitRun * theWitRun,
            int      nOperations,
            char **  operationList,
            int **   nBomPtr );

  // get number of substitute bom entrees - allocate memory and store in it the
  // number of substitute BOM entries for every BOM for every operation
  // NOTE: caller is responsible for memory clean-up
  void
  getNSubsBom ( WitRun * theWitRun,
                int nOperations,
                char ** operationList,
                int * nBom,
                int *** nSubsBomPtr );

  // Allocate and initialize a global shipment schedule.
  // The solution from each implosion iteration will be added to this schedule
  // to arrive at the "global" or final shipment schedule.
  // NOTE: caller is responsible for memory clean-up
  void
  initGlobalShipSchedule ( float **** globalShipSchedulePtr,
                           int nPeriods,
                           int nParts,
                           int * nDemandsList );

// Allocate and initialize a global execution vol schedule.
// The solution from each implosion iteration will be added to this schedule
// to arrive at the "global" or final execution vol schedule.
// NOTE: caller is responsible for memory clean-up
  void
  initGlobalExecSchedule ( float *** globalExecSchedulePtr,
                           int nPeriods,
                           int nOperations );

  
  
  // Allocate and initalize global substitute BOM entry production schedule
  // The solution from each implosion iteration will be added to this schedule
  // to arrive at the "global" or final subs BOM entry prod schedule
  // NOTE: caller is responsible for memory clean-up
  void
  initGlobalSubsBomProdSchedule ( float ***** globalSubsBomProdSchedulePtr,
                                  int nPeriods,
                                  int nOperations,
                                  int * nBom,
                                  int ** nSubsBom );

  // copy the production upper bounds so after we muck up theWitRun, we can 
  // set the production upper bounds back to their original values.
  // NOTE: caller is responsible for memory clean-up
  void
  getExecUpperBounds ( WitRun * theWitRun,
                       int nOperations,
                       char ** opList,
                       float *** globalExecUpperBoundsPtr );
  
  // Update global (ship, production, substitute BOM production) schedules
  // to be called AFTER an implosion has been preformed.
  void
  updateGlobalSchedules( WitRun * theWitRun,
                         int nPeriods,
                         int nParts,
                         int nOperations,
                         char ** partList,
                         char ** operationList,                         
                         int * nDemandsList,
                         char *** allDemandsList,
                         int * nBom,
                         int ** nSubsBom,
                         float *** globalShipSchedule,
                         float ** globalExecSchedule,                         
                         float **** globalSubsBomProdSchedule);

  void
  prepOperationConstraintsForNextImplosionIteration ( WitRun * theWitRun,
						      int nPeriods,
						      int nParts,
						      char ** partList,
						      int nOperations,
						      char ** operationList,
						      float ** globalExecSchedule,
						      float ** globalExecUpperBounds,
						      witBoolean isMinLotSizeUsedAnywhere,
						      witBoolean * isMinLotSizeUsedForOperation );



  // Update global critical list information
  // to be called AFTER an implosion
  void
  updateGlobalCriticalList ( WitRun * theWitRun,
                             witBoolean computeCriticalList,                             
                             int implosionCtr,
                             int ** globalCritLenPtr,
                             int *** globalCritPeriodsPtr,
                             char **** globalCritLisPtr );

  // Update supply vols for each part AFTER an implosion.
  // The supplyVols in theWitRun are changed, so if you
  // need 'em you better save them else where.
  void
  setSupplyVolToExcessVol ( WitRun * theWitRun,
                            int nParts,
                            char ** partList );

  // In going from one iteration of implosion to the next, the
  // production upper bounds in iteration t+1 need to be adjusted
  // to reflect the amount of production in iteration t. 
  // 
  // NOTE: production upper bounds in theWitRun are changed
  void
  updateExecUpperBounds ( WitRun * theWitRun,
                          int nPeriods,
                          int nOperations,
                          char ** operationList);

  // Update minimum lot size AFTER an implosion.
  // The minimum lot sizes in theWitRun are changed,
  // so if you need them you better save them before invoking this method.
  //
  // Need to carry over the production from the previous implosions
  // to count towards the vols needed for minimum lot sizing in the
  // next implosion
  //
  // NOTE: call after you've updated the global production schedule
  void
  updateMinLotSize ( WitRun * theWitRun,
                     int nPeriods,
                     int nOperations,
                     char ** operationList,
                     float ** globalExecSchedule,
                     witBoolean isMinLotSizeUsedAnywhere,
                     witBoolean * isMinLotSizeUsedForOperation );

  
  // stuff global shipment, production, and substitute BOM production schedules
  // into a theWitRun
  // NOTE: it load the solution, but it does NOT post-process.
  void
  stuffGlobalSolutionSchedules (  WitRun * theWitRun,
                                  int nPeriods,
                                  int nParts,
                                  int nOperations,                                  
                                  int * nDemandsList,
                                  char *** allDemandsList,
                                  char ** partList,
                                  char ** operationList,                                  
                                  int * nBom,
                                  int ** nSubsBom,
                                  float *** globalShipSchedule,
                                  float ** globalExecSchedule,
                                  float **** globalSubsBomProdSchedule );

  // Consolidates critical list information created by initGlobalCriticalStructures.
  // Takes the global critical list lengths, periods, and parts from each of the
  // implosions and puts it into one critical list, flitering out the duplicates
  //
  // The new critical list structure is created (caller is responsible for memory
  // clean-up) and the unconsolidated structures are free-ed
  void
  consolidateCriticalLists ( int nPeriods,
                             int nParts,
                             char ** partList,
                             witBoolean computeCriticalList,
                             int nImplosions,
                             int    ** tieCritLenPtr,
                             int   *** tieCritPeriodPtr, 
                             char **** tieCritListPtr,
                             int * critLengthPtr,
                             int ** critPeriodsPtr,
                             char *** critPartListPtr );                              

  // Zero out all the demands in theWitRun
  // This method mucks up the demands, so if you don't want to lose the demand information
  // make a copy somewhere safe.
  void
  zeroOutAllDemands( WitRun * theWitRun,
                     int nPeriods,
                     int nParts,
                     char ** partList,
                     int *  nDemandsList,
                     char *** allDemandsList);

  // set the minLotSizes
  void
  setMinLotSize( WitRun * theWitRun,
                 int nPeriods,
                 int nOperations,
                 char    **  operationList,
                 witBoolean * isMinLotSizeUsedForOperation,
                 float **  minLotSizes );

  // set the supplyVols 
  void
  setSupplyVols( WitRun * theWitRun,
                 int nParts,
                 char    **  partList,
                 float **  allSupplyVols );

  
  // Set demand vols
  // Use this member function to clean-up the mucked demand vols
  void
  setDemandVols ( WitRun * theWitRun,
                  int nParts,
                  char ** partList,
                  int *  nDemandsList,
                  char *** allDemandsList,
                  float *** allDemandsVols );

  // set ship late limits
  // Use this member function to clean-up the mucked ship late limits
  void
  setShipLateLimits ( WitRun * theWitRun,
                      int nParts,
                      char ** partList,
                      int *  nDemandsList,
                      char *** allDemandsList,
                      int ** allShipLateLimits );

  // Set the production hard upper bounds
  // Use this to restore the orginal upper bound values in theWitRun
  void
  setExecUpperBounds( WitRun * theWitRun,
                      int nOperations,
                      char    **  operationList,
                      float **  allExecUpperBounds );  
  
  // default constructor
  LgFrSceIterativeImpHelper();
  
  // copy constructor
  LgFrSceIterativeImpHelper( const LgFrSceIterativeImpHelper& source );
  
  // assignment operator
  LgFrSceIterativeImpHelper&
  operator=(const LgFrSceIterativeImpHelper& rhs);
  
  // destructor
  virtual
  ~LgFrSceIterativeImpHelper();
  
  
  // self-test
  static void test();
  
private:
  // no private data. This class just a helper with out any private data to call its own.
  
};
#endif
