#include <set>
#include <cstdio>


#include "VisorFloatEqual.h"
#include "visorProblem1.h"


//
//----------------------
// Material Methods
//----------------------
void
VISORproblem1::addMaterial(const std::string &location, const std::string &filamentSize, const std::string &plasticType,
                           float quantity, int sharePercent)
{
  assert(filamentSize == "1.75mm" || filamentSize == "2.85mm");
  assert(plasticType == "PETG" || plasticType == "PLA" || plasticType == "ABS" || plasticType == "ONYX");

  std::string ownMatName = ownMaterialName(location, filamentSize, plasticType);
  std::string shrMatName = shrMaterialName(location, filamentSize, plasticType);

  witAddPart(witRun(), ownMatName.c_str(), WitMATERIAL);
  witAddPart(witRun(), shrMatName.c_str(), WitCAPACITY);

  float shrQty = quantity * (float) sharePercent / 100.0f;
  float ownQty = quantity - shrQty;
  witSetNameAttribute(&witGetPartSupplyVol, witSetPartSupplyVol, ownMatName, 0, ownQty);
  witSetNameAttribute(&witGetPartSupplyVol, witSetPartSupplyVol, shrMatName, 0, shrQty);

  std::string baseName = baseMaterialName(location, filamentSize, plasticType);
  materialBaseNames_.insert(baseName);
}

bool VISORproblem1::materialExists(const std::string &location, const std::string &filamentSize,
                                   const std::string &plasticType)
{
  std::string partName = ownMaterialName(location, filamentSize, plasticType);
  return witGetNameAttributeBool(witGetPartExists,partName);
}

void VISORproblem1::getMaterials(
    std::vector<std::string> &locs,
    std::vector<std::string> &filamentSizes,
    std::vector<std::string> &pTypes)
{
  locs.clear();
  filamentSizes.clear();
  pTypes.clear();
#if 0
  int nMtms = materialBaseNames_.size();
  int m;
  for ( m=0; m<nMtms; m++ ) {
    locs.push_back( locationFromMaterialName(materialBaseNames_[m]) );
    filamentSizes.push_back( filamentSizeFromMaterialName(materialBaseNames_[m]) );
    pTypes.push_back( plasticTypeFromMaterialName(materialBaseNames_[m]) );
  }
#else
  std::set<std::string>::const_iterator it;
  for (it = materialBaseNames_.begin(); it != materialBaseNames_.end(); ++it)
  {
    locs.push_back(locationFromMaterialName(*it));
    filamentSizes.push_back(filamentSizeFromMaterialName(*it));
    pTypes.push_back(plasticTypeFromMaterailName(*it));
  }
#endif
}

std::vector<float> VISORproblem1::getOwnSupply(
    const std::string &loc,
    const std::string &nSize,
    const std::string &pType)
{
  std::vector<float> retVal;
  std::string name = ownMaterialName(loc, nSize, pType);
  retVal = witGetNameAttribute(witGetPartSupplyVol, name);
  return retVal;
}

std::vector<float> VISORproblem1::getSharedSupply(
    const std::string &loc,
    const std::string &nSize,
    const std::string &pType)
{
  std::vector<float> retVal;
  std::string name = shrMaterialName(loc, nSize, pType);
  retVal = witGetNameAttribute(witGetPartSupplyVol, name);
  return retVal;
}

//-------------------------------------------------------------------------
// material Name Methods
//-------------------------------------------------------------------------
std::string VISORproblem1::ownMaterialName(const std::string &location, const std::string &filamentSize,
                                           const std::string &plasticType)
{
  return "ownSupply: " + baseMaterialName(location, filamentSize, plasticType);
}

std::string VISORproblem1::shrMaterialName(const std::string &location, const std::string &filamentSize,
                                           const std::string &plasticType)
{
  return "shareableSupply: " + baseMaterialName(location, filamentSize, plasticType);
}

std::string VISORproblem1::baseMaterialName(const std::string &location, const std::string &filamentSize,
                                            const std::string &plasticType)
{
  return " Size " + filamentSize + " Type " + plasticType + " at " + location;
}

std::string VISORproblem1::locationFromMaterialName(const std::string &matName)
{
  return textAfter(matName, " at ");
}

std::string VISORproblem1::filamentSizeFromMaterialName(const std::string &matName)
{
  return textBetween(matName, " Size ", " Type ");
}

std::string VISORproblem1::plasticTypeFromMaterailName(const std::string &matName)
{
  return textBetween(matName, " Type ", " at ");
}

bool VISORproblem1::ownSupply(const std::string &matName)
{
  bool retVal;
  retVal = !beginsWith(matName, "shareableSupply: ");
  return retVal;
}

//
//----------------------
// Printer Methods
//----------------------
void VISORproblem1::addPrinter(
    const std::string &name,
    const std::string &location,
    float prodRate,
    bool F175, bool F285, bool petg, bool pla, bool abs, bool onyx)
{
  std::string printerNm = printerName(name, location);
  std::string printerOperNm = printerOperName(name, location);
  std::string noSupplyPartNm = noSupplyPartName(name, location);
  std::string visorPartNm = visorPartName(name, location);

  // Add parts: Printer, NoSupply, and produced visor
  witAddPart(witRun(), printerNm.c_str(), WitCAPACITY);
  witAddPart(witRun(), noSupplyPartNm.c_str(), WitMATERIAL);
  witAddPart(witRun(), visorPartNm.c_str(), WitCAPACITY);

  // Add opertaion
  witAddOperation(witRun(), printerOperNm.c_str());

  // Add bom connecting operation to printer and noSupplyPart
  witAddBomEntry(witRun(), printerOperNm.c_str(), printerNm.c_str());
  witAddBomEntry(witRun(), printerOperNm.c_str(), noSupplyPartNm.c_str());

  // Connect operation to produced visor
  witAddBopEntry(witRun(), printerOperNm.c_str(), visorPartNm.c_str());

  // Set printer supply volume to be the number that can be produced in a day
  std::vector<float> sv = floatToStlVec(prodRate);
  witSetNameAttribute(witSetPartSupplyVol, printerNm, sv);

  float sumSupplyVol = 1;
  for (int i = 0; i < sv.size(); i++) sumSupplyVol += sv[i];

  // Add Demand for visor and set demandVol to big M
  witAddDemand(witRun(), visorPartNm.c_str(), "demand");
  std::vector<float> dv = floatToStlVec(sumSupplyVol);
  witSetDemandAttribute(witSetDemandDemandVol, visorPartNm, "demand", dv);

  std::vector<float> cumShipRew = floatToStlVec(10.);
  witSetDemandAttribute(witSetDemandCumShipReward, visorPartNm, "demand", cumShipRew);
  std::vector<float> shipRew = floatToStlVec(100.);
  witSetDemandAttribute(witSetDemandShipReward, visorPartNm, "demand", shipRew);

  //Subs Boms for all materaials printer can use
  {
    std::vector<std::string> matLoc, filamentSize, plasticType;
    getMaterials(matLoc, filamentSize, plasticType);

    float *ownSubCost = floatToConstFloatStar(0.0f);
    float *ownShrSubCost = floatToConstFloatStar(1.0f);
    float *shrSubCost = floatToConstFloatStar(2.0f);
    float *shipOffset = floatToConstFloatStar(1.0f);

    //Loop once for each material
    int nSubBomEntries = 0;
    for (int i = 0; i < matLoc.size(); i++)
    {
      // Determine if i'th material can be used with this printer
      //std::cout <<F175 <<" " <<F285 <<" " <<filamentSize[i]+"\n";
      //std::cout <<petg <<" " <<pla <<" " <<abs <<" " <<onyx <<" " <<plasticType[i]+"\n";
      //std::cout <<location <<" " <<matLoc[i] <<"\n";
      bool nozOk = (F175 && filamentSize[i] == "1.75mm") || (F285 && filamentSize[i] == "2.85mm");
      bool typeOk = (petg && plasticType[i] == "PETG")
                    || (pla && plasticType[i] == "PLA")
                    || (abs && plasticType[i] == "ABS")
                    || (onyx && plasticType[i] == "ONYX");
      //std::cout <<"nozOk " <<nozOk <<"typeOk " <<typeOk <<std::endl;
      if (nozOk && typeOk)
      {
        // Material is good to use, so add SubBom Entry
        std::string shrMatName = shrMaterialName(matLoc[i], filamentSize[i], plasticType[i]);
        witAddSubsBomEntry(witRun(), printerOperNm.c_str(), 1, shrMatName.c_str());

        // Set the consRate to be 1/60  (60 per kg)
        float *consRate = floatToConstFloatStar(1. / 60.);
        witSetSubsBomEntryConsRate(witRun(), printerOperNm.c_str(), 1, nSubBomEntries, consRate);
        delete[] consRate;

        if (location == matLoc[i])
        {
          // ownd Share sub cost
          witSetSubsBomEntrySubCost(witRun(), printerOperNm.c_str(), 1, nSubBomEntries, ownShrSubCost);
        } else
        {
          // Share sub cost
          witSetSubsBomEntrySubCost(witRun(), printerOperNm.c_str(), 1, nSubBomEntries, shrSubCost);
          // set shipping offset 1
          witSetSubsBomEntryOffset(witRun(), printerOperNm.c_str(), 1, nSubBomEntries, shipOffset);

        }
        nSubBomEntries++;

        // if material is local then add own supply
        if (location == matLoc[i])
        {
          std::string ownMatName = ownMaterialName(matLoc[i], filamentSize[i], plasticType[i]);
          witAddSubsBomEntry(witRun(), printerOperNm.c_str(), 1, ownMatName.c_str());
          witSetSubsBomEntrySubCost(witRun(), printerOperNm.c_str(), 1, nSubBomEntries, ownSubCost);
          nSubBomEntries++;
        }
      }
    }
    delete[] ownSubCost;
    delete[] ownShrSubCost;
    delete[] shrSubCost;
    delete[] shipOffset;
  }

  std::string baseName = basePrinterName(name, location);
  printerBaseNames_.insert(baseName);
}

void VISORproblem1::getPrinters(
    std::vector<std::string> &names,
    std::vector<std::string> &locs)
{
  names.clear();
  locs.clear();
  std::set<std::string>::const_iterator it;
  for (it = printerBaseNames_.begin(); it != printerBaseNames_.end(); ++it)
  {
    names.push_back(printerFromPrinterName(*it));
    locs.push_back(locationFromPrinterName(*it));
  }
}

bool VISORproblem1::printerExists(const std::string &pNam, std::string &pLoc)
{
  std::string key = basePrinterName(pNam, pLoc);
  return printerBaseNames_.find(key) != printerBaseNames_.end();
}

std::vector<float> VISORproblem1::getPrinterProdRate(
    const std::string &name,
    const std::string &loc)
{
  std::vector<float> retVal;
  std::string nm = printerName(name, loc);
  retVal = witGetNameAttribute(witGetPartSupplyVol, nm);
  return retVal;
}

std::vector<float> VISORproblem1::getPrinterShipVol(
    const std::string &name,
    const std::string &loc)
{
  std::vector<float> retVal;
  std::string visorPartNm = visorPartName(name, loc);
  retVal = witGetDemandAttribute(witGetDemandShipVol, visorPartNm, "demand");
  return retVal;
}

std::vector<float> VISORproblem1::getPrinterProdVol(
    const std::string &name,
    const std::string &loc)
{
  std::vector<float> retVal;
  std::string operNm = printerOperName(name, loc);
  retVal = witGetNameAttribute(witGetOperationExecVol, operNm);
  return retVal;
}


//-------------------------------------------------------------------------
// printer Name Methods
//-------------------------------------------------------------------------
std::string VISORproblem1::printerName(const std::string &name, const std::string &location)
{
  return basePrinterName(name, location);
}

std::string VISORproblem1::printerOperName(const std::string &name, const std::string &location)
{
  return "Make on " + basePrinterName(name, location);
}

std::string VISORproblem1::noSupplyPartName(const std::string &name, const std::string &location)
{
  return "No supply part for " + basePrinterName(name, location);
}

std::string VISORproblem1::visorPartName(const std::string &name, const std::string &location)
{
  return "Visor made on " + basePrinterName(name, location);
}

std::string VISORproblem1::basePrinterName(const std::string &name, const std::string &location)
{
  return "Printer: " + name + " at-> " + location;
}

std::string VISORproblem1::printerFromPrinterName(const std::string &baseName)
{
  return textBetween(baseName, "Printer: ", " at-> ");
}

std::string VISORproblem1::locationFromPrinterName(const std::string &baseName)
{
  return textAfter(baseName, " at-> ");
}

//------------------------------------------
// Subs Bom Entry methods
//-----------------------------------------
void VISORproblem1::getSubVol(
    std::vector<std::string> &printerName, std::vector<std::string> &printerLoc,
    std::vector<std::string> &matLoc, std::vector<std::string> &matSize, std::vector<std::string> &matType,
    std::vector<std::vector<float>> &subVol, std::vector<std::string> &own)
{
  printerName.clear();
  printerLoc.clear();
  matLoc.clear();
  matSize.clear();
  matType.clear();
  subVol.clear();
  own.clear();
  witAttr objItrState;
  witGetObjItrState(witRun(), &objItrState);
  while (true)
  {
    witAdvanceObjItr(witRun());
    witGetObjItrState(witRun(), &objItrState);
    if (objItrState == WitINACTIVE) break;
    if (objItrState == WitAT_SUB_ENTRY)
    {
      char *operationName;
      int bomEntry;
      int subEntry;
      witGetObjItrSubsBomEntry(witRun(), &operationName, &bomEntry, &subEntry);
      std::vector<float> sv = witGetSubArcAttribute(witGetSubsBomEntrySubVol, operationName, bomEntry, subEntry);

      printerName.push_back(printerFromPrinterName(operationName));
      printerLoc.push_back(locationFromPrinterName(operationName));

      char *consPart;
      witGetSubsBomEntryConsumedPart(witRun(), operationName, bomEntry, subEntry, &consPart);

      matLoc.push_back(locationFromMaterialName(consPart));
      matSize.push_back(filamentSizeFromMaterialName(consPart));
      matType.push_back(plasticTypeFromMaterailName(consPart));
      subVol.push_back(sv);
      if (ownSupply(consPart))own.emplace_back("yes");
      else own.emplace_back("no");

      witFree(consPart);
      witFree(operationName);
    }
  }
}

// -----------------------------
// solver methods
// ----------------------------
void VISORproblem1::solve(bool useOptImplode)
{
  if (useOptImplode) witOptImplode(witRun());
  else witHeurImplode(witRun());
}





// default constructor
VISORproblem1::VISORproblem1()
    :
    VISORproblem(),
    materialBaseNames_(),
    printerBaseNames_()
{
  setNPeriods(30);
  witSetIndependentOffsets(witRun(), WitTRUE);
}

// destructor
VISORproblem1::~VISORproblem1()
{
  VISORproblem1::gutsOfDestructor();
}

// copy constructor. Not yet suported
VISORproblem1::VISORproblem1(const VISORproblem1 &source) :
    VISORproblem(source)
{
  gutsOfCopy(source);
}

// assignment operator. Not yet supported
VISORproblem1 &
VISORproblem1::operator=(const VISORproblem1 &rhs)
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
void VISORproblem1::gutsOfCopy(const VISORproblem1 &source)
{
  // Copy Wit Run
  VISORproblem::gutsOfCopy(source);

  // Turn off WIT informational messages
  witSetMesgTimesPrint(wr_, WitTRUE, WitINFORMATIONAL_MESSAGES, 0);
  // Turn off warning msg about long names.
  witSetMesgTimesPrint(wr_, WitTRUE, 338, 0);

  witCopyData(wr_, source.wr_);
}

// destructor
void VISORproblem1::gutsOfDestructor()
{}


#ifdef NDEBUG
#undef NDEBUG
#endif

//------------------------------------------------------------------------------
// Test function
//------------------------------------------------------------------------------
void
VISORproblem1::test()
{
  //OsiRelFltEq eq(1e-05);
  VisorRelFltEq eq(1.e-05);

  {
    VISORproblem1 prob;

    prob.setNPeriods(12);
    assert(prob.getNPeriods() == 12);

    prob.setTitle("wittitle");
    assert(prob.getTitle() == "wittitle");

  }


  {
    // Test materials
    VISORproblem1 prob;
    assert(prob.getNPeriods() == 30);
    prob.setNPeriods(25);
    assert(prob.getNPeriods() == 25);

    prob.addMaterial("Briarcliff", "1.75mm", "PLA", 200.0, 75);

    std::vector<std::string> location, filamentSize, plasticType;
    prob.getMaterials(location, filamentSize, plasticType);
    assert(location.size() == 1);
    assert(filamentSize.size() == 1);
    assert(plasticType.size() == 1);
    assert(location[0] == "Briarcliff");
    assert(filamentSize[0] == "1.75mm");
    assert(plasticType[0] == "PLA");

    std::vector<float> sv = prob.getOwnSupply("Briarcliff", "1.75mm", "PLA");
    assert(eq(sv[0], 50.));
    assert(eq(sv[1], 0.0));
    sv = prob.getSharedSupply("Briarcliff", "1.75mm", "PLA");
    assert(eq(sv[0], 150.));
    assert(eq(sv[3], 0.0));
    assert(sv.size() == 25);


    prob.addMaterial("Amawalk", "1.75mm", "ABS", 100.0, 33);
    prob.getMaterials(location, filamentSize, plasticType);
    assert(location.size() == 2);
    sv = prob.getOwnSupply("Amawalk", "1.75mm", "ABS");
    assert(eq(sv[0], 67.));
    assert(eq(sv[24], 0.0));
    sv = prob.getSharedSupply("Amawalk", "1.75mm", "ABS");
    assert(eq(sv[0], 33.));
    assert(eq(sv[20], 0.0));


    // Test printers
    prob.addPrinter("DigiLab3D45", "Kitchawan Rd", 30.f, true, false, true, true, false, false);
    std::vector<std::string> printerName, printerLoc;
    prob.getPrinters(printerName, printerLoc);
    assert(printerName.size() == 1);
    assert(printerLoc.size() == 1);
    assert(printerName[0] == "DigiLab3D45");
    assert(printerLoc[0] == "Kitchawan Rd");
    std::vector<float> pr = prob.getPrinterProdRate("DigiLab3D45", "Kitchawan Rd");
    assert(eq(pr[0], 30.));
    assert(eq(pr[20], 30.));
    std::vector<float> shipVol = prob.getPrinterShipVol("DigiLab3D45", "Kitchawan Rd");
    assert(eq(shipVol[0], 0.));
    assert(eq(shipVol[10], 0.));

    // Check Subs Boms
    {
      std::vector<std::string> printerName, printerLoc;
      std::vector<std::string> matLoc, matSize, matType, own;
      std::vector<std::vector<float>> subVol;
      prob.getSubVol(
          printerName, printerLoc,
          matLoc, matSize, matType,
          subVol, own);
      assert(printerName[0] == "DigiLab3D45");
      assert(printerLoc[0] == "Kitchawan Rd");
      assert(matLoc[0] == "Briarcliff");
      assert(matSize[0] == "1.75mm");
      assert(matType[0] == "PLA");
      assert(own[0] == "no");


      prob.addPrinter("DigiLab3D45", "Amawalk", 10.f, true, false, false, false, true, false);
      prob.getSubVol(
          printerName, printerLoc,
          matLoc, matSize, matType,
          subVol, own);
      assert(subVol.size() == 3);


    }


  }

}


