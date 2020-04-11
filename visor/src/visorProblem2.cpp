#include <set>
#include <cstring>
#include <iostream>

#include "VisorFloatEqual.h"
#include "visorProblem2.h"
#include "visorUtil.h"


//----------------------------------
// Methods for global attributes.
//----------------------------------

// setEquAllocPerBnd: equitable allocation period bound
void VISORproblem2::setEqAlloPerBnd(int bnd)
{
  eqAllocPerBnd_ = bnd;
}

int VISORproblem2::getEqAllocPerBnd()
{
  return eqAllocPerBnd_;
}
//
//----------------------
// Visor Methods
//----------------------
void VISORproblem2::addVisor(const std::string &name, const std::string &location)
{
  std::string aggVisNm = aggregateVisorName(location);
  std::string aggOperNm = aggregateOperName(location);
  std::string noSupplyPartNm = noSupplyVisorName(location);
  std::string visorPartNm = visorPartName(name, location);

  // If this is the first visor at this location, then add location constructs
  if (!locationExists(location))
  {
    // Location does not exist so add it
    // Add parts: Printer, NoSupply, and produced visor
    witAddPart(witRun(), aggVisNm.c_str(), WitMATERIAL);
    witAddPart(witRun(), noSupplyPartNm.c_str(), WitMATERIAL);

    // Add opertaion
    witAddOperation(witRun(), aggOperNm.c_str());

    // Add bom connecting operation to printer and noSupplyPart
    witAddBomEntry(witRun(), aggOperNm.c_str(), noSupplyPartNm.c_str());

    // Connect operation to produced visor
    witAddBopEntry(witRun(), aggOperNm.c_str(), aggVisNm.c_str());

    std::string baseName = baseLocationName(location);
    locationBaseNames_.insert(location);
    //std::cout <<location+"\n";
  }

  // Add part and SubBom entry to part
  witAddPart(witRun(), visorPartNm.c_str(), WitMATERIAL);
  witAddSubsBomEntry(witRun(), aggOperNm.c_str(), 0, visorPartNm.c_str());

}

void VISORproblem2::setVisorSupplyVol(const std::string &name, const std::string &location,
                                      const std::vector<float> &supplyVol)
{
  std::string visorPartNm = visorPartName(name, location);
  witSetNameAttribute(witSetPartSupplyVol, visorPartNm, supplyVol);
}
void VISORproblem2::setVisorSupplyVol( const std::string &location,
                                      int period, float supplyVol)
{
  std::string visorPartNm = aggregateVisorName(location);
  witSetNameAttribute(witGetPartSupplyVol,witSetPartSupplyVol, visorPartNm, period, supplyVol);
}
bool VISORproblem2::visorExists(const std::string &name,const std::string &loc)
{
  std::string visorPartNm = visorPartName(name, loc);
  return witGetNameAttributeBool(witGetPartExists,visorPartNm);
}
bool VISORproblem2::locationExists(const std::string &loc)
{
  std::string aggOperNm = aggregateOperName(loc);
  return witGetNameAttributeBool(witGetOperationExists,aggOperNm);
}

//-------------------------------------------------------------------------
// Visor and location Name Methods
//-------------------------------------------------------------------------
std::string VISORproblem2::aggregateVisorName(const std::string &location)
{
  return "Visor aggregation " + baseLocationName(location);
}

std::string VISORproblem2::aggregateOperName(const std::string &location)
{
  return "Aggregate  " + baseLocationName(location);
}

std::string VISORproblem2::noSupplyVisorName(const std::string &location)
{
  return "No supply part " + baseLocationName(location);
}

std::string VISORproblem2::visorPartName(const std::string &name, const std::string &location)
{
  return "Visor " + name + " made " + baseLocationName(location);
}

std::string VISORproblem2::baseLocationName(const std::string &location)
{
  return " at-> " + location;
}

std::string VISORproblem2::locationFormAggregateVisorName(const std::string &location)
{
  return textAfter(location, std::string("Visor aggregation ") + std::string(" at-> "));
}

std::set<std::string> VISORproblem2::getLocation()
{ return locationBaseNames_; }


//
//----------------------
// Demands: Hospital Request for Visor
//----------------------
void VISORproblem2::addVisorRequest(
    const std::string &demName,
    int period,
    int requestedQuantity)
{
  std::string visForHospNm = visorForHospitalName(demName);
  std::string aggVisForHospNm = aggregateVisorForHospitalName(demName);
  std::string noSupForHospNm = noSupplyForHospital(demName);

  // Only add demand if it doesn't already exist
  if (!witGetDemandExists(visForHospNm, demName))
  {

    witAddPart(witRun(), visForHospNm.c_str(), WitMATERIAL);
    witAddPart(witRun(), noSupForHospNm.c_str(), WitMATERIAL);

    witAddOperation(witRun(), aggVisForHospNm.c_str());

    witAddBomEntry(witRun(), aggVisForHospNm.c_str(), noSupForHospNm.c_str());
    witAddBopEntry(witRun(), aggVisForHospNm.c_str(), visForHospNm.c_str());
    witAddDemand(witRun(), visForHospNm.c_str(), demName.c_str());
    demandList_.push_back(demName);

    // Loop once for each printer location. Add subsBomEntry from that location
    std::set<std::string> locs = getLocation();
    std::set<std::string>::const_iterator it;
    for (it = locs.begin(); it != locs.end(); ++it)
    {
      std::string aggVisNm = aggregateVisorName(*it);

      witAddSubsBomEntry(witRun(), aggVisForHospNm.c_str(), 0, aggVisNm.c_str());
    }

  }

  // Set demand to the requested number of visors
  witSetDemandAttribute(witGetDemandDemandVol, witSetDemandDemandVol,
                        visForHospNm, demName, period, (float) requestedQuantity);
}

// Return true if demand exist
bool VISORproblem2::witGetDemandExists(const std::string &visorName, const std::string &demandName)
{
  bool retVal = false;

  witBoolean partExists;
  witGetPartExists(mutableWitRun(), visorName.c_str(), &partExists);
  if (!partExists) return retVal;

  int nPartDemands;
  char **demandList;
  witGetPartDemands(mutableWitRun(), visorName.c_str(), &nPartDemands, &demandList);
  for (int d = 0; d < nPartDemands; ++d)
  {
    if (demandList[d] == demandName)
    {
      retVal = true;
    }
    witFree(demandList[d]);
  }
  witFree(demandList);
  return retVal;
}


std::vector<float> VISORproblem2::getVisorShipVol(std::string &demandName)
{
//**  need to change demanded part name 
  //std::cout <<demandName+ "  |  "+producingLocation+"\n";
  std::string visorForHospitalNm = visorForHospitalName(demandName);
  return witGetDemandAttribute(witGetDemandShipVol, visorForHospitalNm, demandName);
}

std::vector<std::string> VISORproblem2::getDemands()
{ return demandList_; }


// For a specific hospital visor request get the printer locations, period and quantity that
// is satisfying the request.
// for a specified demandName (hosptial), return all the visor printer location supplying the hospital,
// and the amount supplied each period
void VISORproblem2::getSubVols(const std::string &demandName, std::vector<std::string> &consPartLocation,
                               std::vector<std::vector<float>> &subsVol)
{
  consPartLocation.clear();
  subsVol.clear();
  std::string operName = aggregateVisorForHospitalName(demandName);

  // Get list of all locations.  Every location making visors can supply the demand
  std::set<std::string> locs = getLocation();
  std::set<std::string>::const_iterator it;
  int i = 0;
  for (it = locs.begin(); it != locs.end(); ++it)
  {
    char *consPart;
    witGetSubsBomEntryConsumedPart(witRun(), operName.c_str(), 0, i, &consPart);
    consPartLocation.push_back(locationFormAggregateVisorName(std::string(consPart)));
    witFree(consPart);

    std::vector<float> sv = witGetSubArcAttribute(witGetSubsBomEntrySubVol, operName, 0, i);
    subsVol.push_back(sv);

    i++;

  }

}

//-------------------------------------------------------------------------
// demand (hospital) Name Methods
//-------------------------------------------------------------------------
std::string VISORproblem2::visorForHospitalName(const std::string &demName)
{ return "Visors for Hospital " + demName; }

std::string VISORproblem2::aggregateVisorForHospitalName(const std::string &demName)
{
  return "aggregateVisors for " + demName;
}

std::string VISORproblem2::noSupplyForHospital(const std::string &demName)
{ return "noSupplyPart for " + demName; }



// -----------------------------
// solver methods
// ----------------------------
void VISORproblem2::solve()
{

  witSetEquitability(witRun(), 100);
  witStartHeurAlloc(witRun());


  int firstEqAllocPeriodBnd = getEqAllocPerBnd();
  //-------------------------------------------------
  // Set up data structures for first witEqHeurAlloc
  //-------------------------------------------------
  {
    std::vector<std::string> allDemands = getDemands();
    int nDems = allDemands.size();
    int listLen = nDems * firstEqAllocPeriodBnd;
    char *demandedPartNameList[listLen];
    char *demandNameList[listLen];
    int shipPeriodList[listLen];
    float desIncVolList[listLen];
    for (int d = 0; d < nDems; d++)
    {
      std::string partIName = visorForHospitalName(allDemands[d]);
      std::string demIName = allDemands[d];
      std::vector<float> demandVol = witGetDemandAttribute(witGetDemandDemandVol, partIName, demIName);
      for (int p = 0; p < firstEqAllocPeriodBnd; p++)
      {

        demandedPartNameList[d * firstEqAllocPeriodBnd + p] = new char[partIName.size() + 1];
        strcpy(demandedPartNameList[d * firstEqAllocPeriodBnd + p], partIName.c_str());

        demandNameList[d * firstEqAllocPeriodBnd + p] = new char[demIName.size() + 1];
        strcpy(demandNameList[d * firstEqAllocPeriodBnd + p], demIName.c_str());

        shipPeriodList[d * firstEqAllocPeriodBnd + p] = p;
        desIncVolList[d * firstEqAllocPeriodBnd + p] = demandVol[p];


        //std::cout <<d*firstEqAllocPeriodBnd+p <<"   " <<demandedPartNameList[d*firstEqAllocPeriodBnd+p]
        //<<"  |  " <<demandNameList[d*firstEqAllocPeriodBnd+p] <<"  |  " <<shipPeriodList[d*firstEqAllocPeriodBnd+p]
        //<<"  |  " <<desIncVolList[d*firstEqAllocPeriodBnd+p] <<"\n";
      }
    }

#if 0
    for ( int i=0; i<listLen; i++)
    {
        std::cout <<i <<"   " <<demandedPartNameList[i]
          <<"  |  " <<demandNameList[i] <<"  |  " <<shipPeriodList[i]
          <<"  |  " <<desIncVolList[i] <<"\n";
    }
#endif
    float *incVolList;
    witEqHeurAlloc(witRun(), listLen, demandedPartNameList, demandNameList, shipPeriodList, desIncVolList, &incVolList);

    for (int i = 0; i < listLen; i++)
    {
      delete[] demandedPartNameList[i];
      delete[] demandNameList[i];
    }

    witFree(incVolList);
  }

  //-------------------------------------------------
  // Set up data structures for second witEqHeurAlloc
  //-------------------------------------------------
  {
    std::vector<std::string> allDemands = getDemands();
    int nDems = allDemands.size();
    int perBlkSz = getNPeriods() - firstEqAllocPeriodBnd;
    int listLen = nDems * perBlkSz;
    char *demandedPartNameList[listLen];
    char *demandNameList[listLen];
    int shipPeriodList[listLen];
    float desIncVolList[listLen];
    for (int d = 0; d < nDems; d++)
    {
      std::string partIName = visorForHospitalName(allDemands[d]);
      std::string demIName = allDemands[d];
      std::vector<float> demandVol = witGetDemandAttribute(witGetDemandDemandVol, partIName, demIName);
      for (int p = 0; p < perBlkSz; p++)
      {

        demandedPartNameList[d * perBlkSz + p] = new char[partIName.size() + 1];
        strcpy(demandedPartNameList[d * perBlkSz + p], partIName.c_str());

        demandNameList[d * perBlkSz + p] = new char[demIName.size() + 1];
        strcpy(demandNameList[d * perBlkSz + p], demIName.c_str());

        shipPeriodList[d * perBlkSz + p] = firstEqAllocPeriodBnd + p;
        desIncVolList[d * perBlkSz + p] = demandVol[firstEqAllocPeriodBnd + p];


        //std::cout <<d*nDems+p <<"   " <<demandedPartNameList[d*nDems+p]
        //<<"  |  " <<demandNameList[d*nDems+p] <<"  |  " <<shipPeriodList[d*nDems+p]
        //<<"  |  " <<desIncVolList[d*nDems+p] <<"\n";
      }
    }

#if 0
    for ( int i=0; i<listLen; i++)
    {
        std::cout <<i <<"   " <<demandedPartNameList[i] 
           <<"  |  " <<demandNameList[i] <<"  |  " <<shipPeriodList[i]
           <<"  |  " <<desIncVolList[i] <<"\n";
    }
#endif

    float *incVolList;
    witEqHeurAlloc(witRun(), listLen, demandedPartNameList, demandNameList, shipPeriodList, desIncVolList, &incVolList);

    for (int i = 0; i < listLen; i++)
    {
      delete[] demandedPartNameList[i];
      delete[] demandNameList[i];
    }

    witFree(incVolList);
  }


  witFinishHeurAlloc(witRun());

}




//std::string VISORproblem2::locationFromFromAggregateVisorName(const std::string & avn)
//{
//  //return textBetween(baseName,"Printer: "," at-> ");
//  return textAfter(avn," at-> ");
//}
//std::string VISORproblem2::locationFromPrinterName(const std::string & baseName)
//{
//  return textAfter(baseName," at-> ");












// default constructor
VISORproblem2::VISORproblem2()
    :
    VISORproblem(),
    eqAllocPerBnd_(5),
    locationBaseNames_(),
    demandList_()
{


  witSetIndependentOffsets(witRun(), WitTRUE);
  setNPeriods(30);
  witSetMultiRoute(witRun(), WitTRUE);

}

// destructor
VISORproblem2::~VISORproblem2()
{
  VISORproblem2::gutsOfDestructor();
}

// copy constructor. Not yet suported
VISORproblem2::VISORproblem2(const VISORproblem2 &source) :
    VISORproblem(source),
    eqAllocPerBnd_(source.eqAllocPerBnd_)
{
  gutsOfCopy(source);
}

// assignment operator. Not yet supported
VISORproblem2 &
VISORproblem2::operator=(const VISORproblem2 &rhs)
{
  if (this != &rhs)
  {
    if (wr_ != nullptr) gutsOfDestructor();
    wr_ = nullptr;
    gutsOfCopy(rhs);
  }
  return *this;
}

// copy object
void VISORproblem2::gutsOfCopy(const VISORproblem2 &source)
{
  // Copy Wit Run
  assert(wr_ == nullptr);
  witNewRun(&wr_);

  // Turn off WIT informational messages
  witSetMesgTimesPrint(wr_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  // Turn off warning msg about long names.
  witSetMesgTimesPrint(wr_, WitTRUE, 338, 0);

  witCopyData(wr_, source.wr_);
  nPeriods_ = source.nPeriods_;
  eqAllocPerBnd_ = source.eqAllocPerBnd_;


}

// destructor
void VISORproblem2::gutsOfDestructor()
{
}


#ifdef NDEBUG
#undef NDEBUG
#endif

//------------------------------------------------------------------------------
// Test function
//------------------------------------------------------------------------------
void
VISORproblem2::test()
{
  //OsiRelFltEq eq(1e-05);
  VisorRelFltEq eq(1.e-05);

  {
    VISORproblem2 prob;

    prob.setNPeriods(12);
    assert(prob.getNPeriods() == 12);

    prob.setTitle("wittitle");
    assert(prob.getTitle() == "wittitle");

  }


  {
    // Test materials
    VISORproblem2 prob;
    assert(prob.getNPeriods() == 30);
    prob.setNPeriods(25);
    assert(prob.getNPeriods() == 25);
  }
  {
    /* Do first and second implosion with BrendaData */
    VISORproblem1 *prob1Ptr = new VISORproblem1();
    VISORproblem2 *prob2Ptr = new VISORproblem2();
    std::string inputDirectory = "../data/BrendaData";
    std::string onHandMaterialFileName = inputDirectory + "/onHandMaterial.csv";
    readOnHandMaterial(onHandMaterialFileName, *prob1Ptr);
    std::string printerFileName = inputDirectory + "/printer.csv";
    readPrinter(printerFileName, prob1Ptr, prob2Ptr);
    std::string requestQuantityFileName = inputDirectory + "/requestQuantity.csv";
    readRequestQuantity(requestQuantityFileName, *prob2Ptr);
    prob1Ptr->solve();
    copyImplosion1ShipVolToImplosion2SupplyVol(*prob1Ptr, *prob2Ptr);
    prob2Ptr->solve();

    std::vector<float> pv = prob1Ptr->getPrinterProdVol("P1", "115 Cherry");
    assert(eq(pv[0], 0.0));
    assert(eq(pv[1], 10.0));

    std::vector<std::string> partLoc;
    std::vector<std::vector<float>> sv;
    prob2Ptr->getSubVols("Urgent Care", partLoc, sv);
    bool found206=false,found210=false,found217=false;
    for (int i = 0; i < partLoc.size(); i++)
    {
      //std::cout <<i <<"  "+partLoc[i] <<std::endl;
      if (partLoc[i]=="206 Carpenter")
      {
        assert( eq(sv[i][0],0.0));
        assert( eq(sv[i][1],5.0));
        found206=true;
      }
      if (partLoc[i]=="210 Gates")
      {
        assert( eq(sv[i][0],0.0));
        assert( eq(sv[i][1],5.0));
        found210=true;
      }
      if (partLoc[i]=="217 Rhodes")
      {
        assert( eq(sv[i][0],0.0));
        assert( eq(sv[i][1],13.0));
        found217=true;
      }
    }
    assert(found206 && found210 && found217);
  }

  {
    /* Do only second implosion with BrendaData */
    VISORproblem1 *prob1Ptr = nullptr;
    VISORproblem2 *prob2Ptr = new VISORproblem2();
    std::string inputDirectory = "../data/BrendaData";

    std::string printerFileName = inputDirectory + "/printer.csv";
    readPrinter(printerFileName, prob1Ptr, prob2Ptr);

    std::string requestQuantityFileName = inputDirectory + "/requestQuantity.csv";
    readRequestQuantity(requestQuantityFileName, *prob2Ptr);

    std::string visorProdPlanFileName = inputDirectory + "/visorProdPlan.csv";
    readVisorProdPlan(visorProdPlanFileName,*prob2Ptr);

    prob2Ptr->solve();

    std::vector<std::string> partLoc;
    std::vector<std::vector<float>> sv;
    prob2Ptr->getSubVols("Urgent Care", partLoc, sv);
    bool found206=false,found210=false,found217=false;
    for (int i = 0; i < partLoc.size(); i++)
    {
      if (partLoc[i]=="206 Carpenter")
      {
        assert( eq(sv[i][0],0.0));
        assert( eq(sv[i][1],5.0));
        found206=true;
      }
      if (partLoc[i]=="210 Gates")
      {
        assert( eq(sv[i][0],0.0));
        assert( eq(sv[i][1],5.0));
        found210=true;
      }
      if (partLoc[i]=="217 Rhodes")
      {
        assert( eq(sv[i][0],0.0));
        assert( eq(sv[i][1],13.0));
        found217=true;
      }
    }
    assert(found206 && found210 && found217);
  }



}