//
// Created by jpfasano on 4/8/20.
//


#include <map>
#include <vector>

#include <cassert>
#include <iostream>
#include <float.h>
#include <math.h>
#include "wit.h"



//#include "CoinFloatEqual.h"
#include "visorUtil.h"
#include "VisorFloatEqual.h"
#include "globalAttrData.h"

//-------------------------------------------------------------------------
// Read onHandMaterial file
//-------------------------------------------------------------------------
void readOnHandMaterial(const std::string& onHandMaterialFileName,VISORproblem1 & printingProb)
{
  VISORonHandMaterial onHandMaterialFile(onHandMaterialFileName);
  VISORonHandMaterialIterator onHandMaterialFileIter(onHandMaterialFile);
  bool dupKeyWarningPrinted = false;

  // loop once for each record in materail file
  int recNo = 1;
  for (; onHandMaterialFileIter() != NULL;)
  {
    recNo++;
    std::string matLoc = onHandMaterialFileIter.location();
    if (matLoc == "")
    {
      std::cout << "WARNING: location not specifed\n";
      std::cout << "   Filename: " + onHandMaterialFileName + "\n";
      std::cout << "   Record number: " << recNo << "\n";
      std::cout << "   Record is ignored\n";
      continue;
    }

    std::string filSze = onHandMaterialFileIter.filamentSize();
    std::string pType = onHandMaterialFileIter.plasticType();
    float qty = onHandMaterialFileIter.quantityAsFloat();
    int shrPer = onHandMaterialFileIter.shareAsInt();

    // Convert strings to lower/upper case. see https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
    transform(filSze.begin(), filSze.end(), filSze.begin(), ::tolower);
    transform(pType.begin(), pType.end(), pType.begin(), ::toupper);

    if (!(filSze == "1.75mm" || filSze == "2.85mm"))
    {
      //std::cout <<"---------------------------------------------\n";
      std::cout << "WARNING: Unrecognized filament size: " + filSze + "\n";
      std::cout << "   Expected values: 1.75mm 2.85mm\n";
      std::cout << "   Filename: " << onHandMaterialFileName << "\n";
      std::cout << "   Record number: " << recNo << "\n";
      std::cout << "   location: " + matLoc + "\n";
      std::cout << "   plasticType: " + pType + "\n";
      std::cout << "   quantity: " << qty << +"\n";
      std::cout << "   share: " << shrPer << +"\n";
      std::cout << "   Record is ignored\n";
      //std::cout <<"---------------------------------------------\n";
      continue;
    }

    if (!(pType == "PETG" || pType == "PLA" || pType == "ABS" || pType == "ONYX"))
    {
      //std::cout <<"---------------------------------------------\n";
      std::cout << "WARNING: Unrecognized plasticType: " + pType + "\n";
      std::cout << "   Expected values: PETG PLA ABS ONYX\n";
      std::cout << "   Filename: " << onHandMaterialFileName << "\n";
      std::cout << "   Record number: " << recNo << "\n";
      std::cout << "   location: " + matLoc + "\n";
      std::cout << "   filamentSize: " << filSze << "\n";
      std::cout << "   quantity: " << qty << +"\n";
      std::cout << "   share: :" << shrPer << +"\n";
      std::cout << "   Record is ignored\n";
      //std::cout <<"---------------------------------------------\n";
      continue;
    }

    if (printingProb.materialExists(matLoc, filSze, pType))
    {
      if (!dupKeyWarningPrinted)
      {
        std::cout << "   Cannot have two materials with same location, filamentSize and plasticType\n";
        std::cout << "   Only first instance used. Others are ignored\n";
        dupKeyWarningPrinted = true;
      }
      continue;
    }


    printingProb.addMaterial(matLoc, filSze, pType, qty, shrPer);
  }
}

//-------------------------------------------------------------------------
// Read printer file
//-------------------------------------------------------------------------
void readPrinter(std::string & printerFileName,
                 VISORproblem1 * printingProbPtr,
                 VISORproblem2 * allocProbPtr)
{
  VISORprinter printerFile(printerFileName);
  VISORprinterIterator printerFileIter(printerFile);
  bool dupKeyWarningPrinted = false;

  // loop once for each record in printer file
  int recNo = 1;
  for (; printerFileIter() != NULL;)
  {
    recNo++;
    std::string pNam = printerFileIter.name();

    if (pNam == "")
    {
      std::cout << "WARNING: name not specifed\n";
      std::cout << "   Filename: " + printerFileName + "\n";
      std::cout << "   Record number: " << recNo << "\n";
      std::cout << "   Record is ignored\n";
      continue;
    }

    std::string pLoc = printerFileIter.location();
    int prodRate = printerFileIter.prodRateAsInt();
    bool f175 = printerFileIter.F175asBool();
    bool f285 = printerFileIter.F285asBool();
    bool petg = printerFileIter.PETGasBool();
    bool pla = printerFileIter.PLAasBool();
    bool abs = printerFileIter.ABSasBool();
    bool onyx = printerFileIter.ONYXasBool();

    bool dupKey =false;
    if (printingProbPtr!=nullptr && printingProbPtr->printerExists(pNam, pLoc))
      dupKey=true;
    else if (allocProbPtr!=nullptr && allocProbPtr->visorExists(pNam, pLoc))
      dupKey=true;
    if (dupKey)
    {
      if (!dupKeyWarningPrinted)
      {
        //std::cout <<"---------------------------------------------\n";
        std::cout << "   Cannot have two printers with same name & location\n";
        std::cout << "   Only first instance used. Others are ignored\n";
        //std::cout <<"---------------------------------------------\n";
        dupKeyWarningPrinted = true;
      }
      continue;
    }
    if (printingProbPtr!= nullptr) printingProbPtr->addPrinter(pNam, pLoc, prodRate, f175, f285, petg, pla, abs, onyx);
    if (allocProbPtr!= nullptr)allocProbPtr->addVisor(pNam, pLoc);
  }
}

//-------------------------------------------------------------------------
// Read requestQuantity file
//-------------------------------------------------------------------------
void readRequestQuantity(std::string & requestQuantityFileName,
                         VISORproblem2 & allocProb)
{
  VISORrequestQuantity requestQuantityFile(requestQuantityFileName);
  VISORrequestQuantityIterator requestQuantityFileIter(requestQuantityFile);

  // loop once for each record in printer file
  int recNo = 1;
  for (; requestQuantityFileIter() != NULL;)
  {
    recNo++;
    std::string loc = requestQuantityFileIter.location();

    if (loc == "")
    {
      //std::cout <<"---------------------------------------------\n";
      std::cout << "WARNING: location not specifed\n";
      std::cout << "   Filename: " + requestQuantityFileName + "\n";
      std::cout << "   Record number: " << recNo << "\n";
      std::cout << "   Record is ignored\n";
      //std::cout <<"---------------------------------------------\n";
      continue;
    }

    int per = requestQuantityFileIter.dateAsInt();
    int quan = requestQuantityFileIter.requestedQuantityAsInt();
    //std::cout <<recNo <<" "+loc <<"\n";

    allocProb.addVisorRequest(loc, per, quan);
  }
}


//-------------------------------------------------------------------------
// Read visorProdPlan file
//-------------------------------------------------------------------------
void readVisorProdPlan(std::string visorProdPlanFileName,
                       VISORproblem2 & allocProb)
{
  VISORvisorProdPlan visorProdPlanFile(visorProdPlanFileName);
  VISORvisorProdPlanIterator visorProdPlanFileIter(visorProdPlanFile);

  // loop once for each record in printer file
  int recNo = 1;
  for (; visorProdPlanFileIter() != NULL;)
  {
    recNo++;
    std::string loc = visorProdPlanFileIter.producingLocation();

    if (!allocProb.locationExists(loc))
    {
      // Error condition. write warning and keep going
      //std::cout <<"---------------------------------------------\n";
      std::cout << "WARNING: specified producingLocation does not exist.\n";
      std::cout << "   location not defined in printer file.\n";
      std::cout << "   producingLocation: " + loc + "\n";
      std::cout << "   Filename: " + visorProdPlanFileName + "\n";
      std::cout << "   Record number: " << recNo << "\n";
      std::cout << "   Record is ignored\n";
      //std::cout <<"---------------------------------------------\n";
      continue;
    }


    int per = visorProdPlanFileIter.dateAsInt();
    int quan = visorProdPlanFileIter.productionQuantityAsFloat();
    allocProb.setVisorSupplyVol(loc,per,quan);
  }
}


void copyImplosion1ShipVolToImplosion2SupplyVol(
    VISORproblem1 &printingProb,
    VISORproblem2 &allocProb)
{
  std::vector<std::string> printerName, printerLoc;
  printingProb.getPrinters(printerName, printerLoc);

  // Loop once for each printer
  for (int p = 0; p < printerName.size(); p++)
  {
    // Get ShipVol from printer
    std::vector<float> sv = printingProb.getPrinterShipVol(printerName[p], printerLoc[p]);

    // Set Supply Vol of 2nd implosion to 1st implosion shipVol
    allocProb.setVisorSupplyVol(printerName[p], printerLoc[p], sv);
  }
}
