//  ----------------------------------------------------------------
//  Usage: whereUsed  <wit_data_file_name> <partName>               
//  ----------------------------------------------------------------
 
 
 
#include <stdio.h>
#include <stdlib.h>




#include <witutil/src/machdep.h>

#include <iostream>
#include <fstream>

#include <assert.h>

#include <wit.h>
// #include <witutil/src/witU5.h>
#include <witutil/src/fullExpl.h>
#include <rw/cstring.h>



// ================================================================ 

int whereUsed(WitRun * theWitRun,
              RWCString & child,
              int level,
              int doExplodeType,
              WUtlFullMrpExploder * fullExploderPtr,
              std::ofstream & outFile);



// ================================================================ 



int main (int argc, char * argv[]) 

{
    WitRun * theWitRun;
    char  ** partList;
    int      nParts;
    int i;
    
   // Mandatory Argument: wit_data_file_name    
   if ( argc < 2 ) {
      cerr << "Error.  Usage is " <<  argv[0] <<
        " <witdata_file>  [full | wit]\n";
      exit(1);
   }

   witNewRun(&theWitRun);

   witSetMesgTimesPrint(theWitRun, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
   witSetMesgTimesPrint(theWitRun, WitTRUE, 82, 0);
   witSetMesgTimesPrint(theWitRun, WitTRUE, 538, 0);
   witSetMesgTimesPrint(theWitRun, WitTRUE, 338, 0);      

   witInitialize(theWitRun);

   witSetWit34Compatible(theWitRun, WitTRUE);

   cerr << "Reading wit data file ..." << endl;
   witReadData(theWitRun, argv[1]);
   

    int doExplodeType = 0;
    if (argc > 2) {
      RWCString explodeArg(argv[2]);
      if (explodeArg.compareTo("full", RWCString::ignoreCase) == 0) {
        doExplodeType = 1;
      }
      else {
        if (explodeArg.compareTo("wit", RWCString::ignoreCase) == 0) {
          doExplodeType = 2;
        }
      }
    }
    
    // get the list of  parts 
    witGetParts(theWitRun, &nParts, &partList);

    
    int nPeriods;
    witGetNPeriods(theWitRun, &nPeriods);    


    WUtlFullMrpExploder * fullExploderPtr;
    int fullExpParam = 1;

    // do explode (if desired)
    if (doExplodeType == 1) {
      if (argc > 3) {
        fullExpParam = atoi(argv[3]);
      }
      cerr << "Creating a fullExploder object ..." << endl;      
      fullExploderPtr = new WUtlFullMrpExploder(theWitRun);
      cerr << "Running full explode(" << fullExpParam << ") engine  ..." << endl;
      fullExploderPtr->explode(fullExpParam);
    }
    if (doExplodeType == 2) {
      cerr << "Running wit explode engine  ..." << endl;      
      witMrp(theWitRun);
    }

    // =======================

    RWCString theComponent;
    RWBoolean tryAnother = TRUE;
    while (tryAnother) {
      cout << "\nEnter Part (Q/q to quit) ===> " << endl;
      cin >> theComponent;

      // see if the user entered q or Q
      if (! theComponent.compareTo("q", RWCString::ignoreCase))
        break;

      // see if the part is legitimate
      witBoolean exists;    
      witGetPartExists(theWitRun, theComponent, &exists);
      if (! exists) {
        cout << "The part " << theComponent << " does not exist!\n";
        continue;
      }

      // see if we can open the output file
      // output file name is partname with .wu appended
      RWCString outFileName(theComponent);
      outFileName.append(".wu");
      std::ofstream outFile(outFileName, ios::out);
      if (! outFile) {
        cout << "Canot open output file: " << outFile << "\n";
        continue;
      }

   
      cout << "Beginning recursive trace: " << theComponent << endl;
      
      // Do a recursive whereused starting at the bottom
      int level = 0;
     
      // TOTAL DEMAND
      float pastDue;
      float * mrpConsVol;
    
      if (doExplodeType == 1) {
        fullExploderPtr->pastDueMrpConsVol(theComponent, &pastDue);
        fullExploderPtr->mrpConsVol(theComponent, &mrpConsVol);      
        outFile << "      - mrpConsVol:";
        outFile << pastDue << "(pd),";      
        for (int t=0; t<nPeriods; t++)
          outFile << mrpConsVol[t] << ",";
        outFile << "\n";
        portableFree(mrpConsVol);
      }
      if (doExplodeType == 2) {
        witGetPartMrpConsVol(theWitRun, theComponent, &mrpConsVol);      
        outFile << "      - mrpConsVol:";
        for (int t=0; t<nPeriods; t++)
          outFile << mrpConsVol[t] << ",";
        outFile << "\n";
        portableFree(mrpConsVol);      
      }    
    
      whereUsed(theWitRun, theComponent, level,
                doExplodeType, fullExploderPtr, outFile);
      outFile.close();
    }

    // user has finished  ... cleanup
    if (doExplodeType == 1) {
      delete fullExploderPtr;
    }

    
    // free the parts 
    for (i=0; i<nParts; i++)  
	portableFree(partList[i]);
    portableFree(partList);

    witDeleteRun(theWitRun);
}



// ===================================================================

int whereUsed(WitRun * theWitRun,
              RWCString & child,
              int level,
              int doExplodeType,
              WUtlFullMrpExploder * fullExploderPtr,
              std::ofstream & outFile)              
{


  int consIndex;
  char * consumingOperationName;
  int bomEntryIndex;
  int nConsumingBomEntries;
  int nPeriods, t;

  witGetNPeriods(theWitRun, &nPeriods);

  // for this part, get the number of parents its got
  witGetPartNConsumingBomEntries(theWitRun,
                                 child,
                                 &nConsumingBomEntries);

  level++;
  // now loop thru each of its parent bom arcs and get the parent name
  for (consIndex=0; consIndex<nConsumingBomEntries; consIndex++) {
    // now get the parent name and the bomEntryIndex of the bom arc
    // of the parent that has theComponent as its child
    witGetPartConsumingBomEntry(theWitRun,
                                child,
                                consIndex,
                                &consumingOperationName,
                                &bomEntryIndex);

    RWCString parent(consumingOperationName);
    
    int lev;
    for (lev=0; lev<level; lev++)
      outFile << " . ";
    outFile << parent << "\n";


    float pastDue;
    float * mrpConsVol;
    // TOTAL DEMAND    
    if (doExplodeType == 1) {
      fullExploderPtr->pastDueMrpConsVol(parent.data(), &pastDue);
      fullExploderPtr->mrpConsVol(parent.data(), &mrpConsVol);
      for (lev=0; lev<level; lev++)
        outFile << "   ";
      outFile << "      - mrpConsVol:";
      outFile << pastDue << "(pd),";      
      for (t=0; t<nPeriods; t++)
        outFile << mrpConsVol[t] << ",";
      outFile << "\n";
      portableFree(mrpConsVol);
    }
    if (doExplodeType == 2) {
      witGetPartMrpConsVol(theWitRun, parent, &mrpConsVol);
      for (lev=0; lev<level; lev++)
        outFile << "   ";
      outFile << "      - mrpConsVol:";
      for (t=0; t<nPeriods; t++)
        outFile << mrpConsVol[t] << ",";
      outFile << "\n";
      portableFree(mrpConsVol);      
    }


    float * mrpProdVol;
    // Prod Vol (amount exploded)
    if (doExplodeType == 1) {
      fullExploderPtr->mrpProdVol(parent.data(), &mrpProdVol);
      for (lev=0; lev<level; lev++)
        outFile << "   ";
      outFile << "      - mrpProdVol:";
      for (t=0; t<nPeriods; t++)
        outFile << mrpProdVol[t] << ",";
      outFile << "\n";
      portableFree(mrpProdVol);
    }
    if (doExplodeType == 2) {
      witGetPartMrpProdVol(theWitRun, parent, &mrpProdVol);
      for (lev=0; lev<level; lev++)
        outFile << "   ";
      outFile << "      - mrpProdVol:";
      for (t=0; t<nPeriods; t++)
        outFile << mrpProdVol[t] << ",";
      outFile << "\n";
      portableFree(mrpProdVol);      
    }
    
    // Supplyvol
    for (lev=0; lev<level; lev++)
      outFile << "   ";
    float * supplyVol;
    witGetPartSupplyVol(theWitRun, parent, &supplyVol);
    outFile << "      - supplyVol (part):";
    for (t=0; t<nPeriods; t++)
      outFile << supplyVol[t] << ",";
    outFile << "\n";
    portableFree(supplyVol);
    

    // USage Rate
    float usageRate;
    witGetBomEntryUsageRate(theWitRun, parent, bomEntryIndex, &usageRate);
    for (lev=0; lev<level; lev++)   // indent
      outFile << "   ";
    outFile << "      - usageRate (bom):" << usageRate << "\n";

    // USage Time
    float usageTime;
    witGetBomEntryUsageTime(theWitRun, parent, bomEntryIndex, &usageTime);
    for (lev=0; lev<level; lev++)   // indent
      outFile << "   ";
    outFile << "      - usageTime (bom):" << usageTime << "\n";
    

    // Fallout
    int fallout;
    witGetBomEntryFallout(theWitRun, parent, bomEntryIndex, &fallout);
    for (lev=0; lev<level; lev++)   // indent
      outFile << "   ";
    outFile << "      - fallout (bom):" << fallout << "\n";
    
    
    // CycleTime
    for (lev=0; lev<level; lev++)
      outFile << "   ";
    float * cycleTime;
    witGetPartCycleTime(theWitRun, parent, &cycleTime);
    outFile << "      - cycleTime (part):";
    for (t=0; t<nPeriods; t++)
      outFile << cycleTime[t] << ",";
    outFile << "\n";
    portableFree(cycleTime);

    // offset
    for (lev=0; lev<level; lev++)
      outFile << "   ";
    float * offset;
    witGetBomEntryOffset(theWitRun, parent, bomEntryIndex, &offset);
    outFile << "      - offset (bom):";
    for (t=0; t<nPeriods; t++)
      outFile << offset[t] << ",";
    outFile << "\n";
    portableFree(offset);
    

    // effectivity
    for (lev=0; lev<level; lev++)
      outFile << "   ";
    int earliest;
    witGetBomEntryEarliestPeriod(theWitRun, parent, bomEntryIndex, &earliest);
    int latest;
    witGetBomEntryLatestPeriod(theWitRun, parent, bomEntryIndex, &latest);
    outFile << "      - effectivity (bom):";
    outFile << " (" << earliest << "," << latest  << ")\n";

    // impactPeriod
    witPostprocess(theWitRun);
    for (lev=0; lev<level; lev++)
      outFile << "   ";
    int * impactPeriod;
    witGetBomEntryImpactPeriod(theWitRun, parent, bomEntryIndex, &impactPeriod);
    outFile << "      - impactPeriod (bom):";
    for (t=0; t<nPeriods; t++)
      outFile << impactPeriod[t] << ",";
    outFile << "\n";
    portableFree(impactPeriod);
    


    
    // Ok, do recursion ...
    whereUsed(theWitRun, parent, level, doExplodeType,
              fullExploderPtr, outFile);
    portableFree(consumingOperationName);
  }
  return nConsumingBomEntries;
  

}





