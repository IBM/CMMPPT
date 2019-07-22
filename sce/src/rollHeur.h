/******************************************************************************
 * rollHeur.h
 *
 ******************************************************************************/

/* API code to preform the rollover heuristic using nPeriod implosions */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wit/src/wit.h>
#include <scenario/src/timeVecs.h>
#include <sce/src/sceCrtLs.h>

extern int  rolloverHeurImplode ( WitRun * const theWitRun,
                                  LgFrSceCritList & theCritList,
                                  int printInformationalWitMessages,
                                  const char * const witMessageFilename);



/* For efficiency, the demands, min lot size information, and product information
 * are gotten once and stored */  
extern void getDemandLists( WitRun    * theWitRun,
                            int         nParts,
                            char    **  partList,
                            int     **  nDemandsPtr,
                            char  ****  demandListPtr);

extern void getMinLotSizeUsed(
                              WitRun       * theWitRun,
                              int            nParts,
                              int            nPeriods,
                              char        ** partList,
                              witBoolean   * partIsProduct,
                              witBoolean   * minLotSizeUsedPtr,
                              witBoolean  ** minLotSizeUsedForPartPtr);

extern void getProductParts(
                            WitRun    * theWitRun,
                            int         nParts,
                            char    **  partList,
                            int     **  productPartsPtr);


/* Calculates the minimum demand vols for the 1st pass implosion of theWorkingWitRun
 * from the cum Ship soft lower bounds of theWitRun.
 * If the minimum demand vols are all zero, no 1st pass implosion is needed
 * and needFirstPass is false.
 * Second pass demand vols are calculated and stored as appData in theWorkingWitRun.
 * The calling program must clean up this memory.
 */
extern void setupPassOne(
                         WitRun *     theWitRun,
                         WitRun *     theWorkingWitRun,
                         int          nPeriods,
                         int          nParts,
                         char **      partList,
                         int *        nDemandsList,
                         char ***     allDemandsList,
                         witBoolean * needFirstPassPtr );
//**********************************************************************************
class MyAppData
{
public:
  MyAppData () {};
  const LgFrTimeVecFloat&  aboveMinDemandVol () const {return aboveMinDemandVol_ ;};
  void                     aboveMinDemandVol (LgFrTimeVecFloat amDemand)
                                                  {aboveMinDemandVol_ = amDemand ;};

private:
  LgFrTimeVecFloat aboveMinDemandVol_ ;
};
//**********************************************************************************
// getNBom - mallocs and sets a vector of length nParts, which stores the
//           number of BOM entries for each Part.             
//         - calling routine is responsible for freeing the memory
 
void  getNBom ( WitRun * theWitRun,
                int      nParts,
                char **  partList,
                int **   nBomPtr);
//**********************************************************************************
// getNSubsBom - mallocs and sets storage to hold the number of substitute BOM entries
//               for every Bom for every Part
//             - calling routines is responsible for freeing the memory

void getNSubsBom (WitRun * theWitRun,
                  int nParts,
                  char ** partList,
                  int * nBom,
                  int *** nSubsBomPtr);
