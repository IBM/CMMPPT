
#include <cassert>
#include <iostream>

//#include "CoinFloatEqual.h"
#include "visorUtil.h"
#include "globalAttrData.h"

bool doFirstImplosion(VISORproblem1 & prob1)
{
  return (&prob1) != nullptr;
}
bool doSecondImplosion(VISORproblem2 & prob2)
{
  return (&prob2) != nullptr;
}

int
main(int argc, char *argv[])
{

  std::cout << "visor Build date: " << __DATE__ << std::endl;
  {

    // Put Parms into a vector
    std::vector<std::string> args;
    for (int i = 0; i < argc; i++)
      args.push_back(argv[i]);

    {
      // Check parm passed to program
      bool parmError = false;

      if (args.size() > 1)
      {
        if (args[1] == "?")
        {
          parmError = true; // set flag to provide usage info
        }
      }

      if (!parmError)
      {
        if (args.size() != 3)
        {
          // Wrong number of parms. Write message
          std::cerr
              << "-------------------------------------------------------" << std::endl
              << "Wrong number of parmeters specified" << std::endl
              << "  Number parameters specified: " << (int) args.size() - 1 << std::endl
              << "  Expected number of parameters: 2" << std::endl
              << "-------------------------------------------------------" << std::endl;
          parmError = true;
        }
      }


      if (parmError)
      {
        // Write correct usage
        std::cerr
            << "-------------------------------------------------------" << std::endl
            << argv[0] << " dataDirectory  solveType " << std::endl
            << "  where:" << std::endl
            << "    dataDirectory:" << std::endl
            << "      Directory where input files are located and" << std::endl
            << "      output files are written." << std::endl
            << "    solveType:" << std::endl
            << "      If allocateVisorsOnly then only the second implosion is done, where " << std::endl
            << "      provided supply of visors is allcated to requesting hosptials" << std::endl
            << "      Any value other than allocateVisorsOnly means that the standard 2 implosions are run" << std::endl
            << "  example usage:" << std::endl
            << "    " << argv[0] << " ../data/BrendaData allocateVisorsOnly" << std::endl
            << "-------------------------------------------------------" << std::endl;
        return 1;
      }
    }

    std::string outputDirectory = args[1];
    std::string inputDirectory = args[1];
    std::cout << "inputDirectory: " << inputDirectory << std::endl;
    std::cout << "solveType: " << args[2] << std::endl;

    VISORproblem1 * printingProbPtr = nullptr;
    VISORproblem2 * allocProbPtr = nullptr;
    if (std::string(args[2]) == "allocateVisorsOnly")
    {
      allocProbPtr = new VISORproblem2();
    }
    else {
      printingProbPtr = new VISORproblem1();
      allocProbPtr = new VISORproblem2();
    }

    VISORproblem1 & printingProb = *printingProbPtr;
    VISORproblem2 & allocProb = *allocProbPtr;
    int nPeriods = 30;
    VisorRelFltEq eq(1.e-05);

    int nWfmvScaleFactors;
    bool writeZeros = true;


    if (doFirstImplosion(printingProb))
    {
       printingProb.setSolverLogFileName(outputDirectory + "/cplexMesgFile.txt");
       printingProb.setNPeriods(nPeriods);
    }
    if (doSecondImplosion(allocProb))
    {
      allocProb.setNPeriods(nPeriods);
    }


    // Read Material file and add to model, if doing first implosion
    if (doFirstImplosion(printingProb))
    {
      std::string onHandMaterialFileName = inputDirectory + "/onHandMaterial.csv";
      readOnHandMaterial(onHandMaterialFileName,printingProb);
    }


    // Read printer file and add to model
    if (doFirstImplosion(printingProb) || doSecondImplosion(allocProb))
    {
      std::string printerFileName = inputDirectory + "/printer.csv";
      readPrinter(printerFileName,printingProbPtr,allocProbPtr);
    }

    if (doSecondImplosion(allocProb))
      // Read visor request file
    {
      std::string requestQuantityFileName = inputDirectory + "/requestQuantity.csv";
      readRequestQuantity(requestQuantityFileName,allocProb);
    }



    // Read global attribute file
    {
      std::string globalAttrFileName = inputDirectory + "/globalAttrData.csv";
      if (SCOfile::fileExist(globalAttrFileName))
      {
        VISORglobalAttrData globalAttrFile(globalAttrFileName);

        // Is eqAllocPerBnd present
        if (globalAttrFile.keyExists("eqAllocPerBnd"))
        {
          // Set EqAllocPerBnd
          int bnd = globalAttrFile.fieldAsInt("eqAllocPerBnd", "attrValue");

          if (bnd < 0 || bnd >= nPeriods)
          {
            std::cout << "WARNING: Out of range eqAllocPerBnd: " << bnd << "\n";
            std::cout << "   Expected values: [1," << nPeriods << ")\n";

            std::cout << "   Filename: " + globalAttrFileName + "\n";
            std::cout << "   Record is ignored\n";
          } else
          {
            allocProb.setEqAlloPerBnd(bnd);
            std::cout << "eqAllocPerBnd set to: " << bnd << std::endl;
          }
        }
      }
    }

    // Do first implosion
    if ( doFirstImplosion(printingProb) )
    {
      //printingProb.writeWitData(outputDirectory + "/wit1.data");
      printingProb.solve();
    }

    if  ( doSecondImplosion(allocProb) )
    {

      //----------------------------------
      // If first implosoin run then get visor production from it
      //----------------------------------
      if (doFirstImplosion(printingProb))
      {
        copyImplosion1ShipVolToImplosion2SupplyVol(printingProb,allocProb);
      } else
      {
        // Supply of Visors is to come from the file visorProdPlan
        std::string visorProdPlanFileName = inputDirectory + "/visorProdPlan.csv";
        readVisorProdPlan(visorProdPlanFileName,allocProb);
      }

      //Do second implosion
      //allocProb.writeWitData(outputDirectory + "/wit2.data");
      allocProb.solve();
    }

    bool writeHeader = true;
    // --------------------------------------------
    // write subVol file
    // --------------------------------------------
    if ( doFirstImplosion(printingProb) )
    {
      std::string subVolFileName = outputDirectory + "/subVol.csv";
      FILE *subVolFilePtr = fopen(subVolFileName.c_str(), "w");

      std::string heading;
      heading = "\"printerName\",\"printerLocation\",\"materialLocation\",\"filamentSize\",\"plasticType\",\"period\",\"quantity\",\"own\"";
      if (writeHeader) fprintf(subVolFilePtr, "%s\n", heading.c_str());

      std::vector<std::string> printerName, printerLoc;
      std::vector<std::string> matLoc, matSize, matType, own;
      std::vector<std::vector<float>> subVol;
      printingProb.getSubVol(
          printerName, printerLoc,
          matLoc, matSize, matType,
          subVol, own);

      // Loop once for each sub bom
      for (int s = 0; s < printerName.size(); s++)
      {
        //std::cout <<printerName[s]+" "+printerLoc[s]+" "+matLoc[s]+" "+matSize[s]+" "+matType[s]+" "+own[s]+"\n";
        for (int t = 0; t < nPeriods; t++)
        {
          if (eq(subVol[s][t], 0.0)) continue;

          fprintf(subVolFilePtr,
                  "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,%f,\"%s\"\n",
                  printerName[s].c_str(), printerLoc[s].c_str(), matLoc[s].c_str(), matSize[s].c_str(),
                  matType[s].c_str(), t, subVol[s][t], own[s].c_str());
        }
      }

      fclose(subVolFilePtr);
    } // finished writing subVol file

    // --------------------------------------------
    // write prodVol file
    // --------------------------------------------
    if ( doFirstImplosion(printingProb) )
    {
      std::string prodVolFileName = outputDirectory + "/prodVol.csv";
      FILE *prodVolFilePtr = fopen(prodVolFileName.c_str(), "w");

      std::string heading;
      heading = "\"printerName\",\"printerLocation\",\"period\",\"quantity\"";
      if (writeHeader) fprintf(prodVolFilePtr, "%s\n", heading.c_str());

      std::vector<std::string> printerName, printerLoc;
      printingProb.getPrinters(printerName, printerLoc);

      // Loop once for each printer
      for (int p = 0; p < printerName.size(); p++)
      {
        // Get printers witOpExecVol
        std::vector<float> ev = printingProb.getPrinterProdVol(printerName[p], printerLoc[p]);
        //std::vector<float> sv=printingProb.getPrinterShipVol(printerName[p],printerLoc[p]);
        for (int t = 0; t < nPeriods; t++)
        {
          //assert( eq(ev[t],sv[t]) );
          if (eq(ev[t], 0.0)) continue;

          fprintf(prodVolFilePtr,
                  "\"%s\",\"%s\",%d,%f\n",
                  printerName[p].c_str(), printerLoc[p].c_str(), t, ev[t]);
        }
      }

      fclose(prodVolFilePtr);
    } // finished writing prodVol file

    // --------------------------------------------
    // write shipQuantity file
    // --------------------------------------------
    {
      std::string shipQuantityFileName = outputDirectory + "/shipQuantity.csv";
      FILE *shipQuantityFilePtr = fopen(shipQuantityFileName.c_str(), "w");

      std::string heading;
      heading = "\"requestingLocation\",\"producingLocation\",\"period\",\"shipQuantity\"";
      if (writeHeader) fprintf(shipQuantityFilePtr, "%s\n", heading.c_str());

      std::vector<std::string> demands = allocProb.getDemands();

      // Loop once for each demand
      for (int d = 0; d < demands.size(); d++)
      {
        // Get shipVol to hospital
        std::string dn = demands[d];

        //get the witSubVol for each location making visors
        std::vector<std::string> partLoc;
        std::vector<std::vector<float>> sv;
        allocProb.getSubVols(dn, partLoc, sv);

        for (int l = 0; l < partLoc.size(); l++)
        {

          std::string pn = partLoc[l];

          for (int t = 0; t < nPeriods; t++)
          {
            if (eq(sv[l][t], 0.0)) continue;

            fprintf(shipQuantityFilePtr,
                    "\"%s\",\"%s\",%d,%f\n",
                    dn.c_str(), pn.c_str(), t, sv[l][t]);
          }
        }
      }

      fclose(shipQuantityFilePtr);
    } // finished writing shipQuantity file

    // --------------------------------------------
    // write stockVolMaterial file
    // --------------------------------------------
    if ( doFirstImplosion(printingProb) )
    {
      std::string stockVolMaterialFileName = outputDirectory + "/stockVolMaterial.csv";
      FILE *stockVolMaterialFilePtr = fopen(stockVolMaterialFileName.c_str(), "w");

      std::string heading;
      heading = R"("location","filamentSize","plasticType","period","stockQuantity")";
      if (writeHeader) fprintf(stockVolMaterialFilePtr, "%s\n", heading.c_str());

      std::vector<std::string> locs, filamentSizes, pTypes;
      printingProb.getMaterials( locs,filamentSizes, pTypes);

      // Loop once for each demand
      for (int i = 0; i < locs.size(); i++)
      {
        // Get stock vol
        std::vector<float> sv= printingProb.getMaterialStockVol(locs[i],filamentSizes[i],pTypes[i]);

        for (int t = 0; t < sv.size(); t++)
          {
            if (eq(sv[t], 0.0)) continue;

            fprintf(stockVolMaterialFilePtr,
                    "\"%s\",\"%s\",\"%s\",%d,%f\n",
                    locs[i].c_str(), filamentSizes[i].c_str(),pTypes[i].c_str(), t, sv[t]);
          }
        }

      fclose(stockVolMaterialFilePtr);
    } // finished writing stockVolMateral
    //---------------------------------------------------------



  }

  std::cout << "visor application is finished." << std::endl;
  return 0;
}
